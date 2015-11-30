#include "nonlinsolverApp.h"
#include "nonlinsolverFrame.h"

IMPLEMENT_APP(nonlinsolverApp)

bool nonlinsolverApp::OnInit()
{
    nonlinsolverFrame* frame = new nonlinsolverFrame(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int nonlinsolverApp::OnExit()
{
	return 0;
}
