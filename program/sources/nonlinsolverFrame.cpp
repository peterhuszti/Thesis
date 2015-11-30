#include <stack>
#include <wx/numformatter.h>
#include <vector>
#include "nonlinsolverFrame.h"
#include "linalg.h"
#include <wx/numformatter.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#include <time.h>
#include "mathplot.h"

#include "Images/nonlinsolverFrame_eq_down_XPM.xpm"
#include "Images/nonlinsolverFrame_eq_up_XPM.xpm"

BEGIN_EVENT_TABLE(nonlinsolverFrame,wxFrame)
	
	EVT_CLOSE(nonlinsolverFrame::OnClose)
	EVT_MENU(ID_MNU_NEW_1003, nonlinsolverFrame::New_Click)
	EVT_MENU(ID_MNU_OPEN_1007, nonlinsolverFrame::Open_Click)
	EVT_MENU(ID_MNU_SAVE_1108, nonlinsolverFrame::Save_Click)
	EVT_MENU(ID_MNU_SAVEGRAPH_1109, nonlinsolverFrame::Savegraph_Click)
	EVT_MENU(ID_MNU_QUIT_1004, nonlinsolverFrame::Quit_Click)
	EVT_MENU(ID_MNU_DOCUMENTATION_1130, nonlinsolverFrame::doc_butClick)
	EVT_MENU(ID_MNU_ABOUT_F1_1006, nonlinsolverFrame::About_Click)
	EVT_TEXT_ENTER(ID_KMAX_BOX,nonlinsolverFrame::kmax_butClick)
	EVT_BUTTON(ID_KMAX_EDIT,nonlinsolverFrame::kmax_editClick)
	EVT_BUTTON(ID_KMAX_BUT,nonlinsolverFrame::kmax_butClick)
	EVT_TEXT_ENTER(ID_K_BOX,nonlinsolverFrame::k_butClick)
	EVT_TEXT_ENTER(ID_EPSILON_BOX,nonlinsolverFrame::epsilon_butClick)
	EVT_BUTTON(ID_K_EDIT,nonlinsolverFrame::k_editClick)
	EVT_BUTTON(ID_K_BUT,nonlinsolverFrame::k_butClick)
	EVT_BUTTON(ID_EPSILON_EDIT,nonlinsolverFrame::epsilon_editClick)
	EVT_BUTTON(ID_EPSILON_BUT,nonlinsolverFrame::epsilon_butClick)
	EVT_BUTTON(ID_BACK,nonlinsolverFrame::backClick)
	EVT_BUTTON(ID_ZOOM_RIGHT,nonlinsolverFrame::zoom_rightClick)
	EVT_BUTTON(ID_ZOOM_LEFT,nonlinsolverFrame::zoom_leftClick)
	EVT_BUTTON(ID_EX_SHOW,nonlinsolverFrame::ex_showClick)
	EVT_BUTTON(ID_EX_EDIT,nonlinsolverFrame::ex_editClick)
	EVT_BUTTON(ID_EX_BUT,nonlinsolverFrame::ex_butClick)
	EVT_TEXT_ENTER(ID_EX_BOX,nonlinsolverFrame::ex_butClick)
	EVT_RADIOBUTTON(ID_DEGREE,nonlinsolverFrame::degreeClick)
	EVT_RADIOBUTTON(ID_RADIAN,nonlinsolverFrame::radianClick)
	EVT_BUTTON(ID_EQ_DOWN,nonlinsolverFrame::eq_downClick)
	EVT_BUTTON(ID_EQ_UP,nonlinsolverFrame::eq_upClick)
	EVT_BUTTON(ID_SOLVE,nonlinsolverFrame::solveClick)
	EVT_BUTTON(ID_STARTVECT_SHOW,nonlinsolverFrame::startvect_showClick)
	EVT_BUTTON(ID_EQ_SHOW,nonlinsolverFrame::eq_showClick)
	EVT_BUTTON(ID_EQ_EDIT,nonlinsolverFrame::eq_editClick)
	EVT_BUTTON(ID_EQ_BUT,nonlinsolverFrame::eq_butClick)
	EVT_TEXT_ENTER(ID_EQ_BOX,nonlinsolverFrame::eq_butClick)
	EVT_BUTTON(ID_STARTVECT_EDIT,nonlinsolverFrame::startvect_editClick)
	EVT_BUTTON(ID_STARTVECT_BUT,nonlinsolverFrame::startvect_butClick)
	EVT_TEXT_ENTER(ID_STARTVECT_BOX,nonlinsolverFrame::startvect_butClick)
	EVT_BUTTON(ID_NUMBEQ_EDIT,nonlinsolverFrame::numbeq_editClick)
	EVT_BUTTON(ID_NUMBEQ_BUT,nonlinsolverFrame::numbeq_butClick)
	EVT_TEXT_ENTER(ID_NUMBEQ_BOX,nonlinsolverFrame::numbeq_butClick)
END_EVENT_TABLE()

nonlinsolverFrame::nonlinsolverFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
	//inicializálás
	edited=-1;
	startvect_showed=false;
	eq_showed=false;
	ex_showed=false;
	solvable=0;
	eq_showed_numb=0;
	result_exact_show=false;
	deg_rad=false;
	solved=false;
	//
	vectorx.resize(0);
	vectory.resize(0);
	vectorx2.resize(0);
	vectory2.resize(0);
	conv1X.resize(0);
	conv1Y.resize(0);
	conv2X.resize(0);
	conv2Y.resize(0);
}

nonlinsolverFrame::~nonlinsolverFrame()
{
}

