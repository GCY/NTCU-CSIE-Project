#include <iostream>

#include "ISBNCheck.cpp"

int main(int argc,char**argv)
{
   if(isIsbnValid(argv[1])){
      std::cout << "yes" << std::endl;
   }
   else{
      std::cout << "no" << std::endl;
   }
}
