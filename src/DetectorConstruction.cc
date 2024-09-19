#include "DetectorConstruction.hh"

#include "G4Tubs.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4SubtractionSolid.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4OpticalSurface.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4ThreeVector u = G4ThreeVector(0,0,1);
  G4ThreeVector v = G4ThreeVector(0,1,0);
  G4ThreeVector w = G4ThreeVector(1,0,0);

  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_AIR");
  nistManager->FindOrBuildMaterial("G4_Al");
  nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  //https://gitlab.cern.ch/geant4/geant4/-/blob/8a51e0bc408e73c60512e61fbc51c2be9c22904d/examples/advanced/Rich/src/RichTbMaterial.cc
  nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");

  G4double a,z,density, temperature, pressure;
  G4String name,symbol;
  G4int numel,natoms; 

  a=12.01*g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C",z=6.,a);
  a=18.998*g/mole;
  G4Element* elF = new G4Element(name="Fluorine",symbol="F",z=9.,a); 
  
  a=26.98*g/mole;
  G4Element* elAl = new G4Element(name="Aluminum", symbol="Al",z=13.,a);

  temperature=298.*kelvin;
  double pressureFactor=0.5;
  pressure = pressureFactor*atmosphere;

  density=2*pressureFactor*0.0018*g/cm3;
 
  G4Material* CF4 =new G4Material(name="CF4",density,numel=2,kStateGas,temperature,pressure);
  CF4->AddElement(elC,natoms=1);
  CF4->AddElement(elF,natoms=4);

  //// ------------ Scintillation Info --------------
 
  std::vector<G4double> photonEnergy = { 1.54981*eV, 1.65313*eV, 1.7712*eV, 1.9074*eV, 2.0664*eV, 2.2543*eV, 2.48*eV, 2.7552*eV, 3.0996*eV, 3.54242*eV, 4.1328*eV, 4.9593*eV, 6.1992*eV};

  //CF4
  std::vector<G4double> refractiveIndexCF4 = {1.000480, 1.000480, 1.000480, 1.0004805, 1.000481, 1.000482, 1.000485, 1.000488, 1.000490, 1.000495, 1.000498, 1.000510, 1.000525   };//Source: R. Abjean, A. Bideau-Mehu, and Y. Guern. Refractive index of carbon tetrafluoride (cf4) in the 300-140 nm wavelength range. Nuclear Instruments and Methods in Physics Research Section A: Accelerators,Spectrometers, Detectors and Associated Equipment, 292(3):593–594, 1990.

  std::vector<G4double> absorptionLengthCF4 = {1.0 * m,  1.0 * m,  1.0 * m,  1.0 * m, 1.0 * m, 1.0 * m, 1.0 * m, 1.0 * m, 1.0 * m, 1.0 * m, 1.0 * m, 1.0 * m, 1.0 * m}; 

  std::vector<G4double> scintilFast = {0.00, 0.00, 0.10, 0.18, 0.10, 0.05, 0.00, 0.00, 0.05, 0.10, 0.28, 0.20, 0.00}; //Source: A. Morozov et. al., Photon yield for ultraviolet and visible emission from cf4 excited with particles. Nuclear Instruments and Methods in Physics Research Section B: Beam Interactions with Materials and Atoms, 268(9):1456–1459, 2010

  G4MaterialPropertiesTable* myScin = new G4MaterialPropertiesTable();
  myScin->AddProperty("RINDEX", photonEnergy, refractiveIndexCF4, 13);
  myScin->AddProperty("ABSLENGTH", photonEnergy, absorptionLengthCF4, 13);
  myScin->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintilFast, false, false);
  myScin->AddConstProperty("SCINTILLATIONYIELD", 200. / MeV);
  myScin->AddConstProperty("RESOLUTIONSCALE", 1.0);
  myScin->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 8.5*ns);//Source: Liu et. al., Primary scintillation characteristics of ar+cf4 gas mixtures excited by proton and alpha particles. Nuclear Instruments and Methods in Physics Research Section A: Accelerators, Spectrometers, Detectors and Associated Equipment, 694:157–161, 2012
  myScin->AddConstProperty("SCINTILLATIONRISETIME1", 1.0*ns);
  myScin->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  myScin->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
  CF4->SetMaterialPropertiesTable(myScin);

  // Air
  std::vector<G4double> refractiveIndexAir = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00}; 
  std::vector<G4double> absorptionLengthAir = {10.0 * m,  10.0 * m,  10.0 * m,  10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m}; 

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndexAir, 13);
  myMPT2->AddProperty("ABSLENGTH", photonEnergy, absorptionLengthAir, 13);

  //// Window Glass
  std::vector<G4double> refractiveIndexWindow = {1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5}; 
  std::vector<G4double> absorptionLengthWindow = {10.0 * m,  10.0 * m,  10.0 * m,  10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m}; 

  G4MaterialPropertiesTable* myMPT3 = new G4MaterialPropertiesTable();
  myMPT3->AddProperty("RINDEX", photonEnergy, refractiveIndexWindow, 13);
  myMPT3->AddProperty("ABSLENGTH", photonEnergy, absorptionLengthWindow, 13);
  

  //Aluminium
  /*std::vector<G4double> refractiveIndexAl = {1.37289, 1.37289, 1.37289, 1.37289, 1.37289, 1.37289, 1.37289, 1.37289, 1.37289, 1.37289, 1.37289, 1.37289, 1.37289};
  std::vector<G4double> absorptionLengthAl = {10.0 * m,  10.0 * m,  10.0 * m,  10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m};
  
  G4MaterialPropertiesTable* myMPT4 = new G4MaterialPropertiesTable();
  myMPT4->AddProperty("RINDEX", photonEnergy, refractiveIndexAl, 13);
  myMPT4->AddProperty("ABSLENGTH", photonEnergy, absorptionLengthAl, 13);
*/

  //Volume of the box
  auto worldSizeXY = 100. * cm;
  auto worldSizeZ  = 100. * cm;

  auto defaultMaterial = G4Material::GetMaterial("G4_AIR");
  auto detectorShellMaterial = G4Material::GetMaterial("G4_Al");
  
  auto WinMat = G4Material::GetMaterial("G4_SILICON_DIOXIDE");

  CF4->SetMaterialPropertiesTable(myScin);
  defaultMaterial->SetMaterialPropertiesTable(myMPT2);
  WinMat->SetMaterialPropertiesTable(myMPT3);
  
  // World
  auto worldS = new G4Box("World",worldSizeXY/2, worldSizeXY/2, worldSizeZ/2);
  auto worldLV= new G4LogicalVolume(worldS,defaultMaterial,"World");         
  auto worldPV= new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0);   

  G4double boxInnerLength = 20*cm;
  G4double boxOuterLength = 22*cm;

  // CF4
  G4double tubeInnerRadius = 3*cm;

  auto CF4Solid = new G4Box("CF4Solid",0.5*boxInnerLength,0.5*boxInnerLength,0.5*boxInnerLength);
  //G4VSolid* CF4Solid_a = new G4SubtractionSolid("CF4Solid_a", CF4Solid, tubeVolume, 0, G4ThreeVector(0.,0.,0.));

  //auto CF4LV= new G4LogicalVolume(CF4Solid_a,CF4,"CF4LV"); 
  //G4VisAttributes yellow(G4Colour::Yellow());
  //CF4LV->SetVisAttributes(yellow);
  //CF4PV = new G4PVPlacement(0,G4ThreeVector(),CF4LV,"CF4PV",worldLV,false,0);

  //======= CF4 inside the source tube
  auto CF4InsideTube = new G4Tubs("CF4InsideTube",0*cm,5.2*0.5*cm,5.*cm,0.,360*deg);
  auto CF4InsideTubeLV = new G4LogicalVolume(CF4InsideTube,CF4,"CF4InsideTubeLV");
  G4VisAttributes yellow(G4Colour::Yellow());
  CF4InsideTubeLV->SetVisAttributes(yellow);
  auto CF4InsideTubePV = new G4PVPlacement(0,G4ThreeVector(),CF4InsideTubeLV,"CF4InsideTubePV",worldLV,false,0);  


  //======= CF4 outside the source tube
  auto outerTubeVolume = new G4Tubs("outerTubeVolume",0*cm,5.4*0.5*cm,5.*cm,0.,360*deg);
  G4VSolid* CF4OutsideTube = new G4SubtractionSolid("CF4OutsideTube", CF4Solid, outerTubeVolume, 0, G4ThreeVector(0.,0.,0.));
  auto CF4OutsideTubeLV = new G4LogicalVolume(CF4OutsideTube,CF4,"CF4OutsideTubeLV");
  G4VisAttributes purple(G4Colour::Magenta());
  CF4OutsideTubeLV->SetVisAttributes(purple);
  auto CF4OutsideTubePV = new G4PVPlacement(0,G4ThreeVector(),CF4OutsideTubeLV,"CF4InsideTubePV",worldLV,false,0);


  //Housing
  auto AlBoxSolid= new G4Box("AlBoxSolid",0.5*boxOuterLength,0.5*boxOuterLength,0.5*boxOuterLength);

  G4VSolid* AlBoxSolid_a = new G4SubtractionSolid("AlBoxSolid_a", AlBoxSolid, CF4Solid,0, G4ThreeVector(0.,0.,0.));  

  auto PMTHoleSolid = new G4Tubs("PMTHoleSolid",0*cm,5.1*0.5*cm,20*cm,0.,360*deg);

  G4VSolid* AlBoxSolid_b = new G4SubtractionSolid("AlBoxSolid_b", AlBoxSolid_a, PMTHoleSolid,0, G4ThreeVector(0.,0.,0.));  
  auto housingLV= new G4LogicalVolume(AlBoxSolid_b,detectorShellMaterial,"housingLV");         
  auto housingPV= new G4PVPlacement(0,G4ThreeVector(0*cm,0*cm,0*cm),housingLV,"housingPV",worldLV,false,0);


  auto QuartzWindowSolid =  new G4Tubs("QuartzWindowSolid",0*cm,5.1*0.5*cm,0.5*0.3175*cm,0.,360*deg);
  auto QuartzWindowLV = new G4LogicalVolume(QuartzWindowSolid,WinMat,"QuartzWindowLV");         
  G4VisAttributes blue(G4Colour::Blue());
  QuartzWindowLV->SetVisAttributes(blue);
  Window0PV= new G4PVPlacement(0,G4ThreeVector(0.,0.,0.5*boxInnerLength + 0.5*cm),QuartzWindowLV,"QuartzWindow0PV",worldLV,false,0);
  Window1PV= new G4PVPlacement(0,G4ThreeVector(0.,0.,-(0.5*boxInnerLength + 0.5*cm)),QuartzWindowLV,"QuartzWindow1PV",worldLV,false,0);

  auto PMTSolid =  new G4Tubs("PMTSolid",0*cm,5.1*0.5*cm,0.5*10*cm,0.,360*deg);
  auto PMTLV = new G4LogicalVolume(PMTSolid,defaultMaterial,"PMTLV");         
  G4VisAttributes red(G4Colour::Red());
  PMTLV->SetVisAttributes(red);
  PMT0PV = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.5*0.3175*cm + 5.5*cm + 0.5*boxInnerLength),PMTLV,"PMT0PV",worldLV,false,0);
  PMT1PV = new G4PVPlacement(0,G4ThreeVector(0.,0.,-(0.5*0.3175*cm + 5.5*cm + 0.5*boxInnerLength)),PMTLV,"PMT1PV",worldLV,false,0);

  //Source Tube
  auto tubeMaterial = G4Material::GetMaterial("G4_POLYETHYLENE");
  auto tubeVolume = new G4Tubs("tubeVolume",5.2*0.5*cm,5.4*0.5*cm,5.*cm,0.,360*deg);
  //auto hollowTube = new G4Tubs("hollowTube",5.2*0.5*cm,5.4*0.5*cm,10*cm,0.,360*deg);

  //G4VSolid* hollowTube_a = new G4SubtractionSolid("hollowTube_a", CF4Solid, hollowTube, 0, G4ThreeVector(0.,0.,0.));
  
  //G4VSolid* hollowTube_b = new G4SubtractionSolid("hollowTube_b", CF4Solid, hollowTube_a, 0, G4ThreeVector(0.,0.,0.));

  auto TubeLV = new G4LogicalVolume(tubeVolume, tubeMaterial, "TubeLV");
  //auto TubeLV = new G4LogicalVolume(tubeVolume, detectorShellMaterial, "TubeLV");

  G4VisAttributes green(G4Colour::Green());
  TubeLV->SetVisAttributes(green);

  auto TubePV = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),TubeLV,"TubePV",worldLV,false,0);


  ////cut out gas volume from solid Al cylinder
  //G4VSolid* AlShellSolid_a = new G4SubtractionSolid("AlShellSolid_a", AlCylinderSolid, CF4Solid,0, G4ThreeVector(0.,0.,0.));  

  ////cut out top hole for PMT
  //G4VSolid* AlShellSolid_b = new G4SubtractionSolid("AlShellSolid_b", AlShellSolid_a, PMTHoleSolid,0, G4ThreeVector(0.*cm,0.*cm,5.*cm));  

  ////Top Quartz Window
  // //Top PMT
  // auto PMTHoleFaceSolid = new G4Box("PMTHoleFaceSolid",0.5*5.2*cm, 1*cm,0.5*5.2*cm);
  // G4VSolid* AlShellSolid_c= new G4SubtractionSolid("AlShellSolid_c", AlShellSolid_b, PMTHoleFaceSolid,G4Transform3D(G4RotationMatrix(u,w,v), G4ThreeVector(6.3*cm,0*cm,0*cm)));
  // G4VSolid* AlShellSolid_d= new G4SubtractionSolid("AlShellSolid_d", AlShellSolid_c, PMTHoleFaceSolid,G4Transform3D(G4RotationMatrix(u,v,w), G4ThreeVector(0*cm,6.3*cm,0*cm)));

  // //cut out hole for PMT
  // G4VSolid* PMTHoleFaceSolidwithHole = new G4SubtractionSolid("PMTHoleFaceSolidwithHole", PMTHoleFaceSolid, PMTHoleSolid,G4Transform3D(G4RotationMatrix(u,w,v), G4ThreeVector(0.,0*cm,0*cm)));
  // 
  // G4VSolid* AlShellSolid_e = new G4UnionSolid("AlShellSolid_e", AlShellSolid_d, PMTHoleFaceSolidwithHole,0, G4ThreeVector(0.,6.3*cm,0*cm));  
  // G4VSolid* AlShellSolid_f = new G4UnionSolid("AlShellSolid_f", AlShellSolid_e, PMTHoleFaceSolidwithHole,G4Transform3D(G4RotationMatrix(u,w,v), G4ThreeVector(6.3*cm,0*cm,0*cm)));  

  //Window1PV= new G4PVPlacement(G4Transform3D(G4RotationMatrix(u,w,v), G4ThreeVector(0*cm,6.3*cm,0*cm)),QuartzWindowLV,"QuartzWindowPV",worldLV,false,0);
  //Window2PV= new G4PVPlacement(G4Transform3D(G4RotationMatrix(u,v,w), G4ThreeVector(6.3*cm,0*cm,0*cm)),QuartzWindowLV,"QuartzWindowPV",worldLV,false,0);

  //PMT1PV = new G4PVPlacement(G4Transform3D(G4RotationMatrix(u,w,v), G4ThreeVector(0*cm,((0.5*0.3175)+(10*0.5)+6.3)*cm,0*cm)),PMTLV,"PMTPV",worldLV,false,0);
  //PMT2PV = new G4PVPlacement(G4Transform3D(G4RotationMatrix(u,v,w), G4ThreeVector(0.5*0.3175*cm+10*0.5*cm+6.3*cm,0*cm,0*cm)),PMTLV,"PMTPV",worldLV,false,0);


  ////====================== Reflective Wrapping =============================
  //
  G4OpticalSurface* OpSurface = new G4OpticalSurface("reflector");
  
  G4LogicalBorderSurface* Surface = new G4LogicalBorderSurface("CF4 reflector", CF4OutsideTubePV , housingPV, OpSurface);
  G4LogicalBorderSurface* SurfaceTube = new G4LogicalBorderSurface("Tube reflector", CF4InsideTubePV, TubePV, OpSurface);

  OpSurface -> SetType(dielectric_metal);
  OpSurface -> SetModel(glisur);
  
  G4MaterialPropertiesTable *OpSurfaceProperty = new G4MaterialPropertiesTable();
  std::vector<G4double> reflectivity = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,1.,1.};
  //std::vector<G4double> reflectivity = {0.868, 0.875, 0.880, 0.907, 0.910, 0.913, 0.918, 0.919, 0.920, 0.921, 0.922, 0.926, 0.930};
  //https://laserbeamproducts.wordpress.com/2014/06/19/reflectivity-of-aluminium-uv-visible-and-infrared/      (For pure uncoated Aluminum)
  std::vector<G4double> efficiency = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1. ,1.};
  OpSurfaceProperty -> AddProperty("REFLECTIVITY",photonEnergy,reflectivity,13);
  OpSurfaceProperty -> AddProperty("EFFICIENCY",photonEnergy,efficiency,13);
  OpSurface -> SetMaterialPropertiesTable(OpSurfaceProperty);


  return worldPV;
}
