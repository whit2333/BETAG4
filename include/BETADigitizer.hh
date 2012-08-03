#ifndef BETADigitizer_HH
#define BETADigitizer_HH 1

#include "globals.hh"
#include "G4VDigitizerModule.hh"
#include "G4VDigiCollection.hh"
#include "G4DigiManager.hh"
#include "G4VDigi.hh"
#include "BETAG4DigiADC.hh"
#include "BETAG4DigiTDC.hh"
#include "InSANETriggerEvent.h"
#include "BETAG4PMTArray.hh"
#include "BETAG4BigcalHit.hh"
#include "BETAG4PMTHit.hh"
#include "BETAEvent.h"
#include "BETASimulationManager.hh"
#include "TRandom.h"
#include "BETAFakePlaneHit.hh"
#include "BETAForwardTrackerHit.hh"
#include "BETAProtvinoCalorimeterHit.hh"
#include "BETARCSCalorimeterHit.hh"
#include "BETAHodoscopePMTHit.hh"
#include "BETAG4PMTHit.hh"

/**  Digitizer module for BETA.
 *
 *  - Digitize() is called from BETARun::RecordEvent() if the event triggered
 *    the BETAG4DAQReadout. This method looks at the detector hits collections
 *    then proceeds to fill the digi collections.
 *  - Next ReadOut() is called which translates the digi collections to
 *    the InSANEDetectorEvent derived classes for ROOT file output.
 *
 * \ingroup Detectors
 */
class BETADigitizer : public G4VDigitizerModule {
public:

   BETADigitizer(G4String modName);
   ~BETADigitizer();

   /** Simulate pedestal event.
    *
    *  Here we "gate" all adcs by making a digi for every channel
    *  The digi has nominal zero. 
    *  
    *  \todo Add pedestal random number for width... 
    *  possibly correlate with real rates (configuration)
    */
   void DigitizePedestals();

   /** Loop over Detector Hit Collections and creates the ADC and TDC Digi collections.
    */
   void Digitize();

   /** Loops over the Digi Collection and creates the InSANEDetectorHits and fills out 
    *  InSANEDetectorEvent data. 
    *  These are the event structures which are emulating real data.
    */
   void ReadOut();

   /** Clears Digi collections
    */
   void Clear();

   virtual void Print();

   void SetTriggerEvent(InSANETriggerEvent* evt) { fTriggerEvent=evt; };

   InSANETriggerEvent * fTriggerEvent;

   BETAEvent * fBetaEvent;

private: 

   bool fIsTriggered;

   void Reset() { fIsTriggered=false;};

   BETAG4DigiADCCollection * fCherenkovADCDC;
   G4int fCherenkovADCDCID;

   BETAG4DigiTDCCollection * fCherenkovTDCDC;
   G4int fCherenkovTDCDCID;

   BETAG4DigiADCCollection * fBigcalADCDC;
   G4int fBigcalADCDCID;

   BETAG4DigiTDCCollection * fBigcalTDCDC;
   G4int fBigcalTDCDCID;

   BETAG4DigiADCCollection * fHodoscopeADCDC;
   G4int fHodoscopeADCDCID;

   BETAG4DigiTDCCollection * fHodoscopeTDCDC;
   G4int fHodoscopeTDCDCID;
  
   BETAG4DigiADCCollection * fTrackerADCDC;
   G4int fTrackerADCDCID;

   BETAG4DigiTDCCollection * fTrackerTDCDC;
   G4int fTrackerTDCDCID;

   BETASimulationManager * fSimulationManager;

   G4int fCherenkovHCID;
   G4int fBigcalHCID;
   G4int fTrackerHCID;
   G4int fHodoscopeHCID;

   TRandom * fRandomNumberGenerator;

   int * fTrackerPhotonCounts;
   int * fTrackerTimings;

private:
   // Geant4 collections, hits, etc....
   BETAG4BigcalHitsCollection * fBigcalHC;
   BETAG4PMTHitsCollection * fGasCherenkovHC;
   BETAHodoscopePMTHitsCollection * fLuciteHodoscopeHC;
   BETAForwardTrackerHitsCollection * fForwardTrackerHC;

   G4double fBigcalChannelThreshold;

};


#endif

