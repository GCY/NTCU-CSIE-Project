#include "App.h"

IMPLEMENT_APP(App)
DECLARE_APP(App)

bool App::OnInit()
{
   wxFrame *frame = new Frame(wxT("BooksManager"),wxSize(1024,600));
   frame->Show(true);
   return true;
}

BEGIN_EVENT_TABLE(BooksListCtrl,wxListCtrl)
   EVT_LIST_KEY_DOWN(ID_LIST_CTRL,BooksListCtrl::OnKeyDown)
   EVT_LIST_ITEM_RIGHT_CLICK(ID_LIST_CTRL,BooksListCtrl::OnEdit)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(Frame,wxFrame)
   EVT_MENU(wxID_OPEN,Frame::OnOpen)
   EVT_MENU(wxID_SAVEAS,Frame::OnSave)
   EVT_MENU(wxID_ADD,Frame::OnAdd)
   EVT_MENU(wxID_DELETE,Frame::OnDelete)
   EVT_MENU(ID_SORT,Frame::OnSort)
   EVT_MENU(wxID_EXIT,Frame::OnExit)
   EVT_MENU(wxID_CLEAR,Frame::OnClear)
   EVT_MENU(ID_LIST_BG_COLOUR,Frame::OnSetBGColour)
   EVT_MENU(ID_LIST_TEXT_COLOUR,Frame::OnSetTextColour)
   EVT_MENU(ID_SHOW_TIP,Frame::OnShowTip)
   EVT_MENU(ID_RECEIVE,Frame::OnRecive)

   EVT_SOCKET(ID_SYNC,Frame::OnSync)

   EVT_MENU(wxID_ABOUT,Frame::OnAbout)
END_EVENT_TABLE()

Frame::Frame(const wxString &title,const wxSize &size):wxFrame(NULL,wxID_ANY,title,wxDefaultPosition,size,wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxSYSTEM_MENU)
{
   SetIcon(book_xpm);

   /**********Menu**********/

   wxMenu *file = new wxMenu;
   wxMenu *edit = new wxMenu;
   wxMenu *help = new wxMenu;
   wxMenu *view = new wxMenu;
   wxMenu *internet = new wxMenu;

   wxMenu *del = new wxMenu;
   del->Append(wxID_CLEAR,wxT("C&lear\tCtrl-c"),wxT("Clear"));
   del->Append(wxID_DELETE,wxT("D&elete\tCtrl-t"),wxT("Delete"));

   wxMenu *color = new wxMenu;
   color->Append(ID_LIST_TEXT_COLOUR,wxT("T&ext\tAlt-t"),wxT("Text"));
   color->Append(ID_LIST_BG_COLOUR,wxT("B&ackground\tAlt-b"),wxT("Background"));

   file->Append(wxID_OPEN,wxT("O&pen\tAlt-o"),wxT("Open"));
   file->Append(wxID_SAVEAS,wxT("S&ave As\tAlt-s"),wxT("Save As"));
   file->Append(wxID_EXIT,wxT("E&xit\tAlt-q"),wxT("Exit"));
   edit->Append(wxID_ADD,wxT("A&dd\tCtrl-a"),wxT("Add"));
   edit->AppendSubMenu(del,wxT("D&elete\tCtrl-d"),wxT("Delete"));
   view->AppendSubMenu(color,wxT("C&olor\tCtrl-v"),wxT("View"));
   view->Append(ID_SORT,wxT("S&ort\tCtrl-s"),wxT("Sort"));
   internet->Append(ID_RECEIVE,wxT("R&ecive\tCtrl-r"),wxT("Recive"));
   help->Append(ID_SHOW_TIP,wxT("S&howTip\tAlt-t"),wxT("ShowTip"));
   help->Append(wxID_ABOUT,wxT("A&bout\tAlt-a"),wxT("About"));

   wxMenuBar *bar = new wxMenuBar;

   bar->Append(file,wxT("File"));
   bar->Append(edit,wxT("Edit"));
   bar->Append(view,wxT("View"));
   bar->Append(internet,wxT("Internet"));
   bar->Append(help,wxT("Help"));
   SetMenuBar(bar);

   /**********Tool**********/

   wxPanel *panel = new wxPanel(this,wxID_ANY);

   wxToolBar *tool = new wxToolBar(panel,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTB_HORIZONTAL | wxNO_BORDER);

   wxBitmap open_bmp(fileopen_xpm);
   wxBitmap saveas_bmp(filesaveas_xpm);
   wxBitmap addbookm_bmp(addbookm_xpm);
   wxBitmap delbookm_bmp(delbookm_xpm);

   tool->AddTool(wxID_OPEN,open_bmp,wxT("Open"));
   tool->AddTool(wxID_SAVEAS,saveas_bmp,wxT("Svae As"));
   tool->AddTool(wxID_ADD,addbookm_bmp,wxT("Add Book"));
   tool->AddTool(wxID_DELETE,delbookm_bmp,wxT("Delete Book"));
   tool->Realize();
   SetToolBar(tool);

   books_list_ctrl = new BooksListCtrl(panel,wxSize(1024,527),books_manager);
   
   CreateStatusBar(2);
   SetStatusText(wxDateTime::Now().Format());
}

