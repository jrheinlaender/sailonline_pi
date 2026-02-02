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
  wxFileName download_target = m_sailonline_pi.GetDataDir();
  download_target.SetFullName("racelist.xml");
  wxLogMessage("Downloading racelist to %s", download_target.GetFullPath());

  // Download racelist
  Connect(wxEVT_DOWNLOAD_EVENT,
          (wxObjectEventFunction)(wxEventFunction)&Sailonline::OnDownloadEvent);

  m_connected = true;
  m_downloading = true;
  m_download_success = true;
  m_download_handle = 0;
  if (!(OCPN_downloadFileBackground(SolApi::kUrlRacelist,
                                    download_target.GetFullPath(), this,
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
  auto status = racelist_doc.load_file(download_target.GetFullPath().mb_str());
  if (!status) {
    wxLogError("Could not parse racelist file: %s", status.description());
    return;
  }

  // Parse xml and fill list of races
  m_races.clear();
  pugi::xml_node node_races = racelist_doc.child("races");

  for (pugi::xml_node node_race = node_races.first_child();
       node_race != nullptr; node_race = node_race.next_sibling()) {
    if (strcmp(node_race.name(), "race") == 0) {
      Race race(m_sailonline_pi);

      for (pugi::xml_node node_race_child = node_race.first_child();
           node_race_child != nullptr;
           node_race_child = node_race_child.next_sibling()) {
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
