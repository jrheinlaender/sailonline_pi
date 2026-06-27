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

// #include <wx/wx.h>

// #include <ocpn_plugin.h>

#include "DcModifyDialog.h"
#include "SailonlineUi.h"
#include "Race.h"

DcModifyDialog::DcModifyDialog(SailonlineUi* pui)
    : DcModifyDialogBase(pui), m_pui(pui) {}

void DcModifyDialog::OnModifyOk(wxCommandEvent&) {
  ApplyModifications();

  Hide();
  SetReturnCode(wxID_OK);
}

void DcModifyDialog::OnModifyApply(wxCommandEvent&) {
  ApplyModifications();
  m_pui->UpdateDcList();
}

void DcModifyDialog::OnModifyCancel(wxCommandEvent&) { Hide(); }

void DcModifyDialog::ApplyModifications() {
  // Note: This dialog is only opened if a race has been selected
  if (m_pcheck_mergedcs->IsChecked())
    m_pui->GetSelectedRace()->SimplifyDcs(
        m_pspin_merge_course_delta->GetValue(),
        m_pspin_merge_twa_delta->GetValue());
  if (m_pcheck_optimize_maneuvers->IsChecked())
    m_pui->GetSelectedRace()->OptimizeManeuvers();
  if (m_pcheck_twa_near_limit->IsChecked())
    m_pui->GetSelectedRace()->ForceTwaNearLimits(
        m_pspin_twa_deviation->GetValue());
}
