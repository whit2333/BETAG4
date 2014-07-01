#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UIGAG.hh"
//#include "G4UIQt.hh"
#include "G4UItcsh.hh"
//#include "G4UIXm.hh"
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
#include "getopt.h"
#include <math.h>
#include <string>

#define no_argument 0
#define required_argument 1 
#define optional_argument 2


void print_usage();
void print_libs();
void print_cflags();
void print_ldflags();
void print_inc();
void print_version();
void print_grid();

/**
 * \mainpage 
 * 
 *  BETAG4 is a simulation designed for the Spin Asymmetries of the Nucleon Experiment (SANE).
 *  <img src="http://quarks.temple.edu/~whit/SANE/analysis_main/images/BETAG4/BETA-event1.jpg" alt="BETA Event" width="300px"/>
 * 
 *  - \subpage intro
 *  - \subpage eventgenerators
 *  - \subpage UIcommands
 *  - \subpage mainprogram
 *
 *   author Whitney Armstrong (whit@temple.edu)
 * <img src="http://quarks.temple.edu/~whit/SANE/analysis_main/images/BETAG4/BETA-top.jpg" alt="Top view of BETA" />
 * 
 */

/*! \page intro Introduction
     In addition to the GEANT4 libraries, BETAG4 relies on having the

     <a href="http://quarks.temple.edu/~whit/code/InSANE++/html/">InSANE libraries</a>
*/

/*!  \page UIcommands User interface commands

     \example para_10k_wiser.mac
     \example para_10k_dis.mac

     See the <a href="http://quarks.temple.edu/~whit/SANE/analysis_main/betag4_commands/_.html">
     user interface commands list </a>

 */

/*! \page eventgenerators Event Generators

    - 
    - 

*/

/*! \page mainprogram BETA program execution
    
    -# The seed is set for the Geant4 random numbers.
    -# G4VSteppingVerbose created
    -# G4RunManager created
       -# G4VUserDetectorConstruction (BETADetectorConstruction) created
       -# G4VUserPhysicsList (BETAPhysicsList)
       -# G4UserRunAction (BETARunAction)
       -# G4UserRunAction (BETARunAction)
       -# G4VUserPrimaryGeneratorAction (BETAPrimaryGeneratorAction)
       -# G4UserEventAction (BETAEventAction)
       -# G4UserStackingAction (BETAStackingAction)
       -# G4UserSteppingAction (BETASteppingAction)
    -# Interactive or script session invoked

*/
/**  BETAG4 : Big Electron Telescope Array Geant4 Simulation
 *
 */
