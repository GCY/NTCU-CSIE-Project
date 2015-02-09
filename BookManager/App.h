#ifndef __APP__
#define __APP__
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include <wx/toolbar.h>
#include <wx/colordlg.h>
#include <wx/tipdlg.h>
#include <wx/socket.h>

#include <list>

#include "BooksManager.h"
#include "AddDialog.h"
#include "DeleteDialog.h"
#include "SortDialog.h"
#include "EditDialog.h"
#include "Receive.h"

#include "./xpm/book.xpm"
#include "./xpm/smile.xpm"
#include "./xpm/fileopen.xpm"
#include "./xpm/filesaveas.xpm"
#include "./xpm/addbookm.xpm"
#include "./xpm/delbookm.xpm"

enum{
   ID_LIST_CTRL = 100,
   ID_LIST_BG_COLOUR,
   ID_LIST_TEXT_COLOUR,
   ID_SHOW_TIP,
   ID_SORT,
   ID_SYNC,
   ID_RECEIVE
};

enum{
   COLUMN_0 = 0,
   COLUMN_1,
   COLUMN_2,
   COLUMN_3,
   COLUMN_4,
   COLUMN_5,
   COLUMN_6
};

class App:public wxApp
{
   public:
      bool OnInit();
};

class BooksListCtrl:public wxListCtrl
{
   public:
      BooksListCtrl(wxWindow *,const wxSize &,BooksManager &);
      
      void Show(const std::list<Book> &);
      void AddItem(const Book&,const int);
      void DelItem(const long);
      void Clear(){if(GetItemCount()){DeleteAllItems();}}

      void OnKeyDown(wxListEvent &);
      void OnEdit(wxListEvent &);
   private:
      BooksManager *books_manager;
      DECLARE_EVENT_TABLE()
};

class Frame:public wxFrame
{
   public:
      Frame(const wxString&,const wxSize&);

      void OnOpen(wxCommandEvent&);
      void OnSave(wxCommandEvent&);
      void OnAdd(wxCommandEvent&);
      void OnDelete(wxCommandEvent&);
      void OnSort(wxCommandEvent&);
      void OnExit(wxCommandEvent&);
      void OnClear(wxCommandEvent&);
      void OnSetBGColour(wxCommandEvent&);
      void OnSetTextColour(wxCommandEvent&);
      void OnShowTip(wxCommandEvent&);
      void OnAbout(wxCommandEvent&);
      void OnRecive(wxCommandEvent&);
      void OnSync(wxSocketEvent&);
   private:
      BooksListCtrl *books_list_ctrl;
      BooksManager books_manager;
      DECLARE_EVENT_TABLE()
};

#endif
