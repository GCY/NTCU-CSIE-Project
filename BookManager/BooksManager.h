#ifndef __BOOKS_MANAGER__
#define __BOOKS_MANAGER__
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>

#include "define.h"
#include "compare.h"

class BooksManager
{
   public:
      void OpenFile(const char*);
      void SaveFile(const char*);
      void Add(Book&);
      void Delete(int);
      void Sort(int,int);
      int SelectionDelete(int,std::string);
      void Edit(int,Book &);

      void Clear(){if(!books.empty())books.clear();}

      bool IsISBNValid(std::string);

      const Book& GetBook(const int);
      const std::list<Book> GetBooks(){return books;}
   private:
      int ISBNDigit2Int(const char);

      std::list<Book> books;
};

#endif
