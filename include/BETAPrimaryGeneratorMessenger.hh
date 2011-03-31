#ifndef BETAPrimaryGeneratorMessenger_h
#define BETAPrimaryGeneratorMessenger_h 1


#include "G4UImessenger.hh"
#include "globals.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
class BETAPrimaryGeneratorAction;
class G4UIdirectory;
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
/**
 *   \ingroup EventGen
 */
class BETAPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    BETAPrimaryGeneratorMessenger(BETAPrimaryGeneratorAction*);
   ~BETAPrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    G4UIcmdWithoutParameter * initGenerator;

    G4UIcmdWithADouble*   setPhiMax;
    G4UIcmdWithADouble*   setPhiMin;
    G4UIcmdWithADouble*   setThetaMin;
    G4UIcmdWithADouble*   setThetaMax;
    G4UIcmdWithADouble*   setEnergyMax;
    G4UIcmdWithADouble*   setEnergyMin;
    G4UIcmdWithAString*   setType;

    BETAPrimaryGeneratorAction* BETAAction;

    G4UIdirectory*               gunDir; 
    G4UIcmdWithADoubleAndUnit*   polarCmd;
    G4UIcmdWithAnInteger*   isotropic;
    G4UIcmdWithADouble*   momentum;
    G4UIcmdWithADouble*   sigmaMomentum;
    G4UIcmdWithADouble*   sete_piRatio;
    G4UIcmdWithADouble*   setpi0Ratio;


};

#endif

