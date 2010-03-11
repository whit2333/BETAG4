// BETARun is a class for accumulating scored quantities over
// an entire run. Event data is accumulated over a run in a G4THitsMap 
// object.
//
#ifndef BETARUN_HH
#define BETARUN_HH

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4THitsMap.hh"
#include <map>
#include "BETAAnalysisManager.hh"
#include "rawBETAEvent.h"

// ROOT //
#include "Riostream.h"
#include "TROOT.h"
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
//#include "rawBETAEvent.hh"
#include "TVector3.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "G4RunManager.hh"
#include "BETAPrimaryGeneratorAction.hh"
#include "BETADetectorConstruction.hh"

// class rawBETAEvent;

//class G4Event;

class BETARun : public G4Run {

public:

  // Constructor 
  BETARun(const int runNumber,const int showThePlots);
  // Destructor
  virtual ~BETARun();

private:
  BETAAnalysisManager* analysisManager;
  G4RunManager * runManager;
  BETAPrimaryGeneratorAction * generator;
  BETADetectorConstruction * construction;

public:

  rawBETAEvent * recordedEvent;
  // Override this method in G4Run
  virtual void RecordEvent(const G4Event*);
  BETAAnalysisManager* getRunAnalysisManager() const { return analysisManager ;}
  // Dump all data
  void DumpData() const;

private:
bool catLastFile;
bool fShowUnrealisticData;
TROOT * theRoot; 
TApplication * theApp;

   TFile * RootFile;
   G4int MirrorHCID;
   G4int PMTHCID;
G4int pmtTotalCount, mirrorTotalCount;
	ofstream MCOutput ;

void DumpHallCMC();

public:

  G4double getPMT_MirrorEfficiency() {return(static_cast<G4double>(pmtTotalCount)/static_cast<G4double>(mirrorTotalCount) ); }

TCanvas * c1;


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
    	TH1F* TfakePlaneElectronEnergy;
    	TH1F* TfakePlanePositronEnergy;
    	TH1F* Tpmt1Hits;
    	TH1F* Tpmt2Hits;
    	TH1F* Tpmt3Hits;
    	TH1F* Tpmt4Hits;
    	TH1F* Tpmt5Hits;
    	TH1F* Tpmt6Hits;
    	TH1F* Tpmt7Hits;
    	TH1F* Tpmt8Hits;
	TH1F* TpmtSumHits;
	TH1F* TBIGCALTotalEnergy;
    	TH1F* ThodoscopepmtSumHits;

    	TH2F* TbigcalHits;
    	TH2F* TmirrorHits;

    	TH2F* Tmirror1PosVsPhotoelectrons;
    	TH2F* Tmirror2PosVsPhotoelectrons;
    	TH2F* Tmirror3PosVsPhotoelectrons;
    	TH2F* Tmirror4PosVsPhotoelectrons;
    	TH2F* Tmirror5PosVsPhotoelectrons;
    	TH2F* Tmirror6PosVsPhotoelectrons;
    	TH2F* Tmirror7PosVsPhotoelectrons;
    	TH2F* Tmirror8PosVsPhotoelectrons;

    	TH2F* Tpmt1PosHist;
    	TH2F* Tpmt2PosHist;
    	TH2F* Tpmt3PosHist;
    	TH2F* Tpmt4PosHist;
    	TH2F* Tpmt5PosHist;
    	TH2F* Tpmt6PosHist;
    	TH2F* Tpmt7PosHist;
    	TH2F* Tpmt8PosHist;

	TH2F* TpointSrcDeviation;
	TH2F* TFTPos;
    	TH1F* TFTcosTheta;

	TTree * testTree;
	TTree * detectorTree;
	TTree * pseudoTree;

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
  std::map<G4int, G4THitsMap<G4double>* > fMap;
};

#endif
