#include <vector>
#include "myFrmZoom.h"

BEGIN_EVENT_TABLE(myFrmZoom,wxFrame)
	EVT_CLOSE(myFrmZoom::OnClose)
END_EVENT_TABLE()

myFrmZoom::myFrmZoom(const std::vector<double>& vectorx, const std::vector<double> &vectory, 
    const std::vector<double> & convX, const std::vector<double> & convY, wxColour col, 
    wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, 
    const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
	
	mpFXYVector* vectorLayer = new mpFXYVector(_(""));
	vectorLayer->SetData(vectorx, vectory);
	vectorLayer->SetContinuity(true);
	wxPen vectorpen(col, 2, wxSOLID);
	vectorLayer->SetPen(vectorpen);
	vectorLayer->SetDrawOutsideMargins(false);

    mpFXYVector* vl=new mpFXYVector(_(""));
    vl->SetData(convX, convY);
	vl->SetContinuity(true);
	wxPen vo(*wxGREEN, 2, wxSOLID);
	vl->SetPen(vo);
	vl->SetDrawOutsideMargins(false);

	wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_plot = new mpWindow( WxPanel1, ID_M_PLOT, wxPoint(0,0), wxSize(800,600), wxSUNKEN_BORDER );
    mpScaleX* xaxis = new mpScaleX(wxT(""), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT(""), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);
    m_plot->SetMargins(20, 30, 30, 40);//Top right bottom left
    m_plot->AddLayer( xaxis );
    m_plot->AddLayer( yaxis );
	m_plot->AddLayer( vectorLayer );
	m_plot->AddLayer(vl);

    m_plot->EnableDoubleBuffer(true);
    m_plot->SetMPScrollbars(false);
    m_plot->Fit(0,vectorx.size()*1.1,0,vectory[0]*1.1);//xmin xmax ymin ymax
}

myFrmZoom::~myFrmZoom()
{
}

void myFrmZoom::CreateGUIControls()
{
	WxFlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	this->SetSizer(WxFlexGridSizer1);
	this->SetAutoLayout(true);

	WxPanel1 = new wxPanel(this, ID_WXPANEL1, wxPoint(5, 5), wxSize(800, 600));
	WxFlexGridSizer1->Add(WxPanel1, 0, wxALIGN_CENTER | wxALL, 5);

	SetTitle(_("Nonlinear equation system solver"));
	SetIcon(wxNullIcon);
	
	Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
}

void myFrmZoom::OnClose(wxCloseEvent& event)
{
	Destroy();
}
