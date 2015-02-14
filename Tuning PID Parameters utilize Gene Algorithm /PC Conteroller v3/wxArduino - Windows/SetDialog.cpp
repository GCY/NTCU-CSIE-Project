#include "SetDialog.h"

IMPLEMENT_CLASS(SetDialog,wxDialog)

BEGIN_EVENT_TABLE(SetDialog,wxDialog)
   EVT_BUTTON(ID_SET_RESET,SetDialog::OnReset)
END_EVENT_TABLE()

SetDialog::SetDialog()
{
   Init();
}

SetDialog::SetDialog(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
   Init();
   Create(parent,id,caption,pos,size,style);
}

inline void SetDialog::Init()
{
	iterate = 100;
	population = 50;
	crossover = 50;
	mutation = 10;
	setpoint = 700;
}

bool SetDialog::Create(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
   SetExtraStyle(wxWS_EX_BLOCK_EVENTS | wxDIALOG_EX_CONTEXTHELP);

   if(!wxDialog::Create(parent,id,caption,pos,size,style)){
      return false;
   }

   CreateControls();

   GetSizer()->Fit(this);
   GetSizer()->SetSizeHints(this);

   Center();

   return true;
}

void SetDialog::CreateControls()
{

   /**********TopBox**********/

   wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
   this->SetSizer(top);

   /**********TextCtrlBox**********/

   wxBoxSizer *TextCtrlBox = new wxBoxSizer(wxVERTICAL);
   top->Add(TextCtrlBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   /**********SpinCtrlBox**********/

   wxBoxSizer *SpinCtrlBox = new wxBoxSizer(wxHORIZONTAL);
   TextCtrlBox->Add(SpinCtrlBox,0,wxGROW | wxALL,5);

   wxStaticText *iterate_lable = new wxStaticText(this,wxID_STATIC,wxT("&Iterate:"),wxDefaultPosition,wxDefaultSize,0);
   SpinCtrlBox->Add(iterate_lable,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxSpinCtrl *iterate_spin = new wxSpinCtrl(this,ID_ITERATE,wxEmptyString,wxDefaultPosition,wxSize(60,-1),wxSP_ARROW_KEYS,1,10000,iterate);
   SpinCtrlBox->Add(iterate_spin,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxStaticText *population_lable = new wxStaticText(this,wxID_STATIC,wxT("&Population:"),wxDefaultPosition,wxDefaultSize,0);
   SpinCtrlBox->Add(population_lable,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxSpinCtrl *population_spin = new wxSpinCtrl(this,ID_POPULATION,wxEmptyString,wxDefaultPosition,wxSize(60,-1),wxSP_ARROW_KEYS,10,70,population);
   SpinCtrlBox->Add(population_spin,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxStaticText *crossover_lable = new wxStaticText(this,wxID_STATIC,wxT("&Crossover(%):"),wxDefaultPosition,wxDefaultSize,0);
   SpinCtrlBox->Add(crossover_lable,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxSpinCtrl *crossover_spin = new wxSpinCtrl(this,ID_CROSSOVER,wxEmptyString,wxDefaultPosition,wxSize(60,-1),wxSP_ARROW_KEYS,1,100,crossover);
   SpinCtrlBox->Add(crossover_spin,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxStaticText *mutation_lable = new wxStaticText(this,wxID_STATIC,wxT("&Mutation(%)"),wxDefaultPosition,wxDefaultSize,0);
   SpinCtrlBox->Add(mutation_lable,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxSpinCtrl *mutation_spin = new wxSpinCtrl(this,ID_MUTATION,wxEmptyString,wxDefaultPosition,wxSize(60,-1),wxSP_ARROW_KEYS,0,100,mutation);
   SpinCtrlBox->Add(mutation_spin,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxStaticText *setpoint_lable = new wxStaticText(this,wxID_STATIC,wxT("&Setpoint"),wxDefaultPosition,wxDefaultSize,0);
   SpinCtrlBox->Add(setpoint_lable,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxSpinCtrl *setpoint_spin = new wxSpinCtrl(this,ID_SETPOINT,wxEmptyString,wxDefaultPosition,wxSize(60,-1),wxSP_ARROW_KEYS,0,1024,setpoint);
   SpinCtrlBox->Add(setpoint_spin,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxStaticLine* line = new wxStaticLine ( this, wxID_STATIC,wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
   TextCtrlBox->Add(line, 0, wxGROW|wxALL, 5);

   /**********ResetOkCancelBox**********/

   wxBoxSizer *ResetOkCancelBox = new wxBoxSizer(wxHORIZONTAL);
   TextCtrlBox->Add(ResetOkCancelBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

   wxButton *reset = new wxButton(this,ID_SET_RESET,wxT("&Reset"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(reset,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxButton *ok = new wxButton(this,wxID_OK,wxT("&Ok"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(ok,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

   wxButton *cancel = new wxButton(this,wxID_CANCEL,wxT("&Cancel"),wxDefaultPosition,wxDefaultSize,0);
   ResetOkCancelBox->Add(cancel,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
}

bool SetDialog::TransferDataToWindow()
{
   wxSpinCtrl *iterate_spin = (wxSpinCtrl*)FindWindow(ID_ITERATE);
   wxSpinCtrl *population_spin = (wxSpinCtrl*)FindWindow(ID_POPULATION);
   wxSpinCtrl *crossover_spin = (wxSpinCtrl*)FindWindow(ID_CROSSOVER);
   wxSpinCtrl *mutation_spin = (wxSpinCtrl*)FindWindow(ID_MUTATION);
   wxSpinCtrl *setpoint_spin = (wxSpinCtrl*)FindWindow(ID_SETPOINT);

   iterate_spin->SetValue(iterate);
   population_spin->SetValue(population);
   crossover_spin->SetValue(crossover);
   mutation_spin->SetValue(mutation);
   setpoint_spin->SetValue(setpoint);

   return true;
}

bool SetDialog::TransferDataFromWindow()
{
   wxSpinCtrl *iterate_spin = (wxSpinCtrl*)FindWindow(ID_ITERATE);
   wxSpinCtrl *population_spin = (wxSpinCtrl*)FindWindow(ID_POPULATION);
   wxSpinCtrl *crossover_spin = (wxSpinCtrl*)FindWindow(ID_CROSSOVER);
   wxSpinCtrl *mutation_spin = (wxSpinCtrl*)FindWindow(ID_MUTATION);
   wxSpinCtrl *setpoint_spin = (wxSpinCtrl*)FindWindow(ID_SETPOINT);

   iterate = iterate_spin->GetValue();
   population = population_spin->GetValue();
   crossover = crossover_spin->GetValue();
   mutation = mutation_spin->GetValue();
   setpoint = setpoint_spin->GetValue();
   return true;
}

void SetDialog::OnReset(wxCommandEvent &event)
{
   Init();
   TransferDataToWindow();
}
