#ifndef BETAEventActionMessenger_h
#define BETAEventActionMessenger_h 1

class BETAEventAction;
class G4UIcmdWithAnInteger;

#include "G4UImessenger.hh"
#include "globals.hh"

class BETAEventActionMessenger: public G4UImessenger
{
  public:
    BETAEventActionMessenger(BETAEventAction* mpga);
    ~BETAEventActionMessenger();

  public:
    void SetNewValue(G4UIcommand * command,G4String newValues);
    G4String GetCurrentValue(G4UIcommand * command);
    void setIsotropic(int);
  private:
    BETAEventAction* target;

  private: //commands
 G4int  isotropic;
    G4UIcmdWithAnInteger*  verboseCmd;

};

#endif


