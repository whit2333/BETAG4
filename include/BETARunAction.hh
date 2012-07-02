#ifndef BETARunAction_h
#define BETARunAction_h 1

#include "BETARunActionMessenger.hh"
#include "TSQLServer.h"
#include "TSQLResult.h"
#include "globals.hh"
#include "G4UserRunAction.hh"
#include "BETARun.hh"
#include "BETASimulationManager.hh"

#include "TFile.h"
#include "TTree.h"

class BETASimulationManager;
class G4Timer;
class G4Run;
class BETARun;
class BETARunActionMessenger;
class BETARunAction : public G4UserRunAction
{
  public:
/**
 *
 *  Run related operations
 *    database entry
 *    run number incrementing --> run.txt file ... should just get it from a database
 *
 */
    BETARunAction();
/**
 *
 *
 */
   ~BETARunAction();

/**
 *  Invoked at beamOn
 *  Creates output ROOT file, allocates event and hit output trees
 */
  G4Run* GenerateRun();

/**
 *This method is invoked before entering the event loop. A typical use of this method would be to initialize and/or book histograms for a particular run. This method is invoked after the calculation of the physics tables. 
 *
 */
    void BeginOfRunAction(const G4Run*);

/**
 *     This method is invoked at the very end of the run processing. It is typically used for a simple analysis of the processed run. 
 *
 */
    void EndOfRunAction(const G4Run*);

/**
 *
 *
 */
    void showPlot(int);

  private:
	std::ifstream input_file ;
	std::ofstream output_file ;

    BETASimulationManager* fSimulationManager;
    G4Timer* timer;
    BETARun* fCurrentRun;
    int fRunNumber;
    int showThePlots;
    BETARunActionMessenger* messenger;
 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*BETARunAction_h*/
