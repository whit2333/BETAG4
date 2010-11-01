// Make this appear first!
#include "G4Timer.hh"
#include <fstream>
#include "BETARunAction.hh"
#include "assert.h"
#include "G4Run.hh"
#include "BETARun.hh"
#include "BETAAnalysisManager.hh"
#include "BETARunActionMessenger.hh"
#include "BETADetectorConstruction.hh"
#include "G4RunManager.hh"
#include "TTimeStamp.h"
#include "TDatime.h"

/**
 *
 *  Run related operations
 *    database entry
 *    run number incrementing --> run.txt file ... should just get it from a database
 *
 */
BETARunAction::BETARunAction() :runNumber ( 0 ),showThePlots ( 0 )
{
   input_file.open ( "run.txt" );
   input_file >> runNumber ;
   input_file.close();
   messenger = new BETARunActionMessenger ( this );
   timer = new G4Timer;
}

/**
 *
 *
 */
BETARunAction::~BETARunAction()
{
   delete timer;
}

/**
 *
 *
 */
void BETARunAction::BeginOfRunAction ( const G4Run* aRun )
{
//G4cout <<"START of RUN ACTION"<< G4endl;
   G4cout <<"=================== RUN #" << runNumber << "===================" << G4endl;
}

/**
 *
 *  Fills the database as much as possible
 *  Grabs the runmanager and
 *
 */
G4Run*  BETARunAction::GenerateRun() {
   G4cout << "RunAction - GenerateRun" << G4endl;
   runNumber++;
   output_file.open ( "run.txt" ,ios::trunc); // this incremtents a number so that it acts like a normal DAQ
   output_file << runNumber ;
   output_file.close();

   timer->Start();

   G4cout<<"Creating user define run class BETARun"<<G4endl;
// analysisManager = BETAAnalysisManager::getInstance(runNumber);
   G4RunManager *   runManager = G4RunManager::GetRunManager();

// Get the runmanager and constructors/messengers and only fill the database if
// you have them!
   double targetAngle = 0.0;
   if (runManager) {
      BETADetectorConstruction *  construction = (BETADetectorConstruction *)runManager->GetUserDetectorConstruction();
      if (construction) {
         targetAngle = construction->myField->fUVAMagnet->fPolarizationAngle*180.0/pi;
         //printf("\n got run manager %f\n",targetAngle);

/// FILL THE DATABASE
         TSQLServer * db = TSQLServer::Connect("mysql://localhost/SANE", "sane", "secret");
         TSQLResult * res;


         TString SQLq("Insert into BETAG4_run_info set "); // dont forget the extra space at the end
//aSQL  "Insert into BETAG4_run_info set " ;

         SQLq +=" run_number=" ; // Dont add space here
         SQLq +=runNumber;
// starting Date and time
         TDatime * dt = new TDatime();
         TString datetime( dt->AsSQLString() );
         TString date(datetime(0,10));
         TString time(datetime(11,8));

         SQLq += ", start_date='";
         SQLq += date.Data();
         SQLq += "', start_time='";
         SQLq += time.Data();
// SQLq.Append(Form(" start_date='%s', start_time='%s' ",date.Data(),time.Data() ));
// SQLq.Append(Form(" start_date='%s', start_time='%s' ",date.Data(),time.Data() ));
//  res = db->Query(Form("Insert into BETAG4_run_info set run_number=%d, start_date='%s',
//  start_time='%s',  target_angle=%f ", runNumber,date.Data(), time.Data(), targetAngle));

         SQLq += "', BETAg4_Lucite_On=" ;
         SQLq += (int)construction->usingLuciteHodoscope;

         SQLq += ", BETAg4_Bigcal_on=" ;
         SQLq += (int)construction->usingBigcal;

         SQLq += ", Betag4_tracker_on=" ;
         SQLq += (int)construction->usingForwardTracker;

         SQLq += ", betag4_cherenkov_on=" ;
         SQLq += (int)construction->usingGasCherenkov;

         SQLq += ", betag4_bigcal_plane_on=" ;
         SQLq += (int)construction->usingFakePlaneAtBigcal;

// Fill MySQL table
         res = db->Query(SQLq.Data());

         cout << SQLq.Data();

         db->Close();
/// DONE with database

      }
   }
   currentRun = new BETARun ( runNumber,showThePlots );
   return currentRun;
}

/**
 *
 *
 */
void BETARunAction::EndOfRunAction ( const G4Run* aRun )
{
//theRun->getRunAnalysisManager()->getTree()->commit();
// BETAAnalysisManager::dispose();
   timer->Stop();
   TSQLServer *db = TSQLServer::Connect("mysql://localhost/SANE", "sane", "secret");
   TSQLResult * res;
   TDatime * dt = new TDatime();
   TString datetime( dt->AsSQLString() );
   TString date(datetime(0,10));
   TString time(datetime(11,8));

   TString SQLq("UPDATE BETAG4_run_info set "); // dont forget the extra space at the end
//aSQL  "Insert into BETAG4_run_info set " ;
   SQLq += " end_date='";
   SQLq += date.Data();
   SQLq += "', end_time='";
   SQLq += time.Data();
   SQLq += "', number_of_events=";
   SQLq += aRun->GetNumberOfEvent();
   SQLq += " Where run_number=";
   SQLq += runNumber;

//printf"\n %s  \n",SQLq.Data())  ;
   res = db->Query(SQLq.Data());
   db->Close();
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
   G4cout <<"=================== END RUN #" << runNumber << "===================" << G4endl;
   G4cout <<"Number of Events Processed:" <<aRun->GetNumberOfEvent() << " events. " <<G4endl;
   G4cout <<"PMT/Mirror Eff  " <<currentRun->getPMT_MirrorEfficiency()  <<G4endl;

   assert ( 0 != currentRun );
   currentRun->DumpData();


}

/**
 *
 *
 */
void BETARunAction::showPlot ( int show )
{
   showThePlots = show;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
