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
#include "BETASimulationMessenger.hh"

BETASimulationManager* BETASimulationManager::fgBETASimulationManager = 0;

BETASimulationManager::BETASimulationManager (  const int showThePlots )
{
   showPlot(showThePlots);
   messenger = new BETASimulationMessenger ( this );
fGasCherenkovVerbosity=0;
fBigcalVerbosity=0;
fLuciteHodoscopeVerbosity=0;
fForwardTrackerVerbosity=0;
}

BETASimulationManager::~BETASimulationManager()
{
   delete messenger;

}

BETASimulationManager* BETASimulationManager::getInstance ( const int showThePlots )
{
   if ( fgBETASimulationManager == 0 )
   {
      fgBETASimulationManager = new BETASimulationManager ( showThePlots ) ;
   }
   else  {
      G4cout << " Returning already existing analysis manager. " << G4endl;
   }
   return fgBETASimulationManager;
}

void BETASimulationManager::dispose()
{
   if ( fgBETASimulationManager != 0 )
   {
      delete fgBETASimulationManager;
      fgBETASimulationManager = 0;
   }
}


void BETASimulationManager::showPlot ( int arg )
{
   plotVis = arg;
}

void BETASimulationManager::write()
{

}

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

