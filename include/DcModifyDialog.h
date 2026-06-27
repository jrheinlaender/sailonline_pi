/***************************************************************************
 *   Copyright (C) 2026 by Jan Rheinländer                              *
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

#ifndef _DCMODIFYDIALOG_H
#define _DCMODIFYDIALOG_H

#include <wx/event.h>
#include <wx/string.h>

#include "SailonlineUiBase.h"

class SailonlineUi;

/**
 * Class that handles the main DC modification functionality.
 */
class DcModifyDialog : public DcModifyDialogBase {
public:
  DcModifyDialog(SailonlineUi* psailonline);

  void OnModifyOk(wxCommandEvent&) override;
  void OnModifyApply(wxCommandEvent&) override;
  void OnModifyCancel(wxCommandEvent&) override;

private:
  SailonlineUi* m_pui;

  void ApplyModifications();
};

#endif
