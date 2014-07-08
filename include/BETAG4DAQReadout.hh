#ifndef BETAG4DAQReadout_HH
#define BETAG4DAQReadout_HH 1

#include "globals.hh"
#include "G4RunManager.hh"
#include "G4VDigitizerModule.hh"
#include "G4VDigiCollection.hh"
#include "G4DigiManager.hh"
#include "G4VDigi.hh"
#include "G4SDManager.hh"
#include "InSANETriggerEvent.h"
#include "BIGCALGeometryCalculator.h"
#include "BETAG4DigiADC.hh"
#include "BETASimulationManager.hh"
#include "BETAG4PMTArray.hh"
#include "BETAG4BigcalHit.hh"
#include "BETAG4PMTHit.hh"
#include "BETAFakePlaneHit.hh"
#include "BETAG4MonteCarloEvent.h"

class BETASimulationManager;

/** Data acquisition readout simulation
 *
 * \ingroup Detectors
 */
class BETAG4DAQReadout : public G4VDigitizerModule {

   private: 

      bool fCherenkovFired;
      bool fBigcalFired;
      bool fIsTriggered;

      BETAG4DigiADCCollection * fCherenkovADCSumDC;
      G4int                   * fCherenkovADCSumDCID;

      BETAG4DigiADCCollection * fBigcalADCSumDC;
      G4int                   * fBigcalADCSumDCID;

      BETASimulationManager   * fSimulationManager;

      G4int                     fCherenkovHCID;
      G4int                     fBigcalHCID;
      G4int                     fBigcalFakePlaneHCID;
      G4int                     fTrackerFakePlaneHCID;
      G4int                     fTrackerFakePlane2HCID;

      G4double                  fBigcalTriggerThreshold;       // TS threshold 
      G4double                  fBigcalBlockReadoutThreshold;  // ADC threshold for readout
      G4double                  fCherenkovTriggerThreshold;    // TS threshold

      G4double                  fTriggerGroupEnergy[4];
      G4double                  fCherenkovTotal ;
      G4int                     fNumberOfTriggeredGroups;

   public:
      BETAG4DAQReadout(G4String modName);

      virtual ~BETAG4DAQReadout();

      void Digitize();

      void ReadOut();

      virtual void Print();

      void Clear();


      void Reset();

      InSANETriggerEvent    * fTriggerEvent;
      BETAG4MonteCarloEvent * mcEvent;
      TClonesArray          * fBCPlaneHits;
      TClonesArray          * fFTPlaneHits;
      TClonesArray          * fFT2PlaneHits;

      BETAG4BigcalHitsCollection  * fBigcalHC;
      BETAG4PMTHitsCollection     * fGasCherenkovHC;
      BETAFakePlaneHitsCollection * fTrackerFakePlaneHC;
      BETAFakePlaneHitsCollection * fTrackerFakePlane2HC;
      BETAFakePlaneHitsCollection * fBigcalFakePlaneHC;

      G4int fNBigcalHits;
      G4int fNCherenkovHits;

      void SetTriggerEvent(InSANETriggerEvent* evt) {fTriggerEvent=evt; };
      Bool_t IsGoodEvent() { return(fIsTriggered); }

};


#endif

