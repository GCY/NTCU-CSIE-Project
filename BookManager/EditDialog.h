#ifndef __EDIT_DIALOG__
#define __EDIT_DIALOG__
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/valtext.h>
#include <wx/valgen.h>

#include "define.h"

enum{
   ID_EDIT_RESET = 400,
   ID_EDIT_BOOK_NAME,
   ID_EDIT_AUTHOR,
   ID_EDIT_PUBLISHER,
   ID_EDIT_ISBN,
   ID_EDIT_YEAR,
   ID_EDIT_QUANTITY
};

class EditDialog:public wxDialog
{
   public:
      EditDialog();

      EditDialog(wxWindow *parent,
	    wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Add Book"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

      bool Create(wxWindow *parent,
	    wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Add Book"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

      void Init();
      void CreateControls();

      void OnReset(wxCommandEvent &event);

      bool TransferDataToWindow();
      bool TransferDataFromWindow();

      void SetBook(Book &temp){this->temp = temp;this->book = temp;}
      Book &GetBook(){return book;}
   private:
      Book book;
      Book temp;

      DECLARE_CLASS(EditDialog)
      DECLARE_EVENT_TABLE()
};

#endif
