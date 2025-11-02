/***************************************************************************
 *   Copyright (C) 2025 by Jan Rheinländer                              *
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

#ifndef _FROMROUTEDIALOG_H
#define _FROMROUTEDIALOG_H

#include <wx/event.h>
#include <wx/string.h>

#include "SailonlineUI.h"

class Sailonline;

/**
 * Class that handles the main DC from track  functionality.
 */
class FromTrackDialog : public FromTrackDialogBase {
public:
  FromTrackDialog(Sailonline* psailonline);

  void OnFromTrackDone(wxCommandEvent& event);

  wxString GetSelectedTrack() const { return m_selected_track; }

private:
  Sailonline* m_psailonline;

  wxArrayString m_track_guids;
  wxString m_selected_track;
};

#endif
