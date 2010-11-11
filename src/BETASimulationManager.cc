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
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4ScoringManager.hh"
#include "G4VSDFilter.hh"
//#include "G4PSPassageCurrent.hh"
#include "G4PSFlatSurfaceFlux.hh"
#include "G4PSTrackLength.hh"

//_________________________________________________________________//

BETASimulationManager* BETASimulationManager::fgBETASimulationManager = 0;
//_________________________________________________________________//

BETASimulationManager::BETASimulationManager () : 
     fIsAppendMode(false),fRunNumber(0),plotVis(0),
     fSimulateCherenkovOptics(true),fSimulateHodoscopeOptics(true),
     fSimulateTrackerOptics(true)
{

  fSimulationMessenger = new BETASimulationMessenger ( this );
   showPlot(plotVis);
   fGasCherenkovVerbosity=0;
   fBigcalVerbosity=0;
   fLuciteHodoscopeVerbosity=0;
   fForwardTrackerVerbosity=0;
   fRootFile=0;
   fDetectorTree=0;
   betaEvent=0;
   hmsEvent=0;
   beamEvent=0;
   mcEvent=0;
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

int BETASimulationManager::SetTreeBranches() 
{
       fDetectorTree=(TTree *) gROOT->FindObject("betaDetectors");
       if (!fDetectorTree) fDetectorTree = new TTree ( "betaDetectors","Detector Tree" );
 
       TBranch *b;
       b = fDetectorTree->GetBranch ("betaDetectorEvent");
       if (!b)  fDetectorTree->Branch ( "betaDetectorEvent","BETAEvent",&betaEvent );
       else b->SetAddress(&betaEvent );
       
       b = fDetectorTree->GetBranch("hmsDetectorEvent");
       if (!b)  fDetectorTree->Branch ( "hmsDetectorEvent","HMSEvent",&hmsEvent );
       else b->SetAddress(&hmsEvent );
 
       b = fDetectorTree->GetBranch("beamDetectorEvent");
       if (!b)  fDetectorTree->Branch ( "beamDetectorEvent","HallCBeamEvent",&beamEvent );
       else b->SetAddress(&beamEvent );
       
       b = fDetectorTree->GetBranch("monteCarloEvent");
       if (!b)  fDetectorTree->Branch ( "monteCarloEvent","BETAG4MonteCarloEvent",&mcEvent );
       else b->SetAddress(&mcEvent );
  return(0);
}
//_________________________________________________________________//

int BETASimulationManager::CreateTreeBranches() 
{
       fDetectorTree = new TTree ( "betaDetectors","Detector Tree" );
       fDetectorTree->Branch ( "betaDetectorEvent","BETAEvent",&betaEvent,32000,2 );
       fDetectorTree->Branch ( "hmsDetectorEvent","HMSEvent",&hmsEvent );
       fDetectorTree->Branch ( "beamDetectorEvent","HallCBeamEvent",&beamEvent );
       fDetectorTree->Branch ( "monteCarloEvent","BETAG4MonteCarloEvent",&mcEvent);
  return(0);
}
//_________________________________________________________________//

int BETASimulationManager::AllocateTreeMemory() {

    fInSANERun = new InSANERun();
/// \todo Put all the memory management calls into a single class
    betaEvent = new BETAEvent();
      betaEvent->AllocateMemory();
      betaEvent->AllocateHitsMemory();
    hmsEvent = new HMSEvent();
    beamEvent = new HallCBeamEvent();
    mcEvent = new BETAG4MonteCarloEvent();
      mcEvent->AllocateMemory();
      mcEvent->AllocateHitsMemory();
  return(0);
}
//_________________________________________________________________//

int BETASimulationManager::Reset()  {

// if(fDetectorTree) delete fDetectorTree;
// fDetectorTree=0;
// if(fRootFile) delete fRootFile;
// fRootFile=0;

//    if(betaEvent)   betaEvent->FreeMemory();
//    if(mcEvent)    mcEvent->FreeMemory();
//   if(betaEvent) delete betaEvent;
//   if(hmsEvent) delete hmsEvent;
//   if(beamEvent) delete beamEvent;
//   if(mcEvent) delete mcEvent;
return(0);
}
//_________________________________________________________________//

int BETASimulationManager::InitScoring()  {
  G4SDManager * sensitiveDetManager = G4SDManager::GetSDMpointer();
  G4String filterName, particleName;

  G4SDParticleFilter* protonFilter;
  G4SDParticleFilter* electronFilter;
  G4SDParticleFilter* opticalPhotonFilter;
// Scoring and sensitive volumes

  myScorer = 
    new G4MultiFunctionalDetector("myCellScorer");
//
  sensitiveDetManager->AddNewDetector(myScorer);


// Sensitive volume filters
  protonFilter = new G4SDParticleFilter(filterName="protonFilter");
protonFilter->add("proton");

  electronFilter = new G4SDParticleFilter(filterName="electronFilter");
protonFilter->add("e-");

// Sensor primitives
  G4PSFlatSurfaceFlux * protonSurfFlux;
    protonSurfFlux   = new G4PSFlatSurfaceFlux("pSurfFlux",1);
    protonSurfFlux->SetFilter(protonFilter);

//   G4PSPassageCurrent * electronSurfFlux;
//     electronSurfFlux   = new G4PSPassageCurrent("ePassCurrent",1);
//     electronSurfFlux->SetFilter(electronFilter);

  G4PSTrackLength* electronTracklength;
    electronTracklength   = new G4PSTrackLength("eTrackLength",1);
    electronTracklength->SetFilter(electronFilter);

// Register Scoring volume with primitive(s)
  myScorer->RegisterPrimitive(electronTracklength);

  return(0);
}
//_________________________________________________________________//


