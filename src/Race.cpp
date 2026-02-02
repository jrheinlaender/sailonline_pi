/***************************************************************************
 *   Copyright (C) 2026 by Jan Rheinländer                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************/

#include <sstream>
#include <ctime>

#include <wx/wx.h>

#include <curl/curl.h>

#include <pugixml.hpp>

#include "sailonline_pi.h"
#include "Sailonline.h"
#include "Race.h"
#include "SolApi.h"

Race::Race(sailonline_pi& plugin) : m_sailonline_pi(plugin) {}

Race::~Race() {}

namespace {
// TODO duplicate code with WR plugin

// Performance loss is half the boat speed after the tack/jibe, in percent.
double get_performance_loss_tack_jibe(const double stw) {
  return 0.5 * stw / 100.0;
}

// Performance loss is ca. 0.07% per degree
// Assumes that first_twa and next_twa have the same sign
double get_performance_loss_course_change(const double first_twa,
                                          const double next_twa) {
  return std::fabs(next_twa - first_twa) / 180.0 * M_PI / 25.0;
}

double get_performance(const double performance, const double theoretical_stw,
                       const double first_twa, const double next_twa) {
  if (performance < 0.93) return performance;

  if (first_twa * next_twa > 0) {
    // Course change
    return performance -
           get_performance_loss_course_change(first_twa, next_twa);
  } else {
    // Tack or jibe
    return performance -
           get_performance_loss_tack_jibe(theoretical_stw * performance);
  }
}

double get_recovery_step(const double performance, const double step_seconds,
                         const double stw) {
  return std::min(1.0, performance + step_seconds * 3.0 / (20.0 * stw) / 100.0);
}

double get_recovery(const double performance, const double time_seconds,
                    const double theoretical_stw) {
  if (performance >= 1.0) return 1.0;

  std::cout << "Recovery from " << performance << " at " << theoretical_stw
            << " kn in " << time_seconds << " s" << std::endl;

  double jump =
      std::min(time_seconds, 30.0);  // TODO Find correct value for jump
  double current_stw = theoretical_stw * performance;
  double newperformance = performance;
  double current_time;

  // TODO Find closed formula for this
  for (current_time = jump; current_time <= time_seconds;
       current_time += jump) {
    newperformance = get_recovery_step(newperformance, jump, current_stw);
    if (newperformance >= 1.0) return 1.0;

    current_stw = theoretical_stw * newperformance;
  }

  // Remaining fractional jump
  double remainder = time_seconds - current_time;
  if (remainder > 0.0)
    return get_recovery_step(newperformance, remainder, current_stw);

  return newperformance;
}
}  // namespace

Dc::Dc(const wxDateTime& timestamp, const double lat_start,
       const double lon_start, const double course, const bool is_twa)
    : m_timestamp(timestamp),
      m_lat_start(lat_start),
      m_lon_start(lon_start),
      m_is_twa(is_twa) {
  if (m_is_twa)
    m_twa = course;
  else
    m_course = course;
}

std::vector<std::string> Race::GetErrors() {
  std::vector<std::string> result;
  std::swap(m_errors, result);
  return result;
}

namespace {
std::string curl_extract_cookie(CURL* curl, const std::string& name) {
  struct curl_slist* cookies = nullptr;
  CURLcode result = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
  std::string cookie("");

  if (result == CURLE_OK && cookies != nullptr) {
    struct curl_slist* each = cookies;
    while (each) {
      std::string c(each->data);
      size_t pos = c.find(name);

      if (pos != std::string::npos) {
        cookie = c.substr(pos + name.size() + 1);
        break;
      }

      each = each->next;
    }
    curl_slist_free_all(cookies);
  }

  return cookie;
}

static size_t curl_write_cb(void* contents, size_t size, size_t nmemb,
                            void* userp) {
  wxLogMessage("Receiving %u bytes of data", size * nmemb);
  size_t realsize = size * nmemb;
  std::string* data = static_cast<std::string*>(userp);
  data->append(static_cast<const char*>(contents), realsize);
  return realsize;
}
}  // namespace

bool Race::CallCurl(CURL* curl) {
  CURLcode result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    m_errors.emplace_back("Curl error: " + std::to_string(result));
    return false;
  }

  return true;
}

