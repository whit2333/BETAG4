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
#include "G4PSPassageCellCurrent.hh"
#include "G4PSFlatSurfaceFlux.hh"
#include "G4PSTrackLength.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
//_________________________________________________________________//

BETASimulationManager* BETASimulationManager::fgBETASimulationManager = 0;
//_________________________________________________________________//

BETASimulationManager::BETASimulationManager () : 
     fIsAppendMode(false),fRunNumber(0),plotVis(0),
     fSimulateCherenkovOptics(true),fSimulateHodoscopeOptics(true),
     fSimulateTrackerOptics(false)
{

  fSimulationMessenger = new BETASimulationMessenger ( this );
   showPlot(plotVis);
   fGasCherenkovVerbosity=1;
   fBigcalVerbosity=0;
   fLuciteHodoscopeVerbosity=0;
   fForwardTrackerVerbosity=0;
   fRootFile=0;
   fDetectorTree=0;
   betaEvent=0;
   hmsEvent=0;
   beamEvent=0;
   mcEvent=0;
  //InitScoring();
}
//_________________________________________________________________//

BETASimulationManager::~BETASimulationManager()
{
   delete fSimulationMessenger;
}
//_________________________________________________________________//

BETASimulationManager* BETASimulationManager::GetInstance (  )
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

int BETASimulationManager::InitializeNewRun() {
  fRunNumber= RetrieveRunNumber();
  return(fRunNumber);
}

void BETASimulationManager::Dispose()
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

///Depreciated
int BETASimulationManager::SetTreeBranches() 
{

  return(0);
}
//_________________________________________________________________//

int BETASimulationManager::CreateTrees() {
    fDetectorTree = new TTree ( "simulatedBetaDetectors","Simulation Detector Tree" );
    fDetectorTree->SetAutoSave();
//         atree = new TTree ( "hmsDetectors","HMS Tree" );
//         atree = new TTree ( "hallcBeam","Hallc Beam Tree" );
//         atree = new TTree ( "monteCarloEvents","HMS Tree" );
//         atree = new TTree ( "triggerData","Trigger Tree" );
  return(0);
}
//_________________________________________________________________//

///Depreciated
int BETASimulationManager::AllocateTreeMemory() {

    fInSANERun = new BETAG4SimulationRun(fRunNumber);

    fEvents = new SANEEvents("simulatedBetaDetectors");
//     betaEvent=events->BETA;
//     hmsEvent=events->HMS;
//     beamEvent=events->BEAM;
//     mcEvent=events->MC;
/// \todo Put all the memory management calls into a single class
//     betaEvent = new BETAEvent();
//       betaEvent->AllocateMemory();
//       betaEvent->AllocateHitsMemory();
//     hmsEvent = new HMSEvent();
//     beamEvent = new HallCBeamEvent();
//     mcEvent = new BETAG4MonteCarloEvent();
//       mcEvent->AllocateMemory();
//       mcEvent->AllocateHitsMemory();
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


// Scoring and sensitive volumes

  fTrackerDetector = 
    new G4MultiFunctionalDetector("tracker");
  fCherenkovDetector = 
    new G4MultiFunctionalDetector("cherenkov");
  fBigcalDetector = 
    new G4MultiFunctionalDetector("bigcal");
  fHodoscopeDetector = 
    new G4MultiFunctionalDetector("hodoscope");
//


// Sensitive volume filters
  protonFilter = new G4SDParticleFilter(filterName="protonFilter");//,"proton");
//    protonFilter->add("proton");
  electronFilter = new G4SDParticleFilter(filterName="electronFilter");
//    electronFilter->add("e-");

  chargeFilter = new G4SDChargedFilter("chargeFilter");
  opticalPhotonFilter = new G4SDParticleFilter(filterName="opticalPhotonFilter");
//    opticalPhotonFilter->add("opticalphoton");
  bigcalEnergyFilter = new G4SDKineticEnergyFilter("bigcalEnergyThreshFilter");

// DefineScoringFilters();

// scoring primitives
    protonSurfFlux   = new G4PSFlatSurfaceFlux("pSurfFlux",1);
    protonSurfFlux->SetFilter(protonFilter);

    electronSurfFlux   = new G4PSPassageCellCurrent("eHit");
    electronSurfFlux->SetFilter(electronFilter);

    chargeSurfFlux   = new G4PSPassageCellCurrent("chargeHit");
    chargeSurfFlux->SetFilter(chargeFilter);

    photonSurfFlux   = new G4PSPassageCellCurrent("photonSurf");
    photonSurfFlux->SetFilter(opticalPhotonFilter);

    electronTracklength   = new G4PSTrackLength("eTrackLength");
    electronTracklength->SetFilter(electronFilter);

   calEnergyDeposit = new G4PSEnergyDeposit("bcEnergyDeposit");
   calEnergyDeposit->SetFilter(bigcalEnergyFilter);

// Register Scoring volume with primitive(s)
  fTrackerDetector->RegisterPrimitive(chargeSurfFlux);
  fHodoscopeDetector->RegisterPrimitive(chargeSurfFlux);
  fCherenkovDetector->RegisterPrimitive(photonSurfFlux);
  calEnergyDeposit->SetMultiFunctionalDetector(fBigcalDetector);
  fBigcalDetector->RegisterPrimitive(calEnergyDeposit);

  fBigcalDetector->RegisterPrimitive(protonSurfFlux);

// Below does not work!
//  fBigcalDetector->RegisterPrimitive((G4VPrimitiveScorer*)calEnergyDeposit);


  sensitiveDetManager->AddNewDetector(fTrackerDetector);
  sensitiveDetManager->AddNewDetector(fCherenkovDetector);
  sensitiveDetManager->AddNewDetector(fBigcalDetector);
  sensitiveDetManager->AddNewDetector(fHodoscopeDetector);

  return(0);
}
//_________________________________________________________________//

int BETASimulationManager::DefineScoringFilters() {
// Sensitive volume filters
    protonFilter->add("proton");

    electronFilter->add("e-");

    opticalPhotonFilter->add("opticalphoton");

    bigcalEnergyFilter->SetKineticEnergy(0.010*GeV,6.0*GeV);


return(0);
}




