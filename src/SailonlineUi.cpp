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

#include <sstream>

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
    m_ppanel->m_pracelist->SetItem(index, 0, race.second->m_id);
    m_ppanel->m_pracelist->SetItem(index, 1, race.second->m_name);
  }

  // Finish
  m_ppanel->m_pracelist->SetColumnWidth(0, wxLIST_AUTOSIZE);
  m_ppanel->m_pracelist->SetColumnWidth(1, wxLIST_AUTOSIZE);
  m_ppanel->m_pracelist->Bind(
      wxEVT_LIST_ITEM_SELECTED, &SailonlineUi::OnRaceSelected, this);

  if (!GetSol()->GetRaces().empty())
    m_ppanel->m_pracelist->SetItemState(m_ppanel->m_pracelist->GetTopItem(),
                                        wxLIST_STATE_SELECTED,
                                        wxLIST_STATE_SELECTED);

  m_ppanel->m_notebook->Bind(
      wxEVT_NOTEBOOK_PAGE_CHANGING, &SailonlineUi::OnPageChanged, this);
  m_ppanel->m_notebook->SetSelection(RaceDescription);  // Show first tab

  m_ppanel->m_pwaypointlist->ClearAll();
  m_ppanel->m_pwaypointlist->InsertColumn(0, _("Id"));
  m_ppanel->m_pwaypointlist->InsertColumn(1, _("Name"));

  m_ppanel->m_pdclist->ClearAll();
  m_ppanel->m_pdclist->InsertColumn(0, _("Time (UTC)"));
  m_ppanel->m_pdclist->InsertColumn(1, _("Type"));
  m_ppanel->m_pdclist->InsertColumn(2, _("Course"));
  m_ppanel->m_pdclist->InsertColumn(3, _("TWA"));
  m_ppanel->m_pdclist->InsertColumn(4, _("Speed"));
  m_ppanel->m_pdclist->InsertColumn(5, _("Opt"));
  m_ppanel->m_pdclist->InsertColumn(6, _("Perf1"));
  m_ppanel->m_pdclist->InsertColumn(7, _("Perf2"));

  m_ppanel->m_pbutton_downloadpolar->Bind(
      wxEVT_COMMAND_BUTTON_CLICKED, &SailonlineUi::OnPolarDownload, this);
  m_ppanel->m_pbutton_downloadpolar->Disable();
  m_ppanel->m_pbutton_tracking->Bind(
      wxEVT_COMMAND_BUTTON_CLICKED, &SailonlineUi::OnStartStopTracking, this);
  m_ppanel->m_pspin_updateinterval->Bind(
      wxEVT_COMMAND_SPINCTRL_UPDATED, &SailonlineUi::OnUpdateInterval, this);
  m_ppanel->m_pbutton_download->Bind(
      wxEVT_COMMAND_BUTTON_CLICKED, &SailonlineUi::OnDcDownload, this);
  m_ppanel->m_pbutton_upload->Bind(
      wxEVT_COMMAND_BUTTON_CLICKED, &SailonlineUi::OnDcUpload, this);
  m_ppanel->m_pbutton_fromtrack->Bind(
      wxEVT_COMMAND_BUTTON_CLICKED, &SailonlineUi::OnDcFromTrack, this);
  m_ppanel->m_pbutton_totrack->Bind(
      wxEVT_COMMAND_BUTTON_CLICKED, &SailonlineUi::OnDcToTrack, this);
  m_ppanel->m_pbutton_modify->Bind(
      wxEVT_COMMAND_BUTTON_CLICKED, &SailonlineUi::OnDcModify, this);
  m_ppanel->m_pbutton_copydcs->Bind(
      wxEVT_COMMAND_BUTTON_CLICKED, &SailonlineUi::OnCopyDcs, this);

  m_interval_boatquery = 30000;  // 30s
  m_tMoveBoat.Bind(
      wxEVT_TIMER, &SailonlineUi::OnMoveBoat, this);
}

