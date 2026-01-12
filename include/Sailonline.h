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
#include <list>

#include "ocpn_plugin.h"
#include <json/json.h>

#include "SailonlineUI.h"

class sailonline_pi;
class FromTrackDialog;

/**
 * Class that handles SOL delayed commands
 */
class Dc {
public:
    wxDateTime m_timestamp;
    double m_lat_start;
    double m_lon_start;
    double m_course;
    double m_tws;           // True wind speed
    double m_twa;           // True wind angle
    double m_stw;            // Boat speed through water
    double m_opt_upwind;    // Optimal angle for going upwind
    double m_opt_downwind;  // Optimal angle for going downwind
    double m_perf_begin;    // Performance directly after course change
    double m_perf_end;      // Performance directly before next course change
    bool m_is_twa;

    Dc(const wxDateTime& m_timestamp, const double m_lat_start, const double m_lon_start,
       const double m_course, const bool m_is_twa);
  };

/**
 * Class that handles the main Sailonline  functionality.
 */
class Sailonline : public SailonlineBase {
private:
  struct Race {
    wxString m_id;
    wxString m_name;
    wxString m_description;
    wxString m_message;
    wxString m_start;
    wxString m_url;

    // This must be list because of element insertion in OnDcModify()
    std::list<Dc> m_dcs;
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
  std::vector<Race>::iterator m_prace;

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

  // Dialogs

  // Messaging
  // Request grib values: True wind speed (knots) and true wind direction
  // (degrees)
  std::pair<double, double> GetWindData(const wxDateTime& t, double lat,
                                        double lon);
  // Request boat data: Boat speed (knots)
  double GetSpeedThroughWater(double tws, double twa);
  // Request boat data: optimal upwind angle (degrees), optimal downwind angle
  // (degrees)
  std::pair<double, double> GetBoatOptimalAngles(double tws);

  /// Update dc panel with data from m_prace->m_dcs
  void FillDcList();
};

#endif
