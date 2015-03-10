#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <sstream>

#ifndef __ASSEMBLER__
#define __ASSEMBLER__

class Assembler
{
   public:
      Assembler(const char*,const char*);
   private:
      bool OpenFile(const char*);
      bool SaveResualt(const char*);
      unsigned int String2Hex(const std::string&);
      std::string Hex2String(const unsigned int&);
      unsigned int String2Int(const std::string&);
      std::string Int2String(const unsigned int);
      bool ComputingLines();
      unsigned int StartLines(std::list<std::list<std::string> >::iterator&);
      void TypeCheck(std::list<std::string>::iterator&,unsigned int&);
      void RemoveNote();
      void InitOpcode();
      bool Compile();

      std::list<std::list<std::string> > code;
      std::vector<int> line_count;
      std::map<std::string,int> symbol;
      std::map<std::string,std::string> opcode;
      std::list<std::string> objcode;
};

#endif