SailonlineUi::~SailonlineUi() {
  std::cout << "Destructor of SailonlineUi" << std::endl;
  m_tMoveBoat.Stop();

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
    case RaceDescription: {
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
    case RaceInformation: {
      if (!m_prace->DownloadPolar() || !m_prace->DownloadWaypoints()) {
        wxString errors;
        for (const auto& e : m_prace->GetErrors())
          errors = errors.append(e).append('\n');
        wxLogMessage(errors.c_str());
        OCPNMessageBox_PlugIn(this, errors,
                              "Error downloading race information", wxOK);
        return;
      }

      m_ppanel->m_polarname->SetLabel(m_prace->m_polarfile);
      m_ppanel->m_pbutton_downloadpolar->Enable(true);

      for (const auto& wp : m_prace->GetWaypoints()) {
        wxListItem item;
        long index = m_ppanel->m_pwaypointlist->InsertItem(
            m_ppanel->m_pwaypointlist->GetItemCount(), item);
        m_ppanel->m_pwaypointlist->SetItem(index, 0, wp->m_GUID);
        m_ppanel->m_pwaypointlist->SetItem(index, 1, wp->m_MarkName);
      }

      for (int i = 0; i < m_ppanel->m_pwaypointlist->GetColumnCount(); ++i)
        m_ppanel->m_pwaypointlist->SetColumnWidth(i, wxLIST_AUTOSIZE);

      break;
    }
    case RaceDcList: {
      m_prace->DownloadWaypoints();
      // m_prace->DownloadDcs();
      FillDcList();

      break;
    }
    case RaceRouting: {
      if (!m_prace->DownloadBoatUrl()) {
        wxString errors;
        for (const auto& e : m_prace->GetErrors())
          errors = errors.append(e).append('\n');
        wxLogMessage(errors.c_str());
        OCPNMessageBox_PlugIn(this, errors,
                              "Error downloading boat position URL", wxOK);
        return;
      }

      // Run now to update data in the tab
      m_tMoveBoat.StartOnce();

      if (!m_prace->DownloadWeatherUrl()) {
        wxString errors;
        for (const auto& e : m_prace->GetErrors())
          errors = errors.append(e).append('\n');
        wxLogMessage(errors.c_str());
        OCPNMessageBox_PlugIn(this, errors,
                              "Error downloading weather data URL", wxOK);
        return;
      }

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
  m_prace = GetSol()->GetRace(racenumber);
  if (m_prace == nullptr) return;
  // TODO Clear panel if nothing is found?

  // Show race description
  m_ppanel->m_notebook->SetSelection(RaceDescription);
}

void SailonlineUi::OnPageChanged(wxBookCtrlEvent& event) {
  if (m_prace == nullptr) return;

  // Prevent timer running outside of the Routing page
  if (event.GetSelection() != RaceRouting) m_tMoveBoat.Stop();

  ShowPage(event.GetSelection());
}

void SailonlineUi::OnStartStopTracking(wxCommandEvent&) {
  if (m_tMoveBoat.IsRunning()) {
    m_tMoveBoat.Stop();
    m_ppanel->m_pbutton_tracking->SetLabel(_("Start tracking"));
  } else {
    m_tMoveBoat.StartOnce();
    m_tMoveBoat.Start(5000);
    m_ppanel->m_pbutton_tracking->SetLabel(_("Stop tracking"));
  }
}

void SailonlineUi::OnUpdateInterval(wxSpinEvent& event) {
  /**
   * Don't allow querying the server more often than every 5s
   * Web GUI updates approx. every 10 seconds with 131 boats
   */
  m_interval_boatquery = std::max(5, event.GetValue()) * 1000;
}

void SailonlineUi::OnPolarDownload(wxCommandEvent&) {}

void SailonlineUi::OnDcDownload(wxCommandEvent&) {}
void SailonlineUi::OnDcUpload(wxCommandEvent&) {}

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
    m_ppanel->m_pdclist->SetItem(
        index, 0, dc->m_timestamp.Format("%Y/%m/%d %H:%M:%S", wxDateTime::UTC));
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
        index, 6, wxString::Format("%03.3f", dc->m_perf_begin * 100.0));
    m_ppanel->m_pdclist->SetItem(
        index, 7, wxString::Format("%03.3f", dc->m_perf_end * 100.0));

    previous_dc = dc;
  }

  for (int i = 0; i < m_ppanel->m_pdclist->GetColumnCount(); ++i)
    m_ppanel->m_pdclist->SetColumnWidth(i, wxLIST_AUTOSIZE);
}

