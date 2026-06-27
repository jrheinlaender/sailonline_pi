///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "SailonlineUiBase.h"

///////////////////////////////////////////////////////////////////////////

SailonlineUiBase::SailonlineUiBase(wxWindow* parent, wxWindowID id,
                                   const wxString& title, const wxPoint& pos,
                                   const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style) {
  this->SetSizeHints(wxSize(600, 450), wxDefaultSize);

  m_menubar3 = new wxMenuBar(0);
  m_menubar3->Hide();

  m_mFile = new wxMenu();
  wxMenuItem* m_mClose;
  m_mClose = new wxMenuItem(m_mFile, wxID_ANY,
                            wxString(_("&Close")) + wxT('\t') + wxT("Ctrl+W"),
                            wxEmptyString, wxITEM_NORMAL);
  m_mFile->Append(m_mClose);

  m_menubar3->Append(m_mFile, _("&File"));

  m_mHelp = new wxMenu();
  wxMenuItem* m_mAbout;
  m_mAbout = new wxMenuItem(m_mHelp, wxID_ANY, wxString(_("&About")),
                            wxEmptyString, wxITEM_NORMAL);
  m_mHelp->Append(m_mAbout);

  m_menubar3->Append(m_mHelp, _("&Help"));

  this->SetMenuBar(m_menubar3);

  m_mContextMenu = new wxMenu();
  wxMenuItem* m_mNew1;
  m_mNew1 = new wxMenuItem(m_mContextMenu, wxID_ANY,
                           wxString(_("&New")) + wxT('\t') + wxT("Ctrl+N"),
                           wxEmptyString, wxITEM_NORMAL);
  m_mContextMenu->Append(m_mNew1);

  m_mEdit1 = new wxMenuItem(m_mContextMenu, wxID_ANY,
                            wxString(_("&Edit")) + wxT('\t') + wxT("Ctrl+E"),
                            wxEmptyString, wxITEM_NORMAL);
  m_mContextMenu->Append(m_mEdit1);

  m_mDelete1 =
      new wxMenuItem(m_mContextMenu, wxID_ANY,
                     wxString(_("&Delete")) + wxT('\t') + wxT("Ctrl+D"),
                     wxEmptyString, wxITEM_NORMAL);
  m_mContextMenu->Append(m_mDelete1);

  this->Connect(
      wxEVT_RIGHT_DOWN,
      wxMouseEventHandler(SailonlineUiBase::SailonlineUiBaseOnContextMenu),
      NULL, this);

  this->Centre(wxBOTH);

  // Connect Events
  this->Connect(wxEVT_CLOSE_WINDOW,
                wxCloseEventHandler(SailonlineUiBase::OnClose));
  this->Connect(wxEVT_SIZE, wxSizeEventHandler(SailonlineUiBase::OnSize));
  m_mFile->Bind(wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(SailonlineUiBase::OnClose), this,
                m_mClose->GetId());
  m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(SailonlineUiBase::OnAbout), this,
                m_mAbout->GetId());
  m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED,
                       wxCommandEventHandler(SailonlineUiBase::OnNew), this,
                       m_mNew1->GetId());
  m_mContextMenu->Bind(
      wxEVT_COMMAND_MENU_SELECTED,
      wxCommandEventHandler(SailonlineUiBase::OnEditConfiguration), this,
      m_mEdit1->GetId());
  m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED,
                       wxCommandEventHandler(SailonlineUiBase::OnDelete), this,
                       m_mDelete1->GetId());
}

SailonlineUiBase::~SailonlineUiBase() {
  // Disconnect Events
  this->Disconnect(wxEVT_CLOSE_WINDOW,
                   wxCloseEventHandler(SailonlineUiBase::OnClose));
  this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(SailonlineUiBase::OnSize));

  delete m_mContextMenu;
}