void nonlinsolverFrame::CreateGUIControls()
{
	wxInitAllImageHandlers();   //inicializálja a grafikus kezelõket

	WxFlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	this->SetSizer(WxFlexGridSizer1);
	this->SetAutoLayout(true);

	WxPanel1 = new wxPanel(this, ID_WXPANEL1, wxPoint(5, 5), wxSize(480, 320));
	WxPanel1->SetBackgroundColour(wxColour(171,171,171));
	WxFlexGridSizer1->Add(WxPanel1, 0, wxALIGN_CENTER | wxALL, 5);

//egyeneltek száma
	numbeq_stat = new wxStaticText(WxPanel1, ID_NUMBEQ_STAT, _("number of equations:"), wxPoint(15, 15), wxDefaultSize, 0, _("numbeq_stat"));
	numbeq_box = new wxTextCtrl(WxPanel1, ID_NUMBEQ_BOX, _(""), wxPoint(145, 15), wxSize(125, 20), wxTE_PROCESS_ENTER, wxDefaultValidator, _("numbeq_box"));
	numbeq_stat2 = new wxStaticText(WxPanel1, ID_NUMBEQ_STAT2, _(""), wxPoint(145, 15), wxDefaultSize, 0, _("numbeq_stat2"));
	numbeq_but = new wxButton(WxPanel1, ID_NUMBEQ_BUT, _("OK"), wxPoint(280, 13), wxSize(30, 25), 0, wxDefaultValidator, _("numbeq_but"));
	numbeq_edit = new wxButton(WxPanel1, ID_NUMBEQ_EDIT, _("Edit"), wxPoint(320, 13), wxSize(35, 25), 0, wxDefaultValidator, _("numbeq_edit"));
	numbeq_edit->Show(false);
//kezdõvektor
	startvect_stat = new wxStaticText(WxPanel1, ID_STARTVECT_STAT, _("starting vector (x0):"), wxPoint(28, 50), wxDefaultSize, 0, _("startvect_stat"));
	startvect_box = new wxTextCtrl(WxPanel1, ID_STARTVECT_BOX, _("x01;x02;...;x0n"), wxPoint(145, 50), wxSize(125, 20), wxTE_PROCESS_ENTER, wxDefaultValidator, _("startvect_box"));
	startvect_box->Enable(false);
	startvect_stat2 = new wxStaticText(WxPanel1, ID_STARTVECT_STAT2, _(""), wxPoint(145, 50), wxDefaultSize, 0, _("startvect_stat2"));
	startvect_stat2->Show(false);
	startvect_but = new wxButton(WxPanel1, ID_STARTVECT_BUT, _("OK"), wxPoint(280, 48), wxSize(30, 25), 0, wxDefaultValidator, _("startvect_but"));
	startvect_but->Enable(false);
	startvect_edit = new wxButton(WxPanel1, ID_STARTVECT_EDIT, _("Edit"), wxPoint(320, 48), wxSize(35, 25), 0, wxDefaultValidator, _("startvect_edit"));
	startvect_edit->Show(false);
	startvect_show = new wxButton(WxPanel1, ID_STARTVECT_SHOW, _("Show startvector"), wxPoint(365, 48), wxSize(115, 25), 0, wxDefaultValidator, _("startvect_show"));
	startvect_show->Show(false);
//egyenletek
	eq_tm = new wxStaticText(WxPanel1, ID_EQ_TM, _("Too much equations to show!"), wxPoint(85, 150), wxDefaultSize, 0, _("eq_tm"));
	eq_tm->Show(false);
	eq_stat = new wxStaticText(WxPanel1, ID_EQ_STAT, _("1. equation:"), wxPoint(10, 150), wxDefaultSize, 0, _("eq_stat"));
	eq_box = new wxTextCtrl(WxPanel1, ID_EQ_BOX, _(""), wxPoint(85, 150), wxSize(165, 20), wxTE_PROCESS_ENTER, wxDefaultValidator, _("eq_box"));
	eq_box->Enable(false);
	eq_stat_nulla = new wxStaticText(WxPanel1, ID_EQ_STAT_NULLA, _("=0"), wxPoint(252, 150), wxDefaultSize, 0, _("eq_stat_nulla"));
	eq_but = new wxButton(WxPanel1, ID_EQ_BUT, _("OK"), wxPoint(280, 148), wxSize(30, 25), 0, wxDefaultValidator, _("eq_but"));
	eq_but->Enable(false);
	eq_edit = new wxButton(WxPanel1, ID_EQ_EDIT, _("Edit"), wxPoint(320, 148), wxSize(35, 25), 0, wxDefaultValidator, _("eq_edit"));
	eq_edit->Show(false);
	wxArrayString arrayStringFor_eq_num;//legördülõ lista
	eq_num = new wxChoice(WxPanel1, ID_EQ_NUM, wxPoint(270, 148), wxSize(40, 23), arrayStringFor_eq_num, 0, wxDefaultValidator, _("eq_num"));
	eq_num->Show(false);
	eq_num->SetSelection(-1);
	eq_show = new wxButton(WxPanel1, ID_EQ_SHOW, _("Show equations"), wxPoint(365, 148), wxSize(115, 25), 0, wxDefaultValidator, _("eq_show"));
	eq_show->Show(false);
	wxBitmap eq_up_BITMAP (nonlinsolverFrame_eq_up_XPM);//arrows
	eq_up = new wxBitmapButton(WxPanel1, ID_EQ_UP, eq_up_BITMAP, wxPoint(4, 249), wxSize(15, 25), wxBU_AUTODRAW, wxDefaultValidator, _("eq_up"));
	eq_up->Show(false);
	wxBitmap eq_down_BITMAP (nonlinsolverFrame_eq_down_XPM);
	eq_down = new wxBitmapButton(WxPanel1, ID_EQ_DOWN, eq_down_BITMAP, wxPoint(4, 274), wxSize(15, 25), wxBU_AUTODRAW, wxDefaultValidator, _("eq_down"));
	eq_down->Show(false);
//megoldás
	solve = new wxButton(WxPanel1, ID_SOLVE, _("Solve"), wxPoint(365, 180), wxSize(115, 60), 0, wxDefaultValidator, _("solve"));
	solve->Show(false);
//eredmény
	result_text = new wxStaticText(WxPanel1, ID_RESULT_TEXT, _("Approximate solution:"), wxPoint(15, 15), wxDefaultSize, 0, _("result_text"));
	result_text->Show(false);
	result = new wxStaticText(WxPanel1, ID_RESULT, _("result"), wxPoint(145, 15), wxDefaultSize, 0, _("result"));
	result->Show(false);
	result_label2 = new wxStaticText(WxPanel1, ID_RESULT_LABEL2, _("h(j) = || x*-xj || = "), wxPoint(270, 69), wxDefaultSize, 0, _("result_label2"));
	result_label2->Show(false);
	result_label1 = new wxStaticText(WxPanel1, ID_RESULT_LABEL1, _("g(j) = || f(xj) || = "), wxPoint(50, 68), wxDefaultSize, 0, _("result_label1"));
	result_label1->Show(false);
	g_res = new wxStaticText(WxPanel1, ID_G_RES, _(""), wxPoint(140, 70), wxDefaultSize, 0, _("g_res"));
	g_res->Show(false);
	h_res = new wxStaticText(WxPanel1, ID_H_RES, _(""), wxPoint(360, 70), wxDefaultSize, 0, _("h_res"));
	h_res->Show(false);
//idõ
	time_text = new wxStaticText(WxPanel1, ID_TIME_TEXT, _("Time:"), wxPoint(15, 40), wxDefaultSize, 0, _("time_text"));
	time_text->Show(false);
	time = new wxStaticText(WxPanel1, ID_TIME, _(""), wxPoint(60, 40), wxDefaultSize, 0, _("time"));
	time->Show(false);
//rádiógombok
	radian = new wxRadioButton(WxPanel1, ID_RADIAN, _("Rad"), wxPoint(365, 15), wxSize(50, 20), 0, wxDefaultValidator, _("radian"));
	radian->Enable(false);
	radian->SetValue(true);
	degree = new wxRadioButton(WxPanel1, ID_DEGREE, _("Deg"), wxPoint(420, 15), wxSize(50, 20), 0, wxDefaultValidator, _("degree"));
	degree->Enable(false);
//pontos megoldás
	ex_stat = new wxStaticText(WxPanel1, ID_EX_STAT, _("Exact solution:"), wxPoint(53, 85), wxDefaultSize, 0, _("ex_stat"));
	ex_box = new wxTextCtrl(WxPanel1, ID_EX_BOX, _(""), wxPoint(145, 85), wxSize(125, 20), wxTE_PROCESS_ENTER, wxDefaultValidator, _("ex_box"));
	ex_box->Enable(false);
	ex_but = new wxButton(WxPanel1, ID_EX_BUT, _("OK"), wxPoint(280, 83), wxSize(30, 25), 0, wxDefaultValidator, _("ex_but"));
	ex_but->Enable(false);
	ex_edit = new wxButton(WxPanel1, ID_EX_EDIT, _("Edit"), wxPoint(320, 83), wxSize(35, 25), 0, wxDefaultValidator, _("ex_edit"));
	ex_edit->Show(false);
	ex_show = new wxButton(WxPanel1, ID_EX_SHOW, _("Show exact solution"), wxPoint(365, 83), wxSize(115, 25), 0, wxDefaultValidator, _("ex_show"));
	ex_show->Show(false);
	ex_stat2 = new wxStaticText(WxPanel1, ID_EX_STAT2, _(""), wxPoint(145, 85), wxDefaultSize, 0, _("ex_stat2"));
	ex_stat2->Show(false);
//zoom
	zoom_left = new wxButton(WxPanel1, ID_ZOOM_LEFT, _("zoom"), wxPoint(1, 84), wxSize(40, 20), 0, wxDefaultValidator, _("zoom_left"));
	zoom_left->Show(false);
	zoom_right = new wxButton(WxPanel1, ID_ZOOM_RIGHT, _("zoom"), wxPoint(439, 84), wxSize(40, 20), 0, wxDefaultValidator, _("zoom_right"));
	zoom_right->Show(false);
//vissza
	back = new wxButton(WxPanel1, ID_BACK, _("Back"), wxPoint(439, 1), wxSize(40, 20), 0, wxDefaultValidator, _("back"));
	back->Show(false);
//paraméterek
//epsilon
	epsilon_but = new wxButton(WxPanel1, ID_EPSILON_BUT, _("OK"), wxPoint(135, 118), wxSize(30, 25), 0, wxDefaultValidator, _("epsilon_but"));
	epsilon_but->Show(false);
	epsilon_edit = new wxButton(WxPanel1, ID_EPSILON_EDIT, _("Edit"), wxPoint(135, 118), wxSize(35, 25), 0, wxDefaultValidator, _("epsilon_edit"));
	epsilon_edit->Enable(false);
	epsilon_stat = new wxStaticText(WxPanel1, ID_EPSILON_STAT, _("Epsilon:"), wxPoint(10, 120), wxDefaultSize, 0, _("epsilon_stat"));
	epsilon_box = new wxTextCtrl(WxPanel1, ID_EPSILON_BOX, _(""), wxPoint(65, 120), wxSize(60, 20), wxTE_PROCESS_ENTER, wxDefaultValidator, _("epsilon_box"));
	epsilon_box->Show(false);
	epsilon_stat2 = new wxStaticText(WxPanel1, ID_EPSILON_STAT2, _("0.001"), wxPoint(65, 120), wxDefaultSize, 0, _("epsilon_stat2"));
//delta
	k_but = new wxButton(WxPanel1, ID_K_BUT, _("OK"), wxPoint(255, 118), wxSize(30, 25), 0, wxDefaultValidator, _("k_but"));
	k_but->Show(false);
	k_edit = new wxButton(WxPanel1, ID_K_EDIT, _("Edit"), wxPoint(255, 118), wxSize(35, 25), 0, wxDefaultValidator, _("k_edit"));
	k_edit->Enable(false);
	k_stat = new wxStaticText(WxPanel1, ID_K_STAT, _("1/delta:"), wxPoint(175, 120), wxDefaultSize, 0, _("k_stat"));
	k_box = new wxTextCtrl(WxPanel1, ID_K_BOX, _(""), wxPoint(220, 120), wxSize(25, 20), wxTE_PROCESS_ENTER, wxDefaultValidator, _("k_box"));
	k_box->Show(false);
	k_stat2 = new wxStaticText(WxPanel1, ID_K_STAT2, _("10"), wxPoint(220, 120), wxDefaultSize, 0, _("k_stat2"));
//maximum lépések száma
	kmax_stat = new wxStaticText(WxPanel1, ID_KMAX_STAT, _("Maximum steps: "), wxPoint(300, 120), wxDefaultSize, 0, _("kmax_stat"));
	kmax_but = new wxButton(WxPanel1, ID_KMAX_BUT, _("OK"), wxPoint(435, 118), wxSize(30, 25), 0, wxDefaultValidator, _("kmax_but"));
	kmax_but->Enable(false);
	kmax_edit = new wxButton(WxPanel1, ID_KMAX_EDIT, _("Edit"), wxPoint(435, 118), wxSize(35, 25), 0, wxDefaultValidator, _("kmax_edit"));
	kmax_edit->Show(false);
	kmax_box = new wxTextCtrl(WxPanel1, ID_KMAX_BOX, _(""), wxPoint(400, 120), wxSize(25, 20), wxTE_PROCESS_ENTER, wxDefaultValidator, _("kmax_box"));
	kmax_box->Enable(false);
	kmax_stat2 = new wxStaticText(WxPanel1, ID_KMAX_STAT2, _(""), wxPoint(400, 120), wxDefaultSize, 0, _("kmax_stat2"));
	kmax_stat2->Show(false);
//menü
	Menu = new wxMenuBar();
	wxMenu *ID_MNU_FILE_1001_Mnu_Obj = new wxMenu();
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_NEW_1003, _("&New\tCTRL+N"), _(""), wxITEM_NORMAL);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_OPEN_1007, _("&Open\tCTRL+O"), _(""), wxITEM_NORMAL);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_SAVE_1108, _("&Save\tCTRL+S"), _(""), wxITEM_NORMAL);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_SAVEGRAPH_1109, _("Save graph"), _(""), wxITEM_NORMAL);
	ID_MNU_FILE_1001_Mnu_Obj->Append(ID_MNU_QUIT_1004, _("&Quit\tALT+Q"), _(""), wxITEM_NORMAL);
	Menu->Append(ID_MNU_FILE_1001_Mnu_Obj, _("File"));
	
	wxMenu *ID_MNU_HELP_1002_Mnu_Obj = new wxMenu();
	ID_MNU_HELP_1002_Mnu_Obj->Append(ID_MNU_DOCUMENTATION_1130, _("Documentation"), _(""), wxITEM_NORMAL);
	ID_MNU_HELP_1002_Mnu_Obj->Append(ID_MNU_ABOUT_F1_1006, _("&About\tF1"), _(""), wxITEM_NORMAL);
	Menu->Append(ID_MNU_HELP_1002_Mnu_Obj, _("&Help"));
	SetMenuBar(Menu);
