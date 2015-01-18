#ifndef BETAPhysicsList_h
#define BETAPhysicsList_h 1

#include "globals.hh"
#include "G4VUserPhysicsList.hh"
#include "FTFP_BERT.hh"
//#include "G4StepLimiterPhysics.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;
class BETAPhysicsListMessenger;

/**
 *  This Geant4 physics list sets all the particles, processes 
 *  and production thresholds (eg a low energy limit where we don't
 *  care anymore).
 *
 *  A good physics list is crucial and if it is not set up properly
 *  a bad simulation can result. 
 *
 */
class BETAPhysicsList : public FTFP_BERT //G4VUserPhysicsList
{
  public:
    BETAPhysicsList();
   ~BETAPhysicsList();

  public:
    void ConstructParticle();
    void ConstructProcess();

    void Print(int level=0);

    void SetCuts();

    //these methods Construct particles
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();

    //these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void ConstructOp();
    
    void SetVerbose(G4int);
    void SetNbOfPhotonsCerenkov(G4int);
    
  private:
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;
    G4double defaultCutValue;
    BETAPhysicsListMessenger* pMessenger;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /* BETAPhysicsList_h */

