#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual~DetectorConstruction(); //there was a virtual before this line

  public:
    virtual G4VPhysicalVolume* Construct();

    // get methods
    const G4VPhysicalVolume* GetCF4PV() const;
    const G4VPhysicalVolume* GetPMTPV(int ID) const;
    const G4VPhysicalVolume* GetWindowPV(int ID) const;
     
  private:
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    
    G4VPhysicalVolume* CF4PV;
    G4VPhysicalVolume* PMT0PV;
    G4VPhysicalVolume* Window0PV;
  
    G4VPhysicalVolume* PMT1PV;
    G4VPhysicalVolume* Window1PV;

    G4VPhysicalVolume* PMT2PV;
    G4VPhysicalVolume* Window2PV;
};
	

inline const G4VPhysicalVolume* DetectorConstruction::GetCF4PV() const  { 
  return CF4PV; 
}

inline const G4VPhysicalVolume* DetectorConstruction::GetPMTPV(int ID) const  { 
  if(ID==0)  return PMT0PV; 
  if(ID==1)  return PMT1PV; 
  if(ID==2)  return PMT2PV; 
  return CF4PV;
}

inline const G4VPhysicalVolume* DetectorConstruction::GetWindowPV(int ID) const  { 
  if(ID==0)  return Window0PV; 
  if(ID==1)  return Window1PV; 
  if(ID==2)  return Window2PV; 
  return CF4PV;
}


#endif

