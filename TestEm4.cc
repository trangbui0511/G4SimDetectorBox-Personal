#include "G4Types.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4SteppingVerbose.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"

#include "ActionInitialization.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
int main(int argc,char** argv) {

  //detect interactive mode (if no arguments) and define UI session
  /*G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc,argv);*/

  //choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  //Use SteppingVerbose with Unit
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);
  
  //construct the run manager
  auto runManager = G4RunManagerFactory::CreateRunManager();
  if (argc==3) {
     G4int nThreads = G4UIcommand::ConvertToInt(argv[2]);
     runManager->SetNumberOfThreads(nThreads);
  }
  //set mandatory initialization classes
  auto detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);
  
  auto physicsList = new FTFP_BERT;
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  //physicsList->RegisterPhysics(new G4OpticalPhysics());
  physicsList->RegisterPhysics(opticalPhysics);
  // physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  //set user action classes
  runManager->SetUserInitialization(new ActionInitialization(detConstruction));

  //Initialize G4 kernel
  runManager->Initialize();
  
  //=============================================
  /*G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();
	
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
	
  UImanager->ApplyCommand("/vis/open OGL");
  UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
  UImanager->ApplyCommand("/vis/drawVolume");
  UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
  UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
  UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
	
  ui->SessionStart();
	
  return 0;*/
  //=============================================

  //initialize visualization
  //auto visManager = new G4VisExecutive;
  //****G4VisManager* visManager = nullptr;****
  
  	G4UIExecutive *ui = 0;
	if(argc == 1)//When no of commnd line argument is 0, 1 is just the sim.. Then generate ui
	{
		ui = new G4UIExecutive(argc, argv);
	}
	
  G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();

  //get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  

  if (ui)  {
  /* //interactive mode
   visManager = new G4VisExecutive;
   visManager->Initialize();
   ui->SessionStart();
   delete ui;*/
   
   UImanager->ApplyCommand("/control/execute vis.mac");
	
	
	ui->SessionStart();
	
	
  }
  else  {
   //batch mode
   G4String command = "/control/execute ";
   G4String fileName = argv[1];
   UImanager->ApplyCommand(command+fileName);
  }

  //job termination
  //delete visManager;
  //delete runManager;
  return 0;
}

