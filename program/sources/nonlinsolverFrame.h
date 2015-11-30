#include "equation_handler.h"
#include "solver.h"
#include "mathplot.h"
#include "myFrmZoom.h"
#include "GraphDial.h"
#include <wx/log.h>

#ifndef __nonlinsolverFrame_H__
#define __nonlinsolverFrame_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

#include <wx/menu.h>
#include <wx/radiobut.h>
#include <wx/bmpbuttn.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#undef nonlinsolverFrame_STYLE
#define nonlinsolverFrame_STYLE wxCAPTION | wxMINIMIZE_BOX | wxCLOSE_BOX

class nonlinsolverFrame : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
//elemek
		wxMenuBar *Menu;
		wxStaticText *time;
		wxStaticText *time_text;
		wxStaticText *h_res;
		wxStaticText *g_res;
		wxStaticText *kmax_stat2;
		wxTextCtrl *kmax_box;
		wxButton *kmax_edit;
		wxButton *kmax_but;
		wxStaticText *kmax_stat;
		wxStaticText *k_stat2;
		wxStaticText *epsilon_stat2;
		wxTextCtrl *k_box;
		wxTextCtrl *epsilon_box;
		wxStaticText *epsilon_stat;
		wxStaticText *k_stat;
		wxButton *k_edit;
		wxButton *k_but;
		wxButton *epsilon_edit;
		wxButton *epsilon_but;
		wxButton *back;
		wxButton *zoom_right;
		wxButton *zoom_left;
		wxStaticText *result_label1;
		wxStaticText *result_label2;
		wxStaticText *ex_stat2;
		wxButton *ex_show;
		wxButton *ex_edit;
		wxButton *ex_but;
		wxTextCtrl *ex_box;
		wxStaticText *ex_stat;
		wxRadioButton *degree;
		wxRadioButton *radian;
		wxBitmapButton *eq_down;
		wxBitmapButton *eq_up;
		wxStaticText *result;
		wxStaticText *result_text;
		wxButton *solve;
		wxButton *startvect_show;
		wxButton *eq_show;
		wxChoice *eq_num;
		wxButton *eq_edit;
		wxButton *eq_but;
		wxStaticText *eq_stat_nulla;
		wxTextCtrl *eq_box;
		wxStaticText *eq_stat;
		wxStaticText *eq_tm;
		wxButton *startvect_edit;
		wxButton *startvect_but;
		wxStaticText *startvect_stat2;
		wxTextCtrl *startvect_box;
		wxStaticText *startvect_stat;
		wxButton *numbeq_edit;
		wxButton *numbeq_but;
		wxStaticText *numbeq_stat2;
		wxTextCtrl *numbeq_box;
		wxStaticText *numbeq_stat;
		wxPanel *WxPanel1;
		wxFlexGridSizer *WxFlexGridSizer1;
		std::vector<wxStaticText*> equations;
		GraphDial* myDial;
		mpWindow *m_plot;
		mpWindow *m_plot2;
		myFrmZoom* zoom;
//segédváltozók
		solver solv;
		equation_handler eh;
		
		int edited;//valamelyik egyenlet módosítva van-e
		
		bool startvect_showed;//ha túl hosszú a feladat, az adott mezők meg vannak-e jelenítv
		bool eq_showed;//
		bool ex_showed;//
		int eq_showed_numb;//első megjelenített egyenlet
		
		int solvable;//minden megfelelően ki van-e töltve
		bool result_exact_show;//meg van-e adva pontos megoldás
		bool deg_rad;//for vagy radián
		bool solved;//meg va-e oldva a feladat
		
		std::vector<double> vectorx;//vektorok az ábrázoláshoz
		std::vector<double> vectory;
		std::vector<double> vectorx2;
		std::vector<double> vectory2;
		std::vector<double> conv1X;
		std::vector<double> conv1Y;
		std::vector<double> conv2X;
		std::vector<double> conv2Y;
