#ifndef BETADetectorMessenger_h
#define BETADetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "BETADetectorConstruction.hh"
#include "BETADetectorMessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
//class BETAPrimaAction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class BETADetectorConstruction;

/**
 * \brief User Interface Messenger for Detector Construction
 */
class BETADetectorMessenger: public G4UImessenger
{
  public:
/// Detector Messenger Constructor
    BETADetectorMessenger(BETADetectorConstruction *  );
   ~BETADetectorMessenger();


    void SetNewValue(G4UIcommand*, G4String); 

    G4double fTargetAngle;

  private:
//    BETAAnalysisAction* BETAAction;
    BETADetectorConstruction * construction;
    G4UIdirectory*               fieldDir; 
    G4UIcmdWithoutParameter*   polSwitch;
    G4UIcmdWithAString*      lookAtField;

    G4UIcmdWithAString*      toggleTargetField;
    G4UIcmdWithAString*      toggleTargetMaterial;


    G4UIcmdWithADouble*        polSet;
    G4UIcmdWithADouble*        rotateToroidalMirrors;
    G4UIcmdWithAString*        rotateMirror;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

