#include "BooksManager.h"

void BooksManager::OpenFile(const char *name)
{
   Clear();

   std::fstream file(name,std::ios::in);
   Book book;

   while(file >> book.name >> book.author >> book.publisher
	 >> book.isbn >> book.year >> book.quantity >> book.date >> book.time){
      books.push_back(book);
   }

   file.close();
}

void BooksManager::SaveFile(const char *name)
{
   std::fstream file(name,std::ios::out);
   std::list<Book>::iterator begin = books.begin(),end = books.end();

   for(;begin != end;++begin){
      file << begin->name << "\t" << begin->author << "\t" << begin->publisher
	 << "\t" << begin->isbn << "\t" << begin->year << "\t" << begin->quantity << "\t" << begin->date << "\t" << begin->time << "\n";
   }

   file.close();
}

void BooksManager::Add(Book &book)
{
   books.push_back(book);
}

void BooksManager::Delete(int number)
{
   if(number < books.size() && !books.empty()){
      std::list<Book>::iterator it = books.begin();
      for(int i = 0;i < number;i++){
	 ++it;
      }
      books.erase(it);
   }
}

void BooksManager::Sort(int selection,int rule)
{
   if(rule == 0){
      switch(selection){
	 case 0:	books.sort(GName());break;
	 case 1:	books.sort(GAuthor());break;
	 case 2:	books.sort(GPublisher());break;
	 case 3:	books.sort(GISBN());break;
	 case 4:	books.sort(GYear());break;
	 case 5:	books.sort(GQuantity());break;
	 case 6:	books.sort(GDate());break;
      }
   }
   else if(rule == 1){
      switch(selection){
	 case 0:	books.sort(LName());break;
	 case 1:	books.sort(LAuthor());break;
	 case 2:	books.sort(LPublisher());break;
	 case 3:	books.sort(LISBN());break;
	 case 4:	books.sort(LYear());break;
	 case 5:	books.sort(LQuantity());break;
	 case 6:	books.sort(LDate());break;
      }
   }
}

int BooksManager::SelectionDelete(int selection,std::string keyword)
{
   std::list<Book>::iterator it = books.begin(),end = books.end();
   int count = books.size();

   books.erase(std::remove_if(it,end,DeleteCompare(selection,keyword)),end);

   return count - books.size();
}

void BooksManager::Edit(int number,Book &book)
{
   std::list<Book>::iterator it = books.begin();
   for(int i = 0;i < number;i++){
      ++it;
   }

   *it = book;
}

const Book& BooksManager::GetBook(const int number)
{
   std::list<Book>::iterator it = books.begin();
   for(int i = 0;i < number;i++){
      ++it;
   }

   return *it;
}

/**
 * 檢查 ISBN-10 或 ISBN-13 碼是否有效。
 * @return      true 表示有效
 * @param isbn  ISBN 字串，無連字號'-'。
 */

bool BooksManager::IsISBNValid(std::string isbn)
{
    if (isbn.length() < 10) {
        return false;
    } else if (isbn.length() < 13) {
        // ISBN-10
        int i, a = 0, b = 0;
        for (i = 0; i < 10; i++) {
            if (ISBNDigit2Int(isbn.at(i)) == -1) {
                return false;
            }
            a += ISBNDigit2Int(isbn.at(i));
            b += a;
        }
        return (b % 11 == 0);
    } else {
        // ISBN-13
        int i;
        int odd_digit_sum = 0, even_digit_sum = 0;
        for (i = 0; i<=12; i += 2) {
            odd_digit_sum += ISBNDigit2Int(isbn.at(i));
            if (ISBNDigit2Int(isbn.at(i)) == -1) {
                return false;
            }
        }
        for (i = 1; i<=11; i += 2) {
            even_digit_sum += ISBNDigit2Int(isbn.at(i));
            if (ISBNDigit2Int(isbn.at(i)) == -1) {
                return false;
            }
        }
        return ((odd_digit_sum+3*even_digit_sum)%10 == 0);
    }
}

/**
 * 把 ASCII 模式的 ISBN 位數轉換為 int 型態
 * @return       int 型態的位數，可爲 0..10; -1 表示 illegal.
 * @param digit  ISBN digit，可爲 '0'-'9', 'X', 'x'。
 */
int BooksManager::ISBNDigit2Int(const char digit)
{
    if (digit == 'x' || digit == 'X'){
        return 10;
    } else if (digit >= '0' || digit <= '9') {
        return (digit-'0');
    } else {
        // illegal
        return -1;
    }

}
