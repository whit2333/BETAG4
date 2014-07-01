#ifndef BETAG4PMTArray_HH
#define BETAG4PMTArray_HH 1

#include "fstream"

#include "G4VSensitiveDetector.hh"
#include "BETAG4PMTHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;


/** A concrete G4VSensitiveDetector class for an array of photomultiplier tubes
 * 
 *  The number of Channels should be set only once.
 *
 * \ingroup Detectors
 */
class BETAG4PMTArray : public G4VSensitiveDetector {

   private:

      G4double lambda[15];
      G4double sensitivity[15];

   protected:

      G4String                   fCollectionName;
      G4int                      fHCID;
      BETAG4PMTHitsCollection  * fHitsCollection;


   public:

      BETAG4PMTArray( G4String name, G4int Nchan = 12, G4String colname = "pmt" );
      virtual ~BETAG4PMTArray();

      virtual void SetNumberOfChannels(int n) { fNumberOfChannels = n; };
      virtual int  GetNumberOfChannels() const { return(fNumberOfChannels); };

      virtual void    Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
      virtual G4bool  ProcessHits(G4Step* aStep,G4TouchableHistory* history);
      virtual void    EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);

      bool   fCountAllPhotons;
      bool   fSavePhotonPositions;
      int    fNumberOfChannels;
      int    fDiscriminatorThreshold;

      //G4LogicalVolume * fPMTLogicalVolume;

   private:

      G4double QE(G4double photonEnergy);

};

#endif

