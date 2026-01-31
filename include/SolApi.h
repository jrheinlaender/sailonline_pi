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

#ifndef _SOLAPI_H_
#define _SOLAPI_H_

#include <string>

/**
 * Namespace that encapsulates all details of the sailonline.org API.
 * Note that the API is "stable but not documented".
 */
namespace SolApi {
    const static std::string kUrlRacelist = "http://www.sailonline.org/webclient/races.xml?filter=active";
    const static std::string kSolUrl = "https://sailonline.org/community/accounts/login/";
    const static std::string kSolReferer = "https://sailonline.org/community/accounts/login/"
                    "?password=$$password&username=$$username";
    const static std::string kSolPost = "next=%2Fwindy%2Frun%2F$$racenumber%2F"
                    "&password=$$password&username=$$username&csrfmiddlewaretoken=";
    const static std::string kSolRaceXmlUrl = "https://www.sailonline.org/webclient/auth_raceinfo_$$racenumber.xml?token=$$token";
};

#endif
