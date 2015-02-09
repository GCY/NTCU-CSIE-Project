#include "DeleteDialog.h"

IMPLEMENT_CLASS(DeleteDialog,wxDialog)

BEGIN_EVENT_TABLE(DeleteDialog,wxDialog)
   EVT_BUTTON(ID_DELETE_RESET,DeleteDialog::OnReset)
END_EVENT_TABLE()

DeleteDialog::DeleteDialog()
{
   Init();
}

DeleteDialog::DeleteDialog(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
   Create(parent,id,caption,pos,size,style);
   Init();
}

inline void DeleteDialog::Init()
{
   item_text_ctrl->Clear();
}

bool DeleteDialog::Create(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
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

void DeleteDialog::CreateControls()
{
   /**********TopBox**********/

   wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
   this->SetSizer(top);

   /**********RadioBoxBox**********/

   wxBoxSizer *RadioBoxBox = new wxBoxSizer(wxVERTICAL);
   top->Add(RadioBoxBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   wxArrayString item;
   item.Add(wxT("書名"));
   item.Add(wxT("作者"));
   item.Add(wxT("出版社"));
   item.Add(wxT("ISBN"));
   item_radio_box = new wxRadioBox(this,wxID_ANY,wxT("Item"),wxDefaultPosition,wxDefaultSize,item,10,wxRA_SPECIFY_COLS);
   RadioBoxBox->Add(item_radio_box,0,wxALIGN_LEFT | wxALL,5);

   /**********TextCtrlBox**********/

   wxBoxSizer *TextCtrlBox = new wxBoxSizer(wxVERTICAL);
   top->Add(TextCtrlBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   item_text_ctrl = new wxTextCtrl(this,wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(item_text_ctrl,0,wxGROW | wxALL,5);

   /**********ResetOkCancelBox**********/

   wxBoxSizer *ResetOkCancelBox = new wxBoxSizer(wxHORIZONTAL);
   top->Add(ResetOkCancelBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   wxButton *reset = new wxButton(this,ID_DELETE_RESET,wxT("&Reset"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(reset,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxButton *ok = new wxButton(this,wxID_OK,wxT("&Ok"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(ok,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxButton *cancel = new wxButton(this,wxID_CANCEL,wxT("&Cancel"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(cancel,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
}

bool DeleteDialog::TransferDataToWindow()
{
   return true;
}

bool DeleteDialog::TransferDataFromWindow()
{
   return true;
}

void DeleteDialog::OnReset(wxCommandEvent &event)
{
   Init();
   TransferDataToWindow();
}
