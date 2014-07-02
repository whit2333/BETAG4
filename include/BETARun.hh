#ifndef BETARUN_HH
#define BETARUN_HH 1

#include <map>
//#include <iostream>
#include <algorithm>
#include <vector>

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4THitsMap.hh"

#include "BETASimulationManager.hh"
#include "BETAEvent.h"
#include "BETAMirrorHit.hh"
#include "InSANERun.h"

#include "TFile.h"
//#include "TNetFile.h"
#include "TRandom.h"
#include "TTree.h"
//#include "TBranch.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
//#include "TH1F.h"
//#include "TObjArray.h"
//#include "TH1.h"
//#include "TH2.h"
//#include "TProfile.h"
#include "TRint.h"
#include "TVector3.h"
//#include "TCanvas.h"
//#include "TApplication.h"
#include "G4RunManager.hh"
#include "BETAPrimaryGeneratorAction.hh"
#include "BETADetectorConstruction.hh"

#include "HallCBeamEvent.h"
#include "HMSEvent.h"
#include "BETAEvent.h"
#include "BETAG4MonteCarloEvent.h"
#include "GasCherenkovEvent.h"
#include "BigcalEvent.h"
#include "GasCherenkovHit.h"

#include "BETASimulationManager.hh"
#include "BETAG4DAQReadout.hh"
#include "BETADigitizer.hh"

#include "BETAHodoscopePMTHit.hh"
#include "BETAForwardTrackerHit.hh"
#include "BETAFakePlaneHit.hh"
#include "BETAG4BigcalSD.hh"
#include "BETAG4BigcalHit.hh"
#include "BETAEvent.h"
#include "BETAMirrorHit.hh"

class BETASimulationManager;
class BETAG4DAQReadout;
class BETADigitizer;
class BETADetectorConstruction;


/** \brief Concrete class implementation of G4Run 
 * 
 * Concrete class implementation of G4Run. 
 * Here is where we select the data we wish to record.
 * The important s here is RecordEvent which is called at then
 * end of each event.
 *
 * \ingroup Runs
 */
class BETARun : public G4Run {

   private:

      bool catLastFile;
      bool fShowUnrealisticData;

      std::ofstream MCOutput ;

   protected:

      BETAG4DAQReadout           * fDAQReadout;
      BETASimulationManager      * fSimulationManager;
      G4RunManager               * runManager;
      BETAPrimaryGeneratorAction * generator;
      BETADetectorConstruction   * construction;

      int   fNUpdate;

   public:

      BETADigitizer         * fBETADigitizer;
      InSANERun             * simulationRun;
      BETAG4MonteCarloEvent * mcEvent;

   public:

      /**  Constructor opens ROOT file.  */
      BETARun(const int runNumber);
      virtual ~BETARun();

      /** Generates 1000 pedestal events before running /run/beamOn */
      void GeneratePedestals();

      /**  G4Run virtual method for recording all sensitive detector data.
       *   Makes use of BETAG4DAQReadout and BETADigitizer classes
       */
      void RecordEvent(const G4Event*);

      void DumpData() const;

      void DumpHallCMC();

   public:

      TCanvas * c1;
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

      //typedef struct {Double_t one,two,three,four,five,six,seven,eight,total;} PE;
      //PE pmtPE;

      //typedef struct {Double_t x, y , energy;} BIGCALEdep;
      //BIGCALEdep BIGCALEdepPos;

      //typedef struct {Int_t cellNumber, a, b ,c;} ForwardTrackerEvent;
      //ForwardTrackerEvent ForwardTrackerAveragePos;
      // Define some simple structures

      //typedef struct {Float_t x,y,z;} POINT;
      //typedef struct {
      //   Int_t ntrack,nseg,nvertex;
      //   UInt_t flag;
      //   Float_t temperature;
      //} EVENTN;

      // Helper function
      void Print(const std::vector<G4String>& title,
            const std::map< G4int, std::vector<G4double> >&out) const;  

      // Data member
      //  std::map<G4int, G4THitsMap<G4double>* > fMap;
};

#endif



