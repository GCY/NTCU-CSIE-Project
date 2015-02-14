#ifndef __WX_ARDUINO__
#define __WX_ARDUINO__

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/progdlg.h>
#include <wx/string.h>
#include <wx/datetime.h>

#include "serialport.h"
#include "connectargsdlg.h"
#include "SetDialog.h"

class Thread;

class App:public wxApp
{
   public:
      bool OnInit();
};

class Frame:public wxFrame
{
public:
	Frame(const wxString&);
	~Frame();

	void CreateUI();

	void OnRun(wxCommandEvent&);

	void OnSetParameter(wxCommandEvent&);
	void OnConnectDevice(wxCommandEvent&);
	void OnReleaseDevice(wxCommandEvent&);
	void OnAbout(wxCommandEvent&);
	void OnExit(wxCommandEvent&);

	void CheckSum(){parameter[MAX-1] = 0;for(int i = 0;i < MAX-1;++i){parameter[MAX-1] ^= parameter[i];}}

private:
	enum{
		ID_RUN = 100,
		ID_SETTING,
		ID_TEXT,
		ID_CONNECT_DEVICE,
		ID_RELEASE_DEVICE,
		ID_ABOUT
	};

	wxMenu *device_path;

	SerialPort serial;

	wxTextCtrl *text;
	friend class Thread;
	Thread *thread;
	wxButton *run;

	wxProgressDialog *progress;

	unsigned int current_progress_index;

	static const unsigned int MAX = 9;
	unsigned char parameter[MAX]; //command iterate-2byte P C M SP-2byte chk

	DECLARE_EVENT_TABLE()
};

class Thread:public wxThread
{
   public:
      Thread(Frame *parent):wxThread(wxTHREAD_DETACHED){frame = parent;}
	  ~Thread(){};

      void* Entry();
   private:
      Frame *frame;
};

#endif
