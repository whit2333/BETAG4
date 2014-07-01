#ifndef BETARCSCellParameterisation_HH
#define BETARCSCellParameterisation_HH

#include "G4VPVParameterisation.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include <vector>

class G4VPhysicalVolume;

class BETARCSCellParameterisation : public G4VPVParameterisation {

public:

  /// Constructor
  BETARCSCellParameterisation();

  /// Destructor
  virtual ~BETARCSCellParameterisation();

  virtual void ComputeTransformation(const G4int copyNo,G4VPhysicalVolume *physVol) const;

private:

  // Data members
  std::vector<G4double> xCell;
  std::vector<G4double> yCell;

};

#endif


