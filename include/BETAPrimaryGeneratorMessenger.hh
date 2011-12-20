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

    /**  Executes internal code when UI command is issued
     */
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    BETAPrimaryGeneratorAction* BETAAction;


    /** @name Commands for setting phase space
     *  These commands are used just to set the first particle's phase space. 
     *  There are no commands for a second particle ( if it is not an inclusive event generator).
     * @{
     */
    G4UIcmdWithADouble*   setPhiMax;
    G4UIcmdWithADouble*   setPhiMin;
    G4UIcmdWithADouble*   setThetaMin;
    G4UIcmdWithADouble*   setThetaMax;
    G4UIcmdWithADouble*   setEnergyMax;
    G4UIcmdWithADouble*   setEnergyMin;
    G4UIcmdWithADouble*   setMomentumMax;
    G4UIcmdWithADouble*   setMomentumMin;
    //@}
    G4UIcmdWithAString*   setType;
    G4UIcmdWithoutParameter * refreshGenerator;

    G4UIcmdWithoutParameter * listPSVariables;


    G4UIcmdWithAString*   setParticle;

    G4UIcmdWithAString*   setPSVariable;


    G4UIdirectory*               gunDir; 
    G4UIcmdWithADoubleAndUnit*   polarCmd;
    G4UIcmdWithAnInteger*   isotropic;
    G4UIcmdWithADouble*   momentum;
    G4UIcmdWithADouble*   sigmaMomentum;
    G4UIcmdWithADouble*   sete_piRatio;
    G4UIcmdWithADouble*   setpi0Ratio;

};

#endif

