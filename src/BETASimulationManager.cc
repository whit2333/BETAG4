#include "BETASimulationManager.hh"
#include "BETASimulationMessenger.hh"
#include <fstream>
#include "G4ios.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4VVisManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include <string>  //For the string functions
#include <sstream>
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4ScoringManager.hh"
#include "G4VSDFilter.hh"
#include "G4PSPassageCellCurrent.hh"
#include "G4PSFlatSurfaceFlux.hh"
#include "G4PSTrackLength.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"



//______________________________________________________________________________
BETASimulationManager* BETASimulationManager::fgBETASimulationManager = 0;
//______________________________________________________________________________
BETASimulationManager::BETASimulationManager () {
   fIsAppendMode            = false;
   fRunNumber               = 0;
   fSimulateCherenkovOptics = true;
   fSimulateHodoscopeOptics = true;
   fSimulateTrackerOptics   = true;
   fDebugLevel              = 3;
   fSimulateTrigger         = true;

   fSimulationMessenger = new BETASimulationMessenger ( this );

   fEvents                   = 0;
   fGasCherenkovVerbosity    = 0;
   fBigcalVerbosity          = 0;
   fLuciteHodoscopeVerbosity = 0;
   fForwardTrackerVerbosity  = 0;
   fRootFile                 = 0;
   fDetectorTree             = 0;

   fBETADetectorPackage      = 0;
   fCherenkovDetector        = 0;
   fBigcalDetector           = 0;
   fHodoscopeDetector        = 0;
   fTrackerDetector          = 0;

   fBeamEnergy               = 5.9*GeV;
   //InitScoring();
}
//_________________________________________________________________//

BETASimulationManager::~BETASimulationManager()
{
   delete fSimulationMessenger;
}
//______________________________________________________________________________
BETASimulationManager* BETASimulationManager::GetInstance (  )
{
   if ( fgBETASimulationManager == 0 )
   {
      fgBETASimulationManager = new BETASimulationManager ( ) ;
   }
   /*   else  {
        G4cout << " Returning already existing simulation manager. " << G4endl;
        }*/
   return fgBETASimulationManager;
}
//______________________________________________________________________________
int BETASimulationManager::InitializeNewRun(int run) {
   fRunNumber = RetrieveRunNumber(run);
   return(fRunNumber);
}
//______________________________________________________________________________

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
   /*   plotVis = arg;*/
}
//_________________________________________________________________//

void BETASimulationManager::write()
{

}
//_________________________________________________________________//

void BETASimulationManager::SetDetectorVerbosity(const char * detName, int level) {
   if( !strcmp(detName,"GasCherenkov") ) {
      std::cout << "Setting Gas Cherenkov verbosity level to " << level << "\n";
      fGasCherenkovVerbosity = level;
   } else if( !strcmp(detName , "Bigcal") ) {
      std::cout << "Setting Bigcal verbosity level to " << level << "\n";
      fBigcalVerbosity = level;
   } else {
      std::cout << "No such detector, " << detName << "\n";
   }
}
//_________________________________________________________________//

int BETASimulationManager::GetDetectorVerbosity(const char * detName) {

   if( !strcmp(detName,"GasCherenkov") ) {
      return(fGasCherenkovVerbosity);
   } else if( !strcmp(detName , "Bigcal") ) {
      return(1);
      //    return(fBigcalVerbosity);
   }  else {
      std::cout << "No such detector, " << detName << "\n";
   }
   return(-1);
}
//______________________________________________________________________________
int BETASimulationManager::RetrieveRunNumber(int run) {
   // If the run argument is supplied the run is simply set to that number.
   // otherwise the run file is used (and incrmeneted).
   if(run == 0){
      std::ifstream input_file;
      input_file.open ( "run.txt" );
      input_file >> fRunNumber ;
      input_file.close(); 
      IncrementRunNumber();
   } else {
      fRunNumber = run;
   }
   return(fRunNumber);
}
//_________________________________________________________________//

