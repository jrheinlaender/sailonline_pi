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

#include <ocpn_plugin.h>

#include "FromTrackDialog.h"
#include "Sailonline.h"

FromTrackDialog::FromTrackDialog(Sailonline* psailonline)
    : FromTrackDialogBase(psailonline), m_psailonline(std::move(psailonline)) {
  m_track_guids = GetTrackGUIDArray();
  m_ptracklist->SetColumns(2);

  for (const auto& guid : m_track_guids) {
    auto ptrack = GetTrack_Plugin(guid);
    m_ptracklist->Append(ptrack->m_NameString);
  }

  m_ptracklist->Connect(wxEVT_CHOICE,
                        wxCommandEventHandler(FromTrackDialog::OnTrack),
                        nullptr, this);

  if (!m_track_guids.empty()) {
    m_ptracklist->Select(0);
    m_selected_track = m_track_guids[0];
  }
}

void FromTrackDialog::OnTrack(wxCommandEvent& event) {
  if (event.GetSelection() >= 0 &&
      size_t(event.GetSelection()) < m_track_guids.GetCount())
    m_selected_track = m_track_guids[event.GetSelection()];
}

void FromTrackDialog::OnFromTrackDone(wxCommandEvent& event) {
  Hide();
  m_ptracklist->Disconnect(wxEVT_CHOICE,
                           wxCommandEventHandler(FromTrackDialog::OnTrack),
                           nullptr, this);
  SetReturnCode(wxID_OK);
}