std::string Race::SetPlaceholders(const std::string& input) const {
  // TODO Should this map be a member of class Race?
  static std::map<std::string, std::string> placeholders{
      {"$$password", "21shukur%3AGozorI21"},
      {"$$username", "Ibis"},
      {"$$racenumber", ""},
      {"$$token", ""}};
  placeholders["$$racenumber"] = m_id;
  placeholders["$$token"] = m_sol_token;

  std::string result = input;

  for (const auto& p : placeholders) {
    auto pos = result.find(p.first);
    if (pos == std::string::npos) continue;

    result = result.replace(pos, p.first.size(), p.second);
  }

  return result;
}

bool Race::Login() {
  // Are we already logged in?
  if (!m_sol_token.empty()) return {};

  // Log in to sailonline.org to get more specific race data
  // Note: wxWebRequest stores cookies in the wxWebSession but they are not
  // accessible
  // Note: OCPN_postDataHttp() does not handle cookies
  // Therefore we must use another method
  wxLogMessage("Logging into race %s", m_id);
  CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
  if (result != CURLE_OK) {
    m_errors.emplace_back("Curl error: " + std::to_string(result));
    return false;
  }

  CURL* curl = curl_easy_init();
  if (curl == nullptr) {
    m_errors.emplace_back("Curl error: curl_easy_init() failed");
    return false;
  }

  curl_easy_setopt(curl, CURLOPT_URL, SolApi::kSolUrl.c_str());
  // Some servers don't like requests without user agent
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  // Enable cookie engine
  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
  // Setup function to catch the page contents. This also suppresses output on
  // stdout
  std::string pagedata;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&pagedata);
  if (!CallCurl(curl)) return false;
  std::string csrftoken = curl_extract_cookie(curl, "csrftoken");
  if (csrftoken.empty()) {
    m_errors.emplace_back("Could not read CSRF token from sailonline.org");
    return false;
  }

  // Note that URL remains the same
  // TODO Configure GUI to enter username and password
  curl_easy_setopt(curl, CURLOPT_REFERER,
                   SetPlaceholders(SolApi::kSolReferer).c_str());
  // Because sailonline.org redirects to
  // https://sailonline.org/windy/run/<racenumber>
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  std::string postdata = SetPlaceholders(SolApi::kSolPost) + csrftoken;
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
  pagedata.clear();
  std::cout << "POST " << postdata << std::endl;
  if (!CallCurl(curl)) return false;

  // TODO Can this be more stable than scanning the javascript code of the page?
  size_t pos = pagedata.find("function getToken()\n{\n\treturn \"");
  std::string racetoken("");
  if (pos == std::string::npos) {
    m_errors.emplace_back("Could not find token for race " + m_id +
                          ", did you register?");
    curl_easy_cleanup(curl);
    return false;
  }

  m_sol_token = pagedata.substr(pos + 31, 32);
  wxLogMessage("Successfully logged in with token '%s' for race %s",
               m_sol_token, m_id);

  curl_easy_cleanup(curl);
  return true;
}

