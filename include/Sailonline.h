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

#include <ocpn_plugin.h>

class sailonline_pi;
class Race;

/**
 * Class that handles the Sailonline data.
 */
class Sailonline {
public:
  Sailonline(sailonline_pi& plugin);
  ~Sailonline();

  /// Return error messages and clear the error store
  std::vector<std::string> GetErrors();

  const std::unordered_map<std::string, std::shared_ptr<Race>>& GetRaces() const {
    return m_races;
  }
  std::shared_ptr<Race> GetRace(const std::string& racenumber) const;

private:
  sailonline_pi& m_sailonline_pi;

  std::vector<std::string> m_errors;

  std::unordered_map<std::string, std::shared_ptr<Race>> m_races;
};

#endif
