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

/**
 *
 * \ingroup Detectors
 */
class BETAHodoscopeCellParameterisation : public G4VPVParameterisation {

public:

  // Constructor
  BETAHodoscopeCellParameterisation(G4Material * aMaterial);

  // Destructor
  virtual ~BETAHodoscopeCellParameterisation();

  // Method
  virtual void ComputeTransformation(  const G4int copyNo,  G4VPhysicalVolume *physVol) const;

  virtual G4VSolid* ComputeSolid(      const G4int copyNo,  G4VPhysicalVolume* physVol);

  virtual G4Material* ComputeMaterial( const G4int copyNo,  G4VPhysicalVolume* physVol,  const G4VTouchable *parentTouch); // material, sensitivity, visAtt
    // Mandatory method, required and reason for this class
//     virtual G4Material* ComputeMaterial(G4VPhysicalVolume *currentVol,
//                                                            const G4int no_lev, 
//                                                            const G4VTouchable *parentTouch=0);
virtual G4int GetNumberOfMaterials() const {return(2);};

  G4Material * theMaterial;
virtual G4Material * 	GetMaterial (G4int idx) const {return(theMaterial);};

private:
G4VSensitiveDetector* HodoscopePMTs;
  G4RotationMatrix hodBarRotateRemove1;
  G4RotationMatrix hodBarRotateRemove2;
  G4RotationMatrix * rotate1;
  G4RotationMatrix * rotate2;
  G4VSolid * theSolid;
  G4VSolid * hodoscopeBar;
  G4VSolid * PMTinquotes;
  G4VSolid * PMTphotocathode;
G4LogicalVolume* PMTphotocathode_log;
G4LogicalVolume* PMTinquotes_log;
G4LogicalVolume* hodoscopeBar_log;
  G4double theta;

  // Data members
  std::vector<G4double> xCell;
  std::vector<G4double> yCell;
  std::vector<G4double> zCell;
};

#endif