wxString Race::GetRaceInfo() {
  wxString result;

  if (m_sol_token.empty()) {
    m_errors.emplace_back("Not logged into race " + m_id +
                          ", did you register?");
    return result;
  }

  // Check if raceinfo was already downloaded
  wxFileName raceinfo =
      m_sailonline_pi.GetDataDir(wxString::Format("Race_%s", m_id.c_str()));
  raceinfo.SetFullName(wxString::Format("auth_raceinfo_%s.xml", m_id.c_str()));
  if (raceinfo.Exists()) {
    wxLogMessage("Reading cached auth_raceinfo_%s.xml", m_id);
    wxFile raceinfo_file(raceinfo.GetFullPath(), wxFile::read);
    raceinfo_file.ReadAll(&result);
    raceinfo_file.Close();
    return result;
  }

  wxLogMessage("Downloading auth_raceinfo_%s.xml", m_id);
  CURLcode cresult = curl_global_init(CURL_GLOBAL_ALL);
  if (cresult != CURLE_OK) {
    m_errors.emplace_back("Curl error: " + std::to_string(cresult));
    return result;
  }

  CURL* curl = curl_easy_init();
  if (curl == nullptr) {
    m_errors.emplace_back("Curl error: curl_easy_init() failed");
    return result;
  }

  std::string race_url = SetPlaceholders(SolApi::kSolRaceXmlUrl);
  curl_easy_setopt(curl, CURLOPT_URL, race_url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPGET,
                   1L);  // Otherwise curl will try to POST
  // Setup function to catch the page contents. This also suppresses output on
  // stdout
  std::string pagedata;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&pagedata);
  if (!CallCurl(curl)) {
    m_errors.emplace_back("Curl error: GET of race XML failed");
    return result;
  }
  if (pagedata == "Bad token") {
    m_errors.emplace_back("Race token is invalid. Try logging in again");
    // TODO but currently the UI doesn't offer any way of re-logging-in ...
    return result;
  }

  // Write race info to file for later use
  wxFile raceinfo_file(raceinfo.GetFullPath(), wxFile::write);
  if (raceinfo_file.Error()) {
    m_errors.emplace_back("Could not write to auth_raceinfo_" + m_id + ".xml");
    return result;
  }
  raceinfo_file.Write(pagedata);
  raceinfo_file.Close();
  wxLogMessage("Cached raceinfo to auth_raceinfo_%s.xml", m_id.c_str());

  // Available information
  // tag <url>:
  // https://www.sailonline.org/webclient/race_1967.xml?token=<TOKEN>
  //    zlib-compressed xml file containing data about every boat in the race
  //    From bash do printf "\x1f\x8b\x08\x00\x00\x00\x00\x00" |cat -
  //    race_1967.xml |gzip -dc|more
  // tag <weatherurl>:
  // https://www.sailonline.org/webclient/weatherinfo_122.xml?token=<TOKEN>
  //    Text file
  //       122 2025/12/21 16:24:15
  //       http://sailonline.org/site_media/weather/xml/weather_122_global_gfs_20251221_1624.xml
  //    From this link the weather data can be downloaded (without token)
  // tag <traceUrl>:
  // https://www.sailonline.org/webclient/traces_1967.xml?token=<TOKEN>
  //    zlib-compressed xml file containing tracks of all boats?
  // tag <boaturl>:
  //    Text file
  //       3092 449865 Ibis 2025/12/01 11:00:00 1764586800UTCC 2025/12/15
  //       18:52:07 1765824727UTCC 0.0 3.53605193784 3.32537832275 0.0
  //       0.966386722578 0.0 0.0 105.5979118 -10.3839613937 3.53605193784 54
  //       0 twa

  // Polar <boat><vpp><tws_splined> integer 0:max, <twa_splined> integer
  // 0:180, <bs_splined> float

  curl_easy_cleanup(curl);

  return result;
}

bool Race::DownloadPolar() {
  pugi::xml_document race_doc;
  auto status = race_doc.load_string(GetRaceInfo());
  if (!status) {
    m_errors.emplace_back("Could not parse race file: %s",
                          status.description());
    return false;
  }

  // Get boat polar
  // TWS: Space-separated list of true wind speeds (columns, integer, knots)
  // TWA: Space-separated list of true wind angles (rows, integer, degrees)
  // BS: Semicolon-separated list of space-separated lists of wind speeds
  // (float, m/s)
  pugi::xml_node node_name = race_doc.select_node("/race/boat/vpp/name").node();
  pugi::xml_node node_tws =
      race_doc.select_node("/race/boat/vpp/tws_splined").node();
  pugi::xml_node node_twa =
      race_doc.select_node("/race/boat/vpp/twa_splined").node();
  pugi::xml_node node_bs =
      race_doc.select_node("/race/boat/vpp/bs_splined").node();

  // Write polar to .csv file for import into weather routing plugin
  std::string polar_name(node_name.first_child().value());
  std::replace(polar_name.begin(), polar_name.end(), ' ', '_');
  wxFileName download_target = m_sailonline_pi.GetDataDir("Polar");
  download_target.SetFullName(
      wxString::Format("SOL_%s_polar.csv", polar_name.c_str()));
  wxLogMessage("Writing boat polar to %s", download_target.GetFullPath());
  wxFile polar_file(download_target.GetFullPath(), wxFile::write);
  if (polar_file.Error()) {
    m_errors.emplace_back("Could not write to polar file");
    return false;
  }

  polar_file.Write("twa/tws;");
  std::stringstream tws_stream(node_tws.first_child().value());
  unsigned tws;
  while (tws_stream >> tws) {
    if (tws_stream.bad()) {
      m_errors.emplace_back(
          "Format error in TWS data, wind speed is not a valid integer");
      return false;
    }
    polar_file.Write(std::to_string(
        tws * 3600.0 / 1852.0));  // TODO is this constant defined somewhere?
    if (!tws_stream.eof()) polar_file.Write(";");
  }
  polar_file.Write("\n");

  std::stringstream twa_stream(node_twa.first_child().value());
  std::stringstream bss_stream(node_bs.first_child().value());
  std::string twa;

  while (twa_stream >> twa) {
    std::string bs_line;
    if (!std::getline(bss_stream, bs_line, ';')) {
      m_errors.emplace_back(
          "Format error in TWA data, could not read next set of wind angles");
      return false;
    }

    polar_file.Write(twa);
    polar_file.Write(";");
    std::replace(bs_line.begin(), bs_line.end(), ' ', ';');
    polar_file.Write(bs_line);
    polar_file.Write("\n");
  }

  polar_file.Close();
  m_polarfile = download_target.GetFullName();
  wxLogMessage("Saved polar data to %s", download_target.GetFullPath());
  return true;
}

