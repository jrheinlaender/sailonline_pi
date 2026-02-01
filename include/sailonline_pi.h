/***************************************************************************
 *   Copyright (C) 2025 by Jan Rheinländer                               *
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
 **************************************************************************/

#ifndef _SAILONLINEPI_H_
#define _SAILONLINEPI_H_

#ifdef DEBUG_BUILD
#define DEBUGSL(x)                                 \
  do {                                             \
    time_t now = time(0);                          \
    tm* localtm = localtime(&now);                 \
    char* stime = asctime(localtm);                \
    stime[strlen(stime) - 1] = 0;                  \
    std::cout << stime << " : " << x << std::endl; \
  } while (0)

#define DEBUGST(x)                    \
  do {                                \
    time_t now = time(0);             \
    tm* localtm = localtime(&now);    \
    char* stime = asctime(localtm);   \
    stime[strlen(stime) - 1] = 0;     \
    std::cout << stime << " : " << x; \
  } while (0)

#define DEBUGCONT(x) \
  do {               \
    std::cout << x;  \
  } while (0)

#define DEBUGEND(x)              \
  do {                           \
    std::cout << x << std::endl; \
  } while (0)
#else
#define DEBUGSL(x) \
  do {             \
  } while (0)
#define DEBUGST(x) \
  do {             \
  } while (0)
#define DEBUGCONT(x) \
  do {               \
  } while (0)
#define DEBUGEND(x) \
  do {              \
  } while (0)
#endif

#include "version.h"

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/window.h>

#include "ocpn_plugin.h"
#include <json/json.h>

class SailonlineUi;
class Sailonline;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

// Request default positioning of toolbar tool
#define SAILONLINE_TOOL_POSITION -1

class sailonline_pi : public opencpn_plugin_121 {
public:
  sailonline_pi(void* ppimgr);
  ~sailonline_pi();

  int Init();
  bool DeInit();

  int GetAPIVersionMajor();
  int GetAPIVersionMinor();
  int GetPlugInVersionMajor();
  int GetPlugInVersionMinor();
  int GetPlugInVersionPatch();
  int GetPlugInVersionPost();

  wxBitmap* GetPlugInBitmap();
  wxString GetCommonName();
  wxString GetShortDescription();
  wxString GetLongDescription();

  //  The required override PlugIn Methods
  int GetToolbarToolCount(void);
  void ShowPreferencesDialog(wxWindow* parent);
  void OnToolbarToolCallback(int id);

  wxWindow* GetParentWindow() { return m_pparent_window; }
  const std::shared_ptr<Sailonline> GetSol() const { return m_psailonline; }

  wxFileConfig* GetConf() { return m_pconfig; }

  /// Return path of directory where all SOL-related data is stored.
  // Optionally append and create a subdirectory
  wxFileName GetDataDir(const wxString& subdir = "") const;

  Json::Value GetJsonMessage() const;

private:
  // No shared_ptr: Must be compatible to wxWindow* for initializing dialogs
  SailonlineUi* m_pui = nullptr;
  std::shared_ptr<Sailonline> m_psailonline = nullptr;

  // Variables to handle messaging
  Json::Value m_received_json_message;
  wxString m_received_message;

  wxWindow* m_pparent_window = nullptr;

  wxFileConfig* m_pconfig;

  int m_leftclick_tool_id;

  void NewSol();

  void SetPluginMessage(wxString& message_id, wxString& message_body);

  bool LoadConfig();
  bool SaveConfig();

  // Icons
  wxBitmap m_plugin_bitmap;
  wxBitmap m_plugin_bitmap_rollover;
  wxBitmap m_panel_bitmap;

#ifdef PLUGIN_USE_SVG
  wxString m_plugin_svg;
  wxString m_plugin_svg_rollover;
  wxString m_plugin_svg_toggled;
#endif
};

#endif
