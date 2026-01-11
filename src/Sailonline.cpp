/***************************************************************************
 *   Copyright (C) 2025 by Jan Rheinl�nder                                 *
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

#include <wx-3.2/wx/event.h>
#include <wx/wx.h>
#include <wx/window.h>
#include <wx/clipbrd.h>
#include <wx/datetime.h>

#include <sstream>
#include <curl/curl.h>

#include <ocpn_plugin.h>
#include <pugixml.hpp>

#include "sailonline_pi.h"
#include "Sailonline.h"
#include "SolApi.h"
#include "FromTrackDialog.h"

namespace {

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

double get_performance(const double performance, const double stw,
                       const double first_twa, const double next_twa) {
  if (performance < 0.93) return performance;

  if (first_twa * next_twa > 0) {
    // Course change
    return performance -
           get_performance_loss_course_change(first_twa, next_twa);
  } else {
    // Tack or jibe
    return performance - get_performance_loss_tack_jibe(stw);
  }
}

double get_recovery(const double performance, const double time_seconds,
                    const double theoretical_stw) {
  if (performance >= 1.0) return 1.0;

  double jump =
      std::min(time_seconds, 20.0);  // TODO Find correct value for jump
  double current_stw = theoretical_stw * performance;
  double newperformance = performance;
  double current_time;

  for (current_time = jump; current_time <= time_seconds;
       current_time += jump) {
    newperformance = newperformance + jump * 3.0 / (20.0 * current_stw) / 100.0;
    if (newperformance >= 1.0) return 1.0;

    current_stw = theoretical_stw * newperformance;
  }

  // Remaining fractional jump
  double remainder = time_seconds - current_time;
  if (remainder > 0.0)
    return std::min(
        1.0, newperformance + remainder * 3.0 / (20.0 * current_stw) / 100.0);

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

Sailonline::Sailonline(wxWindow* parent, sailonline_pi& plugin)
    : SailonlineBase(parent), m_sailonline_pi(plugin), m_ppanel(nullptr) {
  wxLogMessage("Initializing Sailonline GUI");
  // Initialize config
  int conf_version;
  m_sailonline_pi.GetConf()->Read("ConfigVersion", &conf_version, 0);

  // Set up dialog window
  wxBoxSizer* psizer;
  psizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(psizer);
  m_ppanel = new SailonlinePanel(this);
  psizer->Add(m_ppanel, 1, wxEXPAND, 0);
  psizer->SetSizeHints(this);

  // Initialize size with old values from last use
  wxFileConfig* pconf = m_sailonline_pi.GetConf();
  wxRect rect = GetRect();
  int sashpos;
  pconf->Read("DialogX", &rect.x, rect.x);
  pconf->Read("DialogY", &rect.y, rect.y);
  pconf->Read("DialogWidth", &rect.width, 800);
  pconf->Read("DialogHeight", &rect.height, 450);
  pconf->Read("DialogSplit", &sashpos, rect.width / 4);
  SetPosition(rect.GetPosition());
  SetInitialSize(rect.GetSize());
  m_ppanel->m_psplitter->SetSashPosition(sashpos, true);

  m_ppanel->m_pracelist->ClearAll();
  m_ppanel->m_pracelist->InsertColumn(0, _("Number"));
  m_ppanel->m_pracelist->InsertColumn(1, _("Name"));

  // Check if we are online
  if (!OCPN_isOnline()) {
    m_init_errors.emplace_back("No internet access");
    return;
  }

  // Fill racelist
  // Build target filename
  wxString download_target = GetPluginDataDir("sailonline_pi")
                                 .Append(wxFileName::GetPathSeparator())
                                 .Append("data");
  if (!wxDirExists(download_target)) wxMkdir(download_target);
  download_target.Append(wxFileName::GetPathSeparator()).Append("racelist.xml");
  wxLogMessage("Downloading racelist to %s", download_target);

  // Download racelist
  Connect(wxEVT_DOWNLOAD_EVENT,
          (wxObjectEventFunction)(wxEventFunction)&Sailonline::OnDownloadEvent);

  m_connected = true;
  m_downloading = true;
  m_download_success = true;
  m_download_handle = 0;
  m_init_errors.clear();
  if (!(OCPN_downloadFileBackground(SolApi::kUrlRacelist, download_target, this,
                                    &m_download_handle) == OCPN_DL_STARTED)) {
    m_init_errors.emplace_back("Failed to initiate download of racelist " +
                               SolApi::kUrlRacelist);
    return;
  }

  while (m_downloading) {
    wxTheApp->ProcessPendingEvents();
    wxLogMessage("Waiting for download...");
    wxYield();
    wxMilliSleep(1000);
  }
  if (!m_download_success) {
    m_init_errors.emplace_back("Failed to download racelist " +
                               SolApi::kUrlRacelist);
    return;
  }

  // Load racelist into xml parser
  pugi::xml_document racelist_doc;
  auto status = racelist_doc.load_file(download_target.mb_str());
  if (!status) {
    wxLogError("Could not parse racelist file: %s", status.description());
    return;
  }

  // Parse xml and fill list of races
  m_races.clear();
  pugi::xml_node node_races = racelist_doc.child("races");

  for (pugi::xml_node node_race = node_races.first_child(); node_race;
       node_race = node_race.next_sibling()) {
    if (strcmp(node_race.name(), "race") == 0) {
      Race race;

      for (pugi::xml_node node_race_child = node_race.first_child();
           node_race_child; node_race_child = node_race_child.next_sibling()) {
        if (strcmp(node_race_child.name(), "id") == 0) {
          race.m_id = wxString(node_race_child.first_child().value()).Trim();
        } else if (strcmp(node_race_child.name(), "name") == 0) {
          race.m_name = wxString(node_race_child.first_child().value()).Trim();
        } else if (strcmp(node_race_child.name(), "description") == 0) {
          race.m_description =
              wxString(node_race_child.first_child().value()).Trim();
        } else if (strcmp(node_race_child.name(), "message") == 0) {
          race.m_message =
              wxString(node_race_child.first_child().value()).Trim();
        } else if (strcmp(node_race_child.name(), "start_time") == 0) {
          race.m_start = wxString(node_race_child.first_child().value()).Trim();
        } else if (strcmp(node_race_child.name(), "url") == 0) {
          race.m_url = wxString(node_race_child.first_child().value()).Trim();
        }
      }

      wxListItem item;
      long index = m_ppanel->m_pracelist->InsertItem(
          m_ppanel->m_pracelist->GetItemCount(), item);
      m_ppanel->m_pracelist->SetItem(index, 0, race.m_id);
      m_ppanel->m_pracelist->SetItem(index, 1, race.m_name);

      m_races.emplace_back(std::move(race));
    }
  }

  // Finish
  m_ppanel->m_pracelist->SetColumnWidth(0, wxLIST_AUTOSIZE);
  m_ppanel->m_pracelist->SetColumnWidth(1, wxLIST_AUTOSIZE);
  m_ppanel->m_pracelist->Connect(wxEVT_LIST_ITEM_SELECTED,
                                 wxListEventHandler(Sailonline::OnRaceSelected),
                                 nullptr, this);

  m_prace = m_races.begin();
  if (!m_races.empty())
    m_ppanel->m_pracelist->SetItemState(m_ppanel->m_pracelist->GetTopItem(),
                                        wxLIST_STATE_SELECTED,
                                        wxLIST_STATE_SELECTED);

  m_ppanel->m_notebook->SetSelection(0);  // Show first tab

  m_ppanel->m_pdclist->ClearAll();
  m_ppanel->m_pdclist->InsertColumn(0, _("Time"));
  m_ppanel->m_pdclist->InsertColumn(1, _("Type"));
  m_ppanel->m_pdclist->InsertColumn(2, _("Course"));
  m_ppanel->m_pdclist->InsertColumn(3, _("TWA"));
  m_ppanel->m_pdclist->InsertColumn(4, _("Speed"));
  m_ppanel->m_pdclist->InsertColumn(5, _("Opt"));
  m_ppanel->m_pdclist->InsertColumn(6, _("Perf1"));
  m_ppanel->m_pdclist->InsertColumn(7, _("Perf2"));

  m_ppanel->m_pbutton_download->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnDcDownload), nullptr, this);
  m_ppanel->m_pbutton_upload->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnDcUpload), nullptr, this);
  m_ppanel->m_pbutton_fromtrack->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnDcFromTrack), nullptr, this);
  m_ppanel->m_pbutton_modify->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnDcModify), nullptr, this);
  m_ppanel->m_pbutton_copydcs->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnCopyDcs), nullptr, this);
}

Sailonline::~Sailonline() {
  CleanupDownload();

  m_ppanel->m_pracelist->Disconnect(
      wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(Sailonline::OnRaceSelected),
      nullptr, this);
  m_ppanel->m_pbutton_download->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnDcDownload), nullptr, this);
  m_ppanel->m_pbutton_upload->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnDcUpload), nullptr, this);
  m_ppanel->m_pbutton_fromtrack->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnDcFromTrack), nullptr, this);
  m_ppanel->m_pbutton_modify->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnDcModify), nullptr, this);
  m_ppanel->m_pbutton_copydcs->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(Sailonline::OnCopyDcs), nullptr, this);

  wxFileConfig* pconf = m_sailonline_pi.GetConf();

  wxRect rect = GetRect();
  pconf->Write("DialogX", rect.x);
  pconf->Write("DialogY", rect.y);
  pconf->Write("DialogWidth", rect.width);
  pconf->Write("DialogHeight", rect.height);
  pconf->Write("DialogSplit", m_ppanel->m_psplitter->GetSashPosition());
  pconf->Flush();
bool Sailonline::Show(bool show) {
  if (!m_init_errors.empty()) {
    // TODO show dialog
    return false;
  }

  return SailonlineBase::Show(show);
}

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

void Sailonline::OnRaceSelected(wxListEvent& event) {
  // Get race number
  long idx = event.GetIndex();
  wxString racenumber = m_ppanel->m_pracelist->GetItemText(idx, 0);
  if (racenumber.IsEmpty()) return;

  // Fill first tab with information about the race
  m_ppanel->SetLabel(racenumber);

  for (auto race_it = m_races.begin(); race_it != m_races.end(); ++race_it) {
    if (race_it->m_id == racenumber) {
      m_ppanel->m_racename->SetLabel(
          race_it->m_name);  // TODO Show only if race has not started yet.
                             // Otherwise message is enough
      m_ppanel->m_racemsg->SetLabel(race_it->m_message);
      m_ppanel->m_racedesc->SetPage(race_it->m_description);
      m_ppanel->m_racedesc->SetSize(m_ppanel->m_racedata->GetClientSize());

      m_prace = race_it;
      break;
    }
    // TODO Clear panel if nothing is found?
  }

  // Log in to sailonline.org to get more specific data
  // Note: wxWebRequest stores cookies in the wxWebSession but they are not
  // accessible Note: OCPN_postDataHttp() does not handle cookies

  CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl_easy_init();

  if (curl != nullptr) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "https://sailonline.org/community/accounts/login/");
    curl_easy_setopt(
        curl, CURLOPT_USERAGENT,
        "libcurl-agent/1.0");  // Some servers don't like requests without
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");  // Enable cookie engine
    // Setup function to catch the page contents. This also suppresses output on
    // stdout
    std::string pagedata;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&pagedata);
    result = curl_easy_perform(curl);
    std::string csrftoken = curl_extract_cookie(curl, "csrftoken");
    // Note that URL remains the same
    curl_easy_setopt(curl, CURLOPT_REFERER,
                     "https://sailonline.org/community/accounts/login/"
                     "?password=21shukur%3AGozorI21&username=Ibis");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,
                     1L);  // redirects to https://sailonline.org/windy/run/xxxx
    std::string postdata =
        std::string("next=%2Fwindy%2Frun%2F") + racenumber.ToStdString() +
        "%2F&password=21shukur%3AGozorI21&username=Ibis&csrfmiddlewaretoken=" +
        csrftoken;
    wxLogMessage("POST '%s'", postdata);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
    pagedata.clear();
    result = curl_easy_perform(curl);

    size_t pos = pagedata.find("function getToken()\n{\n\treturn \"");
    std::string racetoken("");
    if (pos == std::string::npos) {
      wxLogError("Could not find token for race %s, did you register?",
                 racenumber);
      curl_easy_cleanup(curl);
      return;
    } else
      racetoken = pagedata.substr(pos + 31, 32);
    wxLogMessage("Found token '%s' for race %s", racetoken, racenumber);
    // Download detailed race data (XML format)
    std::string race_url =
        std::string("https://www.sailonline.org/webclient/auth_raceinfo_") +
        racenumber.ToStdString() + ".xml?token=" + racetoken;
    wxLogMessage("Downloading race xml data from %s", race_url);
    curl_easy_setopt(curl, CURLOPT_URL, race_url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET,
                     1L);  // Otherwise curl will try to POST

    pagedata.clear();
    result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    pugi::xml_document race_doc;
    auto status = race_doc.load_string(pagedata.c_str());
    if (!status) {
      wxLogError("Could not parse race file: %s", status.description());
      return;
    }

    // Get boat polar
    // TWS: Space-separated list of true wind speeds (columns, integer, knots)
    // TWA: Space-separated list of true wind angles (rows, integer, degrees)
    // BS: Semicolon-separated list of space-separated lists of wind speeds
    // (float, m/s)
    pugi::xml_node node_name =
        race_doc.select_node("/race/boat/vpp/name").node();
    pugi::xml_node node_tws =
        race_doc.select_node("/race/boat/vpp/tws_splined").node();
    pugi::xml_node node_twa =
        race_doc.select_node("/race/boat/vpp/twa_splined").node();
    pugi::xml_node node_bs =
        race_doc.select_node("/race/boat/vpp/bs_splined").node();
    std::string polar_name(node_name.first_child().value());
    std::replace(polar_name.begin(), polar_name.end(), ' ', '_');
    wxString download_target = GetPluginDataDir("sailonline_pi")
                                   .Append(wxFileName::GetPathSeparator())
                                   .Append("data")
                                   .Append(wxFileName::GetPathSeparator())
                                   .Append("Polar");
    if (!wxDirExists(download_target)) wxMkdir(download_target);
    download_target.Append(wxFileName::GetPathSeparator())
        .Append("SOL_")
        .Append(polar_name.c_str())
        .Append("_polar.csv");
    wxLogMessage("Writing boat polar to %s", download_target);
    wxFile polar_file(download_target, wxFile::write);
    if (polar_file.Error()) return;  // TODO Message

    polar_file.Write("twa/tws;");
    std::stringstream tws_stream(node_tws.first_child().value());
    unsigned tws;
    while (tws_stream >> tws) {
      if (tws_stream.bad()) return;  // TODO Message
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
      if (!std::getline(bss_stream, bs_line, ';')) return;  // TODO Message

      polar_file.Write(twa);
      polar_file.Write(";");
      std::replace(bs_line.begin(), bs_line.end(), ' ', ';');
      polar_file.Write(bs_line);
      polar_file.Write("\n");
    }

    polar_file.Close();
  }

}

void Sailonline::OnDcDownload(wxCommandEvent& event) {}
void Sailonline::OnDcUpload(wxCommandEvent& event) {}

std::pair<double, double> Sailonline::GetWindData(const wxDateTime& t,
                                                  double lat, double lon) {
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

double Sailonline::GetSpeedThroughWater(double tws, double twa) {
  Json::Value v;
  Json::FastWriter writer;

  v["Source"] = "SAILONLINE_PI";
  v["Type"] = "Request";
  v["Msg"] = "WR_BOATDATA_REQUEST";
  v["Data"] = "Speed";
  v["Racenumber"] = m_prace->m_id.ToStdString().c_str();
  v["tws"] = tws;
  v["twa"] = twa;

  SendPluginMessage("WR_BOATDATA_REQUEST", writer.write(v));
  Json::Value reply = m_sailonline_pi.GetJsonMessage();
  if (reply != Json::nullValue && reply.isMember("BOAT SPEED"))
    return reply["BOAT SPEED"].asDouble();

  return -1.0;
}

std::pair<double, double> Sailonline::GetBoatOptimalAngles(double tws) {
  Json::Value v;
  Json::FastWriter writer;

  v["Source"] = "SAILONLINE_PI";
  v["Type"] = "Request";
  v["Msg"] = "WR_BOATDATA_REQUEST";
  v["Data"] = "Angles";
  v["Racenumber"] = m_prace->m_id.ToStdString().c_str();
  v["tws"] = tws;

  SendPluginMessage("WR_BOATDATA_REQUEST", writer.write(v));
  Json::Value reply = m_sailonline_pi.GetJsonMessage();
  if (reply != Json::nullValue && reply.isMember("OPT UP") &&
      reply.isMember("OPT DOWN"))
    return {reply["OPT UP"].asDouble(), reply["OPT DOWN"].asDouble()};

  return {-1.0, -1.0};
}

void Sailonline::FillDcList() {
  if (m_prace == m_races.end()) return;

  m_ppanel->m_pdclist->DeleteAllItems();

  auto previous_dc = m_prace->m_dcs.begin();

  for (auto dc = m_prace->m_dcs.begin(); dc != m_prace->m_dcs.end(); ++dc) {
    // Calculate extra values
    if (dc->m_lat_start == -1.0 && dc != m_prace->m_dcs.begin()) {
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
    dc->m_perf_begin = (previous_dc->m_twa == 0.0)
                           ? 1.0
                           : get_performance(previous_dc->m_perf_end, dc->m_stw,
                                             previous_dc->m_twa, dc->m_twa);
    auto next_dc = dc;
    ++next_dc;
    dc->m_perf_end = (next_dc == m_prace->m_dcs.end())
                         ? 1.0
                         : get_recovery(dc->m_perf_begin,
                                        (next_dc->m_timestamp - dc->m_timestamp)
                                            .GetSeconds()
                                            .ToDouble(),
                                        dc->m_stw);

    wxListItem item;
    long index = m_ppanel->m_pdclist->InsertItem(
        m_ppanel->m_pdclist->GetItemCount(), item);
    m_ppanel->m_pdclist->SetItem(index, 0,
                                 dc->m_timestamp.Format("%Y/%m/%d %H:%M:%S"));
    m_ppanel->m_pdclist->SetItem(index, 1, dc->m_is_twa ? "twa" : "cc");
    m_ppanel->m_pdclist->SetItem(index, 2,
                                 wxString::Format("%03.3f", dc->m_course));
    m_ppanel->m_pdclist->SetItem(index, 3,
                                 wxString::Format("%03.3f", dc->m_twa));
    m_ppanel->m_pdclist->SetItem(index, 4,
                                 wxString::Format("%03.3f", dc->m_stw));
    m_ppanel->m_pdclist->SetItem(
        index, 5,
        wxString::Format("%03.3f", std::fabs(dc->m_twa) < 90.0
                                       ? dc->m_opt_upwind
                                       : dc->m_opt_downwind));
    m_ppanel->m_pdclist->SetItem(
        index, 6, wxString::Format("%03.3f", dc->m_perf_begin * 100));
    m_ppanel->m_pdclist->SetItem(
        index, 7, wxString::Format("%03.3f", dc->m_perf_end * 100));

    previous_dc = dc;
  }

  for (int i = 0; i < m_ppanel->m_pdclist->GetColumnCount(); ++i)
    m_ppanel->m_pdclist->SetColumnWidth(i, wxLIST_AUTOSIZE);
}

void Sailonline::OnDcFromTrack(wxCommandEvent& event) {
  FromTrackDialog dlg(this);

  if (dlg.ShowModal() == wxID_OK) {
    wxString track_guid = dlg.GetSelectedTrack();
    auto ptrack = GetTrack_Plugin(track_guid);
    if (ptrack == nullptr) return;

    if (ptrack->pWaypointList->size() < 2) return;

    auto first_waypoint = ptrack->pWaypointList->begin();
    m_prace->m_dcs.clear();

    for (auto waypoint = first_waypoint;
         waypoint != ptrack->pWaypointList->end(); ++waypoint) {
      if (waypoint == first_waypoint) continue;

      double bearing, distance;
      auto wp = *waypoint;
      auto first_wp = *first_waypoint;
      DistanceBearingMercator_Plugin(wp->m_lat, wp->m_lon, first_wp->m_lat,
                                     first_wp->m_lon, &bearing, &distance);

      m_prace->m_dcs.emplace_back(Dc{first_wp->m_CreateTime, first_wp->m_lat,
                                     first_wp->m_lon, bearing, false});

      first_waypoint = waypoint;
    }

    FillDcList();
  }
}

// Course change required to reach 93% performance is ca. 100.3 degrees
// Add 4 seconds of performance recovery at 5kn
static constexpr double max_recovery = 4.0 * 3.0 / (20.0 * 5.0) / 100.0;
static constexpr double course_change_for_max_loss =
    (0.07 + max_recovery) * 180.0 / M_PI * 25.0;

void Sailonline::OnDcModify(wxCommandEvent& event) {
  if (m_prace->m_dcs.size() < 2) return;

  // Current leg is from first_dc to last_dc
  auto first_dc = m_prace->m_dcs.begin();  // First DC of a leg
  auto second_dc =
      first_dc;  // Required because std::list does not allow first_dc + 1
  ++second_dc;
  auto last_dc = m_prace->m_dcs.begin();  // Last DC that was investigated

  for (auto dc = second_dc; dc != m_prace->m_dcs.end();) {
    double diff_course = std::fabs(dc->m_course - first_dc->m_course);
    double diff_twa = std::fabs(dc->m_twa - first_dc->m_twa);
    if (diff_course > 360.0) diff_course -= 360.0;
    if (diff_twa > 360.0) diff_twa -= 360.0;

    // Check for minimal course or twa changes and delete unnecessary waypoints
    wxLogMessage(
        "First DC: %3.3f, TWA %3.3f, last DC: %3.3f, TWA %3.3f, this DC: "
        "%3.3f, TWA %3.3f",
        first_dc->m_course, first_dc->m_twa, last_dc->m_course, last_dc->m_twa,
        dc->m_course, dc->m_twa);

    if (diff_course < 2.0 && (!first_dc->m_is_twa || first_dc == last_dc)) {
      wxLogMessage("Continuing current leg because of minimal course change");
      first_dc->m_is_twa = false;
      if (last_dc != first_dc)
        last_dc = m_prace->m_dcs.erase(last_dc);
      else
        last_dc = dc;
      dc = last_dc;
      ++dc;
    } else if (diff_twa < 1.0 && (first_dc->m_is_twa || first_dc == last_dc)) {
      wxLogMessage("Continuing current leg because of minimal twa change");
      first_dc->m_is_twa = true;
      if (last_dc != first_dc)
        last_dc = m_prace->m_dcs.erase(last_dc);
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

  // Optimize maneuvers
  // Note: This assumes a symmetric polar throughout
  // Note: A course change of exactly 180 degrees will be treated as a tack
  // (not sure what SOL does)
  double first_twa = m_prace->m_dcs.begin()->m_twa;
  second_dc =
      m_prace->m_dcs
          .begin();  // Old second_dc might have become invalid through erase()
  ++second_dc;

  for (auto p_dc = second_dc; p_dc != m_prace->m_dcs.end(); ++p_dc) {
    double next_twa = p_dc->m_twa;
    double sign = first_twa > 0.0 ? 1.0 : -1.0;
    if (first_twa * next_twa > 0) {
      // Course change. Performance loss is ca. 0.07% per degree
      if (std::fabs(next_twa - first_twa) > course_change_for_max_loss) {
        // Note: std::list::emplace() does not invalidate any iterators
        // Note: Two seconds difference is required to preserve the order of the
        // DCs
        m_prace->m_dcs.emplace(
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

      if (next_stw > 14.0) {
        // Strategy 1
        double twa_delta = 180.0 - std::fabs(first_twa);
        // Change course upwind (delta1), then downwind to 180 degrees (delta2):
        // twa_delta + 2 * delta1 = course_change_for_max_loss
        double delta1 = 0.5 * (course_change_for_max_loss - twa_delta);
        m_prace->m_dcs.emplace(
            p_dc,
            Dc{wxDateTime(p_dc->m_timestamp).Subtract(wxTimeSpan::Seconds(4)),
               -1.0, -1.0, first_twa - sign * delta1, true});
        m_prace->m_dcs.emplace(
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
          m_prace->m_dcs.emplace(
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
      if (loss1 > loss2) {
        m_prace->m_dcs.emplace(
            p_dc,
            Dc{wxDateTime(p_dc->m_timestamp).Subtract(wxTimeSpan::Seconds(2)),
               -1.0, -1.0, -sign * 0.001, true});
        // ... and the existing dc finalizes the course change to next_twa
      }
    }

    first_twa = next_twa;
  }

  FillDcList();
}

void Sailonline::OnCopyDcs(wxCommandEvent& event) {
  wxString dc_list;

  for (const auto& dc : m_prace->m_dcs) {
    wxString timestamp = dc.m_timestamp.Format("%Y/%m/%d %H:%M:%S");
    wxString coursetype = (dc.m_is_twa ? "twa" : "cc");
    wxString course =
        wxString::Format("%03.3f", dc.m_is_twa ? dc.m_twa : dc.m_course);

    wxString line;
    line.Printf("%s %s %s %c", timestamp, coursetype, course, '\n');
    dc_list.Append(std::move(line));
  }

  if (wxTheClipboard->Open()) {
    wxTheClipboard->SetData(
        new wxTextDataObject(dc_list));  // Don't delete, clipboard holds data
    wxTheClipboard->Close();
  }
}

void Sailonline::CleanupDownload() {
  if (m_downloading) OCPN_cancelDownloadFileBackground(m_download_handle);

  if (m_connected)
    Disconnect(
        wxEVT_DOWNLOAD_EVENT,
        (wxObjectEventFunction)(wxEventFunction)&Sailonline::OnDownloadEvent);

  m_connected = false;
  m_downloading = false;
  m_download_handle = 0;
  // Note: m_download_success is set in OnDownloadEvent()
}

void Sailonline::OnDownloadEvent(OCPN_downloadEvent& ev) {
  switch (ev.getDLEventCondition()) {
    case OCPN_DL_EVENT_TYPE_END:
      m_download_success = (ev.getDLEventStatus() == OCPN_DL_NO_ERROR);
      CleanupDownload();
      wxYieldIfNeeded();
      break;

    default:
      break;
  }
}
