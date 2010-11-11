#ifndef BETAG4PMT_HH
#define BETAG4PMT_HH
#include "fstream"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include "G4VSensitiveDetector.hh"
#include "BETAG4PMTHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

/**
 * \brief Represents a simple photomultiplier tube
 */
/**
 * Represents a simple photomultiplier tube attached to a geometry that is a thin surface. 
 *
 */

class BETAG4PMT : public G4VSensitiveDetector {

public:

  // Constructor
  BETAG4PMT( G4String name);

  // Destructor
  virtual ~BETAG4PMT();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  bool countAllPhotons;
  bool savePhotonPositions;
G4LogicalVolume * fPMTLogicalVolume;

private:
  
  G4double QE(G4double photonEnergy);
  G4double lambda[15];
  G4double sensitivity[15];
  // Data members
  G4int HCID;
  BETAG4PMTHitsCollection* fHitsCollection;
  G4String  detname;
  G4int pmt;
gsl_interp * alloc;

};




#endif

