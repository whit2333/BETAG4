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

class BETASimulationManager;

/** 
 * Data acquisition readout simulation
 * 
 */
class BETAG4DAQReadout : public G4VDigitizerModule {
  public:

/**  Constructor
 */
  BETAG4DAQReadout(G4String modName);

/**  Destructor
 */
  ~BETAG4DAQReadout();

/**  Digitize
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

  virtual void Print() {
    std::cout << " Bigcal    Entries : " << fBigcalADCSumDC->entries() << " adc \n";
    std::cout << " Cherenkov Entries : " << fCherenkovADCSumDC->entries() << " adcs.\n";
  };

  void Clear(){
    Reset();
    delete fCherenkovADCSumDC;
    fCherenkovADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "cerADCSums" );
    delete fBigcalADCSumDC;
    fBigcalADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "bigcalADCSums" );
  };

  void SetTriggerEvent(InSANETriggerEvent* evt) {fTriggerEvent=evt; };

  InSANETriggerEvent * fTriggerEvent;

  Bool_t IsGoodEvent() {
    return(fIsTriggered);
  };

  private: 

  bool fCherenkovFired;
  bool fBigcalFired;

  bool fIsTriggered;

  void Reset() {
    fCherenkovTotal = 0;
    fCherenkovFired=false;
    fBigcalFired=false;
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

  G4double fBigcalTriggerThreshold;
  G4double fTriggerGroupEnergy[4];
  G4int fNumberOfTriggeredGroups;
  G4double fCherenkovTotal ;
  G4double fCherenkovTriggerThreshold;
};


#endif

