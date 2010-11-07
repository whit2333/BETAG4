#ifndef BETASimulationMessenger_h
#define BETASimulationMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "BETASimulationManager.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "BETADetectorMessenger.hh"
#include "BETAPhysicsListMessenger.hh"
#include "BETAPrimaryGeneratorMessenger.hh"
#include "BETARunActionMessenger.hh"
#include "BETAEventActionMessenger.hh"

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class BETASimulationManager;

/**
 * Concrete implementation of G4UImessenger which creates/connects geant4 command lines to 
 * appropriate data members or methods. \todo{Consildate the messengers. Should we have one 
 * messenger per class or just on messenger?}
 */
class BETASimulationMessenger: public G4UImessenger
{
  public:
    BETASimulationMessenger(BETASimulationManager* );
   ~BETASimulationMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
//    BETAAnalysisAction* BETAAction;
    BETASimulationManager* manager;
    G4UIdirectory*               analysis; 
    G4UIcmdWithoutParameter*   writeTree;
    G4UIcmdWithAnInteger*	showPlot;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

