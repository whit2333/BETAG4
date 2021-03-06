#ifndef BETAProtvinoCellParameterisation_HH
#define BETAProtvinoCellParameterisation_HH

#include "G4VPVParameterisation.hh"
#include "globals.hh"
#include <vector>
#include "G4LogicalVolume.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;

class G4VPhysicalVolume;

/**
 *
 * \ingroup Detectors
 */
class BETAProtvinoCellParameterisation : public G4VPVParameterisation {

public:

  // Constructor
  BETAProtvinoCellParameterisation();

  // Destructor
  virtual ~BETAProtvinoCellParameterisation();

  // Method
  virtual void ComputeTransformation(const G4int copyNo,G4VPhysicalVolume *physVol) const;
  
private:

  // Data members
  std::vector<G4double> xCell;
  std::vector<G4double> yCell;

};

#endif


