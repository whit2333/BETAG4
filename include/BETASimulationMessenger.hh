#ifndef BETASimulationMessenger_h
#define BETASimulationMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

#include "BETAPhysicsListMessenger.hh"
#include "BETAPrimaryGeneratorMessenger.hh"
#include "BETARunActionMessenger.hh"
#include "BETAEventActionMessenger.hh"
#include "BETADetectorConstruction.hh"
#include "BETASimulationManager.hh"

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class BETASimulationManager;

/**
 * Concrete implementation of G4UImessenger which creates/connects geant4 command lines to 
 * appropriate data members or methods. \todo{Consildate the messengers. Should we have one 
 * messenger per class or just on messenger?}
 */
class BETASimulationMessenger: public G4UImessenger {
   private:
      BETADetectorConstruction * fDetConstruction;
      BETASimulationManager    * fSimManager;

      G4UIdirectory            * fDir_analysis;
      G4UIdirectory            * fDir_simulation;
      G4UIdirectory            * fDir_beta;
      G4UIdirectory            * fDir_fieldDir;

      G4UIcmdWithAString       * fCmd_simType;

      G4UIcmdWithoutParameter  * fCmd_polSwitch;
      G4UIcmdWithAString       * fCmd_lookAtField;

      G4UIcmdWithADouble       * fCmd_setBeamEnergy;

      G4UIcmdWithADouble       * fCmd_polSet;
      G4UIcmdWithADouble       * fCmd_rotateToroidalMirrors;
      G4UIcmdWithAString       * fCmd_rotateMirror;

      G4UIcmdWithAString       * fCmd_toggleTargetField;
      G4UIcmdWithAString       * fCmd_toggleTargetMaterial;

      G4UIcmdWithAString       * fCmd_toggleForwardTracker;
      G4UIcmdWithAString       * fCmd_toggleGasCherenkov;
      G4UIcmdWithAString       * fCmd_toggleLuciteHodoscope;
      G4UIcmdWithAString       * fCmd_toggleBigcal;

   public:
      BETASimulationMessenger(BETASimulationManager* );
      ~BETASimulationMessenger();

      void AddDetectorUICommands();
      void SetNewValue(G4UIcommand*, G4String);

};


#endif