SailonlinePanel::SailonlinePanel(wxWindow* parent, wxWindowID id,
                                 const wxPoint& pos, const wxSize& size,
                                 long style, const wxString& name)
    : wxPanel(parent, id, pos, size, style, name) {
  this->SetMinSize(wxSize(100, 100));

  wxBoxSizer* bSizerMain;
  bSizerMain = new wxBoxSizer(wxVERTICAL);

  m_psplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition,
                                     wxDefaultSize, wxSP_3D);
  m_psplitter->SetSashGravity(0.2);
  m_psplitter->SetMinimumPaneSize(1);

  m_panel11 = new wxPanel(m_psplitter, wxID_ANY, wxDefaultPosition,
                          wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer* bSizer6;
  bSizer6 = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer* sbSizer30;
  sbSizer30 = new wxStaticBoxSizer(
      new wxStaticBox(m_panel11, wxID_ANY, _("Races")), wxVERTICAL);

  wxFlexGridSizer* fgSizer93;
  fgSizer93 = new wxFlexGridSizer(1, 1, 0, 0);
  fgSizer93->AddGrowableCol(0);
  fgSizer93->AddGrowableRow(0);
  fgSizer93->SetFlexibleDirection(wxBOTH);
  fgSizer93->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_pracelist = new wxListCtrl(
      sbSizer30->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize(-1, -1),
      wxLC_HRULES | wxLC_REPORT | wxLC_SINGLE_SEL | wxHSCROLL | wxVSCROLL);
  fgSizer93->Add(m_pracelist, 0, wxALL | wxEXPAND, 5);

  sbSizer30->Add(fgSizer93, 1, wxEXPAND | wxALL, 5);

  bSizer6->Add(sbSizer30, 1, wxEXPAND | wxALL, 5);

  m_panel11->SetSizer(bSizer6);
  m_panel11->Layout();
  bSizer6->Fit(m_panel11);
  m_panel12 = new wxPanel(m_psplitter, wxID_ANY, wxDefaultPosition,
                          wxDefaultSize, wxTAB_TRAVERSAL);
  m_panel12->Hide();

  wxBoxSizer* bSizer5;
  bSizer5 = new wxBoxSizer(wxVERTICAL);

  m_notebook =
      new wxNotebook(m_panel12, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
  m_racedata = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition,
                           wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer* bSizer8;
  bSizer8 = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer* m_psizer_racedata;
  m_psizer_racedata = new wxFlexGridSizer(3, 1, 0, 0);
  m_psizer_racedata->SetFlexibleDirection(wxBOTH);
  m_psizer_racedata->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_racename = new wxStaticText(m_racedata, wxID_ANY, _("Name"),
                                wxDefaultPosition, wxDefaultSize, 0);
  m_racename->Wrap(-1);
  m_racename->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(),
                             wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                             wxFONTWEIGHT_BOLD, false, wxEmptyString));

  m_psizer_racedata->Add(m_racename, 0, wxALL, 5);

  m_racemsg = new wxStaticText(m_racedata, wxID_ANY, _("Message"),
                               wxDefaultPosition, wxDefaultSize, 0);
  m_racemsg->Wrap(-1);
  m_psizer_racedata->Add(m_racemsg, 0, wxALL, 5);

  wxBoxSizer* bSizer12;
  bSizer12 = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer* fgSizer106;
  fgSizer106 = new wxFlexGridSizer(0, 2, 0, 0);
  fgSizer106->SetFlexibleDirection(wxBOTH);
  fgSizer106->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_racedesc = new wxHtmlWindow(m_racedata, wxID_ANY, wxDefaultPosition,
                                wxSize(-1, -1), wxHW_SCROLLBAR_AUTO);
  fgSizer106->Add(m_racedesc, 100, wxALL | wxEXPAND, 5);

  bSizer12->Add(fgSizer106, 1, wxALL | wxEXPAND, 5);

  m_psizer_racedata->Add(bSizer12, 1, wxALL | wxEXPAND, 5);

  bSizer8->Add(m_psizer_racedata, 1, wxALL | wxEXPAND, 5);

  m_racedata->SetSizer(bSizer8);
  m_racedata->Layout();
  bSizer8->Fit(m_racedata);
  m_notebook->AddPage(m_racedata, _("Race"), false);
  m_raceinfo = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition,
                           wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer* bSizer11;
  bSizer11 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* bSizer16;
  bSizer16 = new wxBoxSizer(wxHORIZONTAL);

  m_staticText168 = new wxStaticText(m_raceinfo, wxID_ANY, _("Polar"),
                                     wxDefaultPosition, wxDefaultSize, 0);
  m_staticText168->Wrap(-1);
  m_staticText168->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(),
                                  wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                  wxFONTWEIGHT_BOLD, false, wxEmptyString));

  bSizer16->Add(m_staticText168, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_polarname =
      new wxStaticText(m_raceinfo, wxID_ANY, _("not loaded"), wxDefaultPosition,
                       wxDefaultSize, wxALIGN_LEFT);
  m_polarname->Wrap(-1);
  bSizer16->Add(m_polarname, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_pbutton_downloadpolar = new wxButton(m_raceinfo, wxID_ANY, _("Download"),
                                         wxDefaultPosition, wxDefaultSize, 0);
  bSizer16->Add(m_pbutton_downloadpolar, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  bSizer16->Add(0, 0, 1, wxEXPAND, 5);

  bSizer11->Add(bSizer16, 1, wxALL | wxEXPAND, 5);

  wxBoxSizer* bSizer121;
  bSizer121 = new wxBoxSizer(wxVERTICAL);

  m_staticText170 = new wxStaticText(m_raceinfo, wxID_ANY, _("Waypoints"),
                                     wxDefaultPosition, wxDefaultSize, 0);
  m_staticText170->Wrap(-1);
  m_staticText170->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(),
                                  wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                  wxFONTWEIGHT_BOLD, false, wxEmptyString));

  bSizer121->Add(m_staticText170, 0, wxALL, 5);

  wxBoxSizer* bSizer14;
  bSizer14 = new wxBoxSizer(wxHORIZONTAL);

  m_pwaypointlist = new wxListCtrl(
      m_raceinfo, wxID_ANY, wxDefaultPosition, wxDefaultSize,
      wxLC_HRULES | wxLC_REPORT | wxLC_SORT_ASCENDING | wxHSCROLL | wxVSCROLL);
  bSizer14->Add(m_pwaypointlist, 1, wxALL | wxEXPAND, 5);

  bSizer14->Add(0, 0, 1, wxEXPAND, 5);

  bSizer121->Add(bSizer14, 1, wxALL | wxEXPAND, 5);

  bSizer11->Add(bSizer121, 1, wxALL | wxEXPAND, 5);

  bSizer11->Add(0, 0, 1, wxEXPAND, 5);

  m_raceinfo->SetSizer(bSizer11);
  m_raceinfo->Layout();
  bSizer11->Fit(m_raceinfo);
  m_notebook->AddPage(m_raceinfo, _("Info"), true);
  m_dcs = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                      wxTAB_TRAVERSAL);
  wxBoxSizer* bSizer51;
  bSizer51 = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer* fgSizer17;
  fgSizer17 = new wxFlexGridSizer(2, 1, 0, 0);
  fgSizer17->AddGrowableCol(0);
  fgSizer17->AddGrowableRow(0);
  fgSizer17->SetFlexibleDirection(wxBOTH);
  fgSizer17->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_ALL);

  m_pdclist = new wxListCtrl(m_dcs, wxID_ANY, wxDefaultPosition, wxSize(-1, -1),
                             wxLC_HRULES | wxLC_REPORT | wxHSCROLL | wxVSCROLL);
  fgSizer17->Add(m_pdclist, 0, wxALL | wxEXPAND, 5);

  wxFlexGridSizer* fgSizer18;
  fgSizer18 = new wxFlexGridSizer(1, 0, 0, 0);
  fgSizer18->AddGrowableCol(2);
  fgSizer18->SetFlexibleDirection(wxBOTH);
  fgSizer18->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_pbutton_download = new wxButton(m_dcs, wxID_ANY, _("&Download"),
                                    wxDefaultPosition, wxDefaultSize, 0);
  fgSizer18->Add(m_pbutton_download, 0, wxALL, 5);

  m_pbutton_upload = new wxButton(m_dcs, wxID_ANY, _("&Upload"),
                                  wxDefaultPosition, wxDefaultSize, 0);
  fgSizer18->Add(m_pbutton_upload, 0, wxALL, 5);

  m_pbutton_fromtrack = new wxButton(m_dcs, wxID_ANY, _("From &Track"),
                                     wxDefaultPosition, wxDefaultSize, 0);
  fgSizer18->Add(m_pbutton_fromtrack, 0, wxALL, 5);

  m_pbutton_totrack = new wxButton(m_dcs, wxID_ANY, _("To Track"),
                                   wxDefaultPosition, wxDefaultSize, 0);
  fgSizer18->Add(m_pbutton_totrack, 0, wxALL, 5);

  m_pbutton_modify = new wxButton(m_dcs, wxID_ANY, _("&Modify ..."),
                                  wxDefaultPosition, wxDefaultSize, 0);
  fgSizer18->Add(m_pbutton_modify, 0, wxALL, 5);

  m_pbutton_copydcs = new wxButton(m_dcs, wxID_ANY, _("Copy &DCs"),
                                   wxDefaultPosition, wxDefaultSize, 0);
  fgSizer18->Add(m_pbutton_copydcs, 0, wxALL, 5);

  fgSizer17->Add(fgSizer18, 1, wxALL | wxEXPAND, 5);

  bSizer51->Add(fgSizer17, 1, wxEXPAND | wxALL, 5);

  m_dcs->SetSizer(bSizer51);
  m_dcs->Layout();
  bSizer51->Fit(m_dcs);
  m_notebook->AddPage(m_dcs, _("DC list"), false);
  m_routing = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition,
                          wxDefaultSize, wxTAB_TRAVERSAL);
  wxBoxSizer* bSizer15;
  bSizer15 = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer* sbSizer6;
  sbSizer6 = new wxStaticBoxSizer(
      new wxStaticBox(m_routing, wxID_ANY, _("Position")), wxHORIZONTAL);

  wxFlexGridSizer* fgSizer107;
  fgSizer107 = new wxFlexGridSizer(0, 2, 0, 0);
  fgSizer107->AddGrowableCol(1);
  fgSizer107->SetFlexibleDirection(wxBOTH);
  fgSizer107->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_staticText171 =
      new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("Latitude"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText171->Wrap(-1);
  fgSizer107->Add(m_staticText171, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_latitude = new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("0"),
                                wxDefaultPosition, wxDefaultSize, 0);
  m_latitude->Wrap(-1);
  fgSizer107->Add(m_latitude, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_staticText173 =
      new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("Longitude"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText173->Wrap(-1);
  fgSizer107->Add(m_staticText173, 0, wxALL, 5);

  m_longitude = new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("0"),
                                 wxDefaultPosition, wxDefaultSize, 0);
  m_longitude->Wrap(-1);
  fgSizer107->Add(m_longitude, 0, wxALL, 5);

  m_staticText175 =
      new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("Course"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText175->Wrap(-1);
  fgSizer107->Add(m_staticText175, 0, wxALL, 5);

  m_course = new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("0"),
                              wxDefaultPosition, wxDefaultSize, 0);
  m_course->Wrap(-1);
  fgSizer107->Add(m_course, 0, wxALL, 5);

  m_staticText22 =
      new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("Speed"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText22->Wrap(-1);
  fgSizer107->Add(m_staticText22, 0, wxALL, 5);

  m_speed = new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("0"),
                             wxDefaultPosition, wxDefaultSize, 0);
  m_speed->Wrap(-1);
  fgSizer107->Add(m_speed, 0, wxALL, 5);

  sbSizer6->Add(fgSizer107, 1, wxALL | wxEXPAND, 5);

  wxFlexGridSizer* fgSizer19;
  fgSizer19 = new wxFlexGridSizer(0, 4, 0, 0);
  fgSizer19->SetFlexibleDirection(wxBOTH);
  fgSizer19->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_pbutton_tracking =
      new wxButton(sbSizer6->GetStaticBox(), wxID_ANY, _("Start tracking"),
                   wxDefaultPosition, wxDefaultSize, 0);
  fgSizer19->Add(m_pbutton_tracking, 0, wxALL, 5);

  m_staticText24 =
      new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("Interval"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText24->Wrap(-1);
  fgSizer19->Add(m_staticText24, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_pspin_updateinterval = new wxSpinCtrl(
      sbSizer6->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition,
      wxDefaultSize, wxSP_ARROW_KEYS, 5, 3600, 30);
  fgSizer19->Add(m_pspin_updateinterval, 0, wxALL, 5);

  m_staticText25 = new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("s"),
                                    wxDefaultPosition, wxDefaultSize, 0);
  m_staticText25->Wrap(-1);
  fgSizer19->Add(m_staticText25, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  sbSizer6->Add(fgSizer19, 1, wxEXPAND, 5);

  bSizer15->Add(sbSizer6, 1, wxEXPAND, 5);

  m_routing->SetSizer(bSizer15);
  m_routing->Layout();
  bSizer15->Fit(m_routing);
  m_notebook->AddPage(m_routing, _("Routing"), false);

  bSizer5->Add(m_notebook, 1, wxEXPAND | wxALL, 5);

  m_panel12->SetSizer(bSizer5);
  m_panel12->Layout();
  bSizer5->Fit(m_panel12);
  m_psplitter->SplitVertically(m_panel11, m_panel12, -1);
  bSizerMain->Add(m_psplitter, 1, wxEXPAND | wxALL, 5);

  this->SetSizer(bSizerMain);
  this->Layout();

  // Connect Events
  m_pracelist->Connect(
      wxEVT_LEFT_DCLICK,
      wxMouseEventHandler(SailonlinePanel::OnEditPositionClick), NULL, this);
  m_pracelist->Connect(wxEVT_LEFT_DOWN,
                       wxMouseEventHandler(SailonlinePanel::OnLeftUp), NULL,
                       this);
  m_pracelist->Connect(wxEVT_LEFT_UP,
                       wxMouseEventHandler(SailonlinePanel::OnLeftDown), NULL,
                       this);
  m_pracelist->Connect(wxEVT_COMMAND_LIST_KEY_DOWN,
                       wxListEventHandler(SailonlinePanel::OnPositionKeyDown),
                       NULL, this);
  m_pdclist->Connect(
      wxEVT_LEFT_DCLICK,
      wxMouseEventHandler(SailonlinePanel::OnEditConfigurationClick), NULL,
      this);
  m_pdclist->Connect(
      wxEVT_LEFT_DOWN,
      wxMouseEventHandler(SailonlinePanel::OnWeatherRoutesListLeftDown), NULL,
      this);
  m_pdclist->Connect(wxEVT_LEFT_UP,
                     wxMouseEventHandler(SailonlinePanel::OnLeftUp), NULL,
                     this);
  m_pdclist->Connect(wxEVT_COMMAND_LIST_COL_CLICK,
                     wxListEventHandler(SailonlinePanel::OnWeatherRouteSort),
                     NULL, this);
  m_pdclist->Connect(
      wxEVT_COMMAND_LIST_ITEM_DESELECTED,
      wxListEventHandler(SailonlinePanel::OnWeatherRouteSelected), NULL, this);
  m_pdclist->Connect(
      wxEVT_COMMAND_LIST_ITEM_SELECTED,
      wxListEventHandler(SailonlinePanel::OnWeatherRouteSelected), NULL, this);
  m_pdclist->Connect(wxEVT_COMMAND_LIST_KEY_DOWN,
                     wxListEventHandler(SailonlinePanel::OnWeatherRouteKeyDown),
                     NULL, this);
  m_pbutton_download->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlinePanel::OnDcDownload), NULL, this);
  m_pbutton_upload->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                            wxCommandEventHandler(SailonlinePanel::OnDcUpload),
                            NULL, this);
  m_pbutton_fromtrack->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlinePanel::OnDcFromTrack), NULL, this);
  m_pbutton_modify->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                            wxCommandEventHandler(SailonlinePanel::OnDcModify),
                            NULL, this);
}

