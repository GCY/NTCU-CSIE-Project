#include "bluetoothconnectargsdlg.h"

IMPLEMENT_CLASS(BluetoothConnectArgsDialog,wxDialog)

BluetoothConnectArgsDialog::BluetoothConnectArgsDialog()
{
   Init();
}

BluetoothConnectArgsDialog::BluetoothConnectArgsDialog(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
   Create(parent,id,caption,pos,size,style);
   Init();
}

inline void BluetoothConnectArgsDialog::Init()
{
}

bool BluetoothConnectArgsDialog::Create(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
   SetExtraStyle(wxWS_EX_BLOCK_EVENTS | wxDIALOG_EX_CONTEXTHELP);

   if(!wxDialog::Create(parent,id,caption,pos,size,style)){
      return false;
   }

   CreateControls();

   GetSizer()->Fit(this);
   GetSizer()->SetSizeHints(this);

   Center();

   return true;
}

void BluetoothConnectArgsDialog::CreateControls()
{
   wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
   this->SetSizer(top);

   wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
   top->Add(box,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);

   wxArrayString paths;

   BLUETOOTH_DEVICE_INFO m_device_info = {sizeof(BLUETOOTH_DEVICE_INFO),0,};
   HBLUETOOTH_DEVICE_FIND m_bt_dev = NULL;
   BLUETOOTH_DEVICE_SEARCH_PARAMS m_search_params = {sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
	   1,0,1,1,1,15,NULL};

   m_bt_dev = BluetoothFindFirstDevice(&m_search_params, &m_device_info);
   
   do{
	   //wxMessageBox(wxString(m_device_info.szName));
	   wxString name(m_device_info.szName);
	   paths.Add(name);
	   wxString addr;
	   addr.Printf(wxT("%02x%02x%02x%02x%02x%02x"),m_device_info.Address.rgBytes[5],m_device_info.Address.rgBytes[4],
		   m_device_info.Address.rgBytes[3], m_device_info.Address.rgBytes[2],
		   m_device_info.Address.rgBytes[1], m_device_info.Address.rgBytes[0]);
	   //wxMessageBox(addr);
	   bt_devices.insert(std::make_pair(name,addr));
	   if(!BluetoothFindNextDevice(m_bt_dev, &m_device_info)){
		   break;
	   }
   }while(BluetoothFindNextDevice(m_bt_dev, &m_device_info));

   device_path = new wxChoice(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,paths);
   box->Add(device_path,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);

   wxBoxSizer *ResetOkCancelBox = new wxBoxSizer(wxHORIZONTAL);
   top->Add(ResetOkCancelBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   wxButton *ok = new wxButton(this,wxID_OK,wxT("&Ok"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(ok,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxButton *cancel = new wxButton(this,wxID_CANCEL,wxT("&Cancel"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(cancel,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
}

bool BluetoothConnectArgsDialog::TransferDataToWindow()
{
   return true;
}

bool BluetoothConnectArgsDialog::TransferDataFromWindow()
{
   return true;
}
