#ifndef BETARunAction_h
#define BETARunAction_h 1

#include "BETARunActionMessenger.hh"
#include "TSQLServer.h"
#include "TSQLResult.h"
#include "globals.hh"
#include "G4UserRunAction.hh"
#include "BETARun.hh"
#include "BETASimulationManager.hh"




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
 *This method is invoked at the beginning of BeamOn. Because the user can inherit the class G4Run and create his/her own concrete class to store some information about the run, the GenerateRun() method is the place to instantiate such an object. 
 * It is also the ideal place to set variables which affect the physics table (such as production thresholds) for a particular run, because GenerateRun() is invoked before the calculation of the physics table. 
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
	ifstream input_file ;
	ofstream output_file ;



    BETASimulationManager* fSimulationManager;
    G4Timer* timer;
    BETARun* fCurrentRun;
    int fRunNumber;
    int showThePlots;
    BETARunActionMessenger* messenger;
 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*BETARunAction_h*/
