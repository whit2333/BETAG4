#ifndef BETAFrontTrackerCellParameterisation_HH
#define BETAFrontTrackerCellParameterisation_HH

#include "G4VPVParameterisation.hh"
#include "globals.hh"
#include <vector>

class G4VPhysicalVolume;

class BETAFrontTrackerCellParameterisation : public G4VPVParameterisation {

public:

  // Constructor
  BETAFrontTrackerCellParameterisation(G4Material * aMaterial);

  // Destructor
  virtual ~BETAFrontTrackerCellParameterisation();
virtual G4VSolid*ComputeSolid
     (const G4int copyNo, G4VPhysicalVolume* physVol) ;
  // Method
  virtual void ComputeTransformation(const G4int copyNo,G4VPhysicalVolume *physVol) const;
  
private:
G4VSolid * vertBar;
G4VSolid * horizBar;
G4VSolid * theSolid;
  // Data members
  std::vector<G4double> xCell;
  std::vector<G4double> yCell;
  std::vector<G4double> zCell;
G4Material * theMaterial;
};

#endif


