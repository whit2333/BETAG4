#ifndef BETAForwardTrackerCellParameterisation_HH
#define BETAForwardTrackerCellParameterisation_HH

#include "G4VPVParameterisation.hh"
#include "globals.hh"
#include <vector>

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;


class G4VPhysicalVolume;

/**
 *
 * \ingroup Detectors
 */
class BETAForwardTrackerCellParameterisation : public G4VPVParameterisation {

public:

  // Constructor
  BETAForwardTrackerCellParameterisation(G4Material * aMaterial);

  // Destructor
  virtual ~BETAForwardTrackerCellParameterisation();
virtual G4VSolid*ComputeSolid
     (const G4int copyNo, G4VPhysicalVolume* physVol) ;
  // Method
  virtual void ComputeTransformation(const G4int copyNo,G4VPhysicalVolume *physVol) const;
  virtual G4Material* ComputeMaterial // material, sensitivity, visAtt
     (const G4int copyNo, G4VPhysicalVolume* physVol,
         const G4VTouchable *parentTouch);
G4VPhysicalVolume * motherPhysicalVolume;
G4VPhysicalVolume ** scoringPhysicalVolumes;

inline void SetMotherPhysicalVolume(G4VPhysicalVolume * pvol){ motherPhysicalVolume = pvol; };
inline void SetScoringPhysicalVolumes(G4VPhysicalVolume ** pvols){ scoringPhysicalVolumes = pvols; };


private:
G4VSolid * vertBar;
G4VSolid * horizBar;
G4VSolid * vertBarScore;
G4VSolid * horizBarScore;
// G4LogicalVolume * vertBarScore_log;
// G4LogicalVolume * horizBarScore_log;
G4VSolid * theSolid;
  // Data members
  std::vector<G4double> xCell;
  std::vector<G4double> yCell;
  std::vector<G4double> zCell;
G4Material * theMaterial;
};

#endif


