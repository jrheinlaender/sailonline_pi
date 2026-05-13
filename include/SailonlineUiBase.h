///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/html/htmlwin.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/clrpicker.h>
#include <wx/checkbox.h>
#include <wx/scrolwin.h>
#include <wx/checklst.h>
#include <wx/dialog.h>
#include <wx/choice.h>
#include <wx/textctrl.h>

#include "wxWTranslateCatalog.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class SailonlineUiBase
///////////////////////////////////////////////////////////////////////////////
class SailonlineUiBase : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar3;
		wxMenu* m_mFile;
		wxMenu* m_mPosition;
		wxMenu* m_mConfiguration;
		wxMenuItem* m_mBatch;
		wxMenuItem* m_mEdit;
		wxMenuItem* m_mGoTo;
		wxMenuItem* m_mDelete;
		wxMenuItem* m_mCompute;
		wxMenuItem* m_mComputeAll;
		wxMenuItem* m_mStop;
		wxMenuItem* m_mExport;
		wxMenuItem* m_mExportAll;
		wxMenu* m_mView;
		wxMenu* m_mHelp;
		wxMenuItem* m_mEdit1;
		wxMenuItem* m_mCompute1;
		wxMenuItem* m_mComputeAll1;
		wxMenuItem* m_mDelete1;
		wxMenuItem* m_mGoTo1;
		wxMenuItem* m_mStop1;
		wxMenuItem* m_mBatch1;
		wxMenu* m_menu1;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNewPosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeletePosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteAllPositions( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBatch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditConfiguration( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGoTo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCompute( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComputeAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFilter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSettings( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStatistics( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCursorPosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRoutePosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnManual( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxMenuItem* m_mDeleteAll;
		wxMenu* m_mContextMenu;

		SailonlineUiBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Sailonline"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_NO_TASKBAR|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

		~SailonlineUiBase();

		void SailonlineUiBaseOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( m_mContextMenu, event.GetPosition() );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class SailonlinePanel
///////////////////////////////////////////////////////////////////////////////
class SailonlinePanel : public wxPanel
{
	private:

	protected:
		wxPanel* m_panel11;
		wxPanel* m_panel12;
		wxStaticText* m_staticText168;
		wxStaticText* m_staticText170;
		wxPanel* m_dcs;
		wxPanel* m_routing;
		wxStaticText* m_staticText171;
		wxStaticText* m_staticText173;
		wxStaticText* m_staticText175;
		wxStaticText* m_staticText22;
		wxStaticText* m_staticText24;
		wxStaticText* m_staticText25;

		// Virtual event handlers, override them in your derived class
		virtual void OnEditPositionClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLeftUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLeftDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPositionKeyDown( wxListEvent& event ) { event.Skip(); }
		virtual void OnEditConfigurationClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnWeatherRoutesListLeftDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnWeatherRouteSort( wxListEvent& event ) { event.Skip(); }
		virtual void OnWeatherRouteSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnWeatherRouteKeyDown( wxListEvent& event ) { event.Skip(); }
		virtual void OnDcDownload( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDcUpload( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDcFromTrack( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDcModify( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxSplitterWindow* m_psplitter;
		wxListCtrl* m_pracelist;
		wxNotebook* m_notebook;
		wxPanel* m_racedata;
		wxStaticText* m_racename;
		wxStaticText* m_racemsg;
		wxHtmlWindow* m_racedesc;
		wxPanel* m_raceinfo;
		wxStaticText* m_polarname;
		wxButton* m_pbutton_downloadpolar;
		wxListCtrl* m_pwaypointlist;
		wxListCtrl* m_pdclist;
		wxButton* m_pbutton_download;
		wxButton* m_pbutton_upload;
		wxButton* m_pbutton_fromtrack;
		wxButton* m_pbutton_totrack;
		wxButton* m_pbutton_modify;
		wxButton* m_pbutton_copydcs;
		wxStaticText* m_latitude;
		wxStaticText* m_longitude;
		wxStaticText* m_course;
		wxStaticText* m_speed;
		wxButton* m_pbutton_tracking;
		wxSpinCtrl* m_pspin_updateinterval;

		SailonlinePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,502 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~SailonlinePanel();

};

///////////////////////////////////////////////////////////////////////////////
/// Class SettingsDialogBase
///////////////////////////////////////////////////////////////////////////////
class SettingsDialogBase : public wxDialog
{
	private:

	protected:
		wxScrolledWindow* m_scrolledWindow4;
		wxStaticText* m_staticText74;
		wxStaticText* m_staticText73;
		wxStaticText* m_staticText75;
		wxStaticText* m_staticText70;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText711;
		wxStaticText* m_staticText115;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Help;

		// Virtual event handlers, override them in your derived class
		virtual void OnUpdateColor( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnUpdateSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateColumns( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxColourPickerCtrl* m_cpCursorRoute;
		wxColourPickerCtrl* m_cpDestinationRoute;
		wxSpinCtrl* m_sRouteThickness;
		wxSpinCtrl* m_sIsoChronThickness;
		wxSpinCtrl* m_sAlternateRouteThickness;
		wxSpinCtrl* m_sWindBarbsOnRouteThickness;
		wxCheckBox* m_cbDisplayCursorRoute;
		wxCheckBox* m_cbAlternatesForAll;
		wxCheckBox* m_cbMarkAtPolarChange;
		wxCheckBox* m_cbDisplayCurrent;
		wxCheckBox* m_cbDisplayWindBarbs;
		wxCheckBox* m_cbDisplayApparentWindBarbs;
		wxCheckBox* m_cbDisplayComfort;
		wxSpinCtrl* m_sConcurrentThreads;
		wxCheckListBox* m_cblFields;
		wxCheckBox* m_cbUseLocalTime;

		SettingsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Weather Routing Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

		~SettingsDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialogBase
///////////////////////////////////////////////////////////////////////////////
class AboutDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText135;
		wxStaticText* m_stVersion;
		wxStaticText* m_staticText110;
		wxButton* m_bAboutAuthor;
		wxButton* m_bClose;

		// Virtual event handlers, override them in your derived class
		virtual void OnAboutAuthor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }


	public:

		AboutDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About Weather Routing"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );

		~AboutDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class FromTrackDialogBase
///////////////////////////////////////////////////////////////////////////////
class FromTrackDialogBase : public wxDialog
{
	private:

	protected:
		wxTextCtrl* m_pfilter;
		wxButton* m_button48;

		// Virtual event handlers, override them in your derived class
		virtual void OnFilterText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFromTrackDone( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxChoice* m_ptracklist;
		wxButton* m_pbutton_fromtrack_done;

		FromTrackDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("From Track"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );

		~FromTrackDialogBase();

};

