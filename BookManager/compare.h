#ifndef __COMPARE__
#define __COMPARE__

#include "define.h"

class DeleteCompare
{
   public:
      DeleteCompare(int selection,std::string keyword):selection(selection),keyword(keyword){}
      bool  operator()(const Book book) const{
	 switch(selection)
	 {
	    case 0:
	       {
		  return book.name == keyword;
		  break;
	       }
	    case 1:
	       {
		  return book.author == keyword;
		  break;
	       }
	    case 2:
	       {
		  return book.publisher == keyword;
		  break;
	       }
	    case 3:
	       {
		  return book.isbn == keyword;
		  break;
	       }
	 }
      }
   private:
      int selection;
      std::string keyword;
};

class GName
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.name > book2.name;
      }
};

class GAuthor
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.author > book2.author;
      }
};

class GPublisher
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.publisher > book2.publisher;
      }
};

class GISBN
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.isbn > book2.isbn;
      }
};

class GYear
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.year > book2.year;
      }
};

class GQuantity
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.quantity > book2.quantity;
      }
};

class GDate
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.date > book2.date;
      }
};

class LName
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.name < book2.name;
      }
};

class LAuthor
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.author < book2.author;
      }
};

class LPublisher
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.publisher < book2.publisher;
      }
};

class LISBN
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.isbn < book2.isbn;
      }
};

class LYear
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.year < book2.year;
      }
};

class LQuantity
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.quantity < book2.quantity;
      }
};

class LDate
{
   public:
      bool operator()(Book &book1,Book &book2){
	 return book1.date < book2.date;
      }
};

#endif
