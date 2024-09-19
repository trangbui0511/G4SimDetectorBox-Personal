#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


RunAction::RunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();	

	man->CreateNtuple("Scoring", "Scoring");
	man->CreateNtupleDColumn("c0fEdep");
	man->CreateNtupleDColumn("c1Xgen");
	man->CreateNtupleDColumn("c2Ygen");
	man->CreateNtupleDColumn("c3Zgen");
	man->CreateNtupleDColumn("c4PMT0SC");
	man->CreateNtupleDColumn("c5PMT1SC");
	man->CreateNtupleDColumn("c6PMT2SC");
	man->FinishNtuple();
	
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();  //included in g4root
	
	G4int runID = run->GetRunID();
	
	std::stringstream strRunID;
	strRunID << runID;
	
	man->OpenFile("output"+strRunID.str()+".root");
	
}

void RunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();  //IMPORTANT!
	
	man->CloseFile();
  
}