bool Race::GetWaypoints() {
  pugi::xml_document race_doc;
  auto status = race_doc.load_string(GetRaceInfo());
  if (!status) {
    m_errors.emplace_back("Could not parse race file: %s",
                          status.description());
    return false;
  }

  // Get waypoints
  m_waypoints.clear();

  pugi::xml_node node_course = race_doc.select_node("/race/course").node();
  for (pugi::xml_node node_wp = node_course.first_child(); node_wp != nullptr;
       node_wp = node_wp.next_sibling()) {
    if (strcmp(node_wp.name(), "waypoint") == 0) {
      std::shared_ptr<PlugIn_Waypoint> wp = std::make_shared<PlugIn_Waypoint>();

      for (pugi::xml_node node_wp_child = node_wp.first_child();
           node_wp_child != nullptr;
           node_wp_child = node_wp_child.next_sibling()) {
        if (strcmp(node_wp_child.name(), "order") == 0)
          wp->m_GUID = wxString::Format("SOL_%s_%s", m_id,
                                        node_wp_child.first_child().value());
        else if (strcmp(node_wp_child.name(), "name") == 0)
          wp->m_MarkName = node_wp_child.first_child().value();
        else if (strcmp(node_wp_child.name(), "lon") == 0) {
          double lon;
          if (wxString(node_wp_child.first_child().value()).ToDouble(&lon))
            wp->m_lon = lon;
        } else if (strcmp(node_wp_child.name(), "lat") == 0) {
          double lat;
          if (wxString(node_wp_child.first_child().value()).ToDouble(&lat))
            wp->m_lat = lat;
        }

        m_waypoints.emplace_back(wp);

        // Add permanent waypoint to main application. Note: data is copied
        if (!UpdateSingleWaypoint(wp.get())) AddSingleWaypoint(wp.get(), true);
      }
    }
  }

  return true;
}

const std::list<Dc>& Race::GetDcs() const { return m_dcs; }

std::list<Dc>& Race::GetDcs() { return m_dcs; }

std::pair<double, double> Race::GetWindData(const wxDateTime& t, double lat,
                                            double lon) const {
  Json::Value v;
  Json::FastWriter writer;
  wxDateTime time = t.FromUTC();
  if (!time.IsValid()) return {-1.0, -1.0};

  v["Day"] = time.GetDay();
  v["Month"] = time.GetMonth();
  v["Year"] = time.GetYear();
  v["Hour"] = time.GetHour();
  v["Minute"] = time.GetMinute();
  v["Second"] = time.GetSecond();

  v["Source"] = "SAILONLINE_PI";
  v["Type"] = "Request";
  v["Msg"] = "GRIB_VALUES_REQUEST";
  v["lat"] = lat;
  v["lon"] = lon;
  v["WIND SPEED"] = 1;

  SendPluginMessage("GRIB_VALUES_REQUEST", writer.write(v));
  Json::Value reply = m_sailonline_pi.GetJsonMessage();
  if (reply != Json::nullValue && reply.isMember("WIND SPEED") &&
      reply.isMember("WIND DIR"))
    return {reply["WIND SPEED"].asDouble() * 3600 / 1852,
            reply["WIND DIR"].asDouble()};

  return {-1.0, -1.0};
}