//
	SetTitle(_("Nonlinear equation system solver"));
	SetIcon(wxNullIcon);
	
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
}
    
void nonlinsolverFrame::OnClose(wxCloseEvent& event)
{
    if(eh.getN()!=0)
    {
        if (wxMessageBox(_("Are sure you want to quit?"), _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO )
            return; 
    }
	Destroy();
}

void nonlinsolverFrame::Quit_Click(wxCommandEvent& event)
{    
    if(eh.getN()!=0)
    {
        if (wxMessageBox(_("Are sure you want to quit?"), _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO )
            return; 
    }
	Destroy();
}

void nonlinsolverFrame::About_Click(wxCommandEvent& event)
{
	wxString msg;
    msg.Printf(_T("  Created by Peter Huszti\n Eotvos Lorand University\n          Degree thesis\n                 2014"));
    wxMessageBox(msg, _T("About"),wxOK | wxICON_INFORMATION, this);
}

void nonlinsolverFrame::doc_butClick(wxCommandEvent& event)
{//az alapértelmezett pdf olvasóval nyitja meg
	system("start "" ../documentation/Documentation.pdf");
}

//egyenletek száma
void nonlinsolverFrame::numbeq_butClick(wxCommandEvent& event)//OK
{
    int db=-1;
	wxString num=numbeq_box->GetValue();
    db=wxAtoi(num);
    if(db<=1)//hibás adat van megadva
    {
        wxMessageBox(_("Please insert a positive number!"),_("Invalid number"));
        numbeq_box->SetLabel("");
        return;
    }
	eh.setN(db);      
	
	equations.resize(eh.getN());
	for(int i=1;i<=eh.getN();i++)//legördülõ lista feltöltése
	{
        eq_num->Append(wxString::Format("%i",i));
    }

    numbeq_stat2->Show(true);
    numbeq_but->Show(false);
    numbeq_box->Show(false);
    numbeq_edit->Show(true);
    numbeq_stat2->SetLabel(wxString::Format("%i",eh.getN()));
    
    if(startvect_box->GetValue()!="x01;x02;...;x0n" && eh.getN()>=10)
    {
        startvect_show->Show(true);
    }
    
    if(eh.getN()!=eh.getN_old())//ha változtatva volt
    {
        degree->Enable(true);
        radian->Enable(true);
        
        startvect_stat2->Show(false);
        startvect_box->Show(true);
        startvect_box->Enable(true);
        startvect_but->Show(true);
        startvect_but->Enable(true);
        startvect_edit->Show(false);
        if(eh.getN_old()!=0)//ha nem az elsõ megadásnál
        {
            wxString temp=eh.getX0();
            temp.erase(temp.begin());
            temp.erase(temp.end()-1);
            startvect_box->SetLabel(temp);
            
            if(result_exact_show)//megvan adva a pontos megoldás
            {
                wxString temp2=eh.getEx();
                temp2.erase(temp2.begin());
                temp2.erase(temp2.end()-1);
                ex_box->SetLabel(temp2);
            }
        } 
        
        ex_stat2->Show(false);
        ex_box->Show(true);
        ex_box->Enable(true);
        ex_but->Show(true);
        ex_but->Enable(true);
        ex_edit->Show(false);
        
        epsilon_edit->Enable(true);
        k_edit->Enable(true);
        kmax_but->Enable(true);
        kmax_box->Enable(true);
    }
    if(eh.getN_old()>eh.getN() && eh.getN_old()!=0 && eh.getActeq()>=eh.getN())//ha csökkentettük az egyenletek számát és már több egyenlet meg volt adva
	{
        eq_but->Show(false);
        eq_stat->Show(false);
        eq_stat_nulla->Show(false);
        eq_box->Show(false);
        for(int i=eh.getActeq()-1;i>=eh.getN();i--)
        {
            delete equations[i];
            eh.removeEQ();
        }
        
        eh.setActeq(eh.getN());
        eq_edit->Show(true);
        eq_num->Show(true);
        if(eh.getN()<6)
        {
            eq_down->Show(false);
            eq_up->Show(false);
            for(int i=0;i<eh.getN();i++)
            {
                equations[i]->Show(true);    
            }
            eq_tm->Show(false);
            eq_showed=true;
        }
    }
	else if(eh.getActeq()<eh.getN())//ha kevesebb egyenlet volt megadva, mint az új érték
	{
        eq_stat->SetLabel(wxString::Format(wxT("%i. equation:"),eh.getActeq()+1));
    	eq_stat->Show(true);
    	eq_stat_nulla->Show(true);
    	eq_box->Show(true);
    	eq_but->Show(true);
    	eq_box->Enable(true);
    	eq_but->Enable(true);
    	eq_num->Show(false);
    	eq_edit->Show(false);
    }
    solvable++;
    if(solvable==2+eh.getN())//akkor megoldható a feladat, ha minden ki van töltve
    {
        solve->Enable(true);
    }
    if(eh.getN()>eh.getN_old() && eh.getN_old()!=0)
    {
        solvable--;    
    }
}

void nonlinsolverFrame::numbeq_editClick(wxCommandEvent& event)//Edit
{
    startvect_show->Show(false);
    eq_show->Show(false);
    solvable--;
    solve->Enable(false);
	numbeq_stat2->Show(false);
	numbeq_box->Show(true);
	numbeq_edit->Show(false);
	numbeq_but->Show(true);
	numbeq_box->SetLabel(wxString::Format("%i",eh.getN()));
    eq_num->Clear();
}

//starting vector
void nonlinsolverFrame::startvect_butClick(wxCommandEvent& event)//OK
{
	wxString str=startvect_box->GetValue();
	
	if(str=="x01;x02;...;x0n") startvect_box->SetLabel("");
	
	str+=wxT('\0');
	if(str[0]=='\0')//nincs megadva semmi 
    {
        wxMessageBox(_("Please insert a starting vector!"),_("No starting vector"));
        return;
    }
    solvable++;
    if(solvable==2+eh.getN()) 
    {
        solve->Enable(true);
        solve->Show(true);
    }

	eh.setX0(std::string((str).mb_str()));
	
	startvect_showed=false;
	startvect_show->SetLabel("Show startvector");
	startvect_box->Show(false);
	startvect_stat2->Show(true);
	startvect_but->Show(false);

   	wxString temp(eh.getX0().c_str(),wxConvUTF8);
    if(temp.size()<=28)
    {
   	    for(int i=0;i<temp.size();i++)
            if(temp[i]=='.') temp[i]=',';
    	
    	startvect_stat2->SetLabel(temp);
    	startvect_edit->Show(true);
    }
    else//túl hosszú
    {
        startvect_stat2->SetLabel("Too long to show!");
        startvect_show->Show(true);
    }
}

void nonlinsolverFrame::startvect_editClick(wxCommandEvent& event)//Edit
{
    solvable--;
    solve->Enable(false);
    startvect_edit->Show(false);
	startvect_but->Show(true);
	startvect_stat2->Show(false);
	startvect_box->Show(true);
	startvect_box->Enable(true);
	startvect_but->Enable(true);
	
	wxString str(eh.getX0().c_str(), wxConvUTF8);
	str.erase(str.begin());
	str.erase(str.end()-1);	
	
	for(int i=0;i<str.size();i++)
	   if(str[i]=='.') str[i]=',';
	
	startvect_box->SetLabel(str);
}

void nonlinsolverFrame::startvect_showClick(wxCommandEvent& event)//Show
{
    if(!startvect_showed)//nincs megjelenítve
    {
    	wxString temp(eh.getX0().c_str(),wxConvUTF8);
    	temp.erase(temp.begin());
    	temp.erase(temp.end()-1);   	
    	startvect_box->Show(true);
    	startvect_stat2->Show(false);
    	startvect_box->SetLabel(temp);
    	startvect_show->SetLabel("Hide startvector");
    	startvect_showed=true;
    }
    else//meg van jelenítve
    {
        wxString str=startvect_box->GetValue();
        str+=wxT('\0');
	    if(str[0]=='\0')//nincs megadva semmi
        {
            wxMessageBox(_("Please insert a starting vector!"),_("No starting vector"));
            return;
        }
        
        startvect_stat2->Show(true);
        startvect_stat2->SetLabel("Hid, click to show");
    	startvect_box->Show(false);
    	startvect_show->SetLabel("Show startvector");
    	startvect_showed=false;

	    eh.setX0(std::string((str).mb_str()));
    }
}

//equations
void nonlinsolverFrame::eq_butClick(wxCommandEvent& event)//OK
{
    wxString str=eq_box->GetValue();
    
    bool jo=false;
    for(int i=0;i<str.size();i++)
        if(str[i]!=' ')
        { 
            jo=true;
            break;
        }
    if(!jo)//nincs megadva semmi
    {
        wxMessageBox(_("Please add a real equation"),("Empty equation"));
        return;    
    }
    
    if(edited==-1)//ha nincs módosítás
    {            
        eh.addEQ(std::string(str.mb_str()));
        try 
        {
            eh.lengyel_forma(eh.getActeq());//lengyelforma
        }
        catch(equation_handler::errors err)
        {
            if(err==equation_handler::UNKNOWN)//ismeretlen karakter
            {
                wxMessageBox(wxString::Format("Unknown symbol or operator in %i. equation.",eh.getActeq()+1),_("Error in data"));
                eh.removeEQ();//kitörli a hibás egyenletet
                return;
            }
            if(err==equation_handler::OPERATOR_END)//operátor az egyenlet végén
            {
                wxMessageBox(_("Invalid end of equation."),_("Error in data"));
                eh.removeEQ();
                return;
            }
            if(err==equation_handler::XI_ERROR)//nem jó változó index
            {
                wxMessageBox(_("Invalid variable. You must use variable x1..xn."),_("Invalid variable"));    
                eh.removeEQ();
                return;
            }
            if(err==equation_handler::INVALID_BRACKET)//helytelen zárójelezés
            {
                wxMessageBox(wxString::Format("Invalid brackets in %i equation.",eh.getActeq()+1),_("Invalid brackets"));   
                eh.removeEQ();
                return;
            }
        }
        
        solvable++;
        if(solvable==2+eh.getN()) 
        {
            solve->Enable(true);
            solve->Show(true);
        }
            
        str+="=0";
        equations[eh.getActeq()]=new wxStaticText(this, 3000+eh.getActeq(),
                                    _(str), wxPoint(60, 180+30*eh.getActeq()), 
                                    wxDefaultSize, 0, _("eq_"+eh.getActeq()));
        if(eh.getActeq()<5) 
                equations[eh.getActeq()]->Show(true);
        else//túl hossú
        {
            equations[eh.getActeq()]->Show(false);
            eq_up->Show(true);
            eq_down->Show(true);
            eq_show->Show(true);
            eq_show->SetLabel("Hide equations");
            eq_tm->SetLabel("Hid, click to show");
            eq_showed=true;
        }
    	if(eh.getActeq()<eh.getN()-1)//ha nem az utolsó egyenlet, akkor
    	{
            eh.setActeq(eh.getActeq()+1);
            eq_stat->SetLabel(wxString::Format(wxT("%i. equation:"),eh.getActeq()+1));//frissíteni kell a feliratot
            eq_box->SetLabel("");
        }
        else if(eh.getActeq()==eh.getN()-1)//ha az utolsó egyenlet
        {    
            eh.setActeq(eh.getActeq()+1);
            eq_stat->SetLabel(wxString::Format(wxT("%i. equation:"),eh.getActeq()+1));    
            eq_stat->Show(false);
            eq_stat_nulla->Show(false);
            eq_box->Show(false);
            eq_but->Show(false);  
            eq_edit->Show(true);
            eq_num->Show(true);
            eq_box->SetLabel("");
        }
    }
    else//módosításnál
    {
        std::string temp=eh.getEq(edited);
        eh.editEQ(std::string(str.mb_str()),edited);
        
        try 
        {
            eh.lengyel_forma(edited);
            
            str+="=0";
            equations[edited]->SetLabel(str);
            edited=-1;
            eq_stat->Show(false);
            eq_stat_nulla->Show(false);
            eq_box->Show(false);
            eq_but->Show(false);  
            eq_edit->Show(true);
            eq_num->Show(true);
            eq_box->SetLabel("");
            solvable++;
            if(solvable==2+eh.getN()) 
            {
                solve->Enable(true);
                solve->Show(true);
            }
        }
        catch(equation_handler::errors err)
        {
            if(err==equation_handler::UNKNOWN)
            {
                wxMessageBox(wxString::Format("Unknown symbol or operator in %i. equation.",eh.getActeq()+1),_("Error in data"));
                eh.editEQ(temp,edited);
                eh.lengyel_forma(edited);
                return;
            }
            if(err==equation_handler::OPERATOR_END)
            {
                wxMessageBox(_("Invalid end of equation."),_("Error in data"));
                eh.editEQ(temp,edited);
                eh.lengyel_forma(edited);
                return;
            }
            if(err==equation_handler::XI_ERROR)
            {
                wxMessageBox(_("Invalid variable. You must use variable x1..xn."),_("Invalid variable"));    
                eh.editEQ(temp,edited);
                eh.lengyel_forma(edited);
                return;
            }
            if(err==equation_handler::INVALID_BRACKET)
            {
                wxMessageBox(wxString::Format("Invalid brackets in %i equation.",eh.getActeq()+1),_("Invalid brackets"));   
                eh.editEQ(temp,edited);
                eh.lengyel_forma(edited);
                return;
            }
        }
    }
}

void nonlinsolverFrame::eq_editClick(wxCommandEvent& event)//Edit
{
    solvable--;
    solve->Enable(false);
    int j=eq_num->GetSelection();
    if(j==wxNOT_FOUND)//nincs kiválasztva semmi a listában
    {
        wxMessageBox(_("Please choose an equation!"),_("No equation selected"));
        return;
    }
	eq_box->Show(true);
	eq_edit->Show(false);
	eq_box->Enable(true);
	eq_but->Enable(true);
	eq_num->Show(false);
	eq_stat_nulla->Show(true);
	eq_but->Show(true);
	eq_tm->Show(false);
	
	std::string str=std::string(equations[j]->GetLabel().mb_str());
	int m=str.find("=");
	str.erase(m,2);
	wxString label(str.c_str(),wxConvUTF8);
	eq_box->SetLabel(label);
	edited=j;	
}

void nonlinsolverFrame::eq_showClick(wxCommandEvent& event)//Show
{
    if(!eq_showed)//nincs megjelenítve
    {
        
        eq_up->Show(true);
        eq_down->Show(true);
        eq_tm->Show(false);
        for(int i=0;i<eq_showed_numb+5;i++)
        {
        	equations[i]->Show(true);
        }
        eq_show->SetLabel("Hide equations");
        eq_showed=true;
    }
    else//meg van jelenítve
    {
        eq_up->Show(false);
        eq_down->Show(false);
        for(int i=0;i<eh.getN();i++)
        {
        	equations[i]->Show(false);
        }
        eq_show->SetLabel("Show equations");
        eq_tm->Show(true);
        eq_tm->SetLabel("Hid, click to show");
        eq_showed=false;
    }
}

void nonlinsolverFrame::eq_upClick(wxCommandEvent& event)//up arrow
{
	if(eq_showed_numb>0)//még lehet felfelé menni
    {
       int j=4;
	   for(int i=eq_showed_numb+4;i>=eq_showed_numb;i--)
	   {
            wxString temp(eh.getEq(i-1).c_str(),wxConvUTF8);
            temp.erase(temp.end()-1);
            temp+="=0";
            equations[j]->SetLabel(temp); 
            j--;  
        }
        eq_showed_numb--;
    }
    else return;
}

void nonlinsolverFrame::eq_downClick(wxCommandEvent& event)//down arrow
{
    if(eq_showed_numb<eh.getN()-5)//még lehet lefelé menni
    {
       int j=0;
	   for(int i=eq_showed_numb;i<eq_showed_numb+5;i++)
	   {
            wxString temp(eh.getEq(i+1).c_str(),wxConvUTF8);
            temp.erase(temp.end()-1);
            temp+="=0";
            equations[j]->SetLabel(temp); 
            j++;
        }
        eq_showed_numb++;
    }
    else return;
}

//new project
void nonlinsolverFrame::New_Click(wxCommandEvent& event)
{     
    if(eh.getN()!=0)//van valamilyen adat már
    {
        if (wxMessageBox(_("Are sure you want to overwrite the exiting content?"), _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO )
            return;   
    }
    
    newclick();
}

//open click
void nonlinsolverFrame::Open_Click(wxCommandEvent& event)
{
    if(eh.getN()!=0)//van már valamilyen adat
    {
        if (wxMessageBox(_("Are sure you want to overwrite the exiting content?"), _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO )
            return;   
    }
        
	wxString file;
	wxFileDialog fdlog(this);
	if(fdlog.ShowModal()==wxID_CANCEL) return;
    
    newclick();
	
	file.Clear();
	file=fdlog.GetPath();
	
	wxFileInputStream input(file);
	wxTextInputStream text(input);
	int i=0;
		
	while(input.IsOk() && !input.Eof())//fájl végéig
	{
        wxString line;
        text >> line;
        if(line=="") return;//skip üres sor
        if(i==0)//egyenletek száma
        {
        	int db=-1;
            db=wxAtoi(line);
            if(db<=0)//hibás egyenlet szám
            {
                wxMessageBox(_("Invalid number of equations!"),_("Invalid number"));
                return;
            }
    	    eh.setN(db);
    	    equations.resize(eh.getN());
            numbeq_stat2->SetLabel(wxString::Format("%i",eh.getN()));
            numbeq_stat2->Show(true);
            numbeq_box->Show(false);
            numbeq_but->Show(false);
            numbeq_edit->Show(true);
            solvable=eh.getN()+2;
        }
        else if(i==1)//kezdõvektor
        {
            startvect_stat2->Show(true);
        	startvect_stat->Show(true);
        	startvect_but->Show(false);
        	startvect_box->Show(false);
        	startvect_edit->Show(true);
        	startvect_box->Enable(true);
        	startvect_but->Enable(true);
            
            eh.setX0(std::string((line+wxT('\0')).mb_str()));
           	wxString temp(eh.getX0().c_str(),wxConvUTF8);
           	if(temp.size()<=28)
           	{
            	startvect_stat2->SetLabel(temp);
            }
            else//túl hosszú
            {
                startvect_stat2->SetLabel("Too long to show!");
                startvect_show->Show(true);
                startvect_edit->Show(false);
            }
        }
        else if(i==2)//pontos megoldás
        {
            if(line!="unknown")//meg van adva
            {
                ex_stat2->Show(true);
        	    ex_stat->Show(true);
        	    ex_box->Show(false);
        	    ex_but->Show(false);
         	    ex_box->Enable(true);
         	    ex_but->Enable(true);
        	    ex_edit->Show(true);
        	    result_exact_show=true;
                
                eh.setEx(std::string((line+wxT('\0')).mb_str()));
                wxString temp(eh.getEx().c_str(),wxConvUTF8);
                if(temp.size()<=28)
                {
                	ex_stat2->SetLabel(temp);
                }
                else//túl hoszú
                {
                    ex_stat2->SetLabel("Too long to show!");
                    ex_show->Show(true);
                    ex_edit->Show(false);
                }
            }
            else//nincs megadva
            {
                ex_box->Enable(true);
                ex_but->Enable(true);    
            }
        }
        else//egyenletek
        {
            eh.addEQ(std::string(line.mb_str()));
            try 
            {
                eh.lengyel_forma(eh.getActeq());
            }
            catch(equation_handler::errors err)
            {
                if(err==equation_handler::UNKNOWN)
                {
                    wxMessageBox(wxString::Format("Unknown symbol or operator in %i. equation",eh.getActeq()+1),_("Error in data"));
                    newclick();
                    return;
                }
                if(err==equation_handler::OPERATOR_END)
                {
                    wxMessageBox(_("Invalid end of equation"),_("Error in data"));
                    newclick();
                    return;
                }
                if(err==equation_handler::XI_ERROR)
                {
                    wxMessageBox(_("Invalid variable. You must use variable x1..xn."),_("Invalid variable"));    
                    newclick();
                    return;
                }
                if(err==equation_handler::INVALID_BRACKET)
                {
                    wxMessageBox(wxString::Format("Invalid brackets in %i equation.",eh.getActeq()+1),_("Invalid brackets"));   
                    newclick();
                    return;
                }
            }
            line+="=0";
            equations[eh.getActeq()]=new wxStaticText(this, 3000+eh.getActeq(),
                                        _(line), wxPoint(60, 180+30*eh.getActeq()), 
                                        wxDefaultSize, 0, _("eq_"+eh.getActeq()));
            if(eh.getN()<=5) 
                equations[eh.getActeq()]->Show(true);
            else 
            {
                equations[eh.getActeq()]->Show(false);
                eq_tm->Show(true);
                eq_show->Show(true);
            }
            eh.setActeq(eh.getActeq()+1);
            if(eh.getActeq()==eh.getN())
            {
                solve->Show(true);
                solve->Enable(true);
            }
            
            eq_stat->Show(false);
            eq_box->Show(false);
            eq_box->Enable(true);
            eq_stat_nulla->Show(false);
            eq_but->Show(false);
            eq_but->Enable(true);
            eq_edit->Show(true);
            eq_num->Show(true);
        }
        i++;
    }
    for(int i=1;i<=eh.getN();i++)//legördülõ lista feltöltése
	{
        eq_num->Append(wxString::Format("%i",i));
    }
    
    degree->Enable(true);
    radian->Enable(true);
    
    k_edit->Enable(true);

    epsilon_edit->Enable(true);
    
    kmax_but->Enable(true);
    kmax_box->Enable(true);
}

//solve
void nonlinsolverFrame::solveClick(wxCommandEvent& event)
{  
//minden elrejtése
    numbeq_but->Show(false);
    numbeq_box->Show(false);
    numbeq_stat->Show(false);
    numbeq_stat2->Show(false);
    numbeq_edit->Show(false);
    
    startvect_stat2->Show(false);
    startvect_stat->Show(false);
    startvect_box->Show(false);
    startvect_but->Show(false);
    startvect_edit->Show(false);
    startvect_show->Show(false);
    
    ex_stat2->Show(false);
    ex_stat->Show(false);
    ex_box->Show(false);
    ex_but->Show(false);
    ex_edit->Show(false);
    ex_show->Show(false);
    
    k_stat2->Show(false);
    k_stat->Show(false);
    k_box->Show(false);
    k_but->Show(false);
    k_edit->Show(false);
    
    kmax_stat2->Show(false);
    kmax_stat->Show(false);
    kmax_box->Show(false);
    kmax_but->Show(false);
    kmax_edit->Show(false);
    
    epsilon_stat2->Show(false);
    epsilon_stat->Show(false);
    epsilon_box->Show(false);
    epsilon_but->Show(false);
    epsilon_edit->Show(false);
    
    eq_tm->Show(false);
    eq_but->Show(false);
    eq_stat->Show(false);
    eq_stat_nulla->Show(false);
    eq_box->Show(false);
    eq_num->Show(false);
    eq_num->Clear();
    eq_edit->Show(false);
    eq_show->Show(false);  
    eq_up->Show(false);
    eq_down->Show(false);  
    for(int i=0;i<eh.getN();i++)
    {
        equations[i]->Show(false);
    }
    
    degree->Show(false);
    radian->Show(false);
    
    solve->Show(false);
    
    edited=-1;
    //    
    if(deg_rad) solv.changeDegRad(1);//fok vagy radián beállítása. ha radián
    else solv.changeDegRad(0);//ha fok
	solv.addF(eh.getLengyel());//egyenletrendszer hozzáadása a megoldóhoz
	std::vector<double> ki;
	if(eh.getEx()!="")//ha van pontos megoldás, akkor azt is hozzáadjuk
    {
        result_exact_show=true;
        solv.setExact(eh.getEx_vect());
    }
    double sec;
	try
	{
        clock_t t=clock();//idõ mérése
        ki=solv.cont(eh.getX0_vect());
        t=clock()-t;
        sec=((float)t)/CLOCKS_PER_SEC;
    }
    catch(errors err)
    {
        if(err==NOT_CONVERGENT)
        {//ha nem konvergál
            wxMessageBox("The method is not convergent. Try to edit the epsilon, delta or maximum steps parameters.","Not convergent");
            result_exact_show=false;
            visszaallit();
            return;
        }
    }
    solved=true;
    
    time->Show(true//idõ kiírása
    time_text->Show(true);
    wxString ido=wxNumberFormatter::ToString(sec,3);
    time->SetLabel(ido+" sec");
    
    back->Show(true);
    wxString str="( ";//eredmény kiírása
    int seged=1;
    for(int i=0;i<ki.size();i++)
    {
        wxString temp=wxNumberFormatter::ToString(ki[i],10);
        
        if(str.size()>40*seged)//ha túl hosszú, akkor sortörés
        {
            str+="\n";
            seged++;
        }       
        str+=temp;
        
        if(i+1!=ki.size()) str+=";   ";
    }	
    str+=" )";
	
    result->SetLabel(str);
    result->Show(true);
    result_text->Show(true);

    g_res->Show(true);
    wxString temp=wxNumberFormatter::ToString(norm(solv.eval(ki)),10);
    g_res->SetLabel(temp);
    result_label1->Show(true);
    
    vectorx.resize(0);
    vectory.resize(0);
    vectorx2.resize(0);
    vectory2.resize(0);

//g ábrázolása
    zoom_left->Show(true);
    
	mpFXYVector* vectorLayer = new mpFXYVector(_(""));
	vectory=solv.getXiNorm();
	for (int p=0;p<vectory.size();p++) {
		vectorx.push_back(p);
	}
	vectorLayer->SetData(vectorx, vectory);
	vectorLayer->SetContinuity(true);
	wxPen vectorpen(*wxBLUE, 2, wxSOLID);
	vectorLayer->SetPen(vectorpen);
	vectorLayer->SetDrawOutsideMargins(false);
	
	mpFXYVector* vl=new mpFXYVector(_(""));
	conv1Y=solv.getConv1();
	for(int p=0;p<conv1Y.size();p++)
	{
        conv1X.push_back(p);
    }
    int i=1;
    while(i<conv1Y.size())
    {
        int nexti=i;
        if(conv1Y[i]!=-1)
        {
            for(int j=i+1;j<conv1Y.size();j++)//következõ nem -1 megkeresése
            {
                if(conv1Y[j]!=-1)
                {
                    nexti=j;
                    break;
                }
            }
            if(i==nexti) break;
            for(int k=i+1;k<nexti;k++)
            {
                conv1Y[k]=conv1Y[i]-(k-i)*(conv1Y[i]-conv1Y[nexti])/(nexti-i);//interpoláció
            }
        }
        i=nexti;
    }
    
    vl->SetData(conv1X, conv1Y);
	vl->SetContinuity(true);
	wxPen vo(*wxGREEN, 2, wxSOLID);
	vl->SetPen(vo);
	vl->SetDrawOutsideMargins(false);

	wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_plot = new mpWindow( WxPanel1, ID_M_PLOT, wxPoint(0,105), wxSize(225,225), wxSUNKEN_BORDER );
    mpScaleX* xaxis = new mpScaleX(wxT(""), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT(""), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);
    m_plot->SetMargins(20, 30, 30, 40);//top right bottom left
    m_plot->AddLayer( xaxis );
    m_plot->AddLayer( yaxis );
	m_plot->AddLayer( vectorLayer );
	
	m_plot->AddLayer(vl);

    m_plot->EnableDoubleBuffer(true);
    m_plot->SetMPScrollbars(false);
    m_plot->Fit(0,vectorx.size()*1.1,0,vectory[0]*1.1);//xmin xmax ymin ymax
//h megjelenítése, ha kell
    if(result_exact_show)
    {
       zoom_right->Show(true); 
       
       h_res->Show(true);
       wxString temp=wxNumberFormatter::ToString(norm(ki+eh.getEx_vect()*(-1)),10);
       h_res->SetLabel(temp);
       
       result_label2->Show(true);  
       mpFXYVector* vectorLayer2 = new mpFXYVector(_(""));
       std::vector<double> x_exact=eh.getEx_vect();
       vectory2=solv.getXiVect();
	   for (int p=0;p<solv.getXiVect().size();p++)
       {
	       vectorx2.push_back(p);
	   }
	   vectorLayer2->SetData(vectorx2, vectory2);
	   vectorLayer2->SetContinuity(true);
	   wxPen vectorpen2(*wxRED, 2, wxSOLID);
	   vectorLayer2->SetPen(vectorpen2);
	   vectorLayer2->SetDrawOutsideMargins(false);
	   
	   mpFXYVector* vl2=new mpFXYVector(_(""));
	   conv2Y=solv.getConv2();
	   for(int p=0;p<conv2Y.size();p++)
	   {
            conv2X.push_back(p);
        }
        int i=1;
        while(i<conv2Y.size())
        {
            int nexti=i;
            if(conv2Y[i]!=-1)
            {
                for(int j=i+1;j<conv2Y.size();j++)//következõ nem -1 megkeresése
                {
                    if(conv2Y[j]!=-1)
                    {
                        nexti=j;
                        break;
                    }
                }
                if(i==nexti) break;
                for(int k=i+1;k<nexti;k++)
                {
                    conv2Y[k]=conv2Y[i]-(k-i)*(conv2Y[i]-conv2Y[nexti])/(nexti-i);//interpoláció
                }
            }
            i=nexti;
        }
    
       vl2->SetData(conv2X, conv2Y);
	   vl2->SetContinuity(true);
       wxPen vo2(*wxGREEN, 2, wxSOLID);
	   vl2->SetPen(vo2);
	   vl2->SetDrawOutsideMargins(false);

	   wxFont graphFont2(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
       m_plot2 = new mpWindow( WxPanel1, ID_M_PLOT2, wxPoint(255,105), wxSize(225,225), wxSUNKEN_BORDER );
       mpScaleX* xaxis2 = new mpScaleX(wxT(""), mpALIGN_BOTTOM, true, mpX_NORMAL);
       mpScaleY* yaxis2 = new mpScaleY(wxT(""), mpALIGN_LEFT, true);
       xaxis2->SetFont(graphFont2);
       yaxis2->SetFont(graphFont2);
       xaxis2->SetDrawOutsideMargins(false);
       yaxis2->SetDrawOutsideMargins(false);
       m_plot2->SetMargins(20, 30, 30, 40);//top right bottom left
       m_plot2->AddLayer( xaxis2 );
       m_plot2->AddLayer( yaxis2 );
	   m_plot2->AddLayer( vectorLayer2 );
	   
	   m_plot2->AddLayer(vl2);

       m_plot2->EnableDoubleBuffer(true);
       m_plot2->SetMPScrollbars(false);
       m_plot2->Fit(0,vectorx2.size()*1.1,0,vectory2[0]*1.1);//xmin xmax ymin ymax
    }
}

//exact solution
void nonlinsolverFrame::ex_butClick(wxCommandEvent& event)//OK
{
	wxString str=ex_box->GetValue();
	
	str+=wxT('\0');
	if(str[0]=='\0')//ha ninc megadva semmi 
    {
        wxMessageBox(_("Please insert a real vector!"),_("No exact solution"));
        return;
    }

	eh.setEx(std::string((str).mb_str()));
	
	ex_showed=false;
	ex_show->SetLabel("Show exact solution");
	ex_box->Show(false);
	ex_stat2->Show(true);
	ex_but->Show(false);
    
    
    wxString temp(eh.getEx().c_str(),wxConvUTF8);
    if(temp.size()<=28)
    {	
    	for(int i=0;i<temp.size();i++)
	       if(temp[i]=='.') temp[i]=',';
    	
    	ex_stat2->SetLabel(temp);
    	ex_edit->Show(true);
    }
    else//ha túl hosszú
    {
        ex_stat2->SetLabel("Too long to show!");
        ex_show->Show(true);
    }
    result_exact_show=true;
}

void nonlinsolverFrame::ex_editClick(wxCommandEvent& event)//Edit
{
    ex_edit->Show(false);
	ex_but->Show(true);
	ex_stat2->Show(false);
	ex_box->Show(true);
	ex_box->Enable(true);
	ex_but->Enable(true);
	
	wxString str(eh.getEx().c_str(), wxConvUTF8);
	if(str=="") return;
	str.erase(str.begin());
	str.erase(str.end()-1);	
	
	for(int i=0;i<str.size();i++)
	   if(str[i]=='.') str[i]=',';
	
	ex_box->SetLabel(str);
}

void nonlinsolverFrame::ex_showClick(wxCommandEvent& event)//Show
{
	if(!ex_showed)//ha nincs megjelenítve
    {
    	wxString temp(eh.getEx().c_str(),wxConvUTF8);
    	temp.erase(temp.begin());
    	temp.erase(temp.end()-1);   	
    	ex_box->Show(true);
    	ex_box->SetLabel(temp);
    	ex_show->SetLabel("Hide exact solution");
    	ex_showed=true;
    	ex_stat2->Show(false);
    }
    else//ha meg va jelenítve
    {
        wxString str=ex_box->GetValue();
        str+=wxT('\0');
        
        ex_stat2->Show(true);
        ex_stat2->SetLabel("Hid, click to show");
    	ex_box->Show(false);
    	ex_show->SetLabel("Show exact solution");
    	ex_showed=false;

	    eh.setEx(std::string((str).mb_str()));
    }
}

//radio buttons
void nonlinsolverFrame::degreeClick(wxCommandEvent& event)//fok
{
    degree->SetValue(true);	
    radian->SetValue(false);
    deg_rad=true;
}

void nonlinsolverFrame::radianClick(wxCommandEvent& event)//radián
{
	radian->SetValue(true);
	degree->SetValue(false);
	deg_rad=false;
}

//zoom
void nonlinsolverFrame::zoom_leftClick(wxCommandEvent& event)//g
{
    zoom = new myFrmZoom(vectorx,vectory,conv1X,conv1Y,*wxBLUE,this);
    zoom->Show();
}

void nonlinsolverFrame::zoom_rightClick(wxCommandEvent& event)//h
{
	zoom = new myFrmZoom(vectorx2,vectory2,conv2X,conv2Y,*wxRED,this);
    zoom->Show();
}

//newclick
void nonlinsolverFrame::newclick()
{
//mindent eredeti helyzetbe állít
    numbeq_stat->Show(true);
    numbeq_stat2->Show(false);
    numbeq_box->SetLabel("");
    numbeq_stat2->SetLabel("");
    numbeq_but->Show(true);
    numbeq_box->Show(true);
    numbeq_edit->Show(false);
    
    startvect_box->SetLabel("x01;x02;...;x0n");
    startvect_stat2->Show(false);
    startvect_stat2->SetLabel("");
    startvect_stat->Show(true);
    startvect_box->Show(true);
    startvect_box->Enable(false);
    startvect_but->Enable(false);
    startvect_but->Show(true);
    startvect_edit->Show(false);
    startvect_show->Show(false);
    startvect_show->SetLabel("Show startvector");
    
    ex_stat2->Show(false);
    ex_stat2->SetLabel("");
    ex_stat->Show(true);
    ex_box->Show(true);
    ex_box->SetLabel("");
    ex_box->Enable(false);
    ex_but->Enable(false);
    ex_but->Show(true);
    ex_edit->Show(false);
    ex_show->Show(false);
    ex_show->SetLabel("Show exact solution");
    
    epsilon_stat->Show(true);
    epsilon_stat2->Show(true);
    epsilon_stat2->SetLabel("0.001");
    epsilon_box->Show(false);
    epsilon_but->Show(false);
    epsilon_edit->Show(true);
    epsilon_edit->Enable(false);
    
    k_stat->Show(true);
    k_stat2->Show(true);
    k_stat2->SetLabel("10");
    k_box->Show(false);
    k_box->SetLabel("");
    k_but->Show(false);
    k_edit->Show(true);
    k_edit->Enable(false);
    
    kmax_stat->Show(true);
    kmax_stat2->Show(false);
    kmax_stat2->SetLabel("");
    kmax_box->Show(true);
    kmax_but->Show(true);
    kmax_but->Enable(false);
    kmax_box->Enable(false);
    kmax_box->SetLabel("");
    kmax_edit->Show(false);
    
    eq_tm->Show(false);
    eq_tm->SetLabel("Too much equations to show!");
    eq_but->Show(true);
    eq_stat->Show(true);
    eq_stat_nulla->Show(true);
    eq_box->Show(true);
    eq_box->Enable(false);
    eq_but->Enable(false);
    eq_box->SetLabel("");
    eq_stat->SetLabel("1. equation:");
    eq_num->Show(false);
    eq_num->Clear();
    eq_edit->Show(false);
    eq_show->Show(false);
    eq_show->SetLabel("Show equations");
    eq_up->Show(false);
    eq_down->Show(false);
    for(int i=0;i<eh.getActeq();i++)
    {
        delete equations[i];
    }
    
    degree->Enable(false);
    radian->Enable(false);
    degree->Show(true);
    radian->Show(true);
    radian->SetValue(true);
    degree->SetValue(false);
    back->Show(false);
    
    time->Show(false);
    time_text->Show(false);
    time->SetLabel("");
    
    solve->Show(false);
    g_res->Show(false);
    h_res->Show(false);
    g_res->SetLabel("");
    h_res->SetLabel("");
    
    if(solved) 
    {
        m_plot->Show(false);
        if(result_exact_show)
            m_plot2->Show(false);
    }
    
    result_text->Show(false);
    result->Show(false);
    result->SetLabel("");
	result_label1->Show(false);
	result_label2->Show(false);
	
	zoom_left->Show(false);
	zoom_right->Show(false);
			
	edited=-1;
	startvect_showed=false;
	eq_showed=false;
	ex_showed=false;
	solvable=0;
	eq_showed_numb=0;
	result_exact_show=false;
	deg_rad=false;
	solved=false;
	
	vectorx.resize(0);
	vectory.resize(0);
	vectorx2.resize(0);
	vectory2.resize(0);
	conv1X.resize(0);
	conv1Y.resize(0);
	conv2X.resize(0);
	conv2Y.resize(0);
	
	eh.newclick();
	solv.newclick();    
}

//projekt mentése
void nonlinsolverFrame::Save_Click(wxCommandEvent& event)
{
    if(solvable==2+eh.getN())
    {
        wxFileDialog //hova kiválasztása
            saveFileDialog(this, _("Save project"), "", "",
                           "Text files (*.txt)|*.txt", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
        if (saveFileDialog.ShowModal() == wxID_CANCEL)
            return;

        wxTextFile file( saveFileDialog.GetPath() );
        file.Create();
        file.Open();
        file.Clear();//felülírás
 
        file.AddLine(wxString::Format("%i",eh.getN()));
    
        wxString x0(eh.getX0().c_str(),wxConvUTF8);
        x0.erase(x0.begin());
        x0.erase(x0.end()-1);
        wxString temp="";
        for(int i=0;i<x0.size();i++)
        {
	      if(x0[i]=='.') temp+=',';
	       else if(x0[i]!=' ') temp+=x0[i];
        }
        file.AddLine(temp);
    
        wxString ex(eh.getEx().c_str(),wxConvUTF8);
        if(ex.size()==0) file.AddLine(wxT("unknown"));
        else
        {
            ex.erase(ex.begin());
            ex.erase(ex.end()-1);
            wxString temp2="";
            for(int i=0;i<ex.size();i++)
            {
	          if(ex[i]=='.') temp2+=',';
	           else if(ex[i]!=' ') temp2+=ex[i];
            }
            file.AddLine(temp2);
        }

        for(int i=0;i<eh.getN();i++)
        {
            wxString eq(eh.getEq(i).c_str(),wxConvUTF8);
            eq.erase(eq.end()-1);
            file.AddLine(eq);    
        }
 
        file.Write();
        file.Close();
    }
    else//nincs minden adat kitöltve
    {
        wxMessageBox("Can not save project at this state","Incomplete project");
        return;    
    }
}

//grafikonok mentése
void nonlinsolverFrame::Savegraph_Click(wxCommandEvent& event)
{
    if(solved)//ha már meg van oldva, akkor van mit menteni csak
    {
        if(!result_exact_show)//ha csak egy grafikon van
        {
            wxFileDialog//hova kiválasztása 
                    saveFileDialog(this, _("Save graph"), "", "",
                                   "Bitmap files (*.bmp)|*.bmp", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
            if (saveFileDialog.ShowModal() == wxID_CANCEL)
                return;
        
            m_plot->SaveScreenshot(saveFileDialog.GetPath(),wxBITMAP_TYPE_BMP,wxSize(800,600),false);
        }
        else//ha kétgrafikon is van
        {
            myDial=new GraphDial(this);
            {
                myDial->ShowModal();
                int which=myDial->getWhich();
                myDial->Destroy();
                if(which==1) 
                {
                    wxFileDialog 
                        saveFileDialog(this, _("Save graph"), "", "",
                            "Bitmap files (*.bmp)|*.bmp", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
                    if (saveFileDialog.ShowModal() == wxID_CANCEL)
                        return;
        
                    m_plot->SaveScreenshot(saveFileDialog.GetPath(),wxBITMAP_TYPE_BMP,wxSize(800,600),false);    
                }
                else if(which==2)
                {
                    wxFileDialog 
                        saveFileDialog(this, _("Save graph"), "", "",
                            "Bitmap files (*.bmp)|*.bmp", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
                    if (saveFileDialog.ShowModal() == wxID_CANCEL)
                        return;
        
                    m_plot2->SaveScreenshot(saveFileDialog.GetPath(),wxBITMAP_TYPE_BMP,wxSize(800,600),false);    
                }   
            }
            
        }
    }
    else//nincs mit menteni
    {
        wxMessageBox("There is no graph to save","No graph");
        return;     
    }
}

//back
void nonlinsolverFrame::backClick(wxCommandEvent& event)
{
    visszaallit();
}

//parameters
//epsilon
void nonlinsolverFrame::epsilon_butClick(wxCommandEvent& event)//OK
{
	wxString str=epsilon_box->GetValue();
	
	if(str=="")
    {
        return;
    }

    wxString temp="";
    for(int i=0;i<str.size();i++)
        if(str[i]=='.') temp+=',';
        else temp+=str[i];

    double c=wxAtof(temp);
    if(c<=0)//hibás adat
    {
         wxMessageBox("Invalid parameter. Please enter a positive number!","Invalid parameter");
         return;   
    }
    else
    {
	   solv.setEpsilon(c);
	
	   epsilon_box->Show(false);
	   epsilon_stat2->Show(true);
	   epsilon_but->Show(false);
    
        epsilon_stat2->SetLabel(str);
        epsilon_edit->Show(true);
    }
}

void nonlinsolverFrame::epsilon_editClick(wxCommandEvent& event)//Edit
{
	epsilon_edit->Show(false);
	epsilon_but->Show(true);
	epsilon_stat2->Show(false);
	epsilon_box->Show(true);
	epsilon_box->Enable(true);
	epsilon_but->Enable(true);
	
	wxString str=epsilon_stat2->GetLabel();
	
	for(int i=0;i<str.size();i++)
	   if(str[i]=='.') str[i]=',';
	
	epsilon_box->SetLabel(str);
}
//delta
void nonlinsolverFrame::k_butClick(wxCommandEvent& event)//OK
{
	wxString str=k_box->GetValue();
	
	if(str=="") 
    {
        return;
    }

    double c=wxAtoi(str);
	if(c<=0)//hibás adat
    {
         wxMessageBox("Invalid parameter. Please enter a positive number!","Invalid parameter");
         return;   
    }
    else
    {   
        solv.setK(c);
	
	   k_box->Show(false);
	   k_stat2->Show(true);
	   k_but->Show(false);
    
        k_stat2->SetLabel(str);
        k_edit->Show(true);
    }
}

void nonlinsolverFrame::k_editClick(wxCommandEvent& event)//Eit
{
	k_edit->Show(false);
	k_but->Show(true);
	k_stat2->Show(false);
	k_box->Show(true);
	k_box->Enable(true);
	k_but->Enable(true);
	
	wxString str=k_stat2->GetLabel();
	
	for(int i=0;i<str.size();i++)
	   if(str[i]=='.') str[i]=',';
	
	k_box->SetLabel(str);
}
//maximum steps
void nonlinsolverFrame::kmax_butClick(wxCommandEvent& event)//OK
{
    wxString str=kmax_box->GetValue();
	
	if(str=="") 
    {
        return;
    }
    
    double c=wxAtoi(str);
    if(c<=0)//hibás adat
    {
         wxMessageBox("Invalid parameter. Please enter a positive number!","Invalid parameter");
         return;   
    }
    else
    {
	   solv.setKmax(wxAtoi(str));
	
	   kmax_box->Show(false);
	   kmax_stat2->Show(true);
	   kmax_but->Show(false);
    
        kmax_stat2->SetLabel(str);
        kmax_edit->Show(true);
    }
}

void nonlinsolverFrame::kmax_editClick(wxCommandEvent& event)//Edit
{
    kmax_edit->Show(false);
	kmax_but->Show(true);
	kmax_stat2->Show(false);
	kmax_box->Show(true);
	kmax_box->Enable(true);
	kmax_but->Enable(true);
	
	wxString str=kmax_stat2->GetLabel();
	
	for(int i=0;i<str.size();i++)
	   if(str[i]=='.') str[i]=',';
	
	kmax_box->SetLabel(str);
	
}

//back
void nonlinsolverFrame::visszaallit()
{//mindent a megoldás elõtti helyzetbe állít vissza
    back->Show(false);
    
    time->Show(false);
    time_text->Show(false);

    numbeq_stat->Show(true);
    numbeq_stat2->Show(true);
    numbeq_edit->Show(true);
    
    ex_stat->Show(true);
    std::string temp2=eh.getEx();
    if(temp2=="")
    {
        ex_box->Show(true);
        ex_box->SetLabel("");
        ex_but->Show(true);    
    }
    else
    {
        if(temp2.size()<=28)
        {
            ex_stat2->SetLabel(temp2);
            ex_stat2->Show(true);
            ex_edit->Show(true);
        } 
        else
        {
            if(ex_showed)
            {
                ex_box->Show(true);
                ex_show->Show(true);
            }
            else
            {
                ex_stat2->SetLabel("Hid, click to show!");
                ex_stat2->Show(true);
                ex_show->Show(true); 
            }
        }   
    }
    
    startvect_stat->Show(true);
    std::string temp=eh.getX0();
    if(temp.size()<=28)
    {
        startvect_stat2->SetLabel(temp);
        startvect_stat2->Show(true);
        startvect_edit->Show(true);   
    }
    else
    {
        if(startvect_showed)
        {
            startvect_box->Show(true);
            startvect_show->Show(true);
        }
        else
        {
            startvect_stat2->SetLabel("Hid, click to show!");
            startvect_stat2->Show(true);
            startvect_show->Show(true); 
        }
    }
    
    k_stat2->Show(true);
    k_stat->Show(true);
    k_edit->Show(true);
    k_edit->Enable(true);
    
    kmax_stat2->Show(true);
    kmax_stat->Show(true);
    kmax_edit->Show(true);
    kmax_edit->Enable(true);
    kmax_stat2->SetLabel(wxString::Format("%i",solv.getKmax()));
    
    epsilon_stat2->Show(true);
    epsilon_stat->Show(true);
    epsilon_edit->Show(true);
    epsilon_edit->Enable(true);
    
    eq_num->Show(true);
    for(int i=1;i<=eh.getN();i++)
	{
        eq_num->Append(wxString::Format("%i",i));
    }
    eq_edit->Show(true);
    if(equations.size()<=5)
    {
        for(int i=0;i<eh.getN();i++)
        {
            equations[i]->Show(true);
        }
    }
    else
    {
        eq_show->Show(true);
        if(eq_showed)
        {
            for(int i=0;i<eq_showed_numb+5;i++)
            {
                equations[i]->Show(true);
            }   
            eq_up->Show(true);
            eq_down->Show(true);
        }   
        
        
        else
        {
            eq_tm->Show(true);  
        } 
    }
    
    degree->Show(true);
    radian->Show(true);
    
    solve->Show(true);
    solv.backclick();
    
    vectorx.resize(0);
	vectory.resize(0);
	vectorx2.resize(0);
	vectory2.resize(0);
	conv1X.resize(0);
	conv1Y.resize(0);
	conv2X.resize(0);
	conv2Y.resize(0);
    
    result->Show(false);
    result_text->Show(false);
    result_label1->Show(false);
    zoom_left->Show(false);
    g_res->Show(false);
    h_res->Show(false);
    if(solved)
    {
        m_plot->Show(false);
    }
 
    if(result_exact_show && solved)
    {
       zoom_right->Show(false);  
       result_label2->Show(false);
       m_plot2->Show(false);
    }   
    
    solved=false;
    edited=-1;
}