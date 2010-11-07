#ifndef BETARUN_HH
#define BETARUN_HH

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4THitsMap.hh"
#include <map>
#include "BETASimulationManager.hh"
#include "BETAEvent.h"
#include "BETAMirrorHit.hh"
#include "InSANERun.h"
// ROOT //
#include "Riostream.h"
#include "TFile.h"
#include "TNetFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include "TH1F.h"
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRint.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "G4RunManager.hh"

#include "BETAPrimaryGeneratorAction.hh"
#include "BETADetectorConstruction.hh"
#include "BETAFrontTrackerHit.hh"
#include "BETAProtvinoCalorimeterHit.hh"
#include "BETARCSCalorimeterHit.hh"
#include "BETAHodoscopePMTHit.hh"
#include "BETAPMTHit.hh"
#include "BETAFakePlaneHit.hh"

#include "HallCBeamEvent.h"
#include "HMSEvent.h"
#include "BETAEvent.h"
#include "BETAG4MonteCarloEvent.h"
#include "GasCherenkovEvent.h"
#include "BigcalEvent.h"
#include "GasCherenkovHit.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include "BETASimulationManager.hh"
#include "BETAEvent.h"
#include "BETAMirrorHit.hh"
#include "BETAG4EventRecorder.hh"
class BETASimulationManager;
class BETADetectorConstruction;
class BETAG4EventRecorder;

/**
 * \ingroup Runs
 */

/**
 * \brief Concrete class implementation of G4Run 
 */
/**
 * Concrete class implementation of G4Run. 
 * Here is where we select the data we wish to record.
 * The important s here is RecordEvent which is called at then
 * end of each event.
 */
class BETARun : public G4Run {

public:

/**
 *  Constructor opens ROOT file \todo{ add open new file or append to existing flag...}.)
 */
  BETARun(const int runNumber,const int showThePlots);
/**
 *  Destructor
 */
  ~BETARun();

/**
 * Used to fill out event classes which are trees and branches in the output file
 */
BETAG4EventRecorder * eventRecorder;


private:

  BETASimulationManager * analysisManager;
  G4RunManager * runManager;
  BETAPrimaryGeneratorAction * generator;
  BETADetectorConstruction * construction;

public:

/// Contains all BETA detector's data
  BETAEvent * betaEvent;
/// Contains all the HMS data
  HMSEvent* hmsEvent;
/// Contains all the Hall C beam data
  HallCBeamEvent * beamEvent;
/// Contains all the Thrown and unrealistically obtained Montecarlo data
  BETAG4MonteCarloEvent * mcEvent;
/// Simulation Run object
  InSANERun * simulationRun;


/// Override this method in G4Run
 void RecordEvent(const G4Event*);
/// Returns the Analysis manager singleton
 BETASimulationManager* GetRunAnalysisManager() const { return analysisManager ;}

  // Dump all data
  void DumpData() const;

private:

 TH1F * waveforms;



/**
 *
 */
   BETAHodoscopePMTHitsCollection * hodoscopepmtHC;
   BETAPMTHitsCollection * pmtHC;
   BETARCSCalorimeterHitsCollection * BIGCALHC;
   BETAProtvinoCalorimeterHitsCollection * BIGCALHC2;
   BETAFrontTrackerHitsCollection * FTHC;
   BETAMirrorHitsCollection * mirrorHC ;
   BETAFakePlaneHitsCollection * fakePlaneHC;



bool catLastFile;
bool fShowUnrealisticData;
// TROOT * theRoot; 
// TApplication * theApp;

   TFile * RootFile;


   G4int pmtTotalCount, mirrorTotalCount;
	ofstream MCOutput ;

void DumpHallCMC();

public:

  G4double getPMT_MirrorEfficiency() {return(static_cast<G4double>(pmtTotalCount)/static_cast<G4double>(mirrorTotalCount) ); }

TCanvas * c1;

/**
 * Counters and IDs
 */
private:
int cer_tdc_thresh;
    G4int  CherenkovPMTCount[20];
    G4int pmt1Count;
    G4int pmt2Count;
    G4int pmt3Count;
    G4int pmt4Count;
    G4int pmt5Count;
    G4int pmt6Count;
    G4int pmt7Count;
    G4int pmt8Count;
    G4int mirror1Count;
    G4int mirror2Count;
    G4int mirror3Count;
    G4int mirror4Count;
    G4int mirror5Count;
    G4int mirror6Count;
    G4int mirror7Count;
    G4int mirror8Count;
    G4int hodoscopePMTHCID;
    G4int hodoscopePMTcount;
    G4double BCTE;
    G4int fakePlaneID;

    G4int HHC1ID;
    G4int HHC2ID;
    G4int DHC1ID;
    G4int DHC2ID;
    G4int ECHCID;
    G4int HCHCID;
    G4int numberOfPrim;
    G4int BIGCALID;
    G4int BIGCALID2;
    G4int FTID;
    G4int fakePlaneEventNumber;
    G4int MirrorHCID;
    G4int PMTHCID;



	TTree * testTree;
	TTree * detectorTree;
	TTree * pseudoTree;
    bool triggered;
    int Trigger;
    Double_t EDeposited;
    Int_t FrontTrackerCellNumber;

    Double_t * BigCalEDepTop;
    Double_t * BigCalEDepBottom;
    Double_t * PhotoElectrons;

   typedef struct {
       Double_t Energy,
		Xpos, 
		Ypos, 
		Px, 
		Py, 
		Pz;
          Int_t Type; /// 1=electron 2=positron, 3=pi+, 4=pi-, 5=gamma... should do this another way???
	  Int_t NumberOfHits;
		} fakeDetectorData;
fakeDetectorData fakePlaneData;

Int_t eCountsThisEvent;

   typedef struct {Double_t one,two,three,four,five,six,seven,eight,total;} PE;
PE pmtPE;

   typedef struct {Double_t x, y , energy;} BIGCALEdep;
BIGCALEdep BIGCALEdepPos;

   typedef struct {Int_t cellNumber, a, b ,c;} ForwardTrackerEvent;
ForwardTrackerEvent ForwardTrackerAveragePos;
 // Define some simple structures

   typedef struct {Float_t x,y,z;} POINT;
   typedef struct {
      Int_t ntrack,nseg,nvertex;
      UInt_t flag;
      Float_t temperature;
   } EVENTN;

  // Helper function
  void Print(const std::vector<G4String>& title,
	     const std::map< G4int, std::vector<G4double> >&out) const;  

  // Data member
//  std::map<G4int, G4THitsMap<G4double>* > fMap;
};



#endif



