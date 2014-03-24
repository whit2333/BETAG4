#ifndef BETASimulationMessenger_h
#define BETASimulationMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4RunManager.hh"

#include "BETAPhysicsListMessenger.hh"
#include "BETASimulationManager.hh"

#include "BETADetectorConstruction.hh"
#include "BETAPrimaryGeneratorAction.hh"


class G4UIdirectory;
class G4UIcmdWithoutParameter;

/**
 * Concrete implementation of G4UImessenger which creates/connects geant4 command lines to 
 * appropriate data members or methods. \todo{Consildate the messengers. Should we have one 
 * messenger per class or just on messenger?}
 */
class BETASimulationMessenger: public G4UImessenger {
   private:
      BETASimulationManager       * fSimManager;

      /** @name Commands for setting phase space
       *  These commands are used just to set the first particle's phase space. 
       *  There are no commands for a second particle ( if it is not an inclusive event generator).
       * @{
       */
      G4UIcmdWithADouble        * fCmd_setPhiMax;
      G4UIcmdWithADouble        * fCmd_setPhiMin;
      G4UIcmdWithADouble        * fCmd_setThetaMin;
      G4UIcmdWithADouble        * fCmd_setThetaMax;
      G4UIcmdWithADouble        * fCmd_setEnergyMax;
      G4UIcmdWithADouble        * fCmd_setEnergyMin;
      G4UIcmdWithADouble        * fCmd_setMomentumMax;
      G4UIcmdWithADouble        * fCmd_setMomentumMin;
      ///@}

      G4UIcmdWithAString        * fCmd_setType;
      G4UIcmdWithoutParameter   * fCmd_refreshGenerator;
      G4UIcmdWithoutParameter   * fCmd_listPSVariables;
      G4UIcmdWithAString        * fCmd_setParticle;
      G4UIcmdWithAString        * fCmd_setPSVariable;
      G4UIcmdWithADoubleAndUnit * fCmd_polarCmd;
      G4UIcmdWithAnInteger      * fCmd_isotropic;
      G4UIcmdWithADouble        * fCmd_momentum;
      G4UIcmdWithADouble        * fCmd_setBeamEnergy;
      G4UIcmdWithADouble        * fCmd_sigmaMomentum;
      G4UIcmdWithADouble        * fCmd_sete_piRatio;
      G4UIcmdWithADouble        * fCmd_setpi0Ratio;

      G4UIdirectory             * fDir_gunDir;
      G4UIdirectory             * fDir_analysis;
      G4UIdirectory            * fDir_simulation;
      G4UIdirectory            * fDir_beta;
      G4UIdirectory            * fDir_fieldDir;

      G4UIcmdWithAString       * fCmd_simType;

      G4UIcmdWithoutParameter  * fCmd_polSwitch;
      G4UIcmdWithAString       * fCmd_lookAtField;

      G4UIcmdWithADouble       * fCmd_polSet;
      G4UIcmdWithADouble       * fCmd_rotateToroidalMirrors;
      G4UIcmdWithAString       * fCmd_rotateMirror;

      G4UIcmdWithAString       * fCmd_toggleTargetField;
      G4UIcmdWithAString       * fCmd_toggleTargetMaterial;

      G4UIcmdWithAString       * fCmd_toggleForwardTracker;
      G4UIcmdWithAString       * fCmd_toggleGasCherenkov;
      G4UIcmdWithAString       * fCmd_toggleLuciteHodoscope;
      G4UIcmdWithAString       * fCmd_toggleBigcal;

      G4UIcmdWithAnInteger      * fCmd_simTrigger;

   public:
      BETASimulationMessenger(BETASimulationManager* );
      ~BETASimulationMessenger();

      BETADetectorConstruction   * GetDetectorConstruction(){  return fSimManager->GetDetectorConstruction(); }
      BETAPrimaryGeneratorAction * GetPrimaryGeneratorAction(){ return fSimManager->GetPrimaryGeneratorAction(); }

      void AddDetectorUICommands();
      void SetNewValue(G4UIcommand*, G4String);

};


#endif

