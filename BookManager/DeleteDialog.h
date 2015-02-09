#ifndef __DELETE_DIALOG__
#define __DELETE_DIALOG__
#include <wx/wx.h>

#include "define.h"

enum{
   ID_DELETE_RESET = 500
};

class DeleteDialog:public wxDialog
{
   public:
      DeleteDialog();

      DeleteDialog(wxWindow *parent,
	    wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Delete Book"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

      bool Create(wxWindow *parent,
	    wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Delete Book"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

      void Init();
      void CreateControls();

      void OnReset(wxCommandEvent &event);

      bool TransferDataToWindow();
      bool TransferDataFromWindow();

      const int GetSelection(){return item_radio_box->GetSelection();}
      const wxString GetText(){return item_text_ctrl->GetValue();}
   private:
      wxRadioBox *item_radio_box;
      wxTextCtrl *item_text_ctrl;

      DECLARE_CLASS(DeleteDialog)
      DECLARE_EVENT_TABLE()
};

#endif