SailonlinePanel::~SailonlinePanel() {
  // Disconnect Events
  m_pracelist->Disconnect(
      wxEVT_LEFT_DCLICK,
      wxMouseEventHandler(SailonlinePanel::OnEditPositionClick), NULL, this);
  m_pracelist->Disconnect(wxEVT_LEFT_DOWN,
                          wxMouseEventHandler(SailonlinePanel::OnLeftUp), NULL,
                          this);
  m_pracelist->Disconnect(wxEVT_LEFT_UP,
                          wxMouseEventHandler(SailonlinePanel::OnLeftDown),
                          NULL, this);
  m_pracelist->Disconnect(
      wxEVT_COMMAND_LIST_KEY_DOWN,
      wxListEventHandler(SailonlinePanel::OnPositionKeyDown), NULL, this);
  m_pdclist->Disconnect(
      wxEVT_LEFT_DCLICK,
      wxMouseEventHandler(SailonlinePanel::OnEditConfigurationClick), NULL,
      this);
  m_pdclist->Disconnect(
      wxEVT_LEFT_DOWN,
      wxMouseEventHandler(SailonlinePanel::OnWeatherRoutesListLeftDown), NULL,
      this);
  m_pdclist->Disconnect(wxEVT_LEFT_UP,
                        wxMouseEventHandler(SailonlinePanel::OnLeftUp), NULL,
                        this);
  m_pdclist->Disconnect(wxEVT_COMMAND_LIST_COL_CLICK,
                        wxListEventHandler(SailonlinePanel::OnWeatherRouteSort),
                        NULL, this);
  m_pdclist->Disconnect(
      wxEVT_COMMAND_LIST_ITEM_DESELECTED,
      wxListEventHandler(SailonlinePanel::OnWeatherRouteSelected), NULL, this);
  m_pdclist->Disconnect(
      wxEVT_COMMAND_LIST_ITEM_SELECTED,
      wxListEventHandler(SailonlinePanel::OnWeatherRouteSelected), NULL, this);
  m_pdclist->Disconnect(
      wxEVT_COMMAND_LIST_KEY_DOWN,
      wxListEventHandler(SailonlinePanel::OnWeatherRouteKeyDown), NULL, this);
  m_pbutton_download->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlinePanel::OnDcDownload), NULL, this);
  m_pbutton_upload->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlinePanel::OnDcUpload), NULL, this);
  m_pbutton_fromtrack->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlinePanel::OnDcFromTrack), NULL, this);
  m_pbutton_modify->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(SailonlinePanel::OnDcModify), NULL, this);
}

