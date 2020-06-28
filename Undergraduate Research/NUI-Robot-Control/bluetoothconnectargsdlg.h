#ifndef __BLUETOOTH_CONNECT_ARGS_DIALOG__
#define __BLUETOOTH_CONNECT_ARGS_DIALOG__

#include <map>

#include <wx/wx.h>

#include <bthsdpdef.h>
#include <BluetoothAPIs.h>

class BluetoothConnectArgsDialog:public wxDialog
{
   public:
      BluetoothConnectArgsDialog();

      BluetoothConnectArgsDialog(wxWindow *parent,
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
	  wxString GetDeviceAddr(wxString device_name){return bt_devices[device_name];}

   private:
      wxChoice *device_path;

	  std::map<wxString,wxString> bt_devices;

      DECLARE_CLASS(BluetoothConnectArgsDialog)
};

#endif
