#include <fstream>
#include "G4ios.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4VVisManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "BETAAnalysisManager.hh"
#include <string>  //For the string functions
#include <sstream>
#include "BETASimulationMessenger.hh"

BETAAnalysisManager* BETAAnalysisManager::fgBETAAnalysisManager = 0;

BETAAnalysisManager::BETAAnalysisManager (  const int showThePlots )
{
   showPlot(showThePlots);
   messenger = new BETASimulationMessenger ( this );
fGasCherenkovVerbosity=0;
fBigcalVerbosity=0;
fLuciteHodoscopeVerbosity=0;
fForwardTrackerVerbosity=0;
}

BETAAnalysisManager::~BETAAnalysisManager()
{
   delete messenger;

}

BETAAnalysisManager* BETAAnalysisManager::getInstance ( const int showThePlots )
{
   if ( fgBETAAnalysisManager == 0 )
   {
      fgBETAAnalysisManager = new BETAAnalysisManager ( showThePlots ) ;
   }
   else  {
      G4cout << " Returning already existing analysis manager. " << G4endl;
   }
   return fgBETAAnalysisManager;
}

void BETAAnalysisManager::dispose()
{
   if ( fgBETAAnalysisManager != 0 )
   {
      delete fgBETAAnalysisManager;
      fgBETAAnalysisManager = 0;
   }
}


void BETAAnalysisManager::showPlot ( int arg )
{
   plotVis = arg;
}

void BETAAnalysisManager::write()
{

}

void BETAAnalysisManager::SetDetectorVerbosity( char * detName, int level) {
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


int BETAAnalysisManager::GetDetectorVerbosity( char * detName) {

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