//segéd eljárások
		void newclick();//new
		void visszaallit();//back

		enum
		{
			ID_MNU_FILE_1001 = 1001,
			ID_MNU_NEW_1003 = 1003,
			ID_MNU_OPEN_1007 = 1007,
			ID_MNU_SAVE_1108 = 1108,
			ID_MNU_SAVEGRAPH_1109 = 1109,
			ID_MNU_QUIT_1004 = 1004,
			ID_MNU_HELP_1002 = 1002,
			ID_MNU_DOCUMENTATION_1130 = 1130,
			ID_MNU_ABOUT_F1_1006 = 1006,
			ID_TIME = 1129,
			ID_TIME_TEXT = 1128,
			ID_H_RES = 1127,
			ID_G_RES = 1126,
			ID_KMAX_STAT2 = 1125,
			ID_KMAX_BOX = 1124,
			ID_KMAX_EDIT = 1123,
			ID_KMAX_BUT = 1122,
			ID_KMAX_STAT = 1121,
			ID_K_STAT2 = 1120,
			ID_EPSILON_STAT2 = 1119,
			ID_K_BOX = 1118,
			ID_EPSILON_BOX = 1117,
			ID_EPSILON_STAT = 1116,
			ID_K_STAT = 1115,
			ID_K_EDIT = 1114,
			ID_K_BUT = 1113,
			ID_EPSILON_EDIT = 1112,
			ID_EPSILON_BUT = 1111,
			ID_BACK = 1110,
			ID_ZOOM_RIGHT = 1107,
			ID_ZOOM_LEFT = 1106,
			ID_RESULT_LABEL1 = 1105,
			ID_RESULT_LABEL2 = 1104,
			ID_EX_STAT2 = 1103,
			ID_EX_SHOW = 1102,
			ID_EX_EDIT = 1101,
			ID_EX_BUT = 1100,
			ID_EX_BOX = 1099,
			ID_EX_STAT = 1098,
			ID_DEGREE = 1095,
			ID_RADIAN = 1094,
			ID_EQ_DOWN = 1091,
			ID_EQ_UP = 1090,
			ID_RESULT = 1088,
			ID_RESULT_TEXT = 1087,
			ID_SOLVE = 1086,
			ID_STARTVECT_SHOW = 1084,
			ID_EQ_SHOW = 1083,
			ID_EQ_NUM = 1082,
			ID_EQ_EDIT = 1081,
			ID_EQ_BUT = 1080,
			ID_EQ_STAT_NULLA = 1079,
			ID_EQ_BOX = 1078,
			ID_EQ_STAT = 1077,
			ID_EQ_TM = 1076,
			ID_STARTVECT_EDIT = 1075,
			ID_STARTVECT_BUT = 1074,
			ID_STARTVECT_STAT2 = 1073,
			ID_STARTVECT_BOX = 1072,
			ID_STARTVECT_STAT = 1071,
			ID_NUMBEQ_EDIT = 1070,
			ID_NUMBEQ_BUT = 1069,
			ID_NUMBEQ_STAT2 = 1068,
			ID_NUMBEQ_BOX = 1067,
			ID_NUMBEQ_STAT = 1033,
			ID_WXPANEL1 = 1030,
			ID_M_PLOT = 2000,
			ID_M_PLOT2 = 2001,
		};
		
	public:
		nonlinsolverFrame(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Nonlinear equation system solver"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = nonlinsolverFrame_STYLE);
        void numbeq_butClick(wxCommandEvent& event);
		void About_Click(wxCommandEvent& event);
		virtual ~nonlinsolverFrame();
		void Quit_Click(wxCommandEvent& event);
		void startvect_butClick(wxCommandEvent& event);
		void eq_butClick(wxCommandEvent& event);
		void New_Click(wxCommandEvent& event);
		void numbeq_editClick(wxCommandEvent& event);
		void startvect_editClick(wxCommandEvent& event);
		void eq_editClick(wxCommandEvent& event);
		void Open_Click(wxCommandEvent& event);
		void startvect_showClick(wxCommandEvent& event);
		void eq_showClick(wxCommandEvent& event);
		void solveClick(wxCommandEvent& event);
		void eq_upClick(wxCommandEvent& event);
		void eq_downClick(wxCommandEvent& event);
		void startvect_leftClick(wxCommandEvent& event);
		void startvect_rightClick(wxCommandEvent& event);
		void ex_butClick(wxCommandEvent& event);
		void ex_editClick(wxCommandEvent& event);
		void ex_showClick(wxCommandEvent& event);
		void degreeClick(wxCommandEvent& event);
		void radianClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		void zoom_leftClick(wxCommandEvent& event);
		void zoom_rightClick(wxCommandEvent& event);
		void Mnusave1108Click(wxCommandEvent& event);
		void Save_Click(wxCommandEvent& event);
		void Save_Click0(wxCommandEvent& event);
		void Savegraph_Click(wxCommandEvent& event);
		void Savegraph_Click0(wxCommandEvent& event);
		void backClick(wxCommandEvent& event);
		void epsilon_butClick(wxCommandEvent& event);
		void epsilon_editClick(wxCommandEvent& event);
		void k_butClick(wxCommandEvent& event);
		void k_editClick(wxCommandEvent& event);
		void kmax_editClick(wxCommandEvent& event);
		void kmax_butClick(wxCommandEvent& event);
		void Doc_Click(wxCommandEvent& event);
		void doc_butClick(wxCommandEvent& event);
};

#endif