void Frame::OnOpen(wxCommandEvent &event)
{
   wxFileDialog dlg(this,wxT("選擇檔案"),wxT("."),wxT("*.books"),wxT("BOOKS files (*.books)|*.books"),wxOPEN);

   if(dlg.ShowModal() == wxID_OK){
      books_manager.OpenFile(dlg.GetPath().mb_str());
      books_list_ctrl->Show(books_manager.GetBooks());
   }
}

void Frame::OnSave(wxCommandEvent &event)
{
   wxFileDialog dlg(this,wxT("選擇檔案"),wxT("."),wxT("*.books"),wxT("BOOKS files (*.books)|*.books"),wxSAVE | wxOVERWRITE_PROMPT);

   if(dlg.ShowModal() == wxID_OK){
      books_manager.SaveFile(dlg.GetPath().mb_str());
   }
}

void Frame::OnAdd(wxCommandEvent &event)
{
   AddDialog dlg(this,wxID_ANY,wxT("Add Book"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);

   while(dlg.ShowModal() == wxID_OK){
      Book book = dlg.GetBook();
      if(!book.name.empty() && !book.author.empty() &&
	    !book.publisher.empty() && !book.isbn.empty()){
	 if(books_manager.IsISBNValid(book.isbn)){
	    books_manager.Add(book);
	    books_list_ctrl->AddItem(book,(int)books_manager.GetBooks().size() - 1);
	 }
	 else{
	    wxMessageBox(wxT("ISBN有誤！"),wxT("提示"),wxOK,this);
	 }
      }
      else{
      }
   }
}

void Frame::OnClear(wxCommandEvent &event)
{
   if(wxMessageBox(wxT("確定要清除所有項目？"),wxT("確認"),wxYES_NO,this) == wxYES){
      books_list_ctrl->Clear();
      books_manager.Clear();
   }
}

void Frame::OnDelete(wxCommandEvent &event)
{
   DeleteDialog dlg(this,wxID_ANY,wxT("Delete Book"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);
   if(dlg.ShowModal() == wxID_OK){
      if(wxMessageBox(wxT("確定要清除所有符合項目？"),wxT("確認"),wxYES_NO,this) == wxYES){
	 const int selection = dlg.GetSelection();
	 std::string keyword;
	 keyword = dlg.GetText().mb_str();

	 int count = books_manager.SelectionDelete(selection,keyword);
	 if(count){
	    books_list_ctrl->Show(books_manager.GetBooks());
	    wxString msg;
	    msg.Printf(wxT("一共刪除了%d項"),count);
	    wxMessageBox(msg,wxT("刪除成功！"),wxOK,this);
	 }
	 else{
	    wxMessageBox(wxT("沒有符合項目！"),wxT("確認"),wxOK,this);
	 }
      }
   }
}

void Frame::OnSort(wxCommandEvent &event)
{
   SortDialog dlg(this,wxID_ANY,wxT("Sort Book"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);
   
   if(dlg.ShowModal() == wxID_OK){
      if(wxMessageBox(wxT("確定要排序所有項目？"),wxT("確認"),wxYES_NO,this) == wxYES){
	 books_manager.Sort(dlg.GetSelection(),dlg.GetSortRule());
	 books_list_ctrl->Show(books_manager.GetBooks());
      }
   }
}

void Frame::OnSetBGColour(wxCommandEvent &event)
{
   books_list_ctrl->SetBackgroundColour(wxGetColourFromUser(this));
   books_list_ctrl->Refresh();
}

void Frame::OnSetTextColour(wxCommandEvent &event)
{
   books_list_ctrl->SetTextColour(wxGetColourFromUser(this));
   books_list_ctrl->Refresh();
}

void Frame::OnShowTip(wxCommandEvent &event)
{
   static size_t index = (size_t) - 1;
   if(index == (size_t) - 1){
      srand(time(NULL));
      index = rand() % 50;
   }

   wxTipProvider *tip = wxCreateFileTipProvider(wxT("tip.txt"),0);
   wxShowTip(this,tip,true);
   delete tip;
}

void Frame::OnExit(wxCommandEvent &event)
{
   Close();
}

void Frame::OnAbout(wxCommandEvent &event)
{
   wxMessageBox(wxT("軟體工程 - 第二組製作"),wxT("BooksManager"),wxOK,this);
}

void Frame::OnRecive(wxCommandEvent &event)
{
   wxString hostname = wxGetTextFromUser(wxT("Address"),wxT("Connect"),wxT("localhost"));

   wxIPV4address addr;
   addr.Hostname(hostname);
   addr.Service(3000);

   wxSocketClient *client = new wxSocketClient();
   client->SetEventHandler(*this,ID_SYNC);
   client->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG);
   client->Notify(true);
   client->Connect(addr,false);
}

void Frame::OnSync(wxSocketEvent &event)
{
   wxSocketBase *socket = event.GetSocket();

   switch(event.GetSocketEvent())
   {
      case wxSOCKET_CONNECTION:
	 {
	    new Receive(wxT("sync.books"),socket);
	    break;
	 }
      case wxSOCKET_LOST:
	 {
	    socket->Destroy();
	    break;
	 }
   }
}

BooksListCtrl::BooksListCtrl(wxWindow *parent,const wxSize &size,BooksManager &books_manager):wxListCtrl(parent,ID_LIST_CTRL,wxDefaultPosition,size,wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_EDIT_LABELS)
{
   this->books_manager = &books_manager;

   wxImageList *image = new wxImageList(16,16);
   image->Add(wxIcon(book_xpm));
   image->Add(wxIcon(book_xpm));

   AssignImageList(image,wxIMAGE_LIST_SMALL);

   wxListItem item;

   item.SetText(wxT("書名"));
   item.SetImage(-1);
   InsertColumn(COLUMN_0,item);
   SetColumnWidth(COLUMN_0,200);

   item.SetText(wxT("作者"));
   InsertColumn(COLUMN_1,item);
   SetColumnWidth(COLUMN_1,100);

   item.SetText(wxT("出版社"));
   InsertColumn(COLUMN_2,item);
   SetColumnWidth(COLUMN_2,100);

   item.SetText(wxT("ISBN"));
   InsertColumn(COLUMN_3,item);
   SetColumnWidth(COLUMN_3,100);

   item.SetText(wxT("出版年"));
   InsertColumn(COLUMN_4,item);
   SetColumnWidth(COLUMN_4,100);

   item.SetText(wxT("數量"));
   InsertColumn(COLUMN_5,item);
   SetColumnWidth(COLUMN_5,100);

   item.SetText(wxT("修改時間"));
   InsertColumn(COLUMN_6,item);
   SetColumnWidth(COLUMN_6,200);
}

void BooksListCtrl::AddItem(const Book &book,const int number)
{
   InsertItem(number,wxString(book.name.c_str(),wxConvUTF8),0);
   SetItemData(number,number);
   SetItem(number,COLUMN_1,wxString(book.author.c_str(),wxConvUTF8));
   SetItem(number,COLUMN_2,wxString(book.publisher.c_str(),wxConvUTF8));
   SetItem(number,COLUMN_3,wxString(book.isbn.c_str(),wxConvUTF8));
   SetItem(number,COLUMN_4,wxString::Format(wxT("%i"),book.year));
   SetItem(number,COLUMN_5,wxString::Format(wxT("%i"),book.quantity));
   SetItem(number,COLUMN_6,wxString((book.date + book.time).c_str(),wxConvUTF8));
}

void BooksListCtrl::DelItem(const long number)
{
   DeleteItem(number);
}

void BooksListCtrl::Show(const std::list<Book> &books)
{
   Clear();

   std::list<Book>::const_iterator begin = books.begin(),
      end = books.end();

   for(int i = 0;begin != end;++begin,++i){
      AddItem(*begin,i);
   }
}

void BooksListCtrl::OnKeyDown(wxListEvent &event)
{
   switch(event.GetKeyCode())
   {
      case 127:
	 {
	    if(wxMessageBox(wxT("是否要刪除此項？"),wxT("確認"),wxYES_NO,this) == wxYES){
	       const long index = event.GetIndex();
	       DelItem(index);
	       books_manager->Delete(index);
	       break;
	    }
	 }
   }
}

void BooksListCtrl::OnEdit(wxListEvent &event)
{
   const long index = event.GetIndex();
   Book book = books_manager->GetBook(index);
   book.date = wxDateTime::Now().FormatDate().mb_str();
   book.time = wxDateTime::Now().FormatTime().mb_str();
   EditDialog dlg(this,wxID_ANY,wxT("Edit Book"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);
   dlg.SetBook(book);
   if(dlg.ShowModal() == wxID_OK){
      if(wxMessageBox(wxT("確定要修改？"),wxT("確認"),wxYES_NO,this) == wxYES){
	 book = dlg.GetBook();
	 if(books_manager->IsISBNValid(book.isbn)){
	    books_manager->Edit(index,book);
	    DelItem(index);
	    AddItem(book,index);
	 }
	 else{
	    wxMessageBox(wxT("ISBN有誤！"),wxT("提示"),wxOK,this);
	 }
      }
   }
}
