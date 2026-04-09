///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "SailonlineUiBase.h"

///////////////////////////////////////////////////////////////////////////

SailonlineUiBase::SailonlineUiBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,450 ), wxDefaultSize );

	m_menubar3 = new wxMenuBar( 0 );
	m_menubar3->Hide();

	m_mFile = new wxMenu();
	wxMenuItem* m_mOpen;
	m_mOpen = new wxMenuItem( m_mFile, wxID_ANY, wxString( _("&Open") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_mOpen );

	wxMenuItem* m_mSave;
	m_mSave = new wxMenuItem( m_mFile, wxID_ANY, wxString( _("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_mSave );

	m_mFile->AppendSeparator();

	wxMenuItem* m_mClose;
	m_mClose = new wxMenuItem( m_mFile, wxID_ANY, wxString( _("&Close") ) + wxT('\t') + wxT("Ctrl+W"), wxEmptyString, wxITEM_NORMAL );
	m_mFile->Append( m_mClose );

	m_menubar3->Append( m_mFile, _("&File") );

	m_mPosition = new wxMenu();
	wxMenuItem* m_mNewPosition;
	m_mNewPosition = new wxMenuItem( m_mPosition, wxID_ANY, wxString( _("&New Position") ) , wxEmptyString, wxITEM_NORMAL );
	m_mPosition->Append( m_mNewPosition );

	wxMenuItem* m_mUpdateBoat;
	m_mUpdateBoat = new wxMenuItem( m_mPosition, wxID_ANY, wxString( _("&Update Boat Position") ) + wxT('\t') + wxT("Ctrl+U"), wxEmptyString, wxITEM_NORMAL );
	m_mPosition->Append( m_mUpdateBoat );

	wxMenuItem* m_mDeletePosition;
	m_mDeletePosition = new wxMenuItem( m_mPosition, wxID_ANY, wxString( _("&Delete") ) , wxEmptyString, wxITEM_NORMAL );
	m_mPosition->Append( m_mDeletePosition );

	wxMenuItem* m_mDeleteAllPositions;
	m_mDeleteAllPositions = new wxMenuItem( m_mPosition, wxID_ANY, wxString( _("Delete &All") ) , wxEmptyString, wxITEM_NORMAL );
	m_mPosition->Append( m_mDeleteAllPositions );

	m_menubar3->Append( m_mPosition, _("&Position") );

	m_mConfiguration = new wxMenu();
	wxMenuItem* m_mNew;
	m_mNew = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mNew );

	m_mBatch = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&Batch") ) + wxT('\t') + wxT("Ctrl+B"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mBatch );

	m_mEdit = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&Edit") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mEdit );

	m_mGoTo = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&GoTo") ) + wxT('\t') + wxT("Ctrl+G"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mGoTo );

	m_mDelete = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&Delete") ) + wxT('\t') + wxT("Ctrl+D"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mDelete );

	m_mDeleteAll = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Delete All") ) , wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mDeleteAll );

	m_mConfiguration->AppendSeparator();

	m_mCompute = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&Compute") ) + wxT('\t') + wxT("Ctrl+C"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mCompute );

	m_mComputeAll = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Compute &All") ) + wxT('\t') + wxT("Ctrl+A"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mComputeAll );

	m_mStop = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&Stop") ) + wxT('\t') + wxT("Ctrl+Z"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mStop );

	wxMenuItem* m_mResetAll;
	m_mResetAll = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&Reset All") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mResetAll );

	m_mConfiguration->AppendSeparator();

	m_mExport = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("E&xport") ) + wxT('\t') + wxT("Ctrl+X"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mExport );

	m_mExportAll = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("Export All") ) , wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mExportAll );

	m_mConfiguration->AppendSeparator();

	wxMenuItem* m_mFilter;
	m_mFilter = new wxMenuItem( m_mConfiguration, wxID_ANY, wxString( _("&Filter") ) + wxT('\t') + wxT("Ctrl+F"), wxEmptyString, wxITEM_NORMAL );
	m_mConfiguration->Append( m_mFilter );

	m_menubar3->Append( m_mConfiguration, _("&Configuration") );

	m_mView = new wxMenu();
	wxMenuItem* m_mSettings;
	m_mSettings = new wxMenuItem( m_mView, wxID_ANY, wxString( _("&Settings") ) , wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mSettings );

	wxMenuItem* m_mStatistics;
	m_mStatistics = new wxMenuItem( m_mView, wxID_ANY, wxString( _("S&tatistics") ) + wxT('\t') + wxT("Ctrl+T"), wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mStatistics );

	wxMenuItem* m_mReport;
	m_mReport = new wxMenuItem( m_mView, wxID_ANY, wxString( _("&Report") ) , wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mReport );

	wxMenuItem* m_mPlot;
	m_mPlot = new wxMenuItem( m_mView, wxID_ANY, wxString( _("&Plot") ) + wxT('\t') + wxT("Ctrl+P"), wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mPlot );

	wxMenuItem* m_mCursorPosition;
	m_mCursorPosition = new wxMenuItem( m_mView, wxID_ANY, wxString( _("&Cursor Position") ) , wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mCursorPosition );

	wxMenuItem* m_mRoutePosition;
	m_mRoutePosition = new wxMenuItem( m_mView, wxID_ANY, wxString( _("&Route Position") ) , wxEmptyString, wxITEM_NORMAL );
	m_mView->Append( m_mRoutePosition );

	m_menubar3->Append( m_mView, _("&View") );

	m_mHelp = new wxMenu();
	wxMenuItem* m_mInformation;
	m_mInformation = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("&Information") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mInformation );

	wxMenuItem* m_mManual;
	m_mManual = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("&Manual") ) , wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mManual );

	wxMenuItem* m_mAbout;
	m_mAbout = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("&About") ) , wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mAbout );

	m_menubar3->Append( m_mHelp, _("&Help") );

	this->SetMenuBar( m_menubar3 );

	m_mContextMenu = new wxMenu();
	wxMenuItem* m_mNew1;
	m_mNew1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("&New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mNew1 );

	m_mEdit1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("&Edit") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mEdit1 );

	m_mCompute1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("&Compute") ) + wxT('\t') + wxT("Ctrl+C"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mCompute1 );

	m_mComputeAll1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("Compute &All") ) + wxT('\t') + wxT("Ctrl+A"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mComputeAll1 );

	m_mDelete1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("&Delete") ) + wxT('\t') + wxT("Ctrl+D"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mDelete1 );

	m_mContextMenu->AppendSeparator();

	m_mGoTo1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("&GoTo") ) + wxT('\t') + wxT("Ctrl+G"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mGoTo1 );

	m_mStop1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("&Stop") ) + wxT('\t') + wxT("Ctrl+Z"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mStop1 );

	m_mBatch1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("&Batch") ) + wxT('\t') + wxT("Ctrl+B"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mBatch1 );

	wxMenuItem* m_mResetAll1;
	m_mResetAll1 = new wxMenuItem( m_mContextMenu, wxID_ANY, wxString( _("&Reset All") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_NORMAL );
	m_mContextMenu->Append( m_mResetAll1 );

	m_menu1 = new wxMenu();
	wxMenuItem* m_menu1Item = new wxMenuItem( m_mContextMenu, wxID_ANY, _("View"), wxEmptyString, wxITEM_NORMAL, m_menu1 );
	wxMenuItem* m_mSettings1;
	m_mSettings1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("&Settings") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_mSettings1 );

	wxMenuItem* m_mStatistics1;
	m_mStatistics1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("S&tatistics") ) + wxT('\t') + wxT("Ctrl+T"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_mStatistics1 );

	wxMenuItem* m_mReport1;
	m_mReport1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("&Report") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_mReport1 );

	wxMenuItem* m_mPlot1;
	m_mPlot1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("&Plot") ) + wxT('\t') + wxT("Ctrl+P"), wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_mPlot1 );

	wxMenuItem* m_mCursorPosition1;
	m_mCursorPosition1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("&Cursor Position") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_mCursorPosition1 );

	wxMenuItem* m_mRoutePosition1;
	m_mRoutePosition1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("&Route Position") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_mRoutePosition1 );

	m_mContextMenu->Append( m_menu1Item );

	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( SailonlineUiBase::SailonlineUiBaseOnContextMenu ), NULL, this );


	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SailonlineUiBase::OnClose ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( SailonlineUiBase::OnSize ) );
	m_mFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnOpen ), this, m_mOpen->GetId());
	m_mFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnSave ), this, m_mSave->GetId());
	m_mFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnClose ), this, m_mClose->GetId());
	m_mPosition->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnNewPosition ), this, m_mNewPosition->GetId());
	m_mPosition->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnUpdateBoat ), this, m_mUpdateBoat->GetId());
	m_mPosition->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnDeletePosition ), this, m_mDeletePosition->GetId());
	m_mPosition->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnDeleteAllPositions ), this, m_mDeleteAllPositions->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnNew ), this, m_mNew->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnBatch ), this, m_mBatch->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnEditConfiguration ), this, m_mEdit->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnGoTo ), this, m_mGoTo->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnDelete ), this, m_mDelete->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnDeleteAll ), this, m_mDeleteAll->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnCompute ), this, m_mCompute->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnComputeAll ), this, m_mComputeAll->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnStop ), this, m_mStop->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnResetAll ), this, m_mResetAll->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnExport ), this, m_mExport->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnExportAll ), this, m_mExportAll->GetId());
	m_mConfiguration->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnFilter ), this, m_mFilter->GetId());
	m_mView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnSettings ), this, m_mSettings->GetId());
	m_mView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnStatistics ), this, m_mStatistics->GetId());
	m_mView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnReport ), this, m_mReport->GetId());
	m_mView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnPlot ), this, m_mPlot->GetId());
	m_mView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnCursorPosition ), this, m_mCursorPosition->GetId());
	m_mView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnRoutePosition ), this, m_mRoutePosition->GetId());
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnInformation ), this, m_mInformation->GetId());
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnManual ), this, m_mManual->GetId());
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnAbout ), this, m_mAbout->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnNew ), this, m_mNew1->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnEditConfiguration ), this, m_mEdit1->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnCompute ), this, m_mCompute1->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnComputeAll ), this, m_mComputeAll1->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnDelete ), this, m_mDelete1->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnGoTo ), this, m_mGoTo1->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnStop ), this, m_mStop1->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnBatch ), this, m_mBatch1->GetId());
	m_mContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnResetAll ), this, m_mResetAll1->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnSettings ), this, m_mSettings1->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnStatistics ), this, m_mStatistics1->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnReport ), this, m_mReport1->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnPlot ), this, m_mPlot1->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnCursorPosition ), this, m_mCursorPosition1->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SailonlineUiBase::OnRoutePosition ), this, m_mRoutePosition1->GetId());
}