FromTrackDialogBase::FromTrackDialogBase(wxWindow* parent, wxWindowID id,
                                         const wxString& title,
                                         const wxPoint& pos, const wxSize& size,
                                         long style)
    : wxDialog(parent, id, title, pos, size, style) {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxFlexGridSizer* fgSizer95;
  fgSizer95 = new wxFlexGridSizer(0, 1, 0, 0);
  fgSizer95->SetFlexibleDirection(wxBOTH);
  fgSizer95->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  wxStaticBoxSizer* sbSizer29;
  sbSizer29 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Track")),
                                   wxVERTICAL);

  wxArrayString m_ptracklistChoices;
  m_ptracklist =
      new wxChoice(sbSizer29->GetStaticBox(), wxID_ANY, wxDefaultPosition,
                   wxDefaultSize, m_ptracklistChoices, 0);
  m_ptracklist->SetSelection(0);
  sbSizer29->Add(m_ptracklist, 1, wxALL | wxEXPAND, 5);

  fgSizer95->Add(sbSizer29, 1, wxEXPAND | wxALL, 5);

  wxStaticBoxSizer* sbSizer28;
  sbSizer28 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Filter")),
                                   wxVERTICAL);

  m_pfilter = new wxTextCtrl(sbSizer28->GetStaticBox(), wxID_ANY, wxEmptyString,
                             wxDefaultPosition, wxDefaultSize, 0);
  sbSizer28->Add(m_pfilter, 1, wxALL | wxEXPAND, 5);

  fgSizer95->Add(sbSizer28, 1, wxEXPAND | wxALL, 5);

  wxFlexGridSizer* fgSizer99;
  fgSizer99 = new wxFlexGridSizer(0, 2, 0, 0);
  fgSizer99->SetFlexibleDirection(wxBOTH);
  fgSizer99->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_button48 = new wxButton(this, wxID_ANY, _("Reset All"), wxDefaultPosition,
                            wxDefaultSize, 0);
  fgSizer99->Add(m_button48, 0, wxALL, 5);

  m_pbutton_fromtrack_done = new wxButton(this, wxID_ANY, _("Done"),
                                          wxDefaultPosition, wxDefaultSize, 0);
  fgSizer99->Add(m_pbutton_fromtrack_done, 0, wxALL, 5);

  fgSizer95->Add(fgSizer99, 1, wxEXPAND | wxALL, 5);

  this->SetSizer(fgSizer95);
  this->Layout();
  fgSizer95->Fit(this);

  this->Centre(wxBOTH);

  // Connect Events
  m_pfilter->Connect(wxEVT_COMMAND_TEXT_UPDATED,
                     wxCommandEventHandler(FromTrackDialogBase::OnFilterText),
                     NULL, this);
  m_button48->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                      wxCommandEventHandler(FromTrackDialogBase::OnResetAll),
                      NULL, this);
  m_pbutton_fromtrack_done->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(FromTrackDialogBase::OnFromTrackDone), NULL, this);
}

