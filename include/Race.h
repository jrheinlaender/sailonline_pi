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

#ifndef _RACE_H_
#define _RACE_H_

#include <list>
#include <string>
#include <memory>

#include <wx/datetime.h>

typedef void CURL;
class PlugIn_Waypoint;
class sailonline_pi;

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
  double m_stw;           // Boat speed through water
  double m_opt_upwind;    // Optimal angle for going upwind
  double m_opt_downwind;  // Optimal angle for going downwind
  double m_perf_begin;    // Performance directly after course change
  double m_perf_end;      // Performance directly before next course change
  bool m_is_twa;

  Dc(const wxDateTime& m_timestamp, const double m_lat_start,
     const double m_lon_start, const double m_course, const bool m_is_twa);
};

class Race {
public:
  Race(sailonline_pi& plugin);
  ~Race();

  std::string m_id;
  std::string m_name;
  std::string m_description;
  std::string m_message;
  std::string m_start;
  std::string m_url;
  std::string m_polarfile;

  /// Return error messages and clear the error store
  std::vector<std::string> GetErrors();

  /// Open connection to sailonline.org and get access token
  bool Login();

  /// Extract polar from race XML
  bool DownloadPolar();

  /// Extract waypoints from race XML
  bool DownloadWaypoints();

  const std::vector<std::shared_ptr<PlugIn_Waypoint>>& GetWaypoints() const;

  const std::list<Dc>& GetDcs() const;
  std::list<Dc>& GetDcs();

  /// Enrich the DC list with calculated values for diagnostic purposes
  void EnrichDcs();
  /// Try to shorten the DC list by joining legs with almost identical courses
  void SimplifyDcs();
  /// Try to minimize performance loss when tacking and jibing
  void OptimizeManeuvers();
  /// Create a track from the DC list
  void MakeTrack() const;

private:
  sailonline_pi& m_sailonline_pi;

  /// Stores error messages from functions
  std::vector<std::string> m_errors;

  std::string m_sol_token;

  // This must be list because of element insertion in OnDcModify()
  std::list<Dc> m_dcs;

  std::vector<std::shared_ptr<PlugIn_Waypoint>> m_waypoints;

  /// Download detailed raceinfo XML
  wxString GetRaceInfo();

  // Messaging
  // Request grib values: True wind speed (knots) and true wind direction
  // (degrees)
  std::pair<double, double> GetWindData(const wxDateTime& t, double lat,
                                        double lon) const;
  // Request boat data: Boat speed (knots)
  double GetSpeedThroughWater(double tws, double twa) const;
  // Request boat data: optimal upwind angle (degrees), optimal downwind angle
  // (degrees)
  std::pair<double, double> GetBoatOptimalAngles(double tws) const;

  /// Convenience funtion to shorten curl_easy_perform calls
  bool CallCurl(CURL* curl);

  /// Convencience function for placeholders in URLs
  std::string SetPlaceholders(const std::string& input) const;
};

#endif