SailonlineUiBase::~SailonlineUiBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SailonlineUiBase::OnClose ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( SailonlineUiBase::OnSize ) );

	delete m_mContextMenu;
}

SailonlinePanel::SailonlinePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	this->SetMinSize( wxSize( 100,100 ) );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	m_psplitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_psplitter->SetSashGravity( 0.2 );
	m_psplitter->SetMinimumPaneSize( 1 );

	m_panel11 = new wxPanel( m_psplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer30;
	sbSizer30 = new wxStaticBoxSizer( new wxStaticBox( m_panel11, wxID_ANY, _("Races") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer93;
	fgSizer93 = new wxFlexGridSizer( 1, 1, 0, 0 );
	fgSizer93->AddGrowableCol( 0 );
	fgSizer93->AddGrowableRow( 0 );
	fgSizer93->SetFlexibleDirection( wxBOTH );
	fgSizer93->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_pracelist = new wxListCtrl( sbSizer30->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxHSCROLL|wxVSCROLL );
	fgSizer93->Add( m_pracelist, 0, wxALL|wxEXPAND, 5 );


	sbSizer30->Add( fgSizer93, 1, wxEXPAND|wxALL, 5 );


	bSizer6->Add( sbSizer30, 1, wxEXPAND|wxALL, 5 );


	m_panel11->SetSizer( bSizer6 );
	m_panel11->Layout();
	bSizer6->Fit( m_panel11 );
	m_panel12 = new wxPanel( m_psplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel12->Hide();

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_notebook = new wxNotebook( m_panel12, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_racedata = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* m_psizer_racedata;
	m_psizer_racedata = new wxFlexGridSizer( 3, 1, 0, 0 );
	m_psizer_racedata->SetFlexibleDirection( wxBOTH );
	m_psizer_racedata->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_racename = new wxStaticText( m_racedata, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_racename->Wrap( -1 );
	m_racename->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	m_psizer_racedata->Add( m_racename, 0, wxALL, 5 );

	m_racemsg = new wxStaticText( m_racedata, wxID_ANY, _("Message"), wxDefaultPosition, wxDefaultSize, 0 );
	m_racemsg->Wrap( -1 );
	m_psizer_racedata->Add( m_racemsg, 0, wxALL, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer106;
	fgSizer106 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer106->SetFlexibleDirection( wxBOTH );
	fgSizer106->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_racedesc = new wxHtmlWindow( m_racedata, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxHW_SCROLLBAR_AUTO );
	fgSizer106->Add( m_racedesc, 100, wxALL|wxEXPAND, 5 );


	bSizer12->Add( fgSizer106, 1, wxALL|wxEXPAND, 5 );


	m_psizer_racedata->Add( bSizer12, 1, wxALL|wxEXPAND, 5 );


	bSizer8->Add( m_psizer_racedata, 1, wxALL|wxEXPAND, 5 );


	m_racedata->SetSizer( bSizer8 );
	m_racedata->Layout();
	bSizer8->Fit( m_racedata );
	m_notebook->AddPage( m_racedata, _("Race"), false );
	m_raceinfo = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText168 = new wxStaticText( m_raceinfo, wxID_ANY, _("Polar"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText168->Wrap( -1 );
	m_staticText168->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer16->Add( m_staticText168, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_polarname = new wxStaticText( m_raceinfo, wxID_ANY, _("not loaded"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_polarname->Wrap( -1 );
	bSizer16->Add( m_polarname, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_pbutton_downloadpolar = new wxButton( m_raceinfo, wxID_ANY, _("Download"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_pbutton_downloadpolar, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizer11->Add( bSizer16, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );

	m_staticText170 = new wxStaticText( m_raceinfo, wxID_ANY, _("Waypoints"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText170->Wrap( -1 );
	m_staticText170->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer121->Add( m_staticText170, 0, wxALL, 5 );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	m_pwaypointlist = new wxListCtrl( m_raceinfo, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SORT_ASCENDING|wxHSCROLL|wxVSCROLL );
	bSizer14->Add( m_pwaypointlist, 1, wxALL|wxEXPAND, 5 );


	bSizer14->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizer121->Add( bSizer14, 1, wxALL|wxEXPAND, 5 );


	bSizer11->Add( bSizer121, 1, wxALL|wxEXPAND, 5 );


	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );


	m_raceinfo->SetSizer( bSizer11 );
	m_raceinfo->Layout();
	bSizer11->Fit( m_raceinfo );
	m_notebook->AddPage( m_raceinfo, _("Info"), true );
	m_dcs = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer17->AddGrowableCol( 0 );
	fgSizer17->AddGrowableRow( 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	m_pdclist = new wxListCtrl( m_dcs, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_HRULES|wxLC_REPORT|wxHSCROLL|wxVSCROLL );
	fgSizer17->Add( m_pdclist, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer18->AddGrowableCol( 2 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_pbutton_download = new wxButton( m_dcs, wxID_ANY, _("&Download"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pbutton_download, 0, wxALL, 5 );

	m_pbutton_upload = new wxButton( m_dcs, wxID_ANY, _("&Upload"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pbutton_upload, 0, wxALL, 5 );

	m_pbutton_fromtrack = new wxButton( m_dcs, wxID_ANY, _("From &Track"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pbutton_fromtrack, 0, wxALL, 5 );

	m_pbutton_totrack = new wxButton( m_dcs, wxID_ANY, _("To Track"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pbutton_totrack, 0, wxALL, 5 );

	m_pbutton_modify = new wxButton( m_dcs, wxID_ANY, _("&Modify ..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pbutton_modify, 0, wxALL, 5 );

	m_pbutton_copydcs = new wxButton( m_dcs, wxID_ANY, _("Copy &DCs"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_pbutton_copydcs, 0, wxALL, 5 );


	fgSizer17->Add( fgSizer18, 1, wxALL|wxEXPAND, 5 );


	bSizer51->Add( fgSizer17, 1, wxEXPAND|wxALL, 5 );


	m_dcs->SetSizer( bSizer51 );
	m_dcs->Layout();
	bSizer51->Fit( m_dcs );
	m_notebook->AddPage( m_dcs, _("DC list"), false );
	m_routing = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer107;
	fgSizer107 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer107->AddGrowableCol( 1 );
	fgSizer107->SetFlexibleDirection( wxBOTH );
	fgSizer107->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText171 = new wxStaticText( m_routing, wxID_ANY, _("Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText171->Wrap( -1 );
	fgSizer107->Add( m_staticText171, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_latitude = new wxStaticText( m_routing, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_latitude->Wrap( -1 );
	fgSizer107->Add( m_latitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_pbutton_updateposition = new wxButton( m_routing, wxID_ANY, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer107->Add( m_pbutton_updateposition, 0, wxALL, 5 );

	m_staticText173 = new wxStaticText( m_routing, wxID_ANY, _("Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText173->Wrap( -1 );
	fgSizer107->Add( m_staticText173, 0, wxALL, 5 );

	m_longitude = new wxStaticText( m_routing, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_longitude->Wrap( -1 );
	fgSizer107->Add( m_longitude, 0, wxALL, 5 );


	fgSizer107->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText175 = new wxStaticText( m_routing, wxID_ANY, _("Course"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText175->Wrap( -1 );
	fgSizer107->Add( m_staticText175, 0, wxALL, 5 );

	m_course = new wxStaticText( m_routing, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_course->Wrap( -1 );
	fgSizer107->Add( m_course, 0, wxALL, 5 );


	bSizer15->Add( fgSizer107, 1, wxALL|wxEXPAND, 5 );


	m_routing->SetSizer( bSizer15 );
	m_routing->Layout();
	bSizer15->Fit( m_routing );
	m_notebook->AddPage( m_routing, _("Routing"), false );

	bSizer5->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );


	m_panel12->SetSizer( bSizer5 );
	m_panel12->Layout();
	bSizer5->Fit( m_panel12 );
	m_psplitter->SplitVertically( m_panel11, m_panel12, -1 );
	bSizerMain->Add( m_psplitter, 1, wxEXPAND|wxALL, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();

	// Connect Events
	m_pracelist->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( SailonlinePanel::OnEditPositionClick ), NULL, this );
	m_pracelist->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( SailonlinePanel::OnLeftUp ), NULL, this );
	m_pracelist->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( SailonlinePanel::OnLeftDown ), NULL, this );
	m_pracelist->Connect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( SailonlinePanel::OnPositionKeyDown ), NULL, this );
	m_pdclist->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( SailonlinePanel::OnEditConfigurationClick ), NULL, this );
	m_pdclist->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( SailonlinePanel::OnWeatherRoutesListLeftDown ), NULL, this );
	m_pdclist->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( SailonlinePanel::OnLeftUp ), NULL, this );
	m_pdclist->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( SailonlinePanel::OnWeatherRouteSort ), NULL, this );
	m_pdclist->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( SailonlinePanel::OnWeatherRouteSelected ), NULL, this );
	m_pdclist->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( SailonlinePanel::OnWeatherRouteSelected ), NULL, this );
	m_pdclist->Connect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( SailonlinePanel::OnWeatherRouteKeyDown ), NULL, this );
	m_pbutton_download->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SailonlinePanel::OnDcDownload ), NULL, this );
	m_pbutton_upload->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SailonlinePanel::OnDcUpload ), NULL, this );
	m_pbutton_fromtrack->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SailonlinePanel::OnDcFromTrack ), NULL, this );
	m_pbutton_modify->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SailonlinePanel::OnDcModify ), NULL, this );
}

SailonlinePanel::~SailonlinePanel()
{
	// Disconnect Events
	m_pracelist->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( SailonlinePanel::OnEditPositionClick ), NULL, this );
	m_pracelist->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( SailonlinePanel::OnLeftUp ), NULL, this );
	m_pracelist->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( SailonlinePanel::OnLeftDown ), NULL, this );
	m_pracelist->Disconnect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( SailonlinePanel::OnPositionKeyDown ), NULL, this );
	m_pdclist->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( SailonlinePanel::OnEditConfigurationClick ), NULL, this );
	m_pdclist->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( SailonlinePanel::OnWeatherRoutesListLeftDown ), NULL, this );
	m_pdclist->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( SailonlinePanel::OnLeftUp ), NULL, this );
	m_pdclist->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( SailonlinePanel::OnWeatherRouteSort ), NULL, this );
	m_pdclist->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( SailonlinePanel::OnWeatherRouteSelected ), NULL, this );
	m_pdclist->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( SailonlinePanel::OnWeatherRouteSelected ), NULL, this );
	m_pdclist->Disconnect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( SailonlinePanel::OnWeatherRouteKeyDown ), NULL, this );
	m_pbutton_download->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SailonlinePanel::OnDcDownload ), NULL, this );
	m_pbutton_upload->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SailonlinePanel::OnDcUpload ), NULL, this );
	m_pbutton_fromtrack->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SailonlinePanel::OnDcFromTrack ), NULL, this );
	m_pbutton_modify->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SailonlinePanel::OnDcModify ), NULL, this );

}

