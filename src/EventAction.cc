#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "SteppingAction.hh"
EventAction::EventAction()
:G4UserEventAction(), fTotalEnergyDeposit(0.), PMTTotalCount(0)
{ }

EventAction::~EventAction()
{ }

void EventAction::BeginOfEventAction( const G4Event*)
{ 
 fTotalEnergyDeposit = 0.;
 PMTTotalCount.clear();
 PMTTotalCount.resize(3);
}

void EventAction::EndOfEventAction( const G4Event*)
{   	

  //G4cout << "Energy deposition: " << fTotalEnergyDeposit << G4endl;
  
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  
  man->FillNtupleDColumn(0, fTotalEnergyDeposit);
  for(int i=0;i<3;i++) man->FillNtupleDColumn(4+i, PMTTotalCount[i]);
  man->AddNtupleRow();                       

}
