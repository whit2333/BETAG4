#ifndef BETAHodoscopeCellParameterisation_HH
#define BETAHodoscopeCellParameterisation_HH

#include "G4VPVParameterisation.hh"
#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include <vector>
#include "G4SDManager.hh"
#include "BETAHodoscopePMT.hh"

class G4VPhysicalVolume;

class BETAHodoscopeCellParameterisation : public G4VPVParameterisation {

public:

  // Constructor
  BETAHodoscopeCellParameterisation(G4Material * aMaterial);

  // Destructor
  virtual ~BETAHodoscopeCellParameterisation();

  // Method
  virtual void ComputeTransformation(const G4int copyNo,G4VPhysicalVolume *physVol) const;
  virtual G4VSolid* ComputeSolid(const G4int copyNo, G4VPhysicalVolume* physVol);
  virtual G4Material* ComputeMaterial // material, sensitivity, visAtt
     (const G4int copyNo, G4VPhysicalVolume* physVol,
         const G4VTouchable *parentTouch);
private:
G4VSensitiveDetector* HodoscopePMTs;
  G4RotationMatrix hodBarRotateRemove1;
  G4RotationMatrix hodBarRotateRemove2;
  G4Material * theMaterial;
  G4RotationMatrix * rotate1;
  G4RotationMatrix * rotate2;
  G4VSolid * theSolid;
  G4VSolid * hodoscopeBar;
  G4VSolid * PMTinquotes;
  G4double theta;
  // Data members
  std::vector<G4double> xCell;
  std::vector<G4double> yCell;
  std::vector<G4double> zCell;
};

#endif


