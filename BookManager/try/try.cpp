#include <wx/wx.h>
#include </usr/include/wx-2.8/wx/plot/plot.h>

class MyApp:public wxApp
{
   public:
   bool OnInit();
};

DECLARE_APP(MyApp)
IMPLEMENT_APP(MyApp)

class MyFrame:public wxFrame
{
   public:
      MyFrame(const wxString&);

      void OnDialogDemo(wxCommandEvent&);
      void OnExit(wxCommandEvent&);
      void OnAbout(wxCommandEvent&);
   private:

      enum{
	 ID_DEMO
      };

      DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
   EVT_MENU(wxID_EXIT,MyFrame::OnExit)
   EVT_MENU(wxID_ABOUT,MyFrame::OnAbout)
   EVT_MENU(ID_DEMO,MyFrame::OnDialogDemo)
END_EVENT_TABLE()

bool MyApp::OnInit()
{
   MyFrame *frame = new MyFrame(wxT("Demo"));

   frame->Show(true);

   return true;
}

MyFrame::MyFrame(const wxString &title):wxFrame(NULL,wxID_ANY,title)
{
   wxMenu *file = new wxMenu;
   wxMenu *help = new wxMenu;

   file->Append(ID_DEMO,wxT("ShowDemo"),wxT("Demo"));
   file->Append(wxID_EXIT,wxT("&Exit\tAlt-X"),wxT("Close Window."));

   help->Append(wxID_ABOUT,wxT("&About"),wxT("About"));

   wxMenuBar *bar = new wxMenuBar;

   bar->Append(file,wxT("File"));
   bar->Append(help,wxT("Help"));

   SetMenuBar(bar);
}

void MyFrame::OnExit(wxCommandEvent &eve)
{
   Close();
}

void MyFrame::OnAbout(wxCommandEvent &eve)
{
   wxMessageBox(wxT("This is a demo."),wxT("About"),wxOK,this);
}

void MyFrame::OnDialogDemo(wxCommandEvent &eve)
{
   wxPlotWindow *plot = new wxPlotWindow(this,wxID_ANY,wxDefaultPosition,wxSize(100,100),wxSUNKEN_BORDER | wxPLOT_DEFAULT);
}
