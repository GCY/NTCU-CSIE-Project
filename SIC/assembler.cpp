#include "assembler.h"

void Assembler::InitOpcode()
{
   opcode.insert(std::make_pair("ADD","18"));
   opcode.insert(std::make_pair("AND","40"));
   opcode.insert(std::make_pair("COMP","28"));
   opcode.insert(std::make_pair("DIV","24"));
   opcode.insert(std::make_pair("J","3C"));
   opcode.insert(std::make_pair("JEQ","30"));
   opcode.insert(std::make_pair("JSUB","48"));
   opcode.insert(std::make_pair("JGT","34"));
   opcode.insert(std::make_pair("JLT","38"));
   opcode.insert(std::make_pair("LDA","00"));
   opcode.insert(std::make_pair("LDT","74"));
   opcode.insert(std::make_pair("OR","44"));
   opcode.insert(std::make_pair("RSUB","4C"));
   opcode.insert(std::make_pair("STA","0C"));
   opcode.insert(std::make_pair("STL","14"));
   opcode.insert(std::make_pair("STX","10"));
   opcode.insert(std::make_pair("SUB","1C"));
   opcode.insert(std::make_pair("TD","E0"));
   opcode.insert(std::make_pair("LDX","04"));
   opcode.insert(std::make_pair("LDL","08"));
   opcode.insert(std::make_pair("RD","D8"));
   opcode.insert(std::make_pair("STCH","54"));
   opcode.insert(std::make_pair("LDCH","50"));
   opcode.insert(std::make_pair("TIX","2C"));
   opcode.insert(std::make_pair("WD","DC"));
}

Assembler::Assembler(const char *input,const char *output)
{
   InitOpcode();

   OpenFile(input);

   RemoveNote();

   ComputingLines();

   Compile();

   SaveResualt(output);
}

bool Assembler::OpenFile(const char *input)
{
   std::fstream in(input,std::ios::in);
   if(!in){return false;}

   std::list<std::string> code_temp;
   std::string buffer;
   std::string buffer_temp;
   bool flag = true;

   while(getline(in,buffer)){
      for(size_t i = 0;i < buffer.size();++i){

	 if(buffer.at(i) >= '!' && buffer.at(i) <= '~'){
	    buffer_temp += buffer.at(i);
	    flag = true;
	 }
	 else if(flag){
	    code_temp.push_back(buffer_temp);
	    buffer_temp = "";
	    flag = false;
	 }
      }
      code.push_back(code_temp);
      code_temp.clear();
   }

   in.close();
/*
   std::list<std::list<std::string> >::iterator start = code.begin(),
      end = code.end();
   for(;start != end;++start){
      std::list<std::string>::iterator s = start->begin(),
	 e = start->end();
      for(;s != e;++s){
	 std::cout << *s << " ";
      }
      std::cout << start->size() << std::endl;
   }
*/
   return true;
}

bool Assembler::SaveResualt(const char *output)
{
   std::fstream out(output,std::ios::out);
   if(!out){return false;}

   std::list<std::list<std::string> >::iterator start = code.begin(),
      end = code.end();
   std::list<std::string>::iterator s = start->begin(),
      e = start->end();

   std::vector<int>::iterator vs = line_count.begin(),ve = line_count.end();

   std::list<std::string>::iterator ls = objcode.begin(),le = objcode.end();

   out << std::hex << *vs << "\t";
   out << *s << "\t";
   ++s;
   out << *s << std::endl;

   ++vs;++start;

   for(;start != end;++start){
      if(vs != ve){
	 out << std::hex << *vs << "\t";
	 ++vs;
      }

      s = start->begin(),e = start->end();
      for(;s != e;++s){
	 out << *s << "\t";
      }

      if(ls != le){
	 out << *ls;
	 ++ls;
      }

      out << std::endl;
   }

   out.close();

   return true;
}

unsigned int Assembler::String2Hex(const std::string &str)
{
   std::stringstream ss;

   ss << std::hex << str;

   unsigned int val;

   ss >> val;

   return val;
}

std::string Assembler::Hex2String(const unsigned int &val)
{
   std::stringstream ss;

   ss << std::hex << val;

   std::string str;

   ss >> str;

   return str;
}

unsigned int Assembler::String2Int(const std::string &str)
{
   std::stringstream ss;

   ss << std::dec << str;

   unsigned int val;

   ss >> val;

   return val;
}