SettingsDialogBase::SettingsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	wxFlexGridSizer* fgSizer92;
	fgSizer92 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer92->AddGrowableCol( 0 );
	fgSizer92->AddGrowableRow( 0 );
	fgSizer92->SetFlexibleDirection( wxBOTH );
	fgSizer92->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer100;
	fgSizer100 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer100->AddGrowableCol( 0 );
	fgSizer100->AddGrowableCol( 1 );
	fgSizer100->AddGrowableRow( 0 );
	fgSizer100->SetFlexibleDirection( wxBOTH );
	fgSizer100->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer25;
	sbSizer25 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Display") ), wxVERTICAL );

	sbSizer25->SetMinSize( wxSize( 375,-1 ) );
	m_scrolledWindow4 = new wxScrolledWindow( sbSizer25->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxVSCROLL );
	m_scrolledWindow4->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );

	wxFlexGridSizer* fgSizer42;
	fgSizer42 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer42->SetFlexibleDirection( wxBOTH );
	fgSizer42->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText74 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("Cursor Route Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	fgSizer42->Add( m_staticText74, 0, wxALL, 5 );

	m_cpCursorRoute = new wxColourPickerCtrl( m_scrolledWindow4, wxID_ANY, wxColour( 255, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer42->Add( m_cpCursorRoute, 0, wxALL, 5 );

	m_staticText73 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("Destination Route Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer42->Add( m_staticText73, 0, wxALL, 5 );

	m_cpDestinationRoute = new wxColourPickerCtrl( m_scrolledWindow4, wxID_ANY, wxColour( 255, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer42->Add( m_cpDestinationRoute, 0, wxALL, 5 );

	m_staticText75 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("Route Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer42->Add( m_staticText75, 0, wxALL, 5 );

	m_sRouteThickness = new wxSpinCtrl( m_scrolledWindow4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 4 );
	m_sRouteThickness->SetMaxSize( wxSize( 140,-1 ) );

	fgSizer42->Add( m_sRouteThickness, 0, wxALL|wxEXPAND, 5 );

	m_staticText70 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("Iso Chron Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText70->Wrap( -1 );
	fgSizer42->Add( m_staticText70, 0, wxALL, 5 );

	m_sIsoChronThickness = new wxSpinCtrl( m_scrolledWindow4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 2 );
	m_sIsoChronThickness->SetMaxSize( wxSize( 140,-1 ) );

	fgSizer42->Add( m_sIsoChronThickness, 0, wxALL|wxEXPAND, 5 );

	m_staticText71 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("Alternate Routes Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer42->Add( m_staticText71, 0, wxALL, 5 );

	m_sAlternateRouteThickness = new wxSpinCtrl( m_scrolledWindow4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_sAlternateRouteThickness->SetMaxSize( wxSize( 140,-1 ) );

	fgSizer42->Add( m_sAlternateRouteThickness, 0, wxALL|wxEXPAND, 5 );

	m_staticText711 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("Wind Barbs On Route Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText711->Wrap( -1 );
	fgSizer42->Add( m_staticText711, 0, wxALL, 5 );

	m_sWindBarbsOnRouteThickness = new wxSpinCtrl( m_scrolledWindow4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 6, 2 );
	m_sWindBarbsOnRouteThickness->SetMaxSize( wxSize( 140,-1 ) );

	fgSizer42->Add( m_sWindBarbsOnRouteThickness, 1, wxALL|wxEXPAND, 5 );


	fgSizer18->Add( fgSizer42, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer82;
	fgSizer82 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer82->SetFlexibleDirection( wxBOTH );
	fgSizer82->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbDisplayCursorRoute = new wxCheckBox( m_scrolledWindow4, wxID_ANY, _("Display Cursor Route"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbDisplayCursorRoute->SetValue(true);
	fgSizer82->Add( m_cbDisplayCursorRoute, 0, wxALL, 5 );

	m_cbAlternatesForAll = new wxCheckBox( m_scrolledWindow4, wxID_ANY, _("Alternates for all IsoChrons"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer82->Add( m_cbAlternatesForAll, 0, wxALL, 5 );

	m_cbMarkAtPolarChange = new wxCheckBox( m_scrolledWindow4, wxID_ANY, _("Display mark when Polar changes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbMarkAtPolarChange->SetValue(true);
	fgSizer82->Add( m_cbMarkAtPolarChange, 0, wxALL, 5 );

	m_cbDisplayCurrent = new wxCheckBox( m_scrolledWindow4, wxID_ANY, _("Display current"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbDisplayCurrent->SetValue(true);
	fgSizer82->Add( m_cbDisplayCurrent, 0, wxALL, 5 );

	m_cbDisplayWindBarbs = new wxCheckBox( m_scrolledWindow4, wxID_ANY, _("Display Wind Barbs"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer82->Add( m_cbDisplayWindBarbs, 0, wxALL, 5 );

	m_cbDisplayApparentWindBarbs = new wxCheckBox( m_scrolledWindow4, wxID_ANY, _("Apparent Wind for Barbs On Route (not True Wind)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer82->Add( m_cbDisplayApparentWindBarbs, 0, wxALL, 5 );

	m_cbDisplayComfort = new wxCheckBox( m_scrolledWindow4, wxID_ANY, _("Display Sailing Comfort on Route"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer82->Add( m_cbDisplayComfort, 0, wxALL, 5 );


	fgSizer18->Add( fgSizer82, 1, wxEXPAND, 5 );


	m_scrolledWindow4->SetSizer( fgSizer18 );
	m_scrolledWindow4->Layout();
	fgSizer18->Fit( m_scrolledWindow4 );
	sbSizer25->Add( m_scrolledWindow4, 1, wxEXPAND | wxALL, 5 );


	fgSizer100->Add( sbSizer25, 1, wxEXPAND|wxALL, 5 );

	wxFlexGridSizer* fgSizer101;
	fgSizer101 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer101->AddGrowableCol( 0 );
	fgSizer101->AddGrowableRow( 1 );
	fgSizer101->SetFlexibleDirection( wxBOTH );
	fgSizer101->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer26;
	sbSizer26 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Computation") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer93;
	fgSizer93 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer93->SetFlexibleDirection( wxBOTH );
	fgSizer93->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText115 = new wxStaticText( sbSizer26->GetStaticBox(), wxID_ANY, _("Number of Concurrent threads"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText115->Wrap( -1 );
	fgSizer93->Add( m_staticText115, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_sConcurrentThreads = new wxSpinCtrl( sbSizer26->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 64, 1 );
	m_sConcurrentThreads->SetMaxSize( wxSize( 140,-1 ) );

	fgSizer93->Add( m_sConcurrentThreads, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	sbSizer26->Add( fgSizer93, 1, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );


	fgSizer101->Add( sbSizer26, 1, wxEXPAND|wxALL, 5 );

	wxArrayString m_cblFieldsChoices;
	m_cblFields = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cblFieldsChoices, 0 );
	fgSizer101->Add( m_cblFields, 1, wxALL|wxEXPAND, 5 );

	m_cbUseLocalTime = new wxCheckBox( this, wxID_ANY, _("Use Local Time"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer101->Add( m_cbUseLocalTime, 1, wxALL|wxEXPAND, 5 );


	fgSizer100->Add( fgSizer101, 1, wxEXPAND|wxALL, 5 );


	fgSizer92->Add( fgSizer100, 1, wxEXPAND, 5 );

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Help = new wxButton( this, wxID_HELP );
	m_sdbSizer1->AddButton( m_sdbSizer1Help );
	m_sdbSizer1->Realize();

	fgSizer92->Add( m_sdbSizer1, 1, wxEXPAND|wxALL, 5 );


	this->SetSizer( fgSizer92 );
	this->Layout();
	fgSizer92->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_cpCursorRoute->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( SettingsDialogBase::OnUpdateColor ), NULL, this );
	m_cpDestinationRoute->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( SettingsDialogBase::OnUpdateColor ), NULL, this );
	m_sRouteThickness->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SettingsDialogBase::OnUpdateSpin ), NULL, this );
	m_sIsoChronThickness->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SettingsDialogBase::OnUpdateSpin ), NULL, this );
	m_sAlternateRouteThickness->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SettingsDialogBase::OnUpdateSpin ), NULL, this );
	m_sWindBarbsOnRouteThickness->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SettingsDialogBase::OnUpdateSpin ), NULL, this );
	m_cbDisplayCursorRoute->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbAlternatesForAll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbMarkAtPolarChange->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbDisplayCurrent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbDisplayWindBarbs->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbDisplayApparentWindBarbs->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbDisplayComfort->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cblFields->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( SettingsDialogBase::OnUpdateColumns ), NULL, this );
	m_cbUseLocalTime->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdateColumns ), NULL, this );
	m_sdbSizer1Help->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnHelp ), NULL, this );
}

SettingsDialogBase::~SettingsDialogBase()
{
	// Disconnect Events
	m_cpCursorRoute->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( SettingsDialogBase::OnUpdateColor ), NULL, this );
	m_cpDestinationRoute->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( SettingsDialogBase::OnUpdateColor ), NULL, this );
	m_sRouteThickness->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SettingsDialogBase::OnUpdateSpin ), NULL, this );
	m_sIsoChronThickness->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SettingsDialogBase::OnUpdateSpin ), NULL, this );
	m_sAlternateRouteThickness->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SettingsDialogBase::OnUpdateSpin ), NULL, this );
	m_sWindBarbsOnRouteThickness->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SettingsDialogBase::OnUpdateSpin ), NULL, this );
	m_cbDisplayCursorRoute->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbAlternatesForAll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbMarkAtPolarChange->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbDisplayCurrent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbDisplayWindBarbs->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbDisplayApparentWindBarbs->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cbDisplayComfort->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdate ), NULL, this );
	m_cblFields->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( SettingsDialogBase::OnUpdateColumns ), NULL, this );
	m_cbUseLocalTime->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnUpdateColumns ), NULL, this );
	m_sdbSizer1Help->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingsDialogBase::OnHelp ), NULL, this );

}

