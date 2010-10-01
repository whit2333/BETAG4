#ifndef BETARunAction_h
#define BETARunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "BETARun.hh"
#include "BETAAnalysisManager.hh"
class BETAAnalysisManager;

#include "BETARunActionMessenger.hh"
#include "TSQLServer.h"
#include "TSQLResult.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Timer;
class G4Run;
class BETARun;
class BETARunActionMessenger;
class BETARunAction : public G4UserRunAction
{
  public:
    BETARunAction();
   ~BETARunAction();

  virtual G4Run* GenerateRun();
  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
    void showPlot(int);
  private:
	ifstream input_file ;
	ofstream output_file ;

    BETAAnalysisManager* analysisManager;
    G4Timer* timer;
    BETARun* currentRun;
    int runNumber;
    int showThePlots;
    BETARunActionMessenger* messenger;
 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*BETARunAction_h*/