double Race::GetSpeedThroughWater(double tws, double twa) const {
  Json::Value v;
  Json::FastWriter writer;

  v["Source"] = "SAILONLINE_PI";
  v["Type"] = "Request";
  v["Msg"] = "WR_BOATDATA_REQUEST";
  v["Data"] = "Speed";
  v["Racenumber"] = m_id;
  v["tws"] = tws;
  v["twa"] = twa;

  SendPluginMessage("WR_BOATDATA_REQUEST", writer.write(v));
  Json::Value reply = m_sailonline_pi.GetJsonMessage();
  if (reply != Json::nullValue && reply.isMember("BOAT SPEED"))
    return reply["BOAT SPEED"].asDouble();

  return -1.0;
}

std::pair<double, double> Race::GetBoatOptimalAngles(double tws) const {
  Json::Value v;
  Json::FastWriter writer;

  v["Source"] = "SAILONLINE_PI";
  v["Type"] = "Request";
  v["Msg"] = "WR_BOATDATA_REQUEST";
  v["Data"] = "Angles";
  v["Racenumber"] = m_id;
  v["tws"] = tws;

  SendPluginMessage("WR_BOATDATA_REQUEST", writer.write(v));
  Json::Value reply = m_sailonline_pi.GetJsonMessage();
  if (reply != Json::nullValue && reply.isMember("OPT UP") &&
      reply.isMember("OPT DOWN"))
    return {reply["OPT UP"].asDouble(), reply["OPT DOWN"].asDouble()};

  return {-1.0, -1.0};
}

void Race::SimplifyDcs() {
  if (m_dcs.size() < 2) return;

  // TODO This must use the functionality already implemented in WR plugin to
  // simplify the route

  // Current leg is from first_dc to last_dc
  auto first_dc = m_dcs.begin();  // First DC of a leg
  auto second_dc =
      first_dc;  // Required because std::list does not allow first_dc + 1
  ++second_dc;
  auto last_dc = m_dcs.begin();  // Last DC that was investigated

  for (auto dc = second_dc; dc != m_dcs.end();) {
    double diff_course = std::fabs(dc->m_course - first_dc->m_course);
    double diff_twa = std::fabs(dc->m_twa - first_dc->m_twa);
    if (diff_course > 360.0) diff_course -= 360.0;
    if (diff_twa > 360.0) diff_twa -= 360.0;

    // Check for minimal course or twa changes and delete unnecessary waypoints
    // TODO Make limits for course/twa change configurable and maybe depend on
    // distance between waypoints (how detailed was the WR?)
    // TODO No land collision check!!!
    wxLogMessage(
        "First DC: %3.3f, TWA %3.3f, last DC: %3.3f, TWA %3.3f, this DC: "
        "%3.3f, TWA %3.3f",
        first_dc->m_course, first_dc->m_twa, last_dc->m_course, last_dc->m_twa,
        dc->m_course, dc->m_twa);

    if (diff_course < 2.0 && (!first_dc->m_is_twa || first_dc == last_dc)) {
      wxLogMessage("Continuing current leg because of minimal course change");
      first_dc->m_is_twa = false;
      if (last_dc != first_dc)
        last_dc = m_dcs.erase(last_dc);
      else
        last_dc = dc;
      dc = last_dc;
      ++dc;
    } else if (diff_twa < 1.0 && (first_dc->m_is_twa || first_dc == last_dc)) {
      wxLogMessage("Continuing current leg because of minimal twa change");
      first_dc->m_is_twa = true;
      if (last_dc != first_dc)
        last_dc = m_dcs.erase(last_dc);
      else
        last_dc = dc;
      dc = last_dc;
      ++dc;
    } else {
      // Calculate new course / TWA
      if (first_dc != last_dc) {
        first_dc->m_twa =
            0.5 *
            (first_dc->m_twa +
             last_dc
                 ->m_twa);  // TODO calculate exact twa that will bring us from
                            // start to end waypoint when course is finalized
        double new_dist;
        DistanceBearingMercator_Plugin(
            last_dc->m_lat_start, last_dc->m_lon_start, first_dc->m_lat_start,
            first_dc->m_lon_start, &first_dc->m_course, &new_dist);
        wxLogMessage("Wrote DC: %3.3f, TWA %3.3f", first_dc->m_course,
                     first_dc->m_twa);
        first_dc = last_dc;
        // Note: dc must not be incremented, leg between first_dc and dc has not
        // been investigated yet
      } else {
        wxLogMessage("Wrote DC: %3.3f, TWA %3.3f", first_dc->m_course,
                     first_dc->m_twa);
        first_dc = dc;
        last_dc = dc;
        ++dc;
      }
    }
  }
}

