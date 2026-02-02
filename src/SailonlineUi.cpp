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
#include <wx/clipbrd.h>

// #include <ocpn_plugin.h>

#include "ocpn_plugin.h"
#include "sailonline_pi.h"
#include "SailonlineUi.h"
#include "Sailonline.h"
#include "Race.h"
#include "FromTrackDialog.h"

const std::shared_ptr<Sailonline> SailonlineUi::GetSol() const {
  return m_sailonline_pi.GetSol();
}

SailonlineUi::SailonlineUi(wxWindow* parent, sailonline_pi& plugin)
    : SailonlineUiBase(parent),
      m_sailonline_pi(plugin),
      m_ppanel(nullptr),
      m_prace(nullptr) {
  wxLogMessage("Initializing Sailonline GUI");

  // Image handlers required by controls
  wxImage::AddHandler(
      new wxPNGHandler);  // TODO This was already called in sailonline_pi.cpp

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
  std::cout << "Read Dialog W=" << rect.width << ", H=" << rect.height
            << std::endl;

  m_ppanel->m_pracelist->ClearAll();
  m_ppanel->m_pracelist->InsertColumn(0, _("Number"));
  m_ppanel->m_pracelist->InsertColumn(1, _("Name"));

  // Check if we are online
  if (!OCPN_isOnline()) {
    m_init_errors.emplace_back("No internet access");
    return;
  }

  // Fill list of races
  for (const auto& race : GetSol()->GetRaces()) {
    wxListItem item;
    long index = m_ppanel->m_pracelist->InsertItem(
        m_ppanel->m_pracelist->GetItemCount(), item);
    m_ppanel->m_pracelist->SetItem(index, 0, race.second.m_id);
    m_ppanel->m_pracelist->SetItem(index, 1, race.second.m_name);
  }

  // Finish
  m_ppanel->m_pracelist->SetColumnWidth(0, wxLIST_AUTOSIZE);
  m_ppanel->m_pracelist->SetColumnWidth(1, wxLIST_AUTOSIZE);
  m_ppanel->m_pracelist->Connect(
      wxEVT_LIST_ITEM_SELECTED,
      wxListEventHandler(SailonlineUi::OnRaceSelected), nullptr, this);

  if (!GetSol()->GetRaces().empty())
    m_ppanel->m_pracelist->SetItemState(m_ppanel->m_pracelist->GetTopItem(),
                                        wxLIST_STATE_SELECTED,
                                        wxLIST_STATE_SELECTED);

  m_ppanel->m_notebook->Connect(
      wxEVT_NOTEBOOK_PAGE_CHANGING,
      wxBookCtrlEventHandler(SailonlineUi::OnPageChanged), nullptr, this);
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

  m_ppanel->m_pbutton_downloadpolar->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnPolarDownload), nullptr, this);
  m_ppanel->m_pbutton_download->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcDownload), nullptr, this);
  m_ppanel->m_pbutton_upload->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcUpload), nullptr, this);
  m_ppanel->m_pbutton_fromtrack->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcFromTrack), nullptr, this);
  m_ppanel->m_pbutton_totrack->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcToTrack), nullptr, this);
  m_ppanel->m_pbutton_modify->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcModify), nullptr, this);
  m_ppanel->m_pbutton_copydcs->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnCopyDcs), nullptr, this);
}

SailonlineUi::~SailonlineUi() {
  std::cout << "Destructor of SailonlineUi" << std::endl;

  m_ppanel->m_pracelist->Disconnect(
      wxEVT_LIST_ITEM_SELECTED,
      wxListEventHandler(SailonlineUi::OnRaceSelected), nullptr, this);
  m_ppanel->m_pbutton_download->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcDownload), nullptr, this);
  m_ppanel->m_pbutton_upload->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcUpload), nullptr, this);
  m_ppanel->m_pbutton_fromtrack->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcFromTrack), nullptr, this);
  m_ppanel->m_pbutton_fromtrack->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcToTrack), nullptr, this);
  m_ppanel->m_pbutton_modify->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnDcModify), nullptr, this);
  m_ppanel->m_pbutton_copydcs->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlineUi::OnCopyDcs), nullptr, this);

  // TODO Move to _pi ?
  wxFileConfig* pconf = m_sailonline_pi.GetConf();

  wxRect rect = GetRect();
  pconf->Write("DialogX", rect.x);
  pconf->Write("DialogY", rect.y);
  pconf->Write("DialogWidth", rect.width);
  pconf->Write("DialogHeight", rect.height);
  pconf->Write("DialogSplit", m_ppanel->m_psplitter->GetSashPosition());
  pconf->Flush();
  std::cout << "Wrote Dialog W=" << rect.width << ", H=" << rect.height
            << std::endl;
}

