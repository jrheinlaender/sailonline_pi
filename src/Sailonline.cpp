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
#include <wx/window.h>

#include "ocpn_plugin.h"

#include "sailonline_pi.h"
#include "Sailonline.h"

Sailonline::Sailonline(wxWindow* parent, sailonline_pi& plugin)
    : SailonlineBase(parent), m_sailonline_pi(plugin) {
  int conf_version;
  m_sailonline_pi.GetConf()->Read("ConfigVersion", &conf_version, 0);

  wxBoxSizer* psizer;
  psizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(psizer);
}

Sailonline::~Sailonline() {
  wxFileConfig* pconf = m_sailonline_pi.GetConf();

  wxPoint p = GetPosition();
  pconf->Write("DialogX", p.x);
  pconf->Write("DialogY", p.y);

  // pconf->Write("DialogWidth", m_size.x);
  // pconf->Write("DialogHeight", m_size.y);
  // pconf->Write("DialogSplit", m_panel->m_splitter1->GetSashPosition());
}

bool Sailonline::Show(bool show) { return SailonlineBase::Show(show); }
