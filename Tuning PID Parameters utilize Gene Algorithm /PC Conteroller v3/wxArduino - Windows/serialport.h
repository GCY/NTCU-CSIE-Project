#ifndef __SERIAL_PORT__
#define __SERIAL_PORT__

#include <wx/wx.h>

#include <windows.h>

class SerialPort
{
public:
	SerialPort();
	~SerialPort();
	bool Open(const wxString,const DWORD);
	void Close();
	void Write(unsigned char*,DWORD);
	void Read(unsigned  char*,DWORD);
	bool ReadByte();
	bool IsOpen(){if(com){return true;}else{return false;}}
	unsigned char GetChar(){return read_byte;}
private:
	HANDLE com;
	DCB dcb;
	COMMTIMEOUTS timeouts;
	OVERLAPPED overlapped;
	unsigned char read_byte;
};

#endif