bool SailonlineUi::Show(bool show) {
  if (!m_init_errors.empty()) {
    // TODO show dialog
    return false;
  }

  return SailonlineUiBase::Show(show);
}

void SailonlineUi::ShowPage(const int page) {
  // TODO explain to user why it's not working
  if (m_prace == nullptr) return;

  switch (page) {
    case 0:  // Race description
    {
      // Fill first tab with information about the race
      m_ppanel->SetLabel(m_prace->m_id);

      m_ppanel->m_racename->SetLabel(
          m_prace->m_name);  // TODO Show only if race has not started yet.
                             // Otherwise message is enough
      m_ppanel->m_racemsg->SetLabel(m_prace->m_message);
      m_ppanel->m_racedesc->SetPage(m_prace->m_description);
      m_ppanel->m_racedesc->SetSize(m_ppanel->m_racedata->GetClientSize());

      break;
    }
    case 1:  // Race information
    {
      if (!m_prace->Login() ||
          !m_prace->DownloadPolar() ||
          !m_prace->GetWaypoints()) {
        wxString errors;
        for (const auto& e : m_prace->GetErrors())
            errors = errors.append(e).append('\n');
        wxLogMessage(errors);
        OCPNMessageBox_PlugIn(this, errors, "Error downloading race information", wxOK);
        return;
      }

      m_ppanel->m_polarname->SetLabel(m_prace->m_polarfile);

      break;
    }
    case 2:  // DC list
    {
      // m_prace->Login();
      // m_prace->DownloadDcs();
      FillDcList();

      break;
    }
  }
}

void SailonlineUi::OnRaceSelected(wxListEvent& event) {
  // Get race number
  long idx = event.GetIndex();
  std::string racenumber =
      m_ppanel->m_pracelist->GetItemText(idx, 0).ToStdString();
  if (racenumber.empty()) return;
  // TODO Error message
  std::cout << "Race selected: " << racenumber << std::endl;
  m_prace = GetSol()->GetRace(racenumber);
  if (m_prace == nullptr) return;
  // TODO Clear panel if nothing is found?

  // Show race description
  ShowPage(0);
}

void SailonlineUi::OnPageChanged(wxBookCtrlEvent& event) {
  if (m_prace == nullptr) return;

  ShowPage(event.GetSelection());
}

void SailonlineUi::OnPolarDownload(wxCommandEvent& event) {}

void SailonlineUi::OnDcDownload(wxCommandEvent& event) {}
void SailonlineUi::OnDcUpload(wxCommandEvent& event) {}

void SailonlineUi::FillDcList() {
  // TODO Error message
  if (m_prace == nullptr) return;

  m_ppanel->m_pdclist->DeleteAllItems();

  m_prace->EnrichDcs();
  auto dcs = m_prace->GetDcs();
  auto previous_dc = dcs.begin();

  for (auto dc = dcs.begin(); dc != dcs.end(); ++dc) {
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

void SailonlineUi::OnDcFromTrack(wxCommandEvent& event) {
  if (m_prace == nullptr) return;

  FromTrackDialog dlg(this);

  if (dlg.ShowModal() == wxID_OK) {
    wxString track_guid = dlg.GetSelectedTrack();
    auto ptrack = GetTrack_Plugin(track_guid);
    if (ptrack == nullptr) return;

    if (ptrack->pWaypointList->size() < 2) return;

    auto first_waypoint = ptrack->pWaypointList->begin();
    auto& dcs = m_prace->GetDcs();
    dcs.clear();

    for (auto waypoint = first_waypoint;
         waypoint != ptrack->pWaypointList->end(); ++waypoint) {
      if (waypoint == first_waypoint) continue;

      double bearing, distance;
      auto wp = *waypoint;
      auto first_wp = *first_waypoint;
      DistanceBearingMercator_Plugin(wp->m_lat, wp->m_lon, first_wp->m_lat,
                                     first_wp->m_lon, &bearing, &distance);

      dcs.emplace_back(Dc{first_wp->m_CreateTime, first_wp->m_lat,
                          first_wp->m_lon, bearing, false});

      first_waypoint = waypoint;
    }

    FillDcList();
  }
}

void SailonlineUi::OnDcToTrack(wxCommandEvent& event) {
  if (m_prace == nullptr) return;

  m_prace->MakeTrack();
}

void SailonlineUi::OnDcModify(wxCommandEvent& event) {
  if (m_prace == nullptr) return;

  m_prace->SimplifyDcs();
  m_prace->OptimizeManeuvers();

  FillDcList();
}

void SailonlineUi::OnCopyDcs(wxCommandEvent& event) {
  if (m_prace == nullptr) return;

  wxString dc_list;
  const auto& dcs = m_prace->GetDcs();

  for (const auto& dc : dcs) {
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
