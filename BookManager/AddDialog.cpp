#include "AddDialog.h"

IMPLEMENT_CLASS(AddDialog,wxDialog)

BEGIN_EVENT_TABLE(AddDialog,wxDialog)
   EVT_BUTTON(ID_ADD_RESET,AddDialog::OnReset)
END_EVENT_TABLE()

AddDialog::AddDialog()
{
   Init();
}

AddDialog::AddDialog(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
   Init();
   Create(parent,id,caption,pos,size,style);
}

inline void AddDialog::Init()
{
   book.name = "";
   book.author = "";
   book.publisher = "";
   book.isbn = "";
   book.year = 0;
   book.quantity = 0;
}

bool AddDialog::Create(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
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

void AddDialog::CreateControls()
{

   /**********TopBox**********/

   wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
   this->SetSizer(top);

   /**********TextCtrlBox**********/

   wxBoxSizer *TextCtrlBox = new wxBoxSizer(wxVERTICAL);
   top->Add(TextCtrlBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   wxStaticText *explain = new wxStaticText(this,wxID_STATIC,wxT("Add Book"),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(5,5,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   wxStaticText *name_lable = new wxStaticText(this,wxID_STATIC,wxT("&Name："),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(name_lable,0,wxALIGN_LEFT | wxALL,5);

   wxTextCtrl *name_ctrl = new wxTextCtrl(this,ID_ADD_BOOK_NAME,wxT("Book Name"),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(name_ctrl,0,wxGROW | wxALL,5);

   wxStaticText *author_lable = new wxStaticText(this,wxID_STATIC,wxT("&Author："),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(author_lable,0,wxALIGN_LEFT | wxALL,5);

   wxTextCtrl *author_ctrl = new wxTextCtrl(this,ID_ADD_AUTHOR,wxT("Author"),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(author_ctrl,0,wxGROW | wxALL,5);

   wxStaticText *publisher_lable = new wxStaticText(this,wxID_STATIC,wxT("&Publisher："),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(publisher_lable,0,wxALIGN_LEFT | wxALL,5);

   wxTextCtrl *publisher_ctrl = new wxTextCtrl(this,ID_ADD_PUBLISHER,wxT("Publisher"),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(publisher_ctrl,0,wxGROW | wxALL,5);

   wxStaticText *isbn_lable = new wxStaticText(this,wxID_STATIC,wxT("&ISBN："),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(isbn_lable,0,wxALIGN_LEFT | wxALL,5);

   wxTextCtrl *isbn_ctrl = new wxTextCtrl(this,ID_ADD_ISBN,wxT("ISBN"),wxDefaultPosition,wxDefaultSize,0);
   TextCtrlBox->Add(isbn_ctrl,0,wxGROW | wxALL,5);

   /**********SpinCtrlBox**********/

   wxBoxSizer *SpinCtrlBox = new wxBoxSizer(wxHORIZONTAL);
   TextCtrlBox->Add(SpinCtrlBox,0,wxGROW | wxALL,5);

   wxStaticText *year_lable = new wxStaticText(this,wxID_STATIC,wxT("&Year："),wxDefaultPosition,wxDefaultSize,0);
   SpinCtrlBox->Add(year_lable,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxSpinCtrl *year_spin = new wxSpinCtrl(this,ID_ADD_YEAR,wxEmptyString,wxDefaultPosition,wxSize(60,-1),wxSP_ARROW_KEYS,0,2012,0);
   SpinCtrlBox->Add(year_spin,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxStaticText *quantity_lable = new wxStaticText(this,wxID_STATIC,wxT("&Quantity："),wxDefaultPosition,wxDefaultSize,0);
   SpinCtrlBox->Add(quantity_lable,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxSpinCtrl *quantity_spin = new wxSpinCtrl(this,ID_ADD_QUANTITY,wxEmptyString,wxDefaultPosition,wxSize(60,-1),wxSP_ARROW_KEYS,0,1000,0);
   SpinCtrlBox->Add(quantity_spin,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxStaticLine* line = new wxStaticLine ( this, wxID_STATIC,wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
   TextCtrlBox->Add(line, 0, wxGROW|wxALL, 5);

   /**********ResetOkCancelBox**********/

   wxBoxSizer *ResetOkCancelBox = new wxBoxSizer(wxHORIZONTAL);
   TextCtrlBox->Add(ResetOkCancelBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   wxButton *reset = new wxButton(this,ID_ADD_RESET,wxT("&Reset"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(reset,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxButton *ok = new wxButton(this,wxID_OK,wxT("&Ok"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(ok,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxButton *cancel = new wxButton(this,wxID_CANCEL,wxT("&Cancel"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(cancel,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
}

bool AddDialog::TransferDataToWindow()
{
   wxTextCtrl *name_ctrl = (wxTextCtrl*)FindWindow(ID_ADD_BOOK_NAME);
   wxTextCtrl *author_ctrl = (wxTextCtrl*)FindWindow(ID_ADD_AUTHOR);
   wxTextCtrl *publisher_ctrl = (wxTextCtrl*)FindWindow(ID_ADD_PUBLISHER);
   wxTextCtrl *isbn_ctrl = (wxTextCtrl*)FindWindow(ID_ADD_ISBN);
   wxSpinCtrl *year_spin = (wxSpinCtrl*)FindWindow(ID_ADD_YEAR);
   wxSpinCtrl *quantity_spin = (wxSpinCtrl*)FindWindow(ID_ADD_QUANTITY);

   name_ctrl->SetValue(wxString(book.name.c_str(),wxConvUTF8));
   author_ctrl->SetValue(wxString(book.author.c_str(),wxConvUTF8));
   publisher_ctrl->SetValue(wxString(book.publisher.c_str(),wxConvUTF8));
   isbn_ctrl->SetMaxLength(13);
   isbn_ctrl->SetValue(wxString(book.isbn.c_str(),wxConvUTF8));
   year_spin->SetValue(book.year);
   quantity_spin->SetValue(book.quantity);

   return true;
}

bool AddDialog::TransferDataFromWindow()
{
   wxTextCtrl *name_ctrl = (wxTextCtrl*)FindWindow(ID_ADD_BOOK_NAME);
   wxTextCtrl *author_ctrl = (wxTextCtrl*)FindWindow(ID_ADD_AUTHOR);
   wxTextCtrl *publisher_ctrl = (wxTextCtrl*)FindWindow(ID_ADD_PUBLISHER);
   wxTextCtrl *isbn_ctrl = (wxTextCtrl*)FindWindow(ID_ADD_ISBN);
   wxSpinCtrl *year_spin = (wxSpinCtrl*)FindWindow(ID_ADD_YEAR);
   wxSpinCtrl *quantity_spin = (wxSpinCtrl*)FindWindow(ID_ADD_QUANTITY);

   book.name = name_ctrl->GetValue().mb_str();
   book.author = author_ctrl->GetValue().mb_str();
   book.publisher = publisher_ctrl->GetValue().mb_str();
   book.isbn = isbn_ctrl->GetValue().mb_str();
   book.year = year_spin->GetValue();
   book.quantity = quantity_spin->GetValue();
   book.date = wxDateTime::Now().FormatDate().mb_str();
   book.time = wxDateTime::Now().FormatTime().mb_str();
   return true;
}

void AddDialog::OnReset(wxCommandEvent &event)
{
   Init();
   TransferDataToWindow();
}
