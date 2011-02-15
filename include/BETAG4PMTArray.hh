#ifndef BETAG4PMTArray_HH
#define BETAG4PMTArray_HH
#include "fstream"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include "G4VSensitiveDetector.hh"
#include "BETAG4PMTHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;


/** A concrete G4VSensitiveDetector class for an array of photomultiplier tubes
 * 
 *  The number of Channels should be set only once.
 * 
 */
class BETAG4PMTArray : public G4VSensitiveDetector {

public:
  BETAG4PMTArray( G4String name);

  virtual ~BETAG4PMTArray();

  virtual void SetNumberOfChannels(int n) { fNumberOfChannels = n; };
  virtual int GetNumberOfChannels() { return(fNumberOfChannels); };

  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  bool   fCountAllPhotons;
  bool   fSavePhotonPositions;
  int    fNumberOfChannels;
  int    fDiscriminatorThreshold;

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

