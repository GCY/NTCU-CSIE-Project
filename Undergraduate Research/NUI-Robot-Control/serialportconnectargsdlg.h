#ifndef __SERIAL_PORT_CONNECT_ARGS_DIALOG__
#define __SERIAL_PORT_CONNECT_ARGS_DIALOG__

#include <wx/wx.h>
#include <windows.h>

class SerialPortConnectArgsDialog:public wxDialog
{
public:
	SerialPortConnectArgsDialog();

	SerialPortConnectArgsDialog(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxString &caption = wxT("Connect Args"),
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	bool Create(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxString &caption = wxT("Connect Args"),
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	void Init();
	void CreateControls();

	bool TransferDataToWindow();
	bool TransferDataFromWindow();

	wxString GetDevicePath(){return device_path->GetString(device_path->GetCurrentSelection());}
	long GetBaudRate(){return wxAtoi(baud_rate->GetString(baud_rate->GetCurrentSelection()));}

private:
	wxChoice *device_path;
	wxChoice *baud_rate;

	DECLARE_CLASS(SerialPortConnectArgsDialog)
};

#endif
