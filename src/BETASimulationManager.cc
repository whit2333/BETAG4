#include <fstream>
#include "G4ios.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4VVisManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "BETASimulationManager.hh"
#include <string>  //For the string functions
#include <sstream>
#include "BETASimulationMessenger.hh"s
//_________________________________________________________________//

BETASimulationManager* BETASimulationManager::fgBETASimulationManager = 0;
//_________________________________________________________________//

BETASimulationManager::BETASimulationManager ():fIsAppendMode(false),fRunNumber(0),plotVis(0){

  fSimulationMessenger = new BETASimulationMessenger ( this );
   showPlot(plotVis);
   fGasCherenkovVerbosity=0;
   fBigcalVerbosity=0;
   fLuciteHodoscopeVerbosity=0;
   fForwardTrackerVerbosity=0;
}
//_________________________________________________________________//

BETASimulationManager::~BETASimulationManager()
{
   delete fSimulationMessenger;
}
//_________________________________________________________________//

BETASimulationManager* BETASimulationManager::getInstance (  )
{
   if ( fgBETASimulationManager == 0 )
   {
      fgBETASimulationManager = new BETASimulationManager ( ) ;
   }
   else  {
      G4cout << " Returning already existing simulation manager. " << G4endl;
   }
   return fgBETASimulationManager;
}
//_________________________________________________________________//

void BETASimulationManager::dispose()
{
   if ( fgBETASimulationManager != 0 )
   {
      delete fgBETASimulationManager;
      fgBETASimulationManager = 0;
   }
}
//_________________________________________________________________//

void BETASimulationManager::showPlot ( int arg )
{
   plotVis = arg;
}
//_________________________________________________________________//

void BETASimulationManager::write()
{

}
//_________________________________________________________________//

void BETASimulationManager::SetDetectorVerbosity( char * detName, int level) {
  if( detName == "GasCherenkov" ) {
    std::cout << "Setting Gas Cherenkov verbosity level to " << level << "\n";
    fGasCherenkovVerbosity = level;
  }
  else if( detName == "Bigcal" ) {
    std::cout << "Setting Bigcal verbosity level to " << level << "\n";
    fBigcalVerbosity = level;
  }
  else {
    std::cout << "No such detector, " << detName << "\n";
  }
}
//_________________________________________________________________//

int BETASimulationManager::GetDetectorVerbosity( char * detName) {

  if( detName == "GasCherenkov" ) {
    return(fGasCherenkovVerbosity);
  }
  else if( detName == "Bigcal" ) {
return(1);
//    return(fBigcalVerbosity);
  }  else {
    std::cout << "No such detector, " << detName << "\n";
  }
    return(-1);
}
//_________________________________________________________________//

int BETASimulationManager::RetrieveRunNumber()
{
   ifstream input_file;
   input_file.open ( "run.txt" );
   input_file >> fRunNumber ;
   input_file.close(); 
   return(fRunNumber);
}
//_________________________________________________________________//

int BETASimulationManager::IncrementRunNumber()   
{
   fRunNumber++; 
   ofstream output_file;
   output_file.open ( "run.txt" ,ios::trunc); // this incremtents a number so that it acts like a normal DAQ
   output_file << fRunNumber ;
   output_file.close();

   return fRunNumber;
}
//_________________________________________________________________//
