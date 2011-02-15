/** 
 * \mainpage 
 *  BETAG4 is a simulation designed for the Spin Asymmetries of the Nucleon Experiment (SANE).
 * 
 * \section intro Introduction
 * In addition to the GEANT4 libraries, BETAG4 relies on having the
 * <a href="http://quarks.temple.edu/~whit/code/InSANE++/html/">InSANE libraries</a>
 * 
 *   author Whitney Armstrong (whit@temple.edu)
 * 
 * 
 * 
 */
//
//
//-----------------------------------------------------------

/*! \page intro Introduction
This page introduces the user to the topic.
Now you can proceed to the \ref advanced "advanced section".  
*/

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UIGAG.hh"
#include "G4UIQt.hh"


#include "G4UItcsh.hh"
#include "G4UIXm.hh"
#include "G4ios.hh"

#include "BETADetectorConstruction.hh"
#include "BETAPhysicsList.hh"
#include "BETAPrimaryGeneratorAction.hh"
#include "BETARunAction.hh"
#include "BETAEventAction.hh"
#include "BETAStackingAction.hh"
#include "BETASteppingVerbose.hh"
#include "BETASteppingAction.hh"

#include "Randomize.hh"

#include "TROOT.h"
#include "TRint.h"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4GlobalFastSimulationManager.hh"


/**  BETAG4 : Big Electron Telescope Array Geant4 Simulation
 *
 * 
 *
 *
 */
int main(int argc,char** argv)
{
  // Seed the random number generator manually
  G4long myseed = 98983;
    ifstream input_file ;
    ofstream output_file ;
    input_file.open ( "seed.txt" );
    input_file >> myseed;
    input_file.close();
  myseed++;
    output_file.open ( "seed.txt" ,ios::trunc); // this incremtents a number
    output_file << myseed ;
    output_file.close();
  CLHEP::HepRandom::setTheSeed(myseed);

  // User Verbose output class
  G4VSteppingVerbose* verbosity = new BETASteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);

  // Run manager
  G4RunManager* runManager = new G4RunManager;

  // UserInitialization classes - mandatory
  G4VUserDetectorConstruction* detector = new BETADetectorConstruction;

  runManager->SetUserInitialization(detector);

  // Physics List
  G4VUserPhysicsList* physics = new BETAPhysicsList;
  runManager-> SetUserInitialization(physics);

  // visualization manager
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // UserAction classes
  G4UserRunAction* run_action = new BETARunAction;
  runManager->SetUserAction(run_action);
  
  G4VUserPrimaryGeneratorAction* gen_action = new BETAPrimaryGeneratorAction;
  runManager->SetUserAction(gen_action);
  
  G4UserEventAction* event_action = new BETAEventAction;
  runManager->SetUserAction(event_action);
  
  G4UserStackingAction* stacking_action = new BETAStackingAction;
  runManager->SetUserAction(stacking_action);

  G4UserSteppingAction* stepping_action = new BETASteppingAction;
  runManager->SetUserAction(stepping_action);


  //new TRint("delayPlots", NULL, NULL, NULL, 0);

  // Initialize G4 kernel
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer(); 

  if (argc==1)   // Define UI session for interactive mode
    {
      G4UIsession* session = 0;
#ifdef G4UI_USE_TCSH
// session = new G4UIGAG;      
      session = new G4UIterminal(new G4UItcsh);      
//      session = new G4UIQt(argc, argv);    
#else
      session = new G4UIterminal();
#endif
//      UI->ApplyCommand("/control/execute vis.mac"); 
      session->SessionStart();
      delete session;
   }
  else         // Batch mode, quit at the end
   {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
//     UI->ApplyCommand(command+fileName);
      G4UIsession* session = 0;
//      UI->ApplyCommand("/control/execute vis.mac"); 
     UI->ApplyCommand(command+fileName);
     // session->SessionStart();
     if(session) delete session;
   }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  delete verbosity;

  return 0;
}
