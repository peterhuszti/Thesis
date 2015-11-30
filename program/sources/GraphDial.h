#ifndef __GRAPHDIAL_H__
#define __GRAPHDIAL_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

#include <wx/button.h>
#include <wx/stattext.h>

#undef GraphDial_STYLE
#define GraphDial_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX

class GraphDial : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
			
	private:
		wxButton *cancel;
		wxButton *right;
		wxButton *left;
		wxStaticText *title;
		
		int which;

		enum
		{
			ID_CANCEL = 1004,
			ID_RIGHT = 1003,
			ID_LEFT = 1002,
			ID_TITLE = 1001,
		};
	
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		
	public:
		GraphDial(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Select a graph"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = GraphDial_STYLE);
		virtual ~GraphDial();
		void leftClick(wxCommandEvent& event);
		void rightClick(wxCommandEvent& event);
		void cancelClick(wxCommandEvent& event);
		
		int getWhich();
};

#endif