FromTrackDialogBase::~FromTrackDialogBase() {
  // Disconnect Events
  m_pfilter->Disconnect(
      wxEVT_COMMAND_TEXT_UPDATED,
      wxCommandEventHandler(FromTrackDialogBase::OnFilterText), NULL, this);
  m_button48->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                         wxCommandEventHandler(FromTrackDialogBase::OnResetAll),
                         NULL, this);
  m_pbutton_fromtrack_done->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(FromTrackDialogBase::OnFromTrackDone), NULL, this);
}

DcModifyDialogBase::DcModifyDialogBase(wxWindow* parent, wxWindowID id,
                                       const wxString& title,
                                       const wxPoint& pos, const wxSize& size,
                                       long style)
    : wxDialog(parent, id, title, pos, size, style) {
  this->SetSizeHints(wxSize(-1, -1), wxDefaultSize);

  wxFlexGridSizer* fgSizer92;
  fgSizer92 = new wxFlexGridSizer(0, 1, 0, 0);
  fgSizer92->AddGrowableCol(0);
  fgSizer92->AddGrowableRow(0);
  fgSizer92->SetFlexibleDirection(wxBOTH);
  fgSizer92->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  wxFlexGridSizer* fgSizer18;
  fgSizer18 = new wxFlexGridSizer(0, 1, 0, 0);
  fgSizer18->SetFlexibleDirection(wxBOTH);
  fgSizer18->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_NONE);

  wxStaticBoxSizer* sbSizer6;
  sbSizer6 = new wxStaticBoxSizer(
      new wxStaticBox(this, wxID_ANY, _("Simplification")), wxVERTICAL);

  wxFlexGridSizer* fgSizer181;
  fgSizer181 = new wxFlexGridSizer(0, 1, 0, 0);
  fgSizer181->SetFlexibleDirection(wxBOTH);
  fgSizer181->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_pcheck_mergedcs =
      new wxCheckBox(sbSizer6->GetStaticBox(), wxID_ANY, _("Merge similar DCs"),
                     wxDefaultPosition, wxDefaultSize, 0);
  fgSizer181->Add(m_pcheck_mergedcs, 0, wxALL, 5);

  wxGridSizer* gSizer1;
  gSizer1 = new wxGridSizer(0, 3, 0, 0);

  m_staticText19 = new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY,
                                    _("Max. course delta"), wxDefaultPosition,
                                    wxDefaultSize, 0);
  m_staticText19->Wrap(-1);
  gSizer1->Add(m_staticText19, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_pspin_merge_course_delta = new wxSpinCtrl(
      sbSizer6->GetStaticBox(), wxID_ANY, wxT("2"), wxDefaultPosition,
      wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 0);
  gSizer1->Add(m_pspin_merge_course_delta, 0, wxALL, 5);

  m_staticText20 =
      new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("degrees"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText20->Wrap(-1);
  gSizer1->Add(m_staticText20, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_staticText21 =
      new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("Max. TWA delta"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText21->Wrap(-1);
  gSizer1->Add(m_staticText21, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_pspin_merge_twa_delta = new wxSpinCtrl(
      sbSizer6->GetStaticBox(), wxID_ANY, wxT("1"), wxDefaultPosition,
      wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 0);
  gSizer1->Add(m_pspin_merge_twa_delta, 0, wxALL, 5);

  m_staticText22 =
      new wxStaticText(sbSizer6->GetStaticBox(), wxID_ANY, _("degrees"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText22->Wrap(-1);
  gSizer1->Add(m_staticText22, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  fgSizer181->Add(gSizer1, 1, wxEXPAND | wxLEFT, 25);

  sbSizer6->Add(fgSizer181, 1, wxEXPAND, 5);

  fgSizer18->Add(sbSizer6, 1, wxEXPAND, 5);

  wxStaticBoxSizer* sbSizer7;
  sbSizer7 = new wxStaticBoxSizer(
      new wxStaticBox(this, wxID_ANY, _("Optimization")), wxVERTICAL);

  m_pcheck_optimize_maneuvers = new wxCheckBox(
      sbSizer7->GetStaticBox(), wxID_ANY, _("Optimize tacks and gybes"),
      wxDefaultPosition, wxDefaultSize, 0);
  m_pcheck_optimize_maneuvers->SetValue(true);
  sbSizer7->Add(m_pcheck_optimize_maneuvers, 0, wxALL, 5);

  m_pcheck_twa_near_limit = new wxCheckBox(sbSizer7->GetStaticBox(), wxID_ANY,
                                           _("Use TWA near limit angles"),
                                           wxDefaultPosition, wxDefaultSize, 0);
  m_pcheck_twa_near_limit->SetValue(true);
  sbSizer7->Add(m_pcheck_twa_near_limit, 0, wxALL, 5);

  wxGridSizer* gSizer2;
  gSizer2 = new wxGridSizer(0, 3, 0, 0);

  m_staticText23 = new wxStaticText(sbSizer7->GetStaticBox(), wxID_ANY,
                                    _("Max. deviation from limit"),
                                    wxDefaultPosition, wxDefaultSize, 0);
  m_staticText23->Wrap(-1);
  gSizer2->Add(m_staticText23, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_pspin_twa_deviation = new wxSpinCtrl(
      sbSizer7->GetStaticBox(), wxID_ANY, wxT("2"), wxDefaultPosition,
      wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 0);
  gSizer2->Add(m_pspin_twa_deviation, 0, wxALL, 5);

  m_staticText24 =
      new wxStaticText(sbSizer7->GetStaticBox(), wxID_ANY, _("degrees"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText24->Wrap(-1);
  gSizer2->Add(m_staticText24, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  sbSizer7->Add(gSizer2, 1, wxEXPAND | wxLEFT, 25);

  fgSizer18->Add(sbSizer7, 1, wxEXPAND, 5);

  fgSizer92->Add(fgSizer18, 1, wxEXPAND, 5);

  m_sdbSizer1 = new wxStdDialogButtonSizer();
  m_sdbSizer1OK = new wxButton(this, wxID_OK);
  m_sdbSizer1->AddButton(m_sdbSizer1OK);
  m_sdbSizer1Apply = new wxButton(this, wxID_APPLY);
  m_sdbSizer1->AddButton(m_sdbSizer1Apply);
  m_sdbSizer1Cancel = new wxButton(this, wxID_CANCEL);
  m_sdbSizer1->AddButton(m_sdbSizer1Cancel);
  m_sdbSizer1->Realize();

  fgSizer92->Add(m_sdbSizer1, 1, wxEXPAND | wxALL, 5);

  this->SetSizer(fgSizer92);
  this->Layout();
  fgSizer92->Fit(this);

  this->Centre(wxBOTH);

  // Connect Events
  m_pcheck_optimize_maneuvers->Connect(
      wxEVT_COMMAND_CHECKBOX_CLICKED,
      wxCommandEventHandler(DcModifyDialogBase::OnUpdate), NULL, this);
  m_sdbSizer1Apply->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(DcModifyDialogBase::OnModifyApply), NULL, this);
  m_sdbSizer1Cancel->Connect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(DcModifyDialogBase::OnModifyCancel), NULL, this);
  m_sdbSizer1OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                         wxCommandEventHandler(DcModifyDialogBase::OnModifyOk),
                         NULL, this);
}

DcModifyDialogBase::~DcModifyDialogBase() {
  // Disconnect Events
  m_pcheck_optimize_maneuvers->Disconnect(
      wxEVT_COMMAND_CHECKBOX_CLICKED,
      wxCommandEventHandler(DcModifyDialogBase::OnUpdate), NULL, this);
  m_sdbSizer1Apply->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(DcModifyDialogBase::OnModifyApply), NULL, this);
  m_sdbSizer1Cancel->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(DcModifyDialogBase::OnModifyCancel), NULL, this);
  m_sdbSizer1OK->Disconnect(
      wxEVT_COMMAND_BUTTON_CLICKED,
      wxCommandEventHandler(DcModifyDialogBase::OnModifyOk), NULL, this);
}

AboutDialogBase::AboutDialogBase(wxWindow* parent, wxWindowID id,
                                 const wxString& title, const wxPoint& pos,
                                 const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxFlexGridSizer* fgSizer90;
  fgSizer90 = new wxFlexGridSizer(0, 1, 0, 0);
  fgSizer90->SetFlexibleDirection(wxBOTH);
  fgSizer90->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  wxFlexGridSizer* fgSizer109;
  fgSizer109 = new wxFlexGridSizer(0, 2, 0, 0);
  fgSizer109->SetFlexibleDirection(wxBOTH);
  fgSizer109->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_staticText135 =
      new wxStaticText(this, wxID_ANY, _("Sailonline Plugin Version"),
                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticText135->Wrap(-1);
  fgSizer109->Add(m_staticText135, 0, wxALL, 5);

  m_stVersion = new wxStaticText(this, wxID_ANY, wxEmptyString,
                                 wxDefaultPosition, wxDefaultSize, 0);
  m_stVersion->Wrap(-1);
  fgSizer109->Add(m_stVersion, 0, wxALL, 5);

  fgSizer90->Add(fgSizer109, 1, wxEXPAND, 5);

  m_staticText110 = new wxStaticText(
      this, wxID_ANY,
      _("The sailonline plugin for opencpn is intended to be used with "
        "sailonline.org. It has the aim to simplify the management of "
        "races.\n\nLicense: GPLv3+\n\nSource "
        "Code:\nhttps://github.com/jrheinlaender/sailonline_pi\n\nAuthor:\nJan "
        "Rheinländer\n\nMany thanks to all of the translators and testers."),
      wxDefaultPosition, wxDefaultSize, 0);
  m_staticText110->Wrap(400);
  fgSizer90->Add(m_staticText110, 0, wxALL, 5);

  wxFlexGridSizer* fgSizer91;
  fgSizer91 = new wxFlexGridSizer(0, 2, 0, 0);
  fgSizer91->SetFlexibleDirection(wxBOTH);
  fgSizer91->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_bClose = new wxButton(this, wxID_ANY, _("Close"), wxDefaultPosition,
                          wxDefaultSize, 0);
  fgSizer91->Add(m_bClose, 0, wxALL, 5);

  fgSizer90->Add(fgSizer91, 1, wxEXPAND, 5);

  this->SetSizer(fgSizer90);
  this->Layout();
  fgSizer90->Fit(this);

  this->Centre(wxBOTH);

  // Connect Events
  m_bClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                    wxCommandEventHandler(AboutDialogBase::OnClose), NULL,
                    this);
}

AboutDialogBase::~AboutDialogBase() {
  // Disconnect Events
  m_bClose->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler(AboutDialogBase::OnClose), NULL,
                       this);
}
