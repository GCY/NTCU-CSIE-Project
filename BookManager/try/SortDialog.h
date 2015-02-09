#ifndef __SORT_DIALOG__
#define __SORT_DIALOG__
#include <wx/wx.h>

#include "define.h"

class SortDialog:public wxDialog
{
   public:
      SortDialog();

      SortDialog(wxWindow *parent,
	    wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Sort Book"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

      bool Create(wxWindow *parent,
	    wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Sort Book"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

      void Init();
      void CreateControls();

      bool TransferDataToWindow();
      bool TransferDataFromWindow();

      const int GetSelection(){return item_radio_box->GetSelection();}
      const int GetSortRule(){return rule_choice->GetCurrentSelection();}
   private:
      wxRadioBox *item_radio_box;
      wxChoice *rule_choice;

      DECLARE_CLASS(SortDialog)
};

#endif
