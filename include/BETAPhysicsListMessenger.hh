#ifndef BETAPhysicsListMessenger_h
#define BETAPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class BETAPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAnInteger;


class BETAPhysicsListMessenger: public G4UImessenger
{
  public:  
    BETAPhysicsListMessenger(BETAPhysicsList* );
   ~BETAPhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:  
    BETAPhysicsList*     pPhysicsList;
    
    G4UIdirectory*        BETADir;
    G4UIdirectory*        physDir;
    G4UIcmdWithAnInteger* verboseCmd;
    G4UIcmdWithAnInteger* cerenkovCmd;
};


#endif