namespace {
// Course change required to reach 93% performance is ca. 100.3 degrees
// Add 4 seconds of performance recovery at 5kn
// TODO Make that precise in calculation below
// TODO Give safety margin for weather beyond next forecast
static constexpr double max_recovery = 4.0 * 3.0 / (20.0 * 5.0) / 100.0;
static constexpr double course_change_for_max_loss =
    (0.07 + max_recovery) * 180.0 / M_PI * 25.0;
}  // namespace

void Race::OptimizeManeuvers() {
  // TODO If this is called twice on the same list, it will create duplicate
  // TWAs
  // Note: This assumes a symmetric polar throughout
  // Note: A course change of exactly 180 degrees will be treated as a tack
  // (not sure what SOL does)
  // TODO Take into account that performance may not be 100% to start with
  double first_twa = m_dcs.begin()->m_twa;
  auto second_dc = m_dcs.begin();
  ++second_dc;

  for (auto p_dc = second_dc; p_dc != m_dcs.end(); ++p_dc) {
    double next_twa = p_dc->m_twa;
    double sign = first_twa > 0.0 ? 1.0 : -1.0;
    std::cout << "      DC " << p_dc->m_course
              << ": Checking course change TWA=" << first_twa << " to "
              << next_twa << std::endl;

    if (first_twa * next_twa > 0) {
      // Course change. Performance loss is ca. 0.07% per degree
      // TODO Optimize by calculating performance recovery to reach exactly 93%
      if (std::fabs(next_twa - first_twa) > course_change_for_max_loss) {
        // Note: std::list::emplace() does not invalidate any iterators
        // Note: Two seconds difference is required to preserve the order of the
        // DCs
        m_dcs.emplace(
            p_dc,
            Dc{wxDateTime(p_dc->m_timestamp).Subtract(wxTimeSpan::Seconds(2)),
               -1.0, -1.0, first_twa + sign * course_change_for_max_loss,
               true});
        // ... and the existing dc finalizes the course change to next_twa
      }
    } else if (std::fabs(first_twa - next_twa) > 180.0) {
      // Jibe
      /* The performance loss for a jibe is half the boat speed after the jibe,
       * in percent. The performance loss for a course change (without jibe) is
       * ca. 0.07% per degree There are two possible strategies:
       * 1. Drive performance just below the 93% limit by two course changes.
       *    Then jibe to next_twa without further performance loss.
       *    This only makes sense if boat speed after the jibe is greater than
       * 14 knots, because in this case the jibe performance loss would be
       * greater than 7%
       * 2. Jibe to TWA 180 degrees, then harden in to next_twa
       *    This only makes sense if the added performance loss for jibe and
       * course change is less than the performance loss for directly jibing to
       * next_twa
       */
      double stw_before_wind = GetSpeedThroughWater(p_dc->m_tws, 180.0);
      double next_stw = GetSpeedThroughWater(p_dc->m_tws, next_twa);
      std::cout << "Optimizing jibe, speed twa 180=" << stw_before_wind
                << ", speed after jibe=" << next_stw << std::endl;

      if (next_stw > 14.0) {
        // Strategy 1
        // TODO Optimize by calculating performance recovery to reach exactly
        // 93%
        double twa_delta = 180.0 - std::fabs(first_twa);
        // Change course upwind (delta1), then downwind to 180 degrees (delta2):
        // twa_delta + 2 * delta1 = course_change_for_max_loss
        double delta1 = 0.5 * (course_change_for_max_loss - twa_delta);
        // TODO Take into account loss in VMG due to the course changes and
        // their duration Insert two course changes, filling only required
        // fields
        m_dcs.emplace(
            p_dc,
            Dc{wxDateTime(p_dc->m_timestamp).Subtract(wxTimeSpan::Seconds(4)),
               -1.0, -1.0, first_twa - sign * delta1, true});
        m_dcs.emplace(
            p_dc,
            Dc{wxDateTime(p_dc->m_timestamp).Subtract(wxTimeSpan::Seconds(2)),
               -1.0, -1.0, sign * 180.0, true});
        // ... and the existing dc finalizes the course change to next_twa
      } else {
        // Strategy 2
        // Performance loss for direct jibe to next_twa
        double loss = get_performance_loss_tack_jibe(next_stw);
        // Performance loss for jibe to 180 degrees, then course change to
        // next_twa
        double loss1 = get_performance_loss_tack_jibe(stw_before_wind);
        double loss2 =
            get_performance_loss_course_change(180.0, std::fabs(next_twa));
        if (loss1 + loss2 < loss) {
          // TODO optimize this by jibing to some course between 180 degrees and
          // next_twa
          // TODO Take into account loss in VMG due to the course changes and
          // their duration
          m_dcs.emplace(
              p_dc,
              Dc{wxDateTime(p_dc->m_timestamp).Subtract(wxTimeSpan::Seconds(2)),
                 -1.0, -1.0, sign * 180.0, true});
          // ... and the existing dc finalizes the course change to next_twa
        }
      }
    } else {
      // Tack
      /* The performance loss for a tack is half the boat speed after the tack,
       * in percent. The performance loss for a course change (without tack) is
       * ca. 0.07% per degree Strategy: Tack to 0° (zero boat speed) with no
       * performance loss Change course to next_twa
       */
      double next_speed = GetSpeedThroughWater(p_dc->m_tws, next_twa);
      // Performance loss for direct tack to next_twa
      double loss1 = get_performance_loss_tack_jibe(next_speed);
      // Performance loss for course change from 0° to next_twa
      double loss2 = get_performance_loss_course_change(0.0, next_twa);
      std::cout << "Optimizing tack, speed after tack=" << next_speed
                << ", loss1=" << loss1 << ", loss2=" << loss2 << std::endl;
      if (loss1 > loss2) {
        m_dcs.emplace(
            p_dc,
            Dc{wxDateTime(p_dc->m_timestamp).Subtract(wxTimeSpan::Seconds(2)),
               -1.0, -1.0, -sign * 0.001, true});
        // ... and the existing dc finalizes the course change to next_twa
      }
    }

    first_twa = next_twa;
  }
}

