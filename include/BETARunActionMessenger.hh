#ifndef BETARunActionMessenger_h
#define BETARunActionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "BETARunAction.hh"
//class BETAPrimaAction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class BETARunAction;
#include "G4UIcmdWithAnInteger.hh"


//____________________________________________________
class BETARunActionMessenger: public G4UImessenger
{
  public:
    BETARunActionMessenger( BETARunAction* );
   ~BETARunActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    BETARunAction * runAction;
    G4UIdirectory*               aidaDir; 
    G4UIcmdWithoutParameter*   writeTree;
    G4UIcmdWithAnInteger*	showPlot;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