int main(int argc,char** argv)
{
   int  run_set         = 0;
   bool is_interactive  = true;
   bool is_interactive_with_macro  = false;
   std::string theMacro = "";

   const struct option longopts[] =
   {
      {"run",       required_argument,  0, 'r'},
      {"version",   no_argument,        0, 'v'},
      {"help",      no_argument,        0, 'h'},
      {"macro",     required_argument,  0, 'm'},
      {"cflags",    no_argument,        0, 'c'},
      {"ldflags",   no_argument,        0, 'd'},
      {"int",       no_argument,        0, 'i'},
      {"grid",      no_argument,        0, 'g'},
      {0,0,0,0}
   };

   int index = 0;
   int iarg  = 0;
   opterr    = 1; //turn off getopt error message

   while(iarg != -1) {
      iarg = getopt_long(argc, argv, "vhilcdr:m:", longopts, &index);

      switch (iarg)
      {
         case 'r':
            std::cout << "run should be set to " << optarg << std::endl;
            run_set = atoi(optarg);
            break;

         case 'm':
            std::cout << "Macro set to " << optarg << std::endl;
            theMacro = optarg;
            is_interactive  = false;
            break;

         case 'h':
            print_usage();
            exit(0);
            break;

         case 'v':
            print_version();
            exit(0);
            break;

         case 'l':
            print_libs();
            break;

         case 'c':
            print_cflags();
            break;

         case 'i':
            print_inc();
            is_interactive_with_macro = true;
            break;

         case 'd':
            print_ldflags();
            break;

         case 'g':
            print_grid();
            break;
      }
   }

   std::string theRest  = "";
   for (int i = optind; i < argc; i++) {
      is_interactive  = false;
      theRest        += argv[i];
   }
   std::cout << " the rest of the arguments: " << theRest << std::endl;

   // Seed the random number generator manually
   G4long myseed = 983;
   std::ifstream input_file ;
   std::ofstream output_file ;
   input_file.open ( "seed.txt" );
   input_file >> myseed;
   input_file.close();
   myseed++;
   output_file.open ( "seed.txt" ,std::ios::trunc); // this incremtents a number
   output_file << myseed ;
   output_file.close();
   CLHEP::HepRandom::setTheSeed(myseed);
   TRandom * rand = InSANERunManager::GetRunManager()->GetRandom();
   rand->SetSeed(myseed);

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
   physics->SetVerboseLevel(0);
   runManager->SetUserInitialization(physics);

   // visualization manager
   #ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive;
   visManager->SetVerboseLevel(0);
   visManager->Initialize();
   #endif

   // UserAction classes
   G4UserRunAction* run_action = new BETARunAction(run_set);
   runManager->SetUserAction(run_action);

   G4VUserPrimaryGeneratorAction* gen_action = new BETAPrimaryGeneratorAction;
   runManager->SetUserAction(gen_action);

   G4UserEventAction* event_action = new BETAEventAction;
   runManager->SetUserAction(event_action);

   G4UserStackingAction* stacking_action = new BETAStackingAction;
   runManager->SetUserAction(stacking_action);

   G4UserSteppingAction* stepping_action = new BETASteppingAction;
   runManager->SetUserAction(stepping_action);

   int fnargs = 2;
   char arg0[10];  
   char arg1[10];  
   strcpy(arg0,"BETAG4");
   strcpy(arg1,"-l");
   char * fargs[2] = {arg0,arg1};
   //char * fargs[2] = {"BETAG4","-l"};
   //TRint * fApp = 
   new TRint("BETAG4", &fnargs,&fargs[0], NULL, 2);

   std::cout << " o Initializing G4 kernel " << std::endl;

   // Initialize G4 kernel
   runManager->Initialize();
   std::cout << " o Done initializing G4 kernel " << std::endl;

   // Get the pointer to the User Interface manager
   G4UImanager* UI = G4UImanager::GetUIpointer(); 

   if(is_interactive) {
      G4UIsession* session = 0;
#ifdef G4UI_USE_TCSH
      // session = new G4UIGAG;      
      session = new G4UIterminal(new G4UItcsh);      
      // session = new G4UIQt(argc, argv);    
#else
      session = new G4UIterminal();
#endif
      //UI->ApplyCommand("/control/execute vis.mac"); 
      session->SessionStart();
      delete session;
   } else {
      // Batch mode, quit at the end
      G4String command = "/control/execute ";
      G4String fileName;
      if(!theMacro.empty()) {
         fileName = theMacro;//argv[1];
      } else {
         fileName = theRest;//argv[1];
      }
      //UI->ApplyCommand(command+fileName);
      G4UIsession* session = 0;
      // UI->ApplyCommand("/control/execute vis.mac"); 
      UI->ApplyCommand(command+fileName);
      if(is_interactive_with_macro) {
#ifdef G4UI_USE_TCSH
         // session = new G4UIGAG;      
         session = new G4UIterminal(new G4UItcsh);      
         // session = new G4UIQt(argc, argv);    
#else
         session = new G4UIterminal();
#endif
         //UI->ApplyCommand("/control/execute vis.mac"); 
         session->SessionStart();
      }
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

/*! @defgroup EventGen Event Generator
    Event generator 
*/ 

/*! @defgroup Detectors Detectors
    Detectors
 */


void print_version(){
   //std::cout << "InSANE Version " << InSANE_VERSION_MAJOR << "." << InSANE_VERSION_MINOR << " ";
}

void print_usage(){
   std::cout << " BETA [OPTIONS...] [MACRO FILE] " << std::endl;
   std::cout << "   Giving only the argument [MACRO FILE] runs the macro in batch mode," << std::endl;
   std::cout << "   otherwise GEANT4 runs in interactive mode." << std::endl;
   std::cout << "   -m MACRO     Executes macro in batch mode." << std::endl;
   std::cout << "   -i           Forces interactive mode after running a macro" << std::endl;
   std::cout << "   -h           prints this help " << std::endl;
   //std::cout << "insane-config --libs --cflags --ldflags --inc --grid" << " ";
}

void print_libs(){
   //std::cout << InSANE_CXX_LIBS << " ";
}

void print_inc(){
   //std::cout << InSANE_CXX_INC_DIR << " ";
}

void print_cflags(){
   //std::cout << InSANE_CXX_CFLAGS << " ";
}

void print_ldflags(){
   //std::cout << InSANE_CXX_LDFLAGS << " ";
}

void print_grid(){
   //std::cout << InSANE_GRID_DATA_DIR << " ";
}