void SailonlineUi::OnDcFromTrack(wxCommandEvent&) {
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

      dcs.emplace_back(Dc{first_wp->m_CreateTime.FromUTC(), first_wp->m_lat,
                          first_wp->m_lon, bearing, false});

      first_waypoint = waypoint;
    }

    FillDcList();
  }
}

void SailonlineUi::OnDcToTrack(wxCommandEvent&) {
  if (m_prace == nullptr) return;

  m_prace->MakeTrack();
}

void SailonlineUi::OnDcModify(wxCommandEvent&) {
  if (m_prace == nullptr) return;

  m_prace->SimplifyDcs();
  m_prace->OptimizeManeuvers();

  FillDcList();
}

void SailonlineUi::OnCopyDcs(wxCommandEvent&) {
  if (m_prace == nullptr) return;

  wxString dc_list;
  const auto& dcs = m_prace->GetDcs();

  for (const auto& dc : dcs) {
    wxString timestamp =
        dc.m_timestamp.Format("%Y/%m/%d %H:%M:%S", wxDateTime::UTC);
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

namespace {
std::string DegreesToString(const double lat_lon, const unsigned digits,
                            const char pos, const char neg) {
  double degrees, decimals;
  decimals = std::modf(std::fabs(lat_lon), &degrees);

  wxString format = wxString::Format("%%0%u.0f", digits);
  wxString result = wxString::Format(format, degrees) +
                    wxString::Format("%09.6f", decimals * 60.0) + "," +
                    (lat_lon >= 0 ? pos : neg) + ",";

  return result.ToStdString();
}

std::string addCheckSum(const std::string& sentence) {
  unsigned char XOR = 0;
  for (size_t i = 0; i < sentence.size(); i++)
    XOR ^= (unsigned char)sentence[i];
  std::stringstream tmpss;
  tmpss << std::hex << (int)XOR;
  return "$" + sentence + "*" + tmpss.str();
}
}  // namespace

void SailonlineUi::OnMoveBoat(wxTimerEvent&) {
  /**
   * These can be static. When a race is changed, the page is changed to
   * the description page. This stops the timer. Changing to the routing
   * page calls OnMoveBoat() with a OneShot timer, thus updating the values.
   */
  static int timer_accumulated = 0;
  static double latitude;
  static double longitude;
  static double course;
  static double speed;

  if (timer_accumulated == 0 || m_tMoveBoat.IsOneShot()) {
    std::tie(latitude, longitude, course, speed) = m_prace->GetBoatData();

    if (latitude == 0.0 && longitude == 0.0 && course == 0.0 && speed == 0.0) {
      wxString errors;
      for (const auto& e : m_prace->GetErrors())
        errors = errors.append(e).append('\n');
      wxLogMessage(errors.c_str());
      OCPNMessageBox_PlugIn(
          this, errors, "Error downloading boat position information", wxOK);
      return;
    }

    // Update GUI
    m_ppanel->m_latitude->SetLabel(wxString::Format("%.5f", latitude));
    m_ppanel->m_longitude->SetLabel(wxString::Format("%.5f", longitude));
    m_ppanel->m_course->SetLabel(wxString::Format("%.5f", course));
    m_ppanel->m_speed->SetLabel(wxString::Format("%.5f", speed));

    // Initialize timer for IsOneShot() case
    timer_accumulated = 0;
  }

  /**
   * Move boat to position
   * This must happen regularly to avoid a watchdog timeout
   * Example: $IIGLL,5027.776667,N,412.690754,W,123327,A*26
   */
  PushNMEABuffer(
      addCheckSum(
          "IIGLL," + DegreesToString(latitude, 2, 'N', 'S') +
          DegreesToString(longitude, 3, 'E', 'W') +
          wxDateTime::Now().Format("%H%M%S", wxDateTime::UTC).ToStdString() +
          ",A") +
      "\r\n");
  // Set course and speed
  PushNMEABuffer(addCheckSum("IIVTG," + std::to_string(course) + ",T,,M," +
                             std::to_string(speed) + ",N," +
                             std::to_string(speed * 1.852) + ",K," + "A") +
                 "\r\n");

  timer_accumulated += m_tMoveBoat.GetInterval();
  if (timer_accumulated >= m_interval_boatquery) timer_accumulated = 0;
}
