#ifndef BETAAnalysisMessenger_h
#define BETAAnalysisMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "BETAAnalysisManager.hh"
//class BETAPrimaAction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class BETAAnalysisManager;
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/**
 * Concrete implementation of G4UImessenger which creates/connects geant4 command lines to 
 * appropriate data members or methods. \todo{Consildate the messengers. Should we have one 
 * messenger per class or just on messenger?}
 */
class BETAAnalysisMessenger: public G4UImessenger
{
  public:
    BETAAnalysisMessenger(BETAAnalysisManager* );
   ~BETAAnalysisMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
//    BETAAnalysisAction* BETAAction;
    BETAAnalysisManager* manager;
    G4UIdirectory*               analysis; 
    G4UIcmdWithoutParameter*   writeTree;
    G4UIcmdWithAnInteger*	showPlot;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

