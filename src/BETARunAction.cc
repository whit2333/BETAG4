// Make G4Timer appear first!
#include "G4Timer.hh"
#include <fstream>
#include "BETARunAction.hh"
#include "assert.h"
#include "G4Run.hh"
#include "BETARun.hh"
#include "BETASimulationManager.hh"
#include "BETADetectorConstruction.hh"
#include "G4RunManager.hh"
#include "TTimeStamp.h"
#include "TDatime.h"
#include "SANERunManager.h"
#include "BETAPrimaryGeneratorAction.hh"
#include "InSANEDatabaseManager.h"

//_________________________________________________________________

BETARunAction::BETARunAction(int run) :showThePlots ( 0 ) {

   fSimulationManager = BETASimulationManager::GetInstance();

   fRunNumber = fSimulationManager->InitializeNewRun(run);

   timer = new G4Timer;
   fCurrentRun=0;
}
//______________________________________________________________________________
BETARunAction::~BETARunAction() {
   delete timer;
}
//______________________________________________________________________________
void BETARunAction::BeginOfRunAction ( const G4Run* aRun ) {
   G4cout <<"=================== RUN #" << fRunNumber << " ===================" << G4endl;
}
//______________________________________________________________________________
G4Run*  BETARunAction::GenerateRun() {
   // Generates the InSANERun object
   // Initializes ...

   G4RunManager * runManager = G4RunManager::GetRunManager();

   G4cout << " = BETARunAction - GenerateRun   " << G4endl;

   timer->Start();

   G4cout << "  - Creating Run Number " << fRunNumber << "   " << G4endl;
   SANERunManager::GetRunManager()->SetRun(fRunNumber);
   fSimulationManager->fInSANERun = SANERunManager::GetRunManager()->GetCurrentRun();

   // Opens Root File With a name Depending on the Run number
   std::string fname;
   std::stringstream out;
   if ( fSimulationManager->IsAppendMode() ) {
      // append to existing file
      out << "data/rootfiles/beta.accumulate.root"  ;
      fname = out.str();
      const char * rootName = fname.c_str();
      fSimulationManager->fRootFile = new TFile ( rootName,"UPDATE" );
      fSimulationManager->fEvents = new SANEEvents("betaDetectors0");
   } else {
      // new file
      out << "data/rootfiles/InSANE" << fRunNumber << ".-1.root"  ;
      fname = out.str();
      fSimulationManager->fRootFile     = SANERunManager::GetRunManager()->GetCurrentFile();
      fSimulationManager->fDetectorTree = new TTree("betaDetectors","Simulated BETA Detectors");
      fSimulationManager->fEvents       = new SANEEvents("betaDetectors");
   }

   SANERunManager::GetRunManager()->GetScalerFile()->cd();
   fSimulationManager->fScalerTree  = new TTree("Scalers","The SANE Scaler Data");
   fSimulationManager->fSANEScalers = new SANEScalers("Scalers");
   SANERunManager::GetRunManager()->GetCurrentFile()->cd();

   // What is this again?
   BETAPrimaryGeneratorAction * genAction = (BETAPrimaryGeneratorAction*) runManager->GetUserPrimaryGeneratorAction();
   genAction->SetMCEventAddress( fSimulationManager->fEvents->MC);
   genAction->InitOutput();

   // Creates the detector classes
   fSimulationManager->AddDetectors(fRunNumber);

   fCurrentRun = new BETARun( fRunNumber );

   // Simulate Pedestals before entering actual simulation
   fSimulationManager->UpdateRun();
   fCurrentRun->GeneratePedestals();

   fSimulationManager->UpdateRun();

   fSimulationManager->fInSANERun->Print();
   fSimulationManager->PrintSummary();

   return (G4Run*)fCurrentRun;
}
//_________________________________________________________________//

/** 
 *
 *
 */
