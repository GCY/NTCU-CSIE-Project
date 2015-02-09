#ifndef __ADD_DIALOG__
#define __ADD_DIALOG__
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/valtext.h>
#include <wx/valgen.h>

#include "define.h"

enum{
   ID_ADD_RESET = 300,
   ID_ADD_BOOK_NAME,
   ID_ADD_AUTHOR,
   ID_ADD_PUBLISHER,
   ID_ADD_ISBN,
   ID_ADD_YEAR,
   ID_ADD_QUANTITY
};

class AddDialog:public wxDialog
{
   public:
      AddDialog();

      AddDialog(wxWindow *parent,
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

      Book &GetBook(){return book;}
   private:
      Book book;

      DECLARE_CLASS(AddDialog)
      DECLARE_EVENT_TABLE()
};

#endif
