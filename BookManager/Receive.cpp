#include <wx/zstream.h>

#include "Receive.h"

Receive::Receive(wxString filename,wxSocketBase *socket)
{
   m_filename = filename;
   m_socket = socket;

   Create();
   Run();
}

Receive::~Receive()
{
}

void* Receive::Entry()
{
   m_socket->SetTimeout(30);
   m_socket->SetFlags(wxSOCKET_WAITALL | wxSOCKET_BLOCK);
   wxFileOutputStream FileOutputStream(m_filename);
   wxSocketInputStream SocketInputStream(*m_socket);
   wxZlibInputStream ZlibInputStream(SocketInputStream);
   FileOutputStream.Write(ZlibInputStream);

   m_socket->Destroy();
}