void Race::EnrichDcs() {
  auto previous_dc = m_dcs.begin();

  for (auto dc = m_dcs.begin(); dc != m_dcs.end(); ++dc) {
    // Calculate extra values
    if (dc->m_lat_start == -1.0 && dc != m_dcs.begin()) {
      // DC course change optimization doesn't fill these fields
      double dist =
          previous_dc->m_stw *
          (dc->m_timestamp - previous_dc->m_timestamp).GetSeconds().ToDouble() /
          3600.0;
      PositionBearingDistanceMercator_Plugin(
          previous_dc->m_lat_start, previous_dc->m_lon_start,
          previous_dc->m_course, dist, &dc->m_lat_start, &dc->m_lon_start);
    }

    double twd;
    std::tie(dc->m_tws, twd) =
        GetWindData(dc->m_timestamp, dc->m_lat_start, dc->m_lon_start);
    if (dc->m_is_twa) {
      dc->m_course = twd - dc->m_twa;
      if (dc->m_course > 360.0)
        dc->m_course -= 360.0;
      else if (dc->m_course < 0.0)
        dc->m_course += 360.0;
    } else {
      // Get TWA from course
      dc->m_twa = NAN;
      if (dc->m_tws >= 0.0) {
        dc->m_twa = twd - dc->m_course;  // positive sign: starboard tack
        if (dc->m_twa < -180.0)
          dc->m_twa += 360.0;
        else if (dc->m_twa > 180.0)
          dc->m_twa -= 360.0;
      }
    }

    dc->m_stw = GetSpeedThroughWater(dc->m_tws, dc->m_twa);
    auto [max_up, max_down] = GetBoatOptimalAngles(dc->m_tws);
    if (max_up > 180.0) max_up = 360.0 - max_up;
    if (max_down > 180.0) max_down = 360.0 - max_down;
    double sign = (dc->m_twa > 0 ? 1.0 : -1.0);
    dc->m_opt_upwind = max_up * sign;
    dc->m_opt_downwind = max_down * sign;
    // Performance right after the course change
    // TODO Get parent heading at begin of DC from WR
    dc->m_perf_begin = (previous_dc->m_twa == 0.0)
                           ? 1.0
                           : get_performance(previous_dc->m_perf_end, dc->m_stw,
                                             previous_dc->m_twa, dc->m_twa);
    auto next_dc = dc;
    ++next_dc;
    dc->m_perf_end = (next_dc == m_dcs.end())
                         ? 1.0
                         : get_recovery(dc->m_perf_begin,
                                        (next_dc->m_timestamp - dc->m_timestamp)
                                            .GetSeconds()
                                            .ToDouble(),
                                        dc->m_stw);

    previous_dc = dc;
  }
}

