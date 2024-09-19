
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction)
 : G4VUserActionInitialization(), fDetConstruction(detConstruction)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
 SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new RunAction);
  SetUserAction(new PrimaryGeneratorAction);
  EventAction* eventAction = new EventAction;
  SetUserAction(eventAction);
  SetUserAction(new SteppingAction(fDetConstruction,eventAction));
} 

/*ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
	PrimaryGenerator *generator = new PrimaryGenerator();
	SetUserAction(generator);
	
	RunAction *runAction = new RunAction();
	SetUserAction(runAction);
	//to tell g4 that we have an user defined event action class
	EventAction *eventAction = new EventAction(runAction);
	SetUserAction(eventAction);
	
	SteppingAction *steppingAction = new SteppingAction(eventAction);
	SetUserAction(steppingAction);
}*/








 
