/***************************************************************************
 *   Copyright (C) 2025 by Jan Rheinländer                                 *
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

#include <wx/wx.h>

#include <boost/algorithm/string.hpp>

#include <pugixml.hpp>

#include "sailonline_pi.h"
#include "Sailonline.h"
#include "Race.h"
#include "SolApi.h"

Sailonline::Sailonline(sailonline_pi& plugin) : m_sailonline_pi(plugin) {
  wxLogMessage("Initializing Sailonline");

  // Check if we are online
  if (!OCPN_isOnline()) {
    m_errors.emplace_back("No internet access");
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
  if (!(OCPN_downloadFileBackground(SolApi::kUrlRacelist, download_target, this,
                                    &m_download_handle) == OCPN_DL_STARTED)) {
    m_errors.emplace_back("Failed to initiate download of racelist " +
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
    m_errors.emplace_back("Failed to download racelist " +
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
      Race race(m_sailonline_pi);

      for (pugi::xml_node node_race_child = node_race.first_child();
           node_race_child; node_race_child = node_race_child.next_sibling()) {
        if (strcmp(node_race_child.name(), "id") == 0) {
          race.m_id = node_race_child.first_child().value();
          boost::trim(race.m_id);
        } else if (strcmp(node_race_child.name(), "name") == 0) {
          race.m_name = node_race_child.first_child().value();
          boost::trim(race.m_name);
        } else if (strcmp(node_race_child.name(), "description") == 0) {
          race.m_description = node_race_child.first_child().value();
          boost::trim(race.m_description);
        } else if (strcmp(node_race_child.name(), "message") == 0) {
          race.m_message = node_race_child.first_child().value();
          boost::trim(race.m_message);
        } else if (strcmp(node_race_child.name(), "start_time") == 0) {
          race.m_start = node_race_child.first_child().value();
          boost::trim(race.m_start);
        } else if (strcmp(node_race_child.name(), "url") == 0) {
          race.m_url = node_race_child.first_child().value();
          boost::trim(race.m_url);
        }
      }

      m_races.emplace(race.m_id, std::move(race));
    }
  }
}

Sailonline::~Sailonline() {
  CleanupDownload();

}

std::unique_ptr<Race> Sailonline::GetRace(const std::string& racenumber) const {
  auto prace = m_races.find(racenumber);
  if (prace == m_races.end()) return nullptr;

  return std::make_unique<Race>(prace->second);
}

std::vector<std::string> Sailonline::GetErrors() {
  std::vector<std::string> result;
  std::swap(m_errors, result);
  return result;
}

void Sailonline::OnRaceSelected(wxListEvent& event) {


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
    //
http://sailonline.org/site_media/weather/xml/weather_122_global_gfs_20251221_1624.xml
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
