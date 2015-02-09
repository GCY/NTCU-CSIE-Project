#ifndef __RECIVE__
#define __RECIVE__

#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/sckstrm.h>
#include <wx/zstream.h>
#include <wx/wfstream.h>

class Receive:public wxThread
{
   public:
      Receive(wxString filename,wxSocketBase *Socket);
      virtual ~Receive();

      void* Entry();
      
   private:
      wxSocketBase *m_socket;
      wxString m_filename;
};

#endif
