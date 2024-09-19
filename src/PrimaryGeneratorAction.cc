
#include "PrimaryGeneratorAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"



PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),fParticleGun1(0)
{
  G4int n_particle = 1;
  fParticleGun1  = new G4ParticleGun(n_particle);
  
  // default particle kinematic
  
  G4ParticleDefinition* particle1= G4ParticleTable::GetParticleTable()->FindParticle("alpha");
  fParticleGun1->SetParticleDefinition(particle1);
  fParticleGun1->SetParticleEnergy(5.48*MeV);
  
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun1;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4double Xpos = 0*cm;
  //G4double Ypos = 0;//-8.*cm;
  G4double Ypos = 0*cm;
  //G4double Zpos = 8*cm;
  G4double Zpos = 0*cm;
  
  G4double xdir = (2.*(G4double)G4UniformRand()-1.);
  G4double ydir = (2.*(G4double)G4UniformRand()-1.);
  G4double zdir = (2.*(G4double)G4UniformRand()-1.);
  
  fParticleGun1->SetParticlePosition(G4ThreeVector(Xpos,Ypos,Zpos));
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(xdir, ydir, zdir));

  fParticleGun1->GeneratePrimaryVertex(anEvent);

  //G4cout << Xpos/cm << " " << Ypos/cm << " " << Zpos/cm << G4endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->FillNtupleDColumn(1,Xpos/cm);
  man->FillNtupleDColumn(2,Ypos/cm);
  man->FillNtupleDColumn(3,Zpos/cm);

}
