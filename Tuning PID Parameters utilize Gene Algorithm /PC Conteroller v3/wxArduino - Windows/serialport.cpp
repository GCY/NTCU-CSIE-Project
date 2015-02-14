#include "serialport.h"

SerialPort::SerialPort():dcb(),timeouts(),overlapped()
{
	com = NULL;
}

SerialPort::~SerialPort()
{
   Close();
}

bool SerialPort::Open(const wxString path,const DWORD baud_rate)
{
	Close();

	com = CreateFile(path.wc_str(),GENERIC_READ | GENERIC_WRITE,NULL,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);

	if(com == INVALID_HANDLE_VALUE){
		Close();
		return false;
	}

	memset(&dcb,0,sizeof(dcb));
	wxString def;
	def.Printf(wxT("%d,n,8,1"),baud_rate);
	BuildCommDCB(def.wc_str(),&dcb);
	
	if(!SetCommState(com,&dcb)){
		Close();
		return false;
	}

	SetupComm(com,4096,4096);
	PurgeComm(com,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	memset(&timeouts,0,sizeof(timeouts));
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 0;

	if(!SetCommTimeouts(com,&timeouts)){
		Close();
		return false;
	}

	memset(&overlapped,0,sizeof(overlapped));
	overlapped.Offset = 0;
	overlapped.OffsetHigh = 0;
	overlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	return true;
}

void SerialPort::Close()
{
	if(com){
		CloseHandle(com);
		com = NULL;
	}
}

void SerialPort::Write(unsigned char *data,DWORD length)
{
	if(com){
		PurgeComm(com, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		WriteFile(com,data,length,NULL,&overlapped);
	}
}

void SerialPort::Read(unsigned char *data,DWORD length)
{
	if(com){
		PurgeComm(com,PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		ReadFile(com,data,length,NULL,&overlapped);
	}
}

bool SerialPort::ReadByte()
{
	BOOL result = TRUE;
	BOOL read = TRUE;
	DWORD error = 0;
	DWORD length = 0;
	COMSTAT comstat;
	OVERLAPPED ov;

	ClearCommError(com,&error,&comstat);
	if(comstat.cbInQue == 0){
		return FALSE;
	}
	if(read){
		result = ReadFile(com,&read_byte,1,&length,&ov);
		if(result){
			switch (error = GetLastError())
			{
				case ERROR_IO_PENDING:
					{
						read = FALSE;
						break;
					}
				default:
					{
						break;
					}
			}
		}
		else{
			read = TRUE;
		}
		if(read == FALSE){
			read = TRUE;
			result = GetOverlappedResult(com,&ov,&length,TRUE);
		}
	}

	return read;
}