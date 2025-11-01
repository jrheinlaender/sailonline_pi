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

#include <wx-3.2/wx/event.h>
#include <wx/wx.h>
#include <wx/window.h>

#include <ocpn_plugin.h>
#include <pugixml.hpp>

#include "sailonline_pi.h"
#include "Sailonline.h"
#include "SolApi.h"

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
  m_ppanel->m_pracelist->ClearAll();
  m_ppanel->m_pracelist->InsertColumn(0, _("Number"));
  m_ppanel->m_pracelist->InsertColumn(1, _("Name"));

  // Fill racelist
  // Build target filename
  wxString download_target = GetpPrivateApplicationDataLocation()
                                 ->Append(wxFileName::GetPathSeparator())
                                 .Append("plugins/sailonline_pi/racelist");
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
  m_ppanel->m_pracelist->SetItemState(m_ppanel->m_pracelist->GetTopItem(),
                                      wxLIST_STATE_SELECTED,
                                      wxLIST_STATE_SELECTED);

}

Sailonline::~Sailonline() {
  CleanupDownload();

  m_ppanel->m_pracelist->Disconnect(
      wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(Sailonline::OnRaceSelected),
      nullptr, this);
  wxFileConfig* pconf = m_sailonline_pi.GetConf();

  wxPoint p = GetPosition();
  pconf->Write("DialogX", p.x);
  pconf->Write("DialogY", p.y);
bool Sailonline::Show(bool show) {
  if (!m_init_errors.empty()) {
    // TODO show dialog
    return false;
  }

  return SailonlineBase::Show(show);
}

void Sailonline::OnRaceSelected(wxListEvent& event) {
  // Get race number
  long idx = event.GetIndex();
  wxString racenumber = m_ppanel->m_pracelist->GetItemText(idx, 0);

  // Fill first tab with information about the race
  m_ppanel->SetLabel(racenumber);

  for (const auto& race : m_races) {
    if (race.m_id == racenumber) {
      m_ppanel->m_racename->SetLabel(
          race.m_name);  // TODO Show only if race has not started yet.
                         // Otherwise message is enough
      m_ppanel->m_racemsg->SetLabel(race.m_message);
      m_ppanel->m_racedesc->SetPage(race.m_description);
      m_ppanel->m_racedesc->SetSize(m_ppanel->m_racedata->GetClientSize());

      break;
    }
    // TODO Clear panel if nothing is found?
  }

  // pconf->Write("DialogWidth", m_size.x);
  // pconf->Write("DialogHeight", m_size.y);
  // pconf->Write("DialogSplit", m_panel->m_splitter1->GetSashPosition());
}

void Sailonline::OnDcDownload(wxCommandEvent& event) {}
void Sailonline::OnDcUpload(wxCommandEvent& event) {}

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
