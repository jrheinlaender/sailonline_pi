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
#include <wx/clipbrd.h>

#include <ocpn_plugin.h>
#include <pugixml.hpp>

#include "sailonline_pi.h"
#include "Sailonline.h"
#include "SolApi.h"
#include "FromTrackDialog.h"

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
  pconf->Read("DialogWidth", &rect.width, wxMax(rect.width, 600));
  pconf->Read("DialogHeight", &rect.height, wxMax(rect.height, 450));
  pconf->Read("DialogSplit", &sashpos, rect.width / 4);
  SetPosition(rect.GetPosition());
  SetInitialSize(rect.GetSize());
  m_ppanel->m_psplitter->SetSashPosition(sashpos, true);

  m_ppanel->m_pracelist->ClearAll();
  m_ppanel->m_pracelist->InsertColumn(0, _("Number"));
  m_ppanel->m_pracelist->InsertColumn(1, _("Name"));

  // Fill racelist
  // Build target filename
  wxString download_target = GetPluginDataDir("sailonline_pi")
                                 .Append(wxFileName::GetPathSeparator())
                                 .Append("racelist");
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
  if (!m_races.empty())
    m_ppanel->m_pracelist->SetItemState(m_ppanel->m_pracelist->GetTopItem(),
                                        wxLIST_STATE_SELECTED,
                                        wxLIST_STATE_SELECTED);

  m_prace = m_races.begin();

  m_ppanel->m_pdclist->ClearAll();
  m_ppanel->m_pdclist->InsertColumn(0, _("Time"));
  m_ppanel->m_pdclist->InsertColumn(1, _("Type"));
  m_ppanel->m_pdclist->InsertColumn(2, _("Course"));
  m_ppanel->m_pdclist->SetColumnWidth(0, wxLIST_AUTOSIZE);
  m_ppanel->m_pdclist->SetColumnWidth(1, wxLIST_AUTOSIZE);
  m_ppanel->m_pdclist->SetColumnWidth(2, wxLIST_AUTOSIZE);

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

void Sailonline::OnRaceSelected(wxListEvent& event) {
  // Get race number
  long idx = event.GetIndex();
  wxString racenumber = m_ppanel->m_pracelist->GetItemText(idx, 0);

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

}

void Sailonline::OnDcDownload(wxCommandEvent& event) {}
void Sailonline::OnDcUpload(wxCommandEvent& event) {}

void Sailonline::FillDcList() {
    if (m_prace == m_races.end())
        return;

    m_ppanel->m_pdclist->DeleteAllItems();

    for (const auto& dc : m_prace->m_dcs) {
        wxListItem item;
        long index = m_ppanel->m_pdclist->InsertItem(
            m_ppanel->m_pdclist->GetItemCount(), item);
        m_ppanel->m_pdclist->SetItem(
            index, 0, dc.m_timestamp.Format("%Y/%m/%d %H:%M:%S"));
        m_ppanel->m_pdclist->SetItem(index, 1, dc.m_is_twa ? "twa" : "cc");
        m_ppanel->m_pdclist->SetItem(index, 2,
                                    wxString::Format("%03.3f", dc.m_course));
    }

    m_ppanel->m_pdclist->SetColumnWidth(0, wxLIST_AUTOSIZE);
    m_ppanel->m_pdclist->SetColumnWidth(1, wxLIST_AUTOSIZE);
    m_ppanel->m_pdclist->SetColumnWidth(2, wxLIST_AUTOSIZE);
}

void Sailonline::OnDcFromTrack(wxCommandEvent& event) {
  FromTrackDialog dlg(this);

  if (dlg.ShowModal() == wxID_OK) {
    wxString track_guid = dlg.GetSelectedTrack();
    auto ptrack = GetTrack_Plugin(track_guid);
    if (ptrack == nullptr) return;

    if (ptrack->pWaypointList->size() < 2)
        return;

    auto first_waypoint = ptrack->pWaypointList->begin();
    m_prace->m_dcs.clear();

    for (auto waypoint = first_waypoint; waypoint != ptrack->pWaypointList->end(); ++waypoint) {
        if (waypoint == first_waypoint)
            continue;

      double bearing, distance;
      auto wp = *waypoint;
      auto first_wp = *first_waypoint;
      DistanceBearingMercator_Plugin(wp->m_lat, wp->m_lon,
                                     first_wp->m_lat, first_wp->m_lon, &bearing, &distance);
      m_prace->m_dcs.emplace_back(Dc{first_wp->m_CreateTime, first_wp->m_lat, first_wp->m_lon, bearing, false});
      first_waypoint = waypoint;
    }

    FillDcList();
  }
}

void Sailonline::OnDcModify(wxCommandEvent& event) {
    if (m_prace->m_dcs.size() < 2)
        return;


    auto last_dc = m_prace->m_dcs.begin();

    for (auto dc = last_dc + 1; dc != m_prace->m_dcs.end(); ) {
        if (std::fabs(dc->m_course - last_dc->m_course) < 2.0)  {
            // Course (almost) unchanged, remove this dc
            // last_dc remains unchanged
            dc = m_prace->m_dcs.erase(dc);
        } else {
            // Calculate new course (could be optimized to only calculate when intermediate dcs have been deleted)
            // TODO No land collision check!!!
            double new_dist;
            DistanceBearingMercator_Plugin(dc->m_lat_start, dc->m_lon_start, last_dc->m_lat_start, last_dc->m_lon_start, &last_dc->m_course, &new_dist);

            last_dc = dc;
            ++dc;
        }
    }

    FillDcList();
}

void Sailonline::OnCopyDcs(wxCommandEvent& event) {
  wxString dc_list;

  for (const auto& dc : m_prace->m_dcs) {
    wxString timestamp = dc.m_timestamp.Format("%Y/%m/%d %H:%M:%S");
    wxString coursetype = (dc.m_is_twa ? "twa" : "cc");
    wxString course = wxString::Format("%03.3f", dc.m_course);

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