std::string Assembler::Int2String(const unsigned int val)
{
   std::stringstream ss;

   ss << std::hex << val;	//

   std::string str;

   ss >> str;

   return str;
}

void Assembler::RemoveNote()
{
   std::list<std::list<std::string> >::iterator start = code.begin(),
      end = code.end();

   while(start != end){
      std::list<std::string>::iterator s = start->begin(),
	 e = start->end();
      /*
      for(;s != e;++s){
	 std::cout << *s;
      }
      std::cout << start->size() << std::endl;
   */
      if(s->at(0) == '.'){
	 start = code.erase(start);
	 continue;
      }
      ++start;
   }
}

unsigned int Assembler::StartLines(std::list<std::list<std::string> >::iterator &it)
{
   std::list<std::string>::iterator s = it->begin(),
      e = it->end();

   std::string sym = *s;

   for(int i = 0;i < 2;++i,++s){}

   unsigned int val = String2Hex(*s);

   symbol.insert(make_pair(sym,val));
   line_count.push_back(val);

   ++it;

   return val;
}

void Assembler::TypeCheck(std::list<std::string>::iterator &type,unsigned int &count)
{
   if(*type == "WORD"){
      count += 3;
   }
   else if(*type == "RESB"){
      ++type;
      count += String2Int(*type);
   }
   else if(*type == "RESW"){
      ++type;
      count += (String2Int(*type) * 3);
   }
   else if(*type == "BYTE"){
      ++type;
      char first = type->at(0);
      if(first == 'C'){
	 count += (type->length() - 3);
      }
      else{
	 count += 1;
      }
   }
   else{
      count += 3;
   }
}

bool Assembler::ComputingLines()
{
   std::list<std::list<std::string> >::iterator start = code.begin(),
      end = code.end();
   --end;

   unsigned int count = StartLines(start);

   for(;start != end;++start){
      if(start->size() <= 2){
	 line_count.push_back(count);
	 count += 3;
      }
      else if(start->size() == 3){
	 std::list<std::string>::iterator s = start->begin();

	 symbol.insert(make_pair(*s,count));
	 line_count.push_back(count);

	 TypeCheck(++s,count);
      }
   }
/*
   std::vector<int>::iterator is = line_count.begin(),ie = line_count.end();
   for(;is != ie;is++){
      std::cout << std::hex << *is << std::endl;
   }

   std::map<std::string,int>::iterator iis = symbol.begin(),iie = symbol.end();
   for(;iis != iie;++iis){
      std::cout << iis->first << " " << iis->second << std::endl;
   }
*/
}

bool Assembler::Compile()
{
   std::list<std::list<std::string> >::iterator start = code.begin(),
      end = code.end();
   ++start;--end;

   for(;start != end;++start){
      std::list<std::string>::iterator s = start->begin(),
	 e = start->end();
      std::string op1;
      std::string op2;
      if(start->size() == 2){
	 op1 = opcode[*s];
	 ++s;
	 int found = s->find(',');
	 if(found != -1){
	    std::string temp(*s,0,found);
	    unsigned int val = symbol[temp] + 0x8000;
	    op2 = Hex2String(val);
	 }
	 else{
	    op2 = Hex2String(symbol[*s]);
	 }
      }
      else if(start->size() == 1){
	 op1 = opcode[*s];
	 op2 = "0000";
      }
      else if(start->size() == 3){
	 ++s;
	 if(*s == "BYTE"){
	    ++s;
	    char type = s->at(0);
	    if(type == 'X'){
	       int front = s->find('\'');
	       int back = s->rfind('\'');
	       op1 = std::string(*s,front + 1,back - 2);
	    }
	    else if(type == 'C'){
	       int front = s->find('\'');
	       int back = s->rfind('\'');
	       std::string str(*s,front + 1,back - 2);
	       std::string temp;
	       for(size_t i = 0;i < str.length();++i){
		  op1 += Int2String((unsigned int)str.at(i));	//
	       }
	    }
	 }
	 else if(*s == "WORD"){
	    ++s;
	    op1 = Hex2String(String2Hex(*s));
	    while(op1.length() < 6){
	       op1.insert(0,1,'0');
	    }
	 }
	 else if(*s == "RESW"){
	 }
	 else if(*s == "RESB"){
	 }
	 else{
	    op1 = opcode[*s];
	    ++s;
	    op2 = Hex2String(symbol[*s]);
	 }
      }
      objcode.push_back(op1 + op2);
   }
}
