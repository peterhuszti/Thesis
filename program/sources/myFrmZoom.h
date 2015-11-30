#include <vector>
#include <wx/log.h>
#include "mathplot.h"

#ifndef __MYFRMZOOM_H__
#define __MYFRMZOOM_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

#include <wx/panel.h>
#include <wx/sizer.h>

#undef myFrmZoom_STYLE
#define myFrmZoom_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX

class myFrmZoom : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
		mpWindow *m_plot;
		
		wxPanel *WxPanel1;
		wxFlexGridSizer *WxFlexGridSizer1;
		
		enum
		{
			ID_WXPANEL1 = 1002,
			ID_M_PLOT = 2000,
		};
		
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		
	public:
		myFrmZoom(const std::vector<double>&, const std::vector<double> &, 
            const std::vector<double> &, const std::vector<double> &, wxColour, 
            wxWindow *parent, wxWindowID id = 1, 
            const wxString &title = wxT("Nonlinear equation system solver"), 
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
            long style = myFrmZoom_STYLE);
		virtual ~myFrmZoom();
};

#endif
