#include "wxarduino.h"

DECLARE_APP(App)
IMPLEMENT_APP(App)

bool App::OnInit()
{
   Frame *frame = new Frame(wxT("AI - Genetic Algorithm & PID Debugger"));

   frame->Show(true);

   return true;
}

BEGIN_EVENT_TABLE(Frame,wxFrame)
   EVT_MENU(wxID_EXIT,Frame::OnExit)
   EVT_BUTTON(ID_RUN,Frame::OnRun)
   EVT_MENU(ID_CONNECT_DEVICE,Frame::OnConnectDevice)
   EVT_MENU(ID_RELEASE_DEVICE,Frame::OnReleaseDevice)
   EVT_MENU(ID_SETTING,Frame::OnSetParameter)
   EVT_MENU(ID_ABOUT,Frame::OnAbout)
END_EVENT_TABLE()

Frame::Frame(const wxString &title):wxFrame(NULL,wxID_ANY,title,wxDefaultPosition,wxDefaultSize,wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxSYSTEM_MENU),serial()
{
	parameter[1] = 100 / 255;
	parameter[2] = 100 % 255;
	parameter[3] = 50;
	parameter[4] = 50;
	parameter[5] = 10;
	parameter[6] = 700 / 255;
	parameter[7] = 700 % 255;
	current_progress_index = 0;
	thread = NULL;
	progress = NULL;

	CreateUI();
}

void Frame::CreateUI()
{
   wxMenu *file = new wxMenu;
   file->Append(wxID_EXIT,wxT("E&xit\tAlt-e"),wxT("Exit"));
   wxMenu *tools = new wxMenu;
   tools->Append(ID_SETTING	,wxT("S&et GA Parameter\tAlt-s"),wxT("Set Parameter"));
   tools->AppendSeparator();
   tools->Append(ID_CONNECT_DEVICE,wxT("C&onnect Device\tAlt-c"),wxT("Connect Device"));
   tools->Append(ID_RELEASE_DEVICE,wxT("R&elease Device\tAlt-r"),wxT("Release Device"));
   wxMenu *help = new wxMenu;
   help->Append(ID_ABOUT,wxT("A&bout\tAlt-a"),wxT("About"));

   wxMenuBar *bar = new wxMenuBar;
   bar->Append(file,wxT("File"));
   bar->Append(tools,wxT("Tools"));
   bar->Append(help,wxT("Help"));
   SetMenuBar(bar);

   wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
   this->SetSizer(top);

   wxBoxSizer *text_box = new wxBoxSizer(wxHORIZONTAL);
   top->Add(text_box,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);
   text = new wxTextCtrl(this,ID_TEXT,wxEmptyString,wxDefaultPosition,wxSize(800,800),wxTE_MULTILINE | wxTE_READONLY);
   text_box->Add(text,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);

   wxBoxSizer *button_box = new wxBoxSizer(wxHORIZONTAL);
   top->Add(button_box,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);
   run = new wxButton(this,ID_RUN,wxT("Run"));
   button_box->Add(run,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);
   run->Enable(false);

   CreateStatusBar(2);
   SetStatusText(wxDateTime::Now().Format());

   top->Fit(this);
   top->SetSizeHints(this);
}

void Frame::OnRun(wxCommandEvent &event)
{
	run->Enable(false);
	if(serial.IsOpen()){
		parameter[0] = 'r';
		CheckSum();
		serial.Write(parameter,MAX);
	}
	progress = new wxProgressDialog(wxT("Wait..."),wxT("GA tuning PID Parameter..."),(parameter[1] * 255) + parameter[2],this,
		wxPD_AUTO_HIDE | wxPD_CAN_ABORT | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);
	text->Clear();
}

void Frame::OnSetParameter(wxCommandEvent &event)
{
	SetDialog dlg(this,wxID_ANY,wxT("Setting Parameter"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);

    if(dlg.ShowModal() == wxID_OK){
		parameter[1] = dlg.GetIterate() / 255;
		parameter[2] = dlg.GetIterate() % 255;
		parameter[3] = dlg.GetPopulation();
		parameter[4] = dlg.GetCrossover();
		parameter[5] = dlg.GetMutation();
		parameter[6] = dlg.GetSetpoint() / 255;
		parameter[7] = dlg.GetSetpoint() % 255;
		current_progress_index = 0;
		run->Enable(true);
    }
}

void Frame::OnConnectDevice(wxCommandEvent &event)
{
	ConnectArgsDialog dlg(this,wxID_ANY,wxT("Connect"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);

    if(dlg.ShowModal() == wxID_OK){
		if(serial.IsOpen()){
			serial.Close();
		}
		serial.Open(dlg.GetDevicePath(),dlg.GetBaudRate());
		if(thread){
			thread->Delete();
			thread = NULL;
		}
		thread = new Thread(this);
		thread->Create();
		thread->Run();
		
		run->Enable(true);
    }
}

void Frame::OnReleaseDevice(wxCommandEvent &event)
{
	if(thread){
		thread->Delete();
		thread = NULL;
	}
	if(serial.IsOpen()){
		serial.Close();
	}
	run->Enable(false);
}

void Frame::OnAbout(wxCommandEvent &event)
{
	wxMessageBox(wxT("¦¶¯E¦w\tACS099113\n­g©ú¬u\tACS099111\n³¢©Ó¿Î\tACS099183\n¬x«G\tACS099103\n§d¬F½å\tACS099108\nªL­§½n\tACS099140"),wxT("²Ä¤»²Õ"));
}

void Frame::OnExit(wxCommandEvent &event)
{
	Close();
}

Frame::~Frame()
{
	if(thread){
		thread->Delete();
		thread = NULL;
	}
	if(serial.IsOpen()){
		serial.Close();
	}
}

void* Thread::Entry()
{
	bool flag = false;
   while(!TestDestroy()){
	   frame->SetStatusText(wxDateTime::Now().Format());
	   if(frame->serial.IsOpen()){
		   if(frame->serial.ReadByte()){
			   if(frame->serial.GetChar() == '\n'){
				   flag = true;
			   }
			   wxString str;
			   str.Printf(wxT("%c"),frame->serial.GetChar());
				   frame->text->AppendText(str);
		   }
	   }
	   if(flag){
		   flag = false;
		   //frame->text->AppendText(wxT("======================\n"));
		   if(frame->progress){
			   ++frame->current_progress_index;
			   if(!frame->progress->Update(frame->current_progress_index)){
				   delete frame->progress;
				   frame->progress = NULL;
				   frame->run->Enable(true);
				   frame->current_progress_index = 0;
				   for(int i = 0;i < frame->MAX-1;++i){
					   frame->parameter[i] = i;
				   }
				   frame->CheckSum();
				   if(frame->serial.IsOpen()){
					   frame->serial.Write(frame->parameter,frame->MAX);
				   }
			   }
			   if(frame->current_progress_index > ((frame->parameter[1] * 255) + frame->parameter[2])){
				   delete frame->progress;
				   frame->progress = NULL;
				   frame->run->Enable(true);
				   frame->current_progress_index = 0;
				   for(int i = 0;i < frame->MAX-1;++i){
					   frame->parameter[i] = i;
				   }
				   frame->CheckSum();
				   if(frame->serial.IsOpen()){
					   frame->serial.Write(frame->parameter,frame->MAX);
				   }
			   }
		   }
	   }
   }
   return NULL;
}