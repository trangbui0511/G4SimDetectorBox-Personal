#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"


//==============
#include "RunAction.hh"
//=============
class EventAction : public G4UserEventAction
{
  public:
    EventAction(); 
   ~EventAction();

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    
    void AddEdep(G4double Edep)     {fTotalEnergyDeposit += Edep;};      
    G4double GetEnergyDeposit()     {return fTotalEnergyDeposit;};    
        
    void AddCountPMT(int i) {PMTTotalCount[i] += 1;}
  private:
    G4double fTotalEnergyDeposit;   // Energy deposited 
    std::vector<G4int> PMTTotalCount;
};

#endif


