#ifndef BETAG4DAQReadout_HH
#define BETAG4DAQReadout_HH 1

#include "globals.hh"
#include "G4VDigitizerModule.hh"
#include "G4VDigiCollection.hh"
#include "G4DigiManager.hh"
#include "G4VDigi.hh"
#include "BETAG4DigiADC.hh"
#include "InSANETriggerEvent.h"
#include "BETASimulationManager.hh"
#include "BETAG4PMTArray.hh"
#include "BETAG4BigcalHit.hh"
#include "BETAG4PMTHit.hh"
#include "BETAFakePlaneHit.hh"
#include "BETAG4MonteCarloEvent.h"

class BETASimulationManager;

/** 
 * Data acquisition readout simulation
 *
 * \ingroup Detectors
 */
class BETAG4DAQReadout : public G4VDigitizerModule {
  public:

/**  Constructor
 */
  BETAG4DAQReadout(G4String modName);

/**  Destructor
 */
  ~BETAG4DAQReadout();

/**  Digitize simulated trigger
 * For each event this is called
 * First it looks at the sum of bigcal's trigger groups to see if it is
 * above the energy threshold.
 * 
 * If it is above the energy threshold, it proceeds to fill out the fTriggerEvent
 * with the appropriate values. 
 * 
 * \todo figure out which event types trump the others?
 */
  void Digitize();
  
/** ReadOut() records the MC event thrown and scoring planes
 *  
 */
  void ReadOut();

  virtual void Print() {
    std::cout << "++ DAQ Readout ++\n";
    std::cout << "  Bigcal     (" << fBigcalHCID << "): " << fNBigcalHits << " hits , " << fBigcalHC->entries() << " entries.\n";
    std::cout << "  Cherenkov  (" << fCherenkovHCID << "): " << fNCherenkovHits << " hits, " << fGasCherenkovHC->entries() << " entries.\n";
  };

  void Clear(){
    Reset();
    
    delete fCherenkovADCSumDC;
    fCherenkovADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "cerADCSums" );
    delete fBigcalADCSumDC;
    fBigcalADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "bigcalADCSums" );
  }

  void SetTriggerEvent(InSANETriggerEvent* evt) {fTriggerEvent=evt; };

  InSANETriggerEvent * fTriggerEvent;
  
  
  BETAG4MonteCarloEvent * mcEvent;
  TClonesArray * fBCPlaneHits;
  TClonesArray * fFTPlaneHits;
  
  Bool_t IsGoodEvent() {
    return(fIsTriggered);
  };

  BETAG4BigcalHitsCollection * fBigcalHC;
  BETAG4PMTHitsCollection * fGasCherenkovHC;
  BETAFakePlaneHitsCollection * fTrackerFakePlaneHC;
  BETAFakePlaneHitsCollection * fBigcalFakePlaneHC;

  G4int fNBigcalHits;
  G4int fNCherenkovHits;
  
  private: 

  bool fCherenkovFired;
  bool fBigcalFired;

  bool fIsTriggered;

  void Reset() {
    fCherenkovTotal = 0;
    fCherenkovFired=false;
    fBigcalFired=false;
    fNCherenkovHits=0;
    fNumberOfTriggeredGroups=0;
    fIsTriggered=false;
    for(int i = 1;i<4;i++) fTriggerGroupEnergy[i]=0.0;
  };

  BETAG4DigiADCCollection * fCherenkovADCSumDC;
  G4int * fCherenkovADCSumDCID;

  BETAG4DigiADCCollection * fBigcalADCSumDC;
  G4int * fBigcalADCSumDCID;

  BETASimulationManager * fSimulationManager;

  G4int fCherenkovHCID;
  G4int fBigcalHCID;
  G4int fBigcalFakePlaneHCID;
  G4int fTrackerFakePlaneHCID;

  G4double fBigcalTriggerThreshold;
  G4double fTriggerGroupEnergy[4];
  G4int fNumberOfTriggeredGroups;
  G4double fCherenkovTotal ;
  G4double fCherenkovTriggerThreshold;
};


#endif

