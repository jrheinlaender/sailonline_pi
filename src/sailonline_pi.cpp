/***************************************************************************
 *
 * Project:  OpenCPN Sailonline plugin
 * Author:   Jan Rheinländer
 *
 ***************************************************************************
 *   Copyright (C) 2025 by  Jan Rheinländer                           *
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
 ***************************************************************************
 */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include "sailonline_pi.h"
#include "SailonlineUi.h"
#include "Sailonline.h"

extern "C" DECL_EXP opencpn_plugin* create_pi(void* ppimgr) {
  return new sailonline_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) { delete p; }

//---------------------------------------------------------------------------------------------------------
//
//    sailonline PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

sailonline_pi::sailonline_pi(void* ppimgr) : opencpn_plugin_121(ppimgr) {
  // Get filenames of the plugIn icons
  // TODO All these icons are 32x32, is that correct?
  wxFileName icon_filename(GetDataDir());
  wxLogDebug(wxString("Using icon path: %s", icon_filename.GetFullPath()));

  wxImage::AddHandler(new wxPNGHandler);

  icon_filename.SetFullName("sailonline_pi.png");
  wxImage plugin_icon(icon_filename.GetFullPath());
  if (plugin_icon.IsOk())
    m_plugin_bitmap = wxBitmap(plugin_icon);
  else
    wxLogWarning("Sailonline plugin  icon has NOT been loaded");

  icon_filename.SetFullName("sailonline_pi_rollover.png");
  wxImage plugin_icon_rollover(icon_filename.GetFullPath());
  if (plugin_icon_rollover.IsOk())
    m_plugin_bitmap_rollover = wxBitmap(plugin_icon_rollover);

#ifdef PLUGIN_USE_SVG
  icon_filename.SetFullName("sailonline_pi.svg");
  m_plugin_svg = icon_filename.GetFullPath();
  icon_filename.SetFullName("sailonline_pi_rollover.svg");
  m_plugin_svg_rollover = icon_filename.GetFullPath();
  icon_filename.SetFullName("sailonline_pi_toggled.svg");
  m_plugin_svg_toggled = icon_filename.GetFullPath();
  wxLogMessage(wxString("Loaded toolbar icon:  %s", m_plugin_svg));
#endif

  // Load panel icon
  icon_filename.SetFullName("sailonline_panel.png");
  wxImage panel_icon(icon_filename.GetFullPath());
  if (panel_icon.IsOk())
    m_panel_bitmap = wxBitmap(panel_icon);
  else
    wxLogWarning("Sailonline Navigation Panel icon has NOT been loaded");
}

sailonline_pi::~sailonline_pi() {}

int sailonline_pi::Init(void) {
  std::cout << "SOL: Init()" << std::endl;
  // Adds local language support for the plugin to OCPN
  AddLocaleCatalog(PLUGIN_CATALOG_NAME);

  // Get a pointer to the opencpn configuration object
  m_pconfig = GetOCPNConfigObject();
  m_pconfig->SetPath("/PlugIns/Sailonline");

  // Get a pointer to the opencpn display canvas, to use as a parent for the
  // Sailonline dialog
  m_pparent_window = GetOCPNCanvasWindow();

  m_psailonline.reset();
  m_pui = nullptr;

#ifdef PLUGIN_USE_SVG
  m_leftclick_tool_id = InsertPlugInToolSVG(
      "Sailonline", m_plugin_svg, m_plugin_svg_rollover, m_plugin_svg_toggled,
      wxITEM_CHECK, _("Sailonline"), wxEmptyString, nullptr,
      SAILONLINE_TOOL_POSITION, 0, this);
#else
  // Note: InsertPluginTool takes a copy of these bitmaps
  m_leftclick_tool_id = InsertPlugInTool(
      wxEmptyString, &m_plugin_bitmap, &m_plugin_bitmap_rollover, wxITEM_CHECK,
      _("Sailonline"), wxEmptyString, nullptr, SAILONLINE_TOOL_POSITION, 0,
      this);
#endif

  LoadConfig();

  return (WANTS_TOOLBAR_CALLBACK | WANTS_CONFIG | WANTS_PLUGIN_MESSAGING);
}

bool sailonline_pi::DeInit(void) {
  if (m_pui) m_pui->Close();
  SailonlineUi* pui = m_pui;
  m_pui = nullptr; /* needed first as destructor may call event loop */
  delete pui;

  m_psailonline.reset();

  return true;
}

int sailonline_pi::GetAPIVersionMajor() { return OCPN_API_VERSION_MAJOR; }
int sailonline_pi::GetAPIVersionMinor() { return OCPN_API_VERSION_MINOR; }
int sailonline_pi::GetPlugInVersionMajor() { return PLUGIN_VERSION_MAJOR; }
int sailonline_pi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }
int sailonline_pi::GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }
int sailonline_pi::GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; }

wxBitmap* sailonline_pi::GetPlugInBitmap() { return &m_panel_bitmap; }

wxString sailonline_pi::GetCommonName() { return _(PLUGIN_COMMON_NAME); }

wxString sailonline_pi::GetShortDescription() {
  return _(PLUGIN_SHORT_DESCRIPTION);
}

wxString sailonline_pi::GetLongDescription() {
  return _(PLUGIN_LONG_DESCRIPTION);
}

int sailonline_pi::GetToolbarToolCount(void) { return 1; }

void sailonline_pi::ShowPreferencesDialog(wxWindow* parent) {}

void sailonline_pi::NewSol() {
  if (m_pui) return;

  // Create this first, so that race data can be prepared
  m_psailonline = std::make_shared<Sailonline>(*this);
  auto errors = m_psailonline->GetErrors();

  m_pui = new SailonlineUi(m_pparent_window, *this);
  wxPoint p = m_pui->GetPosition();
  m_pui->Move(0, 0);  // workaround for gtk autocentre dialog behavior
  m_pui->Move(p);
}

void sailonline_pi::SetPluginMessage(wxString& message_id,
                                     wxString& message_body) {
  if (message_id == "GRIB_VALUES" || message_id == "WR_BOATDATA") {
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(static_cast<std::string>(message_body), root)) {
      m_received_json_message = root;
      m_received_message = message_body;
    }
  }
}

bool sailonline_pi::LoadConfig() {
  if (!m_pconfig) return false;

  // TODO fill with life

  return true;
}

bool sailonline_pi::SaveConfig() {
  if (!m_pconfig) return false;

  // TODO fill with life

  return true;
}

void sailonline_pi::OnToolbarToolCallback(int id) {
  if (!m_psailonline) NewSol();

  m_pui->Show(!m_pui->IsShown());
}

wxFileName sailonline_pi::GetDataDir(const wxString& subdir) const {
  wxFileName result;
  result.SetPath(GetPluginDataDir("sailonline_pi"));
  result.AppendDir("data");
  if (!result.DirExists()) result.Mkdir();
  if (!subdir.IsEmpty()) {
    result.AppendDir(subdir);
    if (!result.DirExists()) result.Mkdir();
  }

  return result;
}

Json::Value sailonline_pi::GetJsonMessage() const {
  if (m_received_message == wxEmptyString ||
      m_received_json_message["Type"].asString() != "Reply")
    return Json::nullValue;

  return m_received_json_message;
}
