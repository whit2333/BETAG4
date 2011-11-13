#ifndef BETAG4EventRecorder_HH
#define BETAG4EventRecorder_HH
// ROOT //
#include "Riostream.h"
#include "TROOT.h"
#include "G4Event.hh"

#include "BETAFrontTrackerHit.hh"
#include "BETAProtvinoCalorimeterHit.hh"
#include "BETARCSCalorimeterHit.hh"
#include "BETAHodoscopePMTHit.hh"
#include "BETAPMTHit.hh"
#include "BETAFakePlaneHit.hh"
#include "BETAProtvinoCalorimeterHit.hh"
#include "BETARCSCalorimeterHit.hh"
#include "HallCBeamEvent.h"
#include "HMSEvent.h"
#include "BETAEvent.h"
#include "BETAG4MonteCarloEvent.h"
#include "GasCherenkovEvent.h"
#include "BigcalEvent.h"
#include "InSANETriggerEvent.h"
#include "GasCherenkovHit.h"

#include <iostream>
#include <algorithm>
#include "TH1F.h"
#include <vector>
#include "BETASimulationManager.hh"
#include "SANEEvents.h"
#include "BETAMirrorHit.hh"
#include "BETAPrimaryGeneratorAction.hh"
#include "BETADetectorConstruction.hh"
#include "InSANEFakePlaneHit.h"
#include "BigcalFakePlaneHit.h"

class BETASimulationManager;
class BETADetectorConstruction;


/** Constructs the data to be recorded from the GEANT4
 *
 * Constructs the recorded data from the GEANT4 G4Event derived classes
 * This class defines the data which *should* look like the real data.
 *
 * \todo{ Add multihit TDC type hits.}
 * \todo{ Use ABC G4Digi to emulate TDC? }
 * 
 * 
 * \ingroup Simulation
 * \ingroup Event
 */
class BETAG4EventRecorder {
public : 
/**
 * Default Constructor initializes counters, arrays, pointers, etc....
 */
BETAG4EventRecorder();

/**
 * Constructor initializes counters, arrays, pointers, etc....
 * Also sets the address of the detector events
 */
BETAG4EventRecorder(SANEEvents *);

/**
 * dtor
 */
~BETAG4EventRecorder();


/**
 * Sets the address of the G4THitsCollection derived class 
 * (BETAPMTHitsCollection in this case) to use for filling
 * out the event data.
 */
inline int SetGasCherenkovHitCollection( BETAPMTHitsCollection * aHC) {
  if(aHC) 
  {
    fGasCherenkovHC = aHC;
    return(0);
  } else return(-1) ; };

/**
 * Sets the addresses (requires two!) of the G4THitsCollection derived classes (BETARCSCalorimeterHit and BETAProtvinoCalorimeterHit in this case) 
 * to use for filling
 * out the event data.
 */
inline int SetBigcalHitCollections
(BETARCSCalorimeterHitsCollection * aHC, BETAProtvinoCalorimeterHitsCollection * aHC2) {
if(aHC && aHC2) { fRCSCalorimeterHC = aHC; fProtvinoCalorimeterHC = aHC2; return(0); } else return(-1); };

/**
 * Clears the InSANEDetectorEvent classes
 */
inline void ClearEvent(){
  ;
}

/**
 * Clears the G4THitsCollection classes
 */
inline void ClearHitsCollections() 
  {
  }


/**
 * Fills the InSANEDetectorEvent classes
 */
int FillGasCherenkovEvent(int run,int event);

/**
 * Fills the fBigcalEvent of betaEvent
 */
int FillBigcalEvent(int run,int event);

/**
 * Sets the address of the G4THitsCollection derived class 
 * (BETAPMTHitsCollection in this case) to use for filling
 * out the event data.
 */
inline int SetLuciteHodoscopeHitCollection(BETAHodoscopePMTHitsCollection * aHC) { if(aHC) { fLuciteHodoscopeHC = aHC; return(0); } else return(-1) ; };

/**
 * Fills the fBigcalEvent of betaEvent
 */
int FillBigcalFakePlaneEvent(int run,int event);

/**
 * Sets the address of the G4THitsCollection derived class 
 * (BETAPMTHitsCollection in this case) to use for filling
 * out the event data.
 */
inline int SetBigcalFakePlaneHitCollection(BETAFakePlaneHitsCollection * aHC) { if(aHC) { fFakePlaneHC = aHC; return(0); } else return(-1) ; };
/**
 * Fills the fBigcalEvent of betaEvent
 */
int FillLuciteHodoscopeEvent(int run,int event);

/**
 * Sets the address of the G4THitsCollection derived class 
 * (BETAPMTHitsCollection in this case) to use for filling
 * out the event data.
 */
inline int SetForwardTrackerHitCollection(BETAFrontTrackerHitsCollection * aHC) { if(aHC) { fForwardTrackerHC = aHC; return(0); } else return(-1) ; };

/**
 * Fills the fBigcalEvent of betaEvent
 */
int FillForwardTrackerEvent(int run,int event);

/**
 * Fills fTriggerEvent
 */
int FillTriggerEvent(InSANETriggerEvent * trig);


  TH1F * waveforms;
  G4RunManager * runManager;

  BETASimulationManager * analysisManager ;
  BETAPrimaryGeneratorAction * generator;
  BETADetectorConstruction * construction;


private :

  HMSEvent* fHMSEvent;
  HallCBeamEvent* fHallCBeamEvent;
  BETAG4MonteCarloEvent * fBETAG4MonteCarloEvent;
  BETAEvent* fBETAEvent;
    GasCherenkovEvent* fGasCherenkovEvent;
    BigcalEvent* fBigcalEvent;
    LuciteHodoscopeEvent* fLuciteHodoscopeEvent;
    ForwardTrackerEvent* fForwardTrackerEvent;
    BigcalFakePlaneEvent * fBigcalFakePlaneEvent;
  InSANETriggerEvent * fTriggerEvent;

  BETAPMTHitsCollection * fGasCherenkovHC;
  BETARCSCalorimeterHitsCollection * fRCSCalorimeterHC;
  BETAProtvinoCalorimeterHitsCollection * fProtvinoCalorimeterHC;
  BETAHodoscopePMTHitsCollection * fLuciteHodoscopeHC;
  BETAFrontTrackerHitsCollection * fForwardTrackerHC;
  BETAFakePlaneHitsCollection * fFakePlaneHC;

  BETASimulationManager * fSimulationManager;

// counters
    G4int  CherenkovPMTCount[20];

// iterators and ints (as to not wast time allocating them
int i1;

};

#endif