void Race::MakeTrack() const {
  if (m_dcs.empty()) return;

  PlugIn_Track track;
  // TODO Put timestamp of the route calculation here
  struct tm* timeinfo(gmtime(nullptr));  // Current UTC timestamp
  track.m_NameString = "SOL " + m_id + std::asctime(timeinfo);
  track.m_StartString = "Start";
  track.m_EndString = "End";
  track.m_GUID = GetNewGUID();

  double current_lat = m_dcs.front().m_lat_start;
  double current_lon = m_dcs.front().m_lon_start;
  double performance = 1.0;
  double previous_twa =
      m_dcs.front().m_twa;  // TODO Get parent heading at begin of DC from WR

  // Recalculate the track from the dcs as precisely as possible
  for (auto dc = m_dcs.begin(); dc != m_dcs.end(); ++dc) {
    // Note that pWaypointList stores pointers only, and does not manage their
    // memory
    PlugIn_Waypoint* pwaypoint = new PlugIn_Waypoint(
        current_lat, current_lon, "dot", _("SOL route point"), wxEmptyString);
    pwaypoint->m_CreateTime = dc->m_timestamp;
    track.pWaypointList->Append(pwaypoint);

    auto [tws, twd] = GetWindData(dc->m_timestamp, current_lat, current_lon);
    double twa;
    double course;
    if (dc->m_is_twa) {
      twa = dc->m_twa;
      course = twd - twa;
    } else {
      twa = twd - dc->m_course;  // positive sign: starboard tack
      course = dc->m_course;
    }
    if (twa < -180.0)
      twa += 360.0;
    else if (twa > 180.0)
      twa -= 360.0;
    double theoretical_stw = GetSpeedThroughWater(tws, twa);

    // Performance loss for initial course change of the Dc
    performance =
        get_performance(performance, theoretical_stw, previous_twa, twa);

    auto next_dc = dc;
    ++next_dc;
    double time_seconds =
        (next_dc != m_dcs.end())
            ? (next_dc->m_timestamp - dc->m_timestamp).GetSeconds().ToDouble()
            : 3600.0;  // Go on for one more hour after last Dc

    // Note: Waypoints are only created at DC timestamps, not at every jump
    double jump = std::min(time_seconds, 30.0);
    double current_time;
    double current_stw = theoretical_stw * performance;
    double total_dist = 0.0;

    for (current_time = jump; current_time <= time_seconds;
         current_time += jump) {
      // TODO TODO New wind and theoretical speed!

      // The loop always calculates the performance and distance at current_time
      if (performance < 1.0) {
        performance = get_recovery_step(performance, jump, current_stw);
        current_stw = theoretical_stw * performance;
      }

      // TODO It is not clear whether dist is calculated with old or new
      // performance
      double dist = current_stw * jump / 3600.0;
      if (dc->m_is_twa)
        PositionBearingDistanceMercator_Plugin(
            current_lat, current_lon, course, dist, &current_lat, &current_lon);

      total_dist += dist;
    }

    // Remaining fractional jump
    double remainder = time_seconds - current_time;
    if (remainder > 0.0) {
      performance =
          std::min(1.0, get_recovery_step(performance, remainder, current_stw));
      double dist = theoretical_stw * performance * remainder / 3600.0;
      if (dc->m_is_twa)
        PositionBearingDistanceMercator_Plugin(
            current_lat, current_lon, course, dist, &current_lat, &current_lon);

      total_dist += dist;
    }

    if (!dc->m_is_twa)
      PositionBearingDistanceMercator_Plugin(current_lat, current_lon,
                                             dc->m_course, total_dist,
                                             &current_lat, &current_lon);
  }

  AddPlugInTrack(&track);  // Note: Contents are copied
  // The destructor does not do this
  track.pWaypointList->DeleteContents(true);
  track.pWaypointList->Clear();
}
