#include "SortDialog.h"

IMPLEMENT_CLASS(SortDialog,wxDialog)

SortDialog::SortDialog()
{
   Init();
}

SortDialog::SortDialog(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
   Create(parent,id,caption,pos,size,style);
   Init();
}

inline void SortDialog::Init()
{
}

bool SortDialog::Create(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
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

void SortDialog::CreateControls()
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
   item.Add(wxT("出版年"));
   item.Add(wxT("數量"));
   item.Add(wxT("修改時間"));
   item_radio_box = new wxRadioBox(this,wxID_ANY,wxT("Item"),wxDefaultPosition,wxDefaultSize,item,10,wxRA_SPECIFY_COLS);
   RadioBoxBox->Add(item_radio_box,0,wxALIGN_LEFT | wxALL,5);

   /**********ChoiceBox**********/

   wxBoxSizer *ChoiceBox = new wxBoxSizer(wxVERTICAL);
   top->Add(ChoiceBox,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);

   wxArrayString rule;
   rule.Add(wxT("大 → 小"));
   rule.Add(wxT("小 → 大"));

   rule_choice = new wxChoice(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,rule);
   ChoiceBox->Add(rule_choice,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);

   /**********ResetOkCancelBox**********/

   wxBoxSizer *ResetOkCancelBox = new wxBoxSizer(wxHORIZONTAL);
   top->Add(ResetOkCancelBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   wxButton *ok = new wxButton(this,wxID_OK,wxT("&Ok"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(ok,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxButton *cancel = new wxButton(this,wxID_CANCEL,wxT("&Cancel"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(cancel,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
}

bool SortDialog::TransferDataToWindow()
{
   return true;
}

bool SortDialog::TransferDataFromWindow()
{
   return true;
}