void BETARunAction::EndOfRunAction ( const G4Run* aRun ) {

      if(fSimulationManager->fSANEScalers->fTriggerEvent){
         fSimulationManager->fSANEScalers->fTriggerEvent->fCodaType = 0;
         fSimulationManager->fSANEScalers->fTriggerEvent->fEventNumber = aRun->GetNumberOfEvent()+1;
         std::cout << " LAST EVENT : " << aRun->GetNumberOfEvent()+1 << std::endl;
         fSimulationManager->fSANEScalers->fTriggerEvent->fRunNumber = fSimulationManager->fRunNumber;
      }

      fSimulationManager->fScalerTree->Fill();
      fSimulationManager->fSANEScalers->ClearEvent();
      fSimulationManager->fSANEScalers->fScalerEvent->ClearEvent();

   timer->Stop();
   //         TSQLServer * db = InSANEDatabaseManager::GetManager()->GetServer();// = TSQLServer::Connect("mysql://localhost/SANE", "sane", "secret");
   //    TSQLResult * res;
   //    TDatime * dt = new TDatime();
   //    TString datetime( dt->AsSQLString() );
   //    TString date(datetime(0,10));
   //    TString time(datetime(11,8));
   // 
   //    TString SQLq("UPDATE BETAG4_run_info set "); // dont forget the extra space at the end
   // //aSQL  "Insert into BETAG4_run_info set " ;
   //    SQLq += " end_date='";
   //    SQLq += date.Data();
   //    SQLq += "', end_time='";
   //    SQLq += time.Data();
   //    SQLq += "', number_of_events=";
   //    SQLq += aRun->GetNumberOfEvent();
   //    SQLq += " Where run_number=";
   //    SQLq += fRunNumber;
   // 
   // //printf"\n %s  \n",SQLq.Data())  ;
   //    res = db->Query(SQLq.Data());
   /*   db->Close();*/
   /*
      TSQLServer *db = TSQLServer::Connect("mysql://localhost/SANE", "sane", "secret");
      TSQLResult * res;
   //const TTimeStamp * time=new TTimeStamp();
   TDatime * dt = new TDatime();
   */
   /*
      res = db->Query(
      Form("Update  BETAG4_run_info set stop_date=STR_TO_DATE(\"%04d-%02d-%02d\", '%m-%d-%Y'), stop_time=TIME_FORMAT(\"%
2d:%2d:%2d\", '%H %e %s') where run_number=%d ",
dt->GetYear(), dt->GetMonth(),dt->GetDay(), dt->GetHour(),dt->GetMinute(),dt->GetSecond(), runNumber) );
*/

   // Print interesting data
   G4cout <<"=================== END RUN #" << fRunNumber << "===================" << G4endl;
   G4cout <<"Number of Events Processed:" << aRun->GetNumberOfEvent() << " events. " <<G4endl;
   //    G4cout <<"PMT/Mirror Eff  " <<fCurrentRun->getPMT_MirrorEfficiency()  <<G4endl;
   
   G4cout << "Writing ROOT File\n";
   SANERunManager::GetRunManager()->WriteRun();

   // do these do anything anymore?
   fSimulationManager->fEvents->fTree->Write();
   fSimulationManager->fEvents->fTree->FlushBaskets();
   fSimulationManager->fEvents->fTree->BuildIndex("fRunNumber","fEventNumber");

   SANERunManager::GetRunManager()->GetScalerFile()->cd();
   fSimulationManager->fScalerTree->Write();
   fSimulationManager->fScalerTree->FlushBaskets();
   SANERunManager::GetRunManager()->GetCurrentFile()->cd();

   // Save all objects in this file
   //   fSimulationManager->fDetectorTree->Write();
   //fSimulationManager->fRootFile->Write();
   // Close the file. Note that this is automatically done when you leave
   // the application.

   //  t->BuildIndex("fRunNumber","fEventNumber");

   // fOutputTree->Write();
   // fOutputTree->FlushBaskets();
   // fAnalysisFile->Write();
   // fAnalysisFile->Flush();
   fSimulationManager->fRootFile->Close();
   assert ( 0 != fCurrentRun );
   //   currentRun->DumpData();
}

/**
 *
 *
 */
void BETARunAction::showPlot ( int show )
{
   showThePlots = show;
}
//_________________________________________________________________//