AboutDialogBase::AboutDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer90;
	fgSizer90 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer90->SetFlexibleDirection( wxBOTH );
	fgSizer90->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer109;
	fgSizer109 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer109->SetFlexibleDirection( wxBOTH );
	fgSizer109->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText135 = new wxStaticText( this, wxID_ANY, _("Weather Routing Plugin Version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText135->Wrap( -1 );
	fgSizer109->Add( m_staticText135, 0, wxALL, 5 );

	m_stVersion = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stVersion->Wrap( -1 );
	fgSizer109->Add( m_stVersion, 0, wxALL, 5 );


	fgSizer90->Add( fgSizer109, 1, wxEXPAND, 5 );

	m_staticText110 = new wxStaticText( this, wxID_ANY, _("The weather routing plugin for opencpn is intended to calculate sailing routes based on computerized weather data and boat sailing ability.\n\nLicense: GPLv3+\n\nSource Code:\nhttps://github.com/seandepagnier/weather_routing_pi\n\nAuthor:\nSean D'Epagnier\n\nMany thanks to all of the translators and testers."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText110->Wrap( 400 );
	fgSizer90->Add( m_staticText110, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer91;
	fgSizer91 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer91->SetFlexibleDirection( wxBOTH );
	fgSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_bAboutAuthor = new wxButton( this, wxID_ANY, _("About Author"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer91->Add( m_bAboutAuthor, 0, wxALL, 5 );

	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer91->Add( m_bClose, 0, wxALL, 5 );


	fgSizer90->Add( fgSizer91, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer90 );
	this->Layout();
	fgSizer90->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_bAboutAuthor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnAboutAuthor ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnClose ), NULL, this );
}

AboutDialogBase::~AboutDialogBase()
{
	// Disconnect Events
	m_bAboutAuthor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnAboutAuthor ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnClose ), NULL, this );

}

FromTrackDialogBase::FromTrackDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer95;
	fgSizer95 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer95->SetFlexibleDirection( wxBOTH );
	fgSizer95->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer29;
	sbSizer29 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Track") ), wxVERTICAL );

	wxArrayString m_ptracklistChoices;
	m_ptracklist = new wxChoice( sbSizer29->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ptracklistChoices, 0 );
	m_ptracklist->SetSelection( 0 );
	sbSizer29->Add( m_ptracklist, 1, wxALL|wxEXPAND, 5 );


	fgSizer95->Add( sbSizer29, 1, wxEXPAND|wxALL, 5 );

	wxStaticBoxSizer* sbSizer28;
	sbSizer28 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Filter") ), wxVERTICAL );

	m_pfilter = new wxTextCtrl( sbSizer28->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer28->Add( m_pfilter, 1, wxALL|wxEXPAND, 5 );


	fgSizer95->Add( sbSizer28, 1, wxEXPAND|wxALL, 5 );

	wxFlexGridSizer* fgSizer99;
	fgSizer99 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer99->SetFlexibleDirection( wxBOTH );
	fgSizer99->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_button48 = new wxButton( this, wxID_ANY, _("Reset All"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer99->Add( m_button48, 0, wxALL, 5 );

	m_pbutton_fromtrack_done = new wxButton( this, wxID_ANY, _("Done"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer99->Add( m_pbutton_fromtrack_done, 0, wxALL, 5 );


	fgSizer95->Add( fgSizer99, 1, wxEXPAND|wxALL, 5 );


	this->SetSizer( fgSizer95 );
	this->Layout();
	fgSizer95->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_pfilter->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FromTrackDialogBase::OnFilterText ), NULL, this );
	m_button48->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FromTrackDialogBase::OnResetAll ), NULL, this );
	m_pbutton_fromtrack_done->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FromTrackDialogBase::OnFromTrackDone ), NULL, this );
}

FromTrackDialogBase::~FromTrackDialogBase()
{
	// Disconnect Events
	m_pfilter->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( FromTrackDialogBase::OnFilterText ), NULL, this );
	m_button48->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FromTrackDialogBase::OnResetAll ), NULL, this );
	m_pbutton_fromtrack_done->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FromTrackDialogBase::OnFromTrackDone ), NULL, this );

}
