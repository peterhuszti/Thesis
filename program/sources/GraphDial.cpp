#include "GraphDial.h"

BEGIN_EVENT_TABLE(GraphDial,wxDialog)
	EVT_CLOSE(GraphDial::OnClose)
	EVT_BUTTON(ID_CANCEL,GraphDial::cancelClick)
	EVT_BUTTON(ID_RIGHT,GraphDial::rightClick)
	EVT_BUTTON(ID_LEFT,GraphDial::leftClick)
END_EVENT_TABLE()

GraphDial::GraphDial(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

GraphDial::~GraphDial()
{
} 

void GraphDial::CreateGUIControls()
{
	cancel = new wxButton(this, ID_CANCEL, _("Cancel"), wxPoint(187, 41), wxSize(75, 25), 0, wxDefaultValidator, _("cancel"));

	right = new wxButton(this, ID_RIGHT, _("h function"), wxPoint(103, 41), wxSize(75, 25), 0, wxDefaultValidator, _("right"));

	left = new wxButton(this, ID_LEFT, _("g function"), wxPoint(14, 41), wxSize(75, 25), 0, wxDefaultValidator, _("left"));

	title = new wxStaticText(this, ID_TITLE, _("Which graph do you want to save?"), wxPoint(43, 8), wxDefaultSize, 0, _("title"));

	SetTitle(_("Select a graph"));
	SetIcon(wxNullIcon);
	SetSize(8,8,291,108);
	Center();
}

void GraphDial::OnClose(wxCloseEvent& event)
{
	Destroy();
}

void GraphDial::leftClick(wxCommandEvent& event)
{
	which=1;
	EndModal(1);
}

void GraphDial::rightClick(wxCommandEvent& event)
{
	which=2;
	EndModal(2);
}

void GraphDial::cancelClick(wxCommandEvent& event)
{
	which=-1;
	EndModal(-1);
}

int GraphDial::getWhich()
{
    return which;
}