int BETASimulationManager::IncrementRunNumber()   
{
   fRunNumber++;
   std::ofstream output_file;
   output_file.open ( "run.txt" ,std::ios::trunc); // this incremtents a number so that it acts like a normal DAQ
   output_file << fRunNumber ;
   output_file.close();
   return fRunNumber;
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

   //G4SDManager * sensitiveDetManager = G4SDManager::GetSDMpointer();
   G4String filterName, particleName;


   // Scoring and sensitive volumes

   fTrackerScoring = 
      new G4MultiFunctionalDetector("tracker");
   fCherenkovScoring = 
      new G4MultiFunctionalDetector("cherenkov");
   fBigcalScoring = 
      new G4MultiFunctionalDetector("bigcal");
   fHodoscopeScoring = 
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
   fTrackerScoring->RegisterPrimitive(chargeSurfFlux);
   fHodoscopeScoring->RegisterPrimitive(chargeSurfFlux);
   fCherenkovScoring->RegisterPrimitive(photonSurfFlux);
   calEnergyDeposit->SetMultiFunctionalDetector(fBigcalScoring);
   fBigcalScoring->RegisterPrimitive(calEnergyDeposit);

   fBigcalScoring->RegisterPrimitive(protonSurfFlux);

   // Below does not work!
   //  fBigcalDetector->RegisterPrimitive((G4VPrimitiveScorer*)calEnergyDeposit);

   /*
      sensitiveDetManager->AddNewDetector(fTrackerDetector);
      sensitiveDetManager->AddNewDetector(fCherenkovDetector);
      sensitiveDetManager->AddNewDetector(fBigcalDetector);
      sensitiveDetManager->AddNewDetector(fHodoscopeDetector);*/

   return(0);
}
//______________________________________________________________________________
int BETASimulationManager::DefineScoringFilters() {
   // Sensitive volume filters
   protonFilter->add("proton");

   electronFilter->add("e-");

   opticalPhotonFilter->add("opticalphoton");

   bigcalEnergyFilter->SetKineticEnergy(0.010*GeV,6.0*GeV);

   return(0);
}
//______________________________________________________________________________
int BETASimulationManager::AddDetectors(int runNumber) {
   // Creates the detectors

   fBETADetectorPackage = new BETADetectorPackage("BETADetectorPackage",runNumber);

   fCherenkovDetector = fBETADetectorPackage->fCherenkovDetector;//new GasCherenkovDetector(runNumber);
   fCherenkovDetector->SetEventAddress(fEvents->BETA->fGasCherenkovEvent);

   fBigcalDetector = fBETADetectorPackage->fBigcalDetector;//= new BigcalDetector(runNumber);
   fBigcalDetector->SetEventAddress(fEvents->BETA->fBigcalEvent);

   fHodoscopeDetector = fBETADetectorPackage->fHodoscopeDetector;//= new LuciteHodoscopeDetector(runNumber);
   fHodoscopeDetector->SetEventAddress(fEvents->BETA->fLuciteHodoscopeEvent);

   fTrackerDetector = fBETADetectorPackage->fTrackerDetector;//= new ForwardTrackerDetector(runNumber);
   fTrackerDetector->SetEventAddress(fEvents->BETA->fForwardTrackerEvent);

   return(0);
}
//______________________________________________________________________________
void BETASimulationManager::PrintSummary(){
   std::cout << " fSimulateCherenkovOptics " << fSimulateCherenkovOptics << std::endl;
   std::cout << " fSimulateTrackerOptics   " << fSimulateTrackerOptics   << std::endl;
   std::cout << " fSimulateHodoscopeOptics " << fSimulateHodoscopeOptics << std::endl;
   std::cout << " fSimulateBigcalOptics    " << fSimulateBigcalOptics    << std::endl;
   std::cout << " fSimulateTrigger         " << fSimulateTrigger         << std::endl;
}
//______________________________________________________________________________


