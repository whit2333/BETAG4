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
#include "BETAAnalysisMessenger.hh"

BETAAnalysisManager* BETAAnalysisManager::instance = 0;

BETAAnalysisManager::BETAAnalysisManager ( const int runNumber, const int showThePlots )

{
   showPlot(showThePlots);
   messenger = new BETAAnalysisMessenger ( this );

}

BETAAnalysisManager::~BETAAnalysisManager()
{
   delete messenger;

}

BETAAnalysisManager* BETAAnalysisManager::getInstance ( const int runNumber, const int showThePlots )
{

   if ( instance == 0 )
   {
      instance = new BETAAnalysisManager ( runNumber,showThePlots ) ;
   }
   else  {
      G4cout << " Returning already existing analysis manager. " << G4endl;
   }
   return instance;
}

void BETAAnalysisManager::dispose()
{
   if ( instance != 0 )
   {
      delete instance;
      instance = 0;
   }
}


void BETAAnalysisManager::showPlot ( int arg )
{
   plotVis = arg;
}

void BETAAnalysisManager::write()
{

}
