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

#ifndef _SAILONLINEUI_H_
#define _SAILONLINEUI_H_

#include <memory>

#include "SailonlineUiBase.h"

class sailonline_pi;
class Sailonline;
class Race;

/**
 * Class that handles the Sailonline user interface.
 */
class SailonlineUi : public SailonlineUiBase {
public:
  SailonlineUi(wxWindow* parent, sailonline_pi& plugin);
  ~SailonlineUi();

  bool Show(bool show);

private:
  sailonline_pi& m_sailonline_pi;

  SailonlinePanel* m_ppanel;

  // Current race visible in UI
  std::unique_ptr<Race> m_prace;

  std::vector<std::string> m_init_errors;

  // Show data on selected notebook page
  void ShowPage(const int page);

  // Events
  // Don't destroy, otherwise sailonline_pi::DeInit() will crash
  void OnClose(wxCloseEvent& event) { Hide(); }
  void OnClose(wxCommandEvent& event) { Hide(); }
  void OnRaceSelected(wxListEvent& event);
  void OnPageChanged(wxBookCtrlEvent& event);
  void OnPolarDownload(wxCommandEvent& event);
  void OnDcDownload(wxCommandEvent& event);
  void OnDcUpload(wxCommandEvent& event);
  void OnDcFromTrack(wxCommandEvent& event);
  void OnDcToTrack(wxCommandEvent& event);
  void OnDcModify(wxCommandEvent& event);
  void OnCopyDcs(wxCommandEvent& event);

  /// Update dc panel with data from current race
  void FillDcList();

  // Shortcut to Sailonline data
  const std::shared_ptr<Sailonline> GetSol() const;
};

#endif
