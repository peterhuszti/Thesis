#include <wx/log.h>

#ifndef __nonlinsolverApp_h__
#define __nonlinsolverApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class nonlinsolverApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
