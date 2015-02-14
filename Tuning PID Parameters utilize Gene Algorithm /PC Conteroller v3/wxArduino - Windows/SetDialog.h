#ifndef __ADD_DIALOG__
#define __ADD_DIALOG__
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/valtext.h>
#include <wx/valgen.h>

enum{
   ID_POPULATION = 300,
   ID_ITERATE,
   ID_CROSSOVER,
   ID_MUTATION,
   ID_SETPOINT,
   ID_SET_RESET
};

class SetDialog:public wxDialog
{
   public:
      SetDialog();

      SetDialog(wxWindow *parent,
	    wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Setting Parameter"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

      bool Create(wxWindow *parent,
	    wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Setting Parameter"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

      void Init();
      void CreateControls();

      void OnReset(wxCommandEvent &event);

      bool TransferDataToWindow();
      bool TransferDataFromWindow();

	  unsigned int GetIterate(){return iterate;}
	  unsigned int GetPopulation(){return population;}
	  unsigned int GetCrossover(){return crossover;}
	  unsigned int GetMutation(){return mutation;}
	  unsigned int GetSetpoint(){return setpoint;}

   private:

	   unsigned int iterate,population,crossover,mutation,setpoint;

      DECLARE_CLASS(SetDialog)
      DECLARE_EVENT_TABLE()
};

#endif
