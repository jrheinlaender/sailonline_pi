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

#ifndef _SAILONLINE_H_
#define _SAILONLINE_H_

#include <string>
#include <vector>

#include "ocpn_plugin.h"

#include "SailonlineUI.h"

class sailonline_pi;
class FromTrackDialog;

/**
 * Class that handles the main Sailonline  functionality.
 */
class Sailonline : public SailonlineBase {
private:
    struct Dc {
      wxDateTime m_timestamp;
      double m_lat_start;
      double m_lon_start;
      double m_course;
      bool m_is_twa;
  };
  struct Race {
    wxString m_id;
    wxString m_name;
    wxString m_description;
    wxString m_message;
    wxString m_start;
    wxString m_url;

    std::vector<Dc> m_dcs;
  };

public:
  Sailonline(wxWindow* parent, sailonline_pi& plugin);
  ~Sailonline();

  bool Show(bool show);

private:
  sailonline_pi& m_sailonline_pi;

  SailonlinePanel* m_ppanel;

  std::vector<std::string> m_init_errors;

  std::vector<Race> m_races;
  std::vector<Race>::iterator  m_prace;

  // Events
  void OnClose(wxCloseEvent& event) {
    Hide();
  }  // Don't destroy, otherwise sailonline_pi::DeInit() will crash
  void OnClose(wxCommandEvent& event) { Hide(); }
  void OnRaceSelected(wxListEvent& event);
  void OnDcDownload(wxCommandEvent& event);
  void OnDcUpload(wxCommandEvent& event);
  void OnDcFromTrack(wxCommandEvent& event);
  void OnDcModify(wxCommandEvent& event);
  void OnCopyDcs(wxCommandEvent& event);

  // Downloading
  void OnDownloadEvent(OCPN_downloadEvent& ev);
  bool m_connected;  // Download event is connected
  long m_download_handle;
  bool m_downloading;  // Flag to discover end of download
  bool m_download_success;
  void CleanupDownload();

  // Utility functions
  /// Update dc panel with data from m_prace->m_dcs
  void FillDcList();
};

#endif
