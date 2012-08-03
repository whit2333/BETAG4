#ifndef BETAForwardTracker_HH
#define BETAForwardTracker_HH
#include "BETAForwardTracker.hh"
#include "G4VSensitiveDetector.hh"
#include "BETAForwardTrackerHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

/**  Forward tracker sensitive detector.
 *   The detector consists of (from front to back)
 *   - 72 X1 scintillators
 *   - 132 Y1 scintillators 
 *   - 132 Y2 scintillators 
 *   - A total of 336 scintillators.
 *
 * \ingroup Detectors
 */
class BETAForwardTracker : public G4VSensitiveDetector {
public:
   
   /**
    */
   BETAForwardTracker( G4String name);
   virtual ~BETAForwardTracker();

   /** Initializes the tracker hit collection. 
    *  There is a single hits collection for the whole detector containing a hit
    *  for each detected photon.   
    */
   virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);

   /** Fills the hits collection and kills the photon track when it reaches the end 
    *  of the scintillator.  
    */ 
   virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

   virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
 
   int   GetScintLayer(int number){
      if(number < 72 && number >= 0) return (0);
      else if(number >= 72 && number < 72 + 132) return (1);
      else if(number >= 72 + 132 && number < 72 + 2*132) return (2);
      else return(-1);
   }

private:
   G4int HCID;
   BETAForwardTrackerHitsCollection* fHitsCollection;
   G4String  detname;
   G4int pmt;
};

#endif

