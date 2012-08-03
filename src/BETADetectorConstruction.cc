/////////////////////////////////////////////////////////////////////
//
//   DetectorConstruction
//   Big Electron Telescope Array ( BETA ) for the SANE experiment
//
//   The detector package consists of a forward tracker, Gas
//   Cherenkov, Lucite Hodoscope and Calorimeter (BIGCAL)
//
//   author Whitney Armstrong (whit@temlpe.edu)
//
/////////////////////////////////////////////////////////////////////
// VGM
//#ifdef VGM_SYSTEM
//#include "Geant4GM/volumes/Factory.h" 
//#include "RootGM/volumes/Factory.h" 
//#include "XmlVGM/GDMLExporter.h"
//#endif
#include "BETAG4BigcalSD.hh"
#include "TGeoManager.h"
#include "TROOT.h"
#include "TSQLServer.h"
#include "TSQLResult.h"
#include "G4PropagatorInField.hh"
#include "G4RunManager.hh"
#include "fstream"
#include "math.h"
#define PI 3.141592654
#include "BETAPMT.hh"
#include "BETAMirror.hh"
#include "BETARCSCellParameterisation.hh"
#include "BETAProtvinoCellParameterisation.hh"
#include "BETAForwardTrackerCellParameterisation.hh"
#include "BETARCSCalorimeter.hh"
#include "BETAProtvinoCalorimeter.hh"
#include "BETAHodoscopeCellParameterisation.hh"
#include "BETAForwardTracker.hh"
#include "BETADetectorMessenger.hh"
#include "BETASimulationManager.hh"
#include "BETAFakePlane.hh"
#include "BETAFakePlaneHit.hh"
#include "TRint.h"
#define MAGROT PI*-0./180. 
//Rotate the whole target magnet by some angle
#include "G4PVReplica.hh"
#include "BIGCALGeometryCalculator.h"
#include "UVAOxfordMagneticField.h"
#include "BETAField.hh"
#include "BETAG4PMTArray.hh"
#include "BETASimulationManager.hh"
#include "BETADetectorMessenger.hh"
#include "BETADetectorConstruction.hh"
#include "G4VPVParameterisation.hh"
//___________________________________________________________________

BETADetectorConstruction::BETADetectorConstruction() : constructed ( false )
{

   fSimulationManager = BETASimulationManager::GetInstance();
   messenger = new BETADetectorMessenger ( this );
   fMagneticField = 0;

   usingGasCherenkov       = true;
   usingBigcal             = true;
   usingLuciteHodoscope    = true;
   usingForwardTracker     = true;
   usingFakePlaneAtBigcal  = true;
   usingFakePlaneAtForwardTracker = true;

   fIsBlackLineVis = true;

   tracker_log=0;
   tracker_phys=0;
   fTrackerHorizBar_log=0;
   fTrackerVertBar_log=0;
   fTrackerHorizBarScore_log=0;
   fTrackerVertBarScore_log=0;
   trackerY1_log=0;
   trackerY2_log=0;
   trackerX1_log=0;
   frontTrackerSD=0;

   tank_log=0;
   cherenkovTank_phys=0;
   cherenkovContainer_log=0;
   fCerFrontWindow_log=0;
   fCerBackWindow_log=0;
   AlCans_log=0;
   fCerFrameTopBottom_log=0;
   fCerToroidalMirror_log=0;
   fCerSphericalMirror_log=0;
   MUPipe_log=0;
   container_log=0;
   pmtFace_log=0;

   hodoscopeContainerBox_log=0;
   hodoscopeContainerBox_phys=0;
   fLuciteHodoPMTphotocathode_log=0;
   fLuciteHodoPMTinquotes_log=0;
   fLuciteHodoBar_log=0;
   HodoscopePMTSD=0;

   calorimeterTop_log=0;
   calorimeterTop_phys=0;
   calorimeterBottom_log=0;
   calorimeterBottom_phys=0;
   cellLogicalBottom=0;
   cellLogical=0;

   fBigCalFakePlane_log = 0;
   fTrackerFakePlane_log = 0;

   fHeBagExtenderBox_log = 0;
   fHeBagExtenderAngle1_log=0;
   fHeBagExtenderAngle2_log=0;
   fHeBagExtenderAngle3_log=0;
   fHeBagExtenderAngle4_log=0;
   fHeBagExtenderHorizSupport_log=0;
   fHeBagExtenderVertSupport_log=0;
   fHeBagExtenderHorizWindow_log=0;
   fHeBagExtenderVertWindow_log=0;
   fHeBagExtenderFrontWindow_log=0;

   logicTCan=0;
   logicRad=0;
   logicWinCan=0;
   logicBeamWin=0;
   logicLN2Shield=0;
   logicLN2BeamWin=0;
   logicLN2Can=0;
   logicHelium=0;
   logicNose=0;
   logicTail=0;
   logicCell=0;
   logicCWall=0;
   logicMagnet=0;
   logicCoil=0;
   logicBrace1=0;
   logicBrace2=0;
   logicBrace3=0;
   logicBrace4=0;

   expHall_x = expHall_y = expHall_z = 10.0*m;

// These should be in the cherenkov construction
   // alpha and beta for mirrors
   alpha2 =-14.0*pi/180.0; //16.35*pi/180,   // 14
   beta2 = -32.5*pi/180.0;//31.7*pi/180;  // 32.5
   alpha4 = -7.50000048*pi/180.0;//8.75*pi/180; //7.50000048
   beta4 = -33.0*pi/180.0; // 32.62*pi/180;  // 33.0
   alpha6 = 7.50000048*pi/180.0; //-8.75*pi/180; //trial1: 9.1 //-.50000048
   beta6 = -32.5*pi/180.0;//32.25*pi/180; // trial2: 32.25 //32.5
   alpha8 = 14.0*pi/180.0;//-16.35*pi/180; // -14
   beta8 = -32.5*pi/180.0;//31.7*pi/180; //32.5*

   alpha1 = -3.5*pi/180.0;
   beta1= -9.0*pi/180.0;
   alpha3 =  2.0*pi/180.0;
   beta3 =  -9.0*pi/180.0;
   alpha5 = -2.0*pi/180.0;
   beta5 =  -9.0*pi/180.0;
   alpha7 = 4.0*pi/180.0;
   beta7 =  -9.0*pi/180.0;

   DetectorAngle = 40.*pi/180.;
   DetectorLength = 5.0*m;
   DetectorWidth  = 75.*2.54*cm+0.5*m;
   rTarget = 50.*cm;
   rTracker= 55.*cm;
   rCherenkov= 60.*cm;
   rHodoscope = 240.*cm;
   rBigcal = 345.*cm;


/// Copy of Justin Wrights Code:

   //define some OVC constants
   OVCIR = 45.60*cm;               //inner radius of can
   OVCCthick = 2.3749*cm;          //thickness of can (0.935 inch Al)
   OVCCheight = 77.47*cm;          //length of can along axis
   OVCWthick = 0.03302*cm;         //window thickness (0.013 inch)
   OVCWheight = 46.00*cm;          //height of window along can axis
   OVCBrad = 5.08*cm;              //beam window radius (2.00 inches)
   OVCBthick = 0.0508*cm;          //beam window thickness (0.02 inch Be)
   OVCdis = 0.0*cm;//1.8542*cm;   //The displacement of the OVC windows in the Z direction

   //define some LN2 shield constants
   LN2IR = 41.50*cm;               //inner radius of can
   LN2Cthick = 2.3749*cm;          //thickness of can (0.935 inch Al)
   LN2Cheight = 78.74*cm;          //length of can along axis (30.5 inches)
   LN2Wthick = 0.00508*cm;         //window thickness (0.002 inch)
   LN2Wheight = 44.00*cm;          //height of window along can axis
   LN2Brad = 5.715*cm;             //beam window radius (2.25 inches)
   LN2Bthick = 0.03302*cm;         //beam window thickness (0.013 inch Be)
   LN2dis = 7.2898*cm;             //vertical displacement of windows and
   //target from center of can.

   //define some constants for the nosepiece and target cell
   HeIR = 4.*cm;                   //Inner radius of the 4K He shield
   HeShThick = .00381*cm;          //Thickness of the 4K shield
   HeLength = LN2Cheight;           //Length of the 4K shield (same as LN2 shield)
   NoseIR = ( 2.1-.00254 ) *cm;    //Inner radius of the Nose
   NoseThick = .00254*2*cm;        //Thickness of the Nosepiece
   NoseLength = LN2Cheight;        //Length of the nosepiece (same as LN2Shield)
   CellOR = 1.25*cm;               //OuterRadius of the cell
   CellLength = 3.0*cm;            //Axial length of the cell
   CellThick = 0.0127*cm;          //Thickness of the cell wall

   ULimits = 200.*MeV;


   fTargetState = 1;
   physiTCan=0;
   physiLN2Can=0;

   ConstructVisAtt();
}

//___________________________________________________________________
BETADetectorConstruction::~BETADetectorConstruction()
{
   delete messenger;
}

void BETADetectorConstruction::ConstructVisAtt(){

   fBlackLineVisAtt = new G4VisAttributes ( G4Colour::Black() );
   fBlackLineVisAtt->SetForceWireframe( true );
   fBlackLineVisAtt->SetDaughtersInvisible( false );

   fInvisibleVisAtt = new G4VisAttributes();
   fInvisibleVisAtt->SetVisibility ( false );

   /// using color scheme from web ... 
   /*
   #####  Color Palette by Color Scheme Designer
   #####  Palette URL: http://colorschemedesigner.com/#3K61Tw0w0w0w0
   #####  Color Space: RGB; 
   *** Primary Color:
   var. 1 = #1142AA = rgb(17,66,170)
   var. 2 = #2A4580 = rgb(42,69,128)
   var. 3 = #06276F = rgb(6,39,111)
   var. 4 = #4573D5 = rgb(69,115,213)
   var. 5 = #6C8DD5 = rgb(108,141,213)
   */
   fPrimaryColorLineVisAtt = new G4VisAttributes( G4Colour( 17.0/255.0,66.0/255.0,170.0/255.0 ) );
   fPrimaryColorLineVisAtt->SetForceWireframe( true );

   /*
   *** Secondary Color A:
   var. 1 = #3714B0 = rgb(55,20,176)
   var. 2 = #402C84 = rgb(64,44,132)
   var. 3 = #1F0772 = rgb(31,7,114)
   var. 4 = #6949D7 = rgb(105,73,215)
   var. 5 = #866FD7 = rgb(134,111,215)
   */
   fSecondaryColorALineVisAtt = new G4VisAttributes( G4Colour (55.0/255.0,20.0/255.0,176.0/255.0 ) );
   fSecondaryColorALineVisAtt->SetForceWireframe ( true );
   /*
   *** Secondary Color B:
   var. 1 = #009B95 = rgb(0,155,149)
   var. 2 = #1D7471 = rgb(29,116,113)
   var. 3 = #006561 = rgb(0,101,97)
   var. 4 = #33CDC7 = rgb(51,205,199)
   var. 5 = #5CCDC9 = rgb(92,205,201)
   */
   fSecondaryColorBLineVisAtt = new G4VisAttributes ( G4Colour ( 0.0/255.0,155.0/255.0,149.0/255.0) );
   fSecondaryColorBLineVisAtt ->SetForceWireframe ( true );
   /*
   *** Complementary Color:
   var. 1 = #FFA900 = rgb(255,169,0)
   var. 2 = #BF8F30 = rgb(191,143,48)
   var. 3 = #A66E00 = rgb(166,110,0)
   var. 4 = #FFBE40 = rgb(255,190,64)
   var. 5 = #FFCF73 = rgb(255,207,115)
   */
   fComplementaryColorLineVisAtt = new G4VisAttributes ( G4Colour ( 255.0/255.0,169.0/255.0,0.0/255.0) );
   fComplementaryColorLineVisAtt->SetForceWireframe ( true );


   AlVisAtt = new G4VisAttributes ( G4Colour ( .5,.5,.5 ) );
   AlVisAtt->SetVisibility ( true );

   Invisible = new G4VisAttributes();
   Invisible->SetVisibility ( false );

   LeadVis = new G4VisAttributes ( G4Colour ( .8,.8,.8 ) );
   LeadVis->SetVisibility ( true );
   LeadVis->SetForceSolid ( true );

   Opaque = new G4VisAttributes ( G4Colour ( .3,.5,.7 ) );
   Opaque->SetForceSolid ( true );

}

void BETADetectorConstruction::SetVisAtt(){

   if(fIsBlackLineVis) {
      
      /// BigCal - individual cells are invisible
      if(calorimeterTop_log)calorimeterTop_log->SetVisAttributes(fBlackLineVisAtt);
      if(calorimeterBottom_log)calorimeterBottom_log->SetVisAttributes(fBlackLineVisAtt);
      if(cellLogical)cellLogical->SetVisAttributes(fInvisibleVisAtt);
      if(cellLogicalBottom)cellLogicalBottom->SetVisAttributes(fInvisibleVisAtt);

      /// Cherenkov - tank invisible
      if(tank_log)tank_log->SetVisAttributes(fBlackLineVisAtt);
      if(fCerFrontWindow_log)fCerFrontWindow_log->SetVisAttributes(fBlackLineVisAtt);
      if(fCerBackWindow_log)fCerBackWindow_log->SetVisAttributes(fBlackLineVisAtt);
      if(AlCans_log)AlCans_log->SetVisAttributes(fInvisibleVisAtt);
      if(fCerFrameTopBottom_log)fCerFrameTopBottom_log->SetVisAttributes(fBlackLineVisAtt);
      if(fCerToroidalMirror_log)fCerToroidalMirror_log->SetVisAttributes(fBlackLineVisAtt);
      if(fCerSphericalMirror_log)fCerSphericalMirror_log->SetVisAttributes(fBlackLineVisAtt);
      if(container_log)container_log->SetVisAttributes(fInvisibleVisAtt);
      if(MUPipe_log)MUPipe_log->SetVisAttributes(fBlackLineVisAtt);
      if(pmtFace_log)pmtFace_log->SetVisAttributes(fBlackLineVisAtt);

      /// Forward Tracker - 
      if(tracker_log)tracker_log->SetVisAttributes(fInvisibleVisAtt);
      if(fTrackerHorizBar_log)fTrackerHorizBar_log->SetVisAttributes(fInvisibleVisAtt);
      if(fTrackerVertBar_log)fTrackerVertBar_log->SetVisAttributes(fInvisibleVisAtt);
      if(fTrackerHorizBarScore_log)fTrackerHorizBarScore_log->SetVisAttributes(fInvisibleVisAtt);
      if(fTrackerVertBarScore_log)fTrackerVertBarScore_log->SetVisAttributes(fInvisibleVisAtt);
      if(trackerY1_log)trackerY1_log->SetVisAttributes(fBlackLineVisAtt);
      if(trackerY2_log)trackerY2_log->SetVisAttributes(fBlackLineVisAtt);
      if(trackerX1_log)trackerX1_log->SetVisAttributes(fBlackLineVisAtt);

      /// Lucite Hodoscoope
      if(hodoscopeContainerBox_log)hodoscopeContainerBox_log->SetVisAttributes(fInvisibleVisAtt);
      if(fLuciteHodoPMTphotocathode_log)fLuciteHodoPMTphotocathode_log->SetVisAttributes(fInvisibleVisAtt);
      if(fLuciteHodoPMTinquotes_log)fLuciteHodoPMTinquotes_log->SetVisAttributes(fInvisibleVisAtt);
      if(fLuciteHodoBar_log)fLuciteHodoBar_log->SetVisAttributes(fBlackLineVisAtt);

      /// Fake Scoring Planes - invisible
      if(fBigCalFakePlane_log)fBigCalFakePlane_log->SetVisAttributes(fInvisibleVisAtt);
      if(fTrackerFakePlane_log)fTrackerFakePlane_log->SetVisAttributes(fInvisibleVisAtt);

      /// Helium bag - invisible
      if(fHeBagExtenderBox_log)fHeBagExtenderBox_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderAngle1_log)fHeBagExtenderAngle1_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderAngle2_log)fHeBagExtenderAngle2_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderAngle3_log)fHeBagExtenderAngle3_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderAngle4_log)fHeBagExtenderAngle4_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderHorizSupport_log)fHeBagExtenderHorizSupport_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderVertSupport_log)fHeBagExtenderVertSupport_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderHorizWindow_log)fHeBagExtenderHorizWindow_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderVertWindow_log)fHeBagExtenderVertWindow_log->SetVisAttributes(fInvisibleVisAtt);
      if(fHeBagExtenderFrontWindow_log)fHeBagExtenderFrontWindow_log->SetVisAttributes(fInvisibleVisAtt);

      /// Polarized Target Can 
      if(logicTCan)logicTCan->SetVisAttributes(fBlackLineVisAtt);
      if(logicRad)logicRad->SetVisAttributes(fInvisibleVisAtt);
      if(logicWinCan)logicWinCan->SetVisAttributes(fInvisibleVisAtt);
      if(logicBeamWin)logicBeamWin->SetVisAttributes(fInvisibleVisAtt);

      /// Polarized Target LN2 shield 
      if(logicLN2Shield)logicLN2Shield->SetVisAttributes(fInvisibleVisAtt);
      if(logicLN2BeamWin)logicLN2BeamWin->SetVisAttributes(fInvisibleVisAtt);
      if(logicLN2Can)logicLN2Can->SetVisAttributes(fInvisibleVisAtt);
      if(logic4KSH)logic4KSH->SetVisAttributes(fInvisibleVisAtt);
      if(logicHelium)logicHelium->SetVisAttributes(fInvisibleVisAtt);

      /// Polarized Target NosePiece and Target Cell
      if(logicNose)logicNose->SetVisAttributes(fInvisibleVisAtt);
      if(logicTail)logicTail->SetVisAttributes(fInvisibleVisAtt);
      if(logicCell)logicCell->SetVisAttributes(fInvisibleVisAtt);
      if(logicCWall)logicCWall->SetVisAttributes(fInvisibleVisAtt);

      /// Polarized Target Magnet
      if(logicMagnet)logicMagnet->SetVisAttributes(fInvisibleVisAtt);
      if(logicCoil)logicCoil->SetVisAttributes(fBlackLineVisAtt);
      if(logicBrace1)logicBrace1->SetVisAttributes(fBlackLineVisAtt);
      if(logicBrace2)logicBrace2->SetVisAttributes(fBlackLineVisAtt);
      if(logicBrace3)logicBrace3->SetVisAttributes(fBlackLineVisAtt);
      if(logicBrace4)logicBrace4->SetVisAttributes(fBlackLineVisAtt);

   }
   /*calorimeterBottom_log->SetVisAttributes(fBlackLineVisAtt);
   calorimeterBottom_log->SetVisAttributes(fBlackLineVisAtt);*/

}

//___________________________________________________________________
void BETADetectorConstruction::ConstructForwardTracker()
{
   ////////////////////////////////////////////////////////////////
   // Front Tracker  //////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////

   //Delete existing stuff
   if(tracker_phys) delete tracker_phys;
   if(tracker_log) delete tracker_log;
   if(fTrackerHorizBar_log) delete fTrackerHorizBar_log;
   if(fTrackerVertBar_log) delete fTrackerVertBar_log;
   if(fTrackerHorizBarScore_log) delete fTrackerHorizBarScore_log;
   if(fTrackerVertBarScore_log) delete fTrackerVertBarScore_log;
   if(trackerY1_log) delete trackerY1_log;
   if(trackerY2_log) delete trackerY2_log;
   if(trackerX1_log) delete trackerX1_log;
//    if(frontTrackerSD) {frontTrackerSD->Activate(false);delete frontTrackerSD;}

   G4double smallSeparation = 0.01*mm;
   G4RotationMatrix trackerRot;
   trackerRot.rotateZ (-1.0*PI/2.0 ); 

   G4Box  *tracker_box = new G4Box ( "tracker_container_box",42.*cm/2., 24.*cm/2., 10.*mm/2. );
   tracker_log         = new G4LogicalVolume ( tracker_box,Air,"tracker_container_box_log",0,0,0 );
//   tracker_log->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 5.0*ns ) );
/*
   G4UserLimits(G4double ustepMax = DBL_MAX,
		G4double utrakMax = DBL_MAX,
		G4double utimeMax = DBL_MAX,
		G4double uekinMin = 0.,
		G4double urangMin = 0. );
fMaxStep;
// max allowed Step size in this volume
fMaxTrack;
// max total track length
fMaxTime;
// max global time
fMinEkine;
// min kinetic energy remaining (only for charged particles)
fMinRange;
// min remaining range (only for charged particles)
*/

   // check out http://www.colorschemer.com/online.html   
   G4VisAttributes* tracker_log_attr = new G4VisAttributes ( G4Colour ( 204.0/255.0, 204.0/255.0, 51.0/255.0, 0.1 ) );
   //tracker_log_attr->SetVisibility(false);
   tracker_log_attr->SetForceWireframe(true);
   tracker_log->SetVisAttributes ( tracker_log_attr );
   tracker_log->SetVisAttributes ( G4VisAttributes::Invisible );

   G4Box* trackerY1_box = new G4Box ("trackerY1_box", // Name
                                      134.0*(3.*mm/2.0+smallSeparation),         // x half length
                                      22.*cm/2.0 + 4.0*smallSeparation,         // y half length
                                      3.*mm/2.0 + smallSeparation/2.0 );     // z half length
   G4Box* trackerY2_box = new G4Box ( "trackerY2_box", // Name
                                      134.0*(3.*mm/2.0+smallSeparation),         // x half length
                                      22.*cm/2.0 + 4.0*smallSeparation,         // y half length
                                      3.*mm/2.0 + smallSeparation/2.0 );     // z half length
   G4Box* trackerX1_box = new G4Box ( "trackerY1_box", // Name
                                      40.*cm/2.0 + 4.0*smallSeparation,         // x half length
                                      74.0*(3.*mm/2.0  +  smallSeparation),         // x half length
                                      3.*mm/2.0 + smallSeparation/2.0 );     // z half length

   //----- Y Plane Scintillator and scoring surface (photon counter)
   G4Box* vertBar = new G4Box ( "tracker_Y_plane_scint", // Name
                                3.*mm/2.0,         // x half length
                                22.*cm/2.0+smallSeparation,         // y half length
                                3.*mm/2.0 );     // z half length
   G4Box* vertBarScore = new G4Box ( "tracker_Y_plane_scorer", // Name
                         3.*mm/2.0,         // x half length
                         smallSeparation,         // y half length
                         3.*mm/2.0 );     // z half length

   //----- X Plane Scintillator and scoring surface (photon counter)
   G4Box* horizBar = new G4Box ( "tracker_X_plane_scint", // Name
                          40.*cm/2.0+smallSeparation,         // x half length
                          3.*mm/2.0,         // y half length
                          3.*mm/2.0 );     // z half length

   G4Box* horizBarScore = new G4Box ( "tracker_X_plane_scorer", // Name
                          smallSeparation,         // x half length
                          3.*mm/2.0,         // y half length
                          3.*mm/2.0 );     // z half length
   
   if(fSimulationManager->fSimulateTrackerOptics) {
     fTrackerHorizBar_log      = new G4LogicalVolume( horizBar,     TrackerScint,         "tracker_fTrackerHorizBar_log" );
     fTrackerVertBar_log       = new G4LogicalVolume( vertBar,      TrackerScint,         "tracker_fTrackerVertBar_log" );
     fTrackerHorizBarScore_log = new G4LogicalVolume( horizBarScore,TrackerScint,"tracker_fTrackerHorizBarScore_log" );
     fTrackerVertBarScore_log  = new G4LogicalVolume( vertBarScore, TrackerScint,"tracker_fTrackerVertBarScore_log" );
   } else {
     fTrackerHorizBar_log      = new G4LogicalVolume( horizBar,     TrackerScint_NoOptics,"tracker_fTrackerHorizBar_log" );
     fTrackerVertBar_log       = new G4LogicalVolume( vertBar,      TrackerScint_NoOptics,"tracker_fTrackerVertBar_log" );
     fTrackerHorizBarScore_log = new G4LogicalVolume( horizBarScore,TrackerScint_NoOptics,"tracker_fTrackerHorizBarScore_log" );
     fTrackerVertBarScore_log  = new G4LogicalVolume( vertBarScore, TrackerScint_NoOptics,"tracker_fTrackerVertBarScore_log" );   
   }
  
   // check out http://www.colorschemer.com/online.html   
   G4VisAttributes* fTrackerHorizBar_log_attr = new G4VisAttributes ( G4Colour ( 204.0/255.0, 128.0/255.0, 51.0/255.0, 0.5 ) );
   fTrackerHorizBar_log_attr->SetVisibility(false);
   fTrackerHorizBar_log_attr->SetDaughtersInvisible(false);
   fTrackerHorizBar_log->SetVisAttributes ( fTrackerHorizBar_log_attr );
   fTrackerHorizBar_log->SetVisAttributes ( G4VisAttributes::Invisible );// use this to set to invisible

   G4VisAttributes* fTrackerVertBar_log_attr = new G4VisAttributes ( G4Colour ( 204.0/255.0, 51.0/255.0, 51.0/255.0, 0.5 ) );
   fTrackerVertBar_log_attr->SetVisibility(false);
   fTrackerVertBar_log_attr->SetDaughtersInvisible(false);
   fTrackerVertBar_log->SetVisAttributes ( fTrackerVertBar_log_attr );
   fTrackerVertBar_log->SetVisAttributes ( G4VisAttributes::Invisible );

   G4VisAttributes* barScore_log_attr = new G4VisAttributes ( G4Colour ( 51.0/255.0, 51.0/255.0, 204.0/255.0, 0.5 ) );
   barScore_log_attr->SetForceSolid(true);
   fTrackerVertBarScore_log->SetVisAttributes ( barScore_log_attr );
   fTrackerHorizBarScore_log->SetVisAttributes ( barScore_log_attr );

   //----- Tracker Container Placement - placed at the front of the "BETA Detector" box 
   tracker_phys = new G4PVPlacement ( G4Transform3D(trackerRot,G4ThreeVector ( 0,0,-1.0* DetectorLength/2+11.*mm/2 )),tracker_log,"tracker_phys",BETADetector_log,false,0 );

   trackerY1_log = new G4LogicalVolume ( trackerY1_box,Air,"tracker_Y1_log",0,0,0 );
   trackerY2_log = new G4LogicalVolume ( trackerY2_box,Air,"tracker_Y2_log",0,0,0 );
   trackerX1_log = new G4LogicalVolume ( trackerX1_box,Air,"tracker_X1_log",0,0,0 );

   // check out http://www.colorschemer.com/online.html   
   G4VisAttributes* tracker_Y1plane_attr = new G4VisAttributes ( G4Colour ( 204.0/255.0, 51.0/255.0, 51.0/255.0, 0.5 ) );
   //tracker_Y1plane_attr->SetForceSolid(true);
   tracker_Y1plane_attr->SetDaughtersInvisible(false);
   G4VisAttributes* tracker_Y2plane_attr = new G4VisAttributes ( G4Colour ( 204.0/255.0, 51.0/255.0, 128.0/255.0, 0.5 ) );
   //tracker_Y2plane_attr->SetForceSolid(true);
   tracker_Y2plane_attr->SetDaughtersInvisible(false);
   G4VisAttributes* tracker_X1plane_attr = new G4VisAttributes ( G4Colour ( 204.0/255.0, 51.0/255.0, 204.0/255.0, 0.5 ) );
   //tracker_X1plane_attr->SetForceSolid(true);
   tracker_X1plane_attr->SetDaughtersInvisible(false);

   trackerY1_log->SetVisAttributes ( tracker_Y1plane_attr );
   trackerY2_log->SetVisAttributes ( tracker_Y2plane_attr );
   trackerX1_log->SetVisAttributes ( tracker_X1plane_attr );

   //----- X1 Plane placed closest to the target
   G4VPhysicalVolume * trackerX1_phys =    
      new G4PVPlacement ( 0,G4ThreeVector (0,0,-3.0*mm-2.0*smallSeparation ),trackerX1_log, "tracker_X1_plane_phys", tracker_log,false,0 );
   G4VPhysicalVolume * trackerY1_phys = 
      new G4PVPlacement ( 0,G4ThreeVector (0,0,0),trackerY1_log, "tracker_Y1_plane_phys", tracker_log,false,0 );
   G4VPhysicalVolume * trackerY2_phys =    
      new G4PVPlacement ( 0,G4ThreeVector (0,0,3.0*mm+2.0*smallSeparation),trackerY2_log, "tracker_Y2_plane_phys", tracker_log,false,0 );

// VGM does not like the following replicas
//    G4VPhysicalVolume* trackerY1_phys = 
//      new G4PVReplica("trackerY1_phys", fTrackerVertBar_log,trackerY1_log, kXAxis, 132, 3.0*mm+smallSeparation);
//    G4VPhysicalVolume* trackerY2_phys = 
//      new G4PVReplica("trackerY2_phys", fTrackerVertBar_log,trackerY2_log, kXAxis, 132, 3.0*mm+smallSeparation);
//    G4VPhysicalVolume* trackerX1_phys = 
//      new G4PVReplica("trackerX1_phys", fTrackerHorizBar_log,trackerX1_log, kYAxis, 72, 3.0*mm+smallSeparation);

   // work around
   // Also sets the copy number used to get the correct scint
   for(int i0=0;i0<72;i0++) {
      new G4PVPlacement(0,G4ThreeVector (0,(3.0*mm+smallSeparation/2.0)*(Double_t)(i0-72/2),0),fTrackerHorizBar_log,
                        "TrackerHorizontalX1",trackerX1_log,true,i0);
   }
   for(int i0=0;i0<132;i0++) {
      new G4PVPlacement(0,G4ThreeVector ((3.0*mm+smallSeparation/2.0)*(Double_t)(i0-132/2),0,0),fTrackerVertBar_log,
                        "TrackerHorizontalY1",trackerY1_log,true,i0+72);
   }
   for(int i0=0;i0<132;i0++) {
      new G4PVPlacement(0,G4ThreeVector ((3.0*mm+smallSeparation/2.0)*(Double_t)(i0-132/2),0,0),fTrackerVertBar_log,
                        "TrackerHorizontalY2",trackerY2_log,true,i0+72+132);
   }
   new G4PVPlacement ( 0,G4ThreeVector (0,22.0*cm/2.-smallSeparation/2.0,0 ),fTrackerVertBarScore_log, "tracker_Y_scint_scorer_phys", fTrackerVertBar_log,true,0 );
// //    new G4PVPlacement ( 0,G4ThreeVector (0,-(22.0*cm/2.0+smallSeparation/2.0),0 ),fTrackerVertBarScore_log, "verttracker+leftBox+rightbox", fTrackerVertBar_log,true,0 );
// 
   new G4PVPlacement ( 0,G4ThreeVector (40.0*cm/2.0-smallSeparation/2.0,0,0 ),fTrackerHorizBarScore_log, "tracker_X_scint_scorer_phys", fTrackerHorizBar_log,true,0 );
// //    new G4PVPlacement ( 0,G4ThreeVector (-(40.0*cm/2.0+smallSeparation/2.0),0,0 ),fTrackerHorizBarScore_log, "horztracker+leftBox+rightbox", fTrackerHorizBar_log,true,0 );
// 
// 
// 

   const G4int NUM = 2;

   G4double pp[NUM] = {2.038*eV, 4.144*eV};
   G4double specularlobe[NUM] = {0.3, 0.3};
   G4double specularspike[NUM] = {0.2, 0.2};
   G4double backscatter[NUM] = {0.1, 0.1};
   G4double rindex[NUM] = {1.48, 1.48};
   G4double reflectivity[NUM] = {0.0, 0.0};
   G4double efficiency[NUM] = {1.0,1.0};
   G4double reflectivity2[NUM] = {0.9, 0.9};
   G4double efficiency2[NUM] = {0.10,0.10};

   G4MaterialPropertiesTable* SMPT = new G4MaterialPropertiesTable();
   SMPT -> AddProperty("RINDEX",pp,rindex,NUM);
//   SMPT -> AddProperty("SPECULARLOBECONSTANT",pp,specularlobe,NUM);
//   SMPT -> AddProperty("SPECULARSPIKECONSTANT",pp,specularspike,NUM);
//   SMPT -> AddProperty("BACKSCATTERCONSTANT",pp,backscatter,NUM);
   SMPT -> AddProperty("REFLECTIVITY",pp,reflectivity,NUM);
   SMPT -> AddProperty("EFFICIENCY",pp,efficiency,NUM);

   G4MaterialPropertiesTable* SMPT2 = new G4MaterialPropertiesTable();
   SMPT2 -> AddProperty("RINDEX",pp,rindex,NUM);
//   SMPT2 -> AddProperty("SPECULARLOBECONSTANT",pp,specularlobe,NUM);
//   SMPT2 -> AddProperty("SPECULARSPIKECONSTANT",pp,specularspike,NUM);
//   SMPT2 -> AddProperty("BACKSCATTERCONSTANT",pp,backscatter,NUM);
   SMPT2 -> AddProperty("REFLECTIVITY",pp,reflectivity2,NUM);
//   SMPT2 -> AddProperty("EFFICIENCY",pp,efficiency2,NUM);


   //----- Optical Surfaces
   G4OpticalSurface* forwardTrackerSurface = new G4OpticalSurface ( "TrackerBarOpticalSurface" );
     forwardTrackerSurface->SetModel ( unified );
     forwardTrackerSurface->SetType ( dielectric_dielectric );
//     forwardTrackerSurface->SetFinish (  polishedteflonair );
//     forwardTrackerSurface->SetFinish (  polished );
     forwardTrackerSurface->SetFinish ( polishedfrontpainted);
     forwardTrackerSurface->SetMaterialPropertiesTable(SMPT2);

   G4OpticalSurface* scoringSurface = new G4OpticalSurface ( "scoringTrackerBarOpticalSurface" );
     scoringSurface->SetModel ( unified );
//     scoringSurface->SetType ( dielectric_metal);
     scoringSurface->SetType ( dielectric_dielectric);
//     scoringSurface->SetFinish ( groundbackpainted);
     scoringSurface->SetFinish ( ground);
     scoringSurface->SetMaterialPropertiesTable(SMPT);
   
if(fSimulationManager->fSimulateTrackerOptics) {
   new G4LogicalSkinSurface ( "vertTrackerBarSurf",  fTrackerVertBar_log, forwardTrackerSurface );
   new G4LogicalSkinSurface ( "horizTrackerBarSurf", fTrackerHorizBar_log, forwardTrackerSurface );
   new G4LogicalSkinSurface ( "vertTrackerBarScoreSurf", fTrackerVertBarScore_log, scoringSurface );
   new G4LogicalSkinSurface ( "horizTrackerBarScoreSurf",fTrackerHorizBarScore_log, scoringSurface );
} else {
//   new G4LogicalSkinSurface ( "vertTrackerBarSurf",  fTrackerVertBar_log, scoringSurface );
//   new G4LogicalSkinSurface ( "horizTrackerBarSurf", fTrackerHorizBar_log, scoringSurface );
//   new G4LogicalSkinSurface ( "vertTrackerBarScoreSurf", fTrackerVertBarScore_log, scoringSurface );
//   new G4LogicalSkinSurface ( "horizTrackerBarScoreSurf",fTrackerHorizBarScore_log, scoringSurface );
}


// Detection
   G4SDManager* manager = G4SDManager::GetSDMpointer();
/*   manager->ListTree();*/
   if(!frontTrackerSD) { 
      frontTrackerSD =  new BETAForwardTracker ( "ForwardTracker" );
   // Register detector with manager
      manager->AddNewDetector ( frontTrackerSD );
   }
   // Attach detector to scoring volume
//    fTrackerHorizBar_log->SetSensitiveDetector ( fSimulationManager->fTrackerDetector );
//    fTrackerVertBar_log->SetSensitiveDetector ( fSimulationManager->fTrackerDetector );
   if(fSimulationManager->fSimulateTrackerOptics) {
      fTrackerHorizBarScore_log->SetSensitiveDetector ( frontTrackerSD );
      fTrackerVertBarScore_log->SetSensitiveDetector ( frontTrackerSD );
   }

////// end tracker


////// Visualization
/*
   trackerX1_log->SetVisAttributes(G4Colour(0.2,0.6,0.0));
trackerY1_log->SetVisAttributes(G4Colour(0.6,0.2,0.));
trackerY2_log->SetVisAttributes(G4Colour(0.6,0.2,0.));
fTrackerHorizBarScore_log->SetVisAttributes(G4Colour(0.0,0.0,0.8));
fTrackerVertBarScore_log->SetVisAttributes(G4Colour(0.0,0.,0.80));
   G4VisAttributes* FTAttributes = new G4VisAttributes ( G4Colour ( 0.5,0.2,0.2,0.1 ) );
   tracker_log->SetVisAttributes ( FTAttributes );
//   trackerDummyCell_log->SetVisAttributes (  G4VisAttributes::Invisible  );
  G4VisAttributes* lightCollectionFTAttributes = new G4VisAttributes ( G4Colour ( 0.0,0.2,0.8,0.80 ) );
lightCollectionFTAttributes->SetForceSolid(true);
fTrackerHorizBar_log->SetVisAttributes(lightCollectionFTAttributes);
fTrackerVertBar_log->SetVisAttributes(lightCollectionFTAttributes);
*/
}
//___________________________________________________________________


/**
 * Lucite Hodoscope construction.
 * 
 * This **Should** do the following:
 * Get info on geometry from lucite geo DB
 * Make all construction based on these inputs.
 * Make the logical volume .
 * Place the volume physically
 * Register UI commands to adjust parameters and rebuild.
 * 
 * Possible other things:
 * Visualization: look member functions, one see through or "naked",  
 *                one for a fully opaque real looking "picutre", 
 *                one a little naked to see nice geometry and events, etc...
 * 
 *
*/
void BETADetectorConstruction::ConstructHodoscope()
{

   if(hodoscopeContainerBox_phys)delete hodoscopeContainerBox_phys;
   if(hodoscopeContainerBox_log)delete hodoscopeContainerBox_log;
   if(fLuciteHodoPMTphotocathode_log)delete fLuciteHodoPMTphotocathode_log;
   if(fLuciteHodoPMTinquotes_log)delete fLuciteHodoPMTinquotes_log;
   if(fLuciteHodoBar_log)delete fLuciteHodoBar_log;

   G4Box * hodoscopeContainerBox = new G4Box ( "hodoscope",75.0*cm ,28.0*6.4*cm/2.0,30.0*cm );
   hodoscopeContainerBox_log = new G4LogicalVolume ( hodoscopeContainerBox, Air ,"hodoscope_log" );



   G4double hodocenter = -DetectorLength/2 -50.*cm+ 240.*cm;
   G4RotationMatrix rotOldCoords;
//   rotOldCoords.rotateX ( pi/2. );

   hodoscopeContainerBox_phys = new G4PVPlacement ( 0,G4ThreeVector ( 0,0,hodocenter+5.0*cm+25*cm  ), hodoscopeContainerBox_log,  "hodoscope_Physical",  BETADetector_log, false, 0 );

//G4Box * hodoscopePMT = new G4Box("hodoscopePMTinquotes", 6.*cm/2.,std::sqrt(2.*3.5*3.5)*cm/2.,std::sqrt(2.*3.5*3.5)*cm/2.);
//G4LogicalVolume * hodoscopePMT_log = new G4LogicalVolume(hodoscopePMT, Lucite ,"hodoscopefLuciteHodoPMTinquotes_log");


   G4RotationMatrix hodBarRotate;
   hodBarRotate.rotateZ ( -pi/2. );
   hodBarRotate.rotateX ( -pi/2. );

   /*
   G4RotationMatrix hodBarRotateRemove1;
   hodBarRotateRemove1.rotateZ((23.)*pi/(180.*2)-pi/4.);
   G4RotationMatrix hodBarRotateRemove2;
   hodBarRotateRemove2.rotateZ((-23.)*pi/(180.*2)+pi/4.);
   G4double theta = 23.*pi/(180.*2.0);
   */

   G4VSolid* hodoscope = new G4Tubs ( "Lucite-barContainer", 240.*cm-10.*cm, ( 240.+3.5 ) *cm+10.*cm,6.*28.*cm/2. +10.*cm, -26.*pi/2.0/180.,26.*pi/180. );// made a little bigger around surfaces of reflection
// Lets try a box
  G4LogicalVolume* hodoscope_log =
      new G4LogicalVolume ( hodoscope,     // Solid
                            Air,                    // Material
                            "hodoscope_Logical" ); // Name

   G4VSolid* hodoscopeSingleBarSection = new G4Tubs ( "Lucite-Singlebar", 240.*cm-10.*cm, ( 240.+3.5 ) *cm+10.*cm,6.*cm/2. , -26.*pi/2.0/180.,26.*pi/180. );// made a little bigger around surfaces of reflection


  G4RotationMatrix hodBarRotateRemove1;
  G4RotationMatrix hodBarRotateRemove2;
  G4RotationMatrix * rotate1;
  G4RotationMatrix * rotate2;
  G4VSolid * theSolid;
  G4VSolid * hodoscopeBar;
  G4VSolid * PMTinquotes;
  G4VSolid * PMTphotocathode;

  G4double theta;

  // Data members
  std::vector<G4double> xCell;
  std::vector<G4double> yCell;
  std::vector<G4double> zCell;

   rotate1 = &hodBarRotateRemove1;
   rotate2 = &hodBarRotateRemove2;
   double sideA = std::sqrt ( 2.*3.5*3.5 )  ;
   double senseThickness = 0.1 ;

   // Initialise
   G4int i ( 0 );
   theta = 23.*pi/ ( 180.*2.0 );
// First 28 are the curved bars
   for ( i=0; i<28; i++ )
   {
//for BOX mother    xCell.push_back(-240*cm);
      xCell.push_back ( 0*cm );
      yCell.push_back ( 0*cm );
      zCell.push_back ( ( i-13 ) * ( 6.0*cm+2.54*cm*0.002 ) - ( 3*cm+2.54*cm*0.002/2.0 ) );
   }
// second 28 are the angled pmt light guides
   for ( i=28; i<28*2; i++ )
   {
      xCell.push_back ( ( 240.+3.5/2.0 ) *cm*std::cos ( theta ) );
      yCell.push_back ( ( 240.+3.5/2.0 ) *cm*std::sin ( theta ) );
      zCell.push_back ( ( i-13 - 28 ) * ( 6.0*cm+2.54*cm*0.002 ) - ( 3*cm+2.54*cm*0.002/2.0 ) );
   }
// third 28 are the angled pmt light guides
   for ( i=28*2; i<28*3; i++ )
   {
      xCell.push_back ( ( 240.+3.5/2.0 ) *cm*std::cos ( theta ) );
      yCell.push_back ( -1.0* ( 240.+3.5/2.0 ) *cm*std::sin ( theta ) );
      zCell.push_back ( (G4double)( i-13 -28*2 ) * ( 6.0*cm+2.54*cm*0.002 ) - ( 3.0*cm+2.54*cm*0.002/2.0 ) );
   }


//Sensors
   for ( i=28; i<28*2; i++ )
   {
      xCell.push_back ( ( 240.+3.5/2.0+(sideA+senseThickness)/2.0 ) *cm*std::cos ( theta ) );
      yCell.push_back ( ( 240.+3.5/2.0 +(sideA+senseThickness)/2.0 ) *cm*std::sin ( theta ) );
      zCell.push_back ( ( i-13 - 28 ) * ( 6.0*cm+2.54*cm*0.002 ) - ( 3*cm+2.54*cm*0.002/2.0 ) );
   }
// third 28 are the angled pmt light guides
   for ( i=28*2; i<28*3; i++ )
   {
      xCell.push_back ( ( 240.+3.5/2.0+(sideA+senseThickness)/2.0 ) *cm*std::cos ( theta ) );
      yCell.push_back ( -1.0*( 240.+3.5/2.0 +(sideA+senseThickness)/2.0 ) *cm*std::sin ( theta ) );
      zCell.push_back ( (G4double)( i-13 -28*2 ) * ( 6.0*cm+2.54*cm*0.002 ) - ( 3.0*cm+2.54*cm*0.002/2.0 ) );
   }

 hodBarRotateRemove1.rotateZ ( ( 23. ) *pi/ ( 180.*2 )-pi/4. );

   hodBarRotateRemove2.rotateZ ( ( -23. ) *pi/ ( 180.*2 ) +pi/4. );

   theta = 23.*pi/ ( 180.*2.0 );

//   G4VSolid* hodoscope = new G4Box ( "Lucitebar-container", 45*2.54*cm/2., 45*2.54*cm/2.,6.*28.*cm/2. +10*cm );
   G4VSolid* hoBar= new G4Tubs ( "Lucitebar", 240.*cm, ( 240.+3.5 ) *cm,6.*cm/2. , -23.*pi/2.0/180.,23.*pi/180. );
   G4Box * hodoscopeRemoveBox = new G4Box ( "hodoscoperemovebox", 100.*cm,std::sqrt ( 2.*3.5*3.5 ) *cm/2.  ,100.*cm );

   hodoscopeBar = new G4SubtractionSolid ( "Frame top",hoBar , hodoscopeRemoveBox,G4Transform3D
                                           ( hodBarRotateRemove1,G4ThreeVector ( ( 240.+3.5 ) *cm*std::cos ( theta ), ( 240.+3.5 ) *cm*std::sin ( theta ),0*cm ) ) );


   hodoscopeBar = new G4SubtractionSolid ( "Frame top",hodoscopeBar, hodoscopeRemoveBox,
                                           G4Transform3D ( hodBarRotateRemove2, G4ThreeVector ( ( 240.+3.5 ) *cm*cos ( theta ),
                                                                                                -1.0* ( 240.+3.5 ) *cm*sin ( theta ),0*cm ) ) );
    PMTinquotes = new G4Box ( "hodoscopePMTinquotes", std::sqrt ( 2.*3.5*3.5 ) *cm/2.,std::sqrt ( 2.*3.5*3.5 ) *cm/2.,6.*cm/2. );
 hodoscopeBar = new G4UnionSolid ( "Frame top",hodoscopeBar, PMTinquotes,
              G4Transform3D ( hodBarRotateRemove1, 
              G4ThreeVector ( ( 240.+3.5/*/2.0+(sideA)/2.0*/ ) *cm*std::cos ( theta ),
                              ( 240.+3.5/*/2.0 +(sideA)/2.0 */) *cm*std::sin ( theta ) ,0*cm ) ) );
 hodoscopeBar = new G4UnionSolid ( "Frame top",hodoscopeBar, PMTinquotes,
              G4Transform3D ( hodBarRotateRemove2, 
              G4ThreeVector ( ( 240.+3.5/*/2.0+(sideA)/2.0*/ ) *cm*std::cos ( theta ),
                              -1.0*( 240.+3.5/*/2.0 +(sideA)/2.0*/ ) *cm*std::sin ( theta ) ,0*cm ) ) );
  fLuciteHodoBar_log =
      new G4LogicalVolume ( hodoscopeBar,     // Solid
                            Lucite,                    // Material
                            "fLuciteHodoBar_log" ); // Name


   PMTphotocathode = new G4Box ( "hodoscopePMTinquotes", /*std::sqrt ( 2.*3.5*3.5 ) */std::sqrt ( 2.*3.5*3.5 ) *cm/2.,0.1*cm/2.,6.0*cm/2. );

   fLuciteHodoPMTphotocathode_log =
      new G4LogicalVolume ( PMTphotocathode,     // Solid
                            Lucite,                    // Material
                            "fLuciteHodoPMTphotocathode_log" ); // Name


      new G4PVPlacement ( G4Transform3D ( hodBarRotateRemove1, 
              G4ThreeVector( ( 240.+3.5/*/2.0+(sideA)/2.0*/ ) *cm*std::cos ( theta ),
                              ( 240.+3.5/*/2.0 +(sideA)/2.0*/ ) *cm*std::sin ( theta ) ,0*cm ) ), fLuciteHodoPMTphotocathode_log,     // Logical volume
                       "hodoscope_sensitive",     // Name
                       fLuciteHodoBar_log,true,1);
      new G4PVPlacement ( G4Transform3D ( hodBarRotateRemove2, 
              G4ThreeVector ( ( 240.+3.5/*/2.0+(sideA)/2.0*/) *cm*std::cos ( theta ),
                             -1.0* ( 240.+3.5/*/2.0 +(sideA)/2.0*/) *cm*std::sin ( theta ) ,0*cm ) ) ,fLuciteHodoPMTphotocathode_log,     // Logical volume
                       "hodoscope_sensitive",     // Name
                       fLuciteHodoBar_log,true,2);


for(int k = 0;k<28;k++) {
      new G4PVPlacement (  G4Transform3D(hodBarRotate, 
   G4ThreeVector (0.0*cm, ( k-13 )*( 6.0*cm+2.54*cm*0.002 ) -( 3*cm+2.54*cm*0.002/2.0 ),-1.0*cm*( 240.+3.5/2.0 ) ) ) 
                      ,fLuciteHodoBar_log,     // Logical volume
                       "hodoscope_bar_and_pmt",     // Name
                       hodoscopeContainerBox_log,true,k);
}

//   G4VSolid* hodoscope = new G4Box("Lucitebar-container", 45*2.54*cm/2., 45*2.54*cm/2.,6.*28.*cm/2. +10*cm);
/*
   G4VSolid* hoBar= new G4Tubs ( "Lucitebar", 240.*cm, ( 240.+3.5 ) *cm,6.*cm/2. , -23.*pi/2.0/180.,23.*pi/180. ); 
   G4LogicalVolume* fLuciteHodoBar_log;

  if(fSimulationManager->fSimulateHodoscopeOptics) {
    fLuciteHodoBar_log = new G4LogicalVolume ( hoBar, Lucite,"hodoscopeBAAAAAR_Logical" );
  } else {
    fLuciteHodoBar_log = new G4LogicalVolume ( hoBar, Lucite_NoOptics,"hodoscopeBAAAAAR_Logical" );
  }
   G4VPhysicalVolume * hodoscope_phys = new G4PVPlacement ( G4Transform3D ( hodBarRotate,G4ThreeVector ( 0*m,0*m,-240.*cm ) ), hodoscope_log,  // Logical volume
                                                            "hodoscope_Physical",     // Name
                                                            hodoscopeContainerBox_log,             // Mother volume
                                                            false,                      // Unused boolean
                                                            0 );
   G4VPVParameterisation* HodoscopeCellParam;
   if(fSimulationManager->fSimulateHodoscopeOptics) {
     HodoscopeCellParam= new BETAHodoscopeCellParameterisation ( Lucite );
   } else {
     HodoscopeCellParam = new BETAHodoscopeCellParameterisation ( Lucite_NoOptics );
   }
// G4VNestedParameterisation* HodoscopePMTParam = new BETAHodoscopePMTParameterisation();

// uncomment for placement
   G4VPhysicalVolume * hodoscopeBar_phys =   new G4PVParameterised ( "Hodoscope_Cell_Physical",  // Name
         fLuciteHodoBar_log,        // Logical volume
         hodoscope_phys, // Mother volume
         kZAxis,             // Axis
         28*3,                // Number of replicas
         HodoscopeCellParam );        // Parameterisation
*/
//  G4VPhysicalVolume * hodoscopePMT_phys =   new G4PVParameterised("Hodoscope_PMT_Physical",    // Name
//                         hodoscopePMT_log,        // Logical volume
//                         hodoscope_phys, // Mother volume
//                         kZAxis,             // Axis
//                         28*2,                // Number of replicas
//                         HodoscopePMTParam);         // Parameterisation



// LUCITE                                   TOTAL INTERNAL REFLECTION
//    G4double LRefrac[num] = {1.49,1.49};
/// \note A 5eV photon has a wavelength of about 27nm
/// 1/(5.0 * 1.602*10^(-19)/(6.626*10^-34))/(3*10^-17)

/// \note formula From energy (eV) to wavelength (nm) is lambda=137.86/E
/// 
/// \note we hardly need to go up to 10 eV in photon energy. We should gust do ~680nm t0 ~100nm
/// which correspons to about 0.2 to 1.4
// Lucite
//
//    G4MaterialPropertiesTable* myMPTL = new G4MaterialPropertiesTable();
//    myMPTL->AddProperty ( "RINDEX", LucitePhotonEnergy, LuciteRefractiveIndex, lucitedatapoints );
//    myMPTL->AddProperty ( "ABSLENGTH",    LucitePhotonEnergy, LuciteAbsLength,     lucitedatapoints );
//    Lucite->SetMaterialPropertiesTable ( myMPTL );

// OPTICS
   const G4int num = 2;
   G4double Ephoton[num] = {0.2*eV, 1.4*eV};

   G4double RefractiveIndex[num] = {1.49, 1.49};
   G4double SpecularLobe[num]    = {0.3, 0.3};
   G4double SpecularSpike[num]   = {0.2, 0.2};
   G4double Backscatter[num]     = {0.2, 0.2};
   G4double Reflectivity[num] = {0.3, 0.5};
   G4double Efficiency[num]   = {1.0, 1.0};

   G4double EphotonLucite[2] = {0.2*eV, 1.4*eV};
   G4double LReflectivity[2] = {0.99, 0.99};
   G4double luciteRefractiveIndex[2] = {1.49, 1.49};


   G4OpticalSurface* LuciteSurface = new G4OpticalSurface ( "LuciteSurface" );

   G4MaterialPropertiesTable* LuciteSurfacePTable = new G4MaterialPropertiesTable();
   LuciteSurfacePTable->AddProperty ( "RINDEX", EphotonLucite, luciteRefractiveIndex, 2);
   LuciteSurfacePTable->AddProperty ( "REFLECTIVITY", EphotonLucite, LReflectivity, 2 );
/*   LuciteSurfacePTable->AddProperty ( "ABSLENGTH",    EphotonLucite, LuciteAbsLength,     lucitedatapoints );*/
//   LuciteSurfacePTable->AddProperty("SPECULARLOBECONSTANT",  EphotonLucite, MSpecularLobe,    num);
//   LuciteSurfacePTable->AddProperty("SPECULARSPIKECONSTANT", EphotonLucite, MSpecularSpike,   num);
//   LuciteSurfacePTable->AddProperty("BACKSCATTERCONSTANT",   EphotonLucite, MBackscatter,     num);
//   LuciteSurfacePTable->AddProperty("EFFICIENCY",   EphotonLucite, MEfficiency,   num);

   LuciteSurface->SetMaterialPropertiesTable ( LuciteSurfacePTable );
   LuciteSurface->SetModel ( unified );
   LuciteSurface->SetType ( dielectric_dielectric );
   LuciteSurface->SetFinish ( polishedbackpainted );


//     myPVolume = myLVolume->GetDaughter(i);
//for(int qq = 0; qq<28;qq++) {
//if(qq != 0) new G4LogicalBorderSurface("Border Lucite", fLuciteHodoBar_log->GetDaughter(qq-1), fLuciteHodoBar_log->GetDaughter(qq), LuciteSurface);


//uncomment FOR HODOSCOPE
//   new G4LogicalBorderSurface ( "Border Lucite1",hodoscopeBar_phys, hodoscope_phys, LuciteSurface );
  G4LogicalSkinSurface * LuciteBarSurface = new G4LogicalSkinSurface ( "LuciteBarsurface", fLuciteHodoBar_log, LuciteSurface );


// G4cout << (fLuciteHodoBar_log->GetDaughter(qq))->GetName() << G4endl;
//if(qq != 27) new G4LogicalBorderSurface("Border Lucite1", fLuciteHodoBar_log->GetDaughter(qq+1), fLuciteHodoBar_log->GetDaughter(qq), LuciteSurface);
//}


   G4SDManager* manager = G4SDManager::GetSDMpointer();

   if(!HodoscopePMTSD) {
      HodoscopePMTSD =  new BETAHodoscopePMT("LuciteHodoscope");
      // Register detector with manager
      manager->AddNewDetector(HodoscopePMTSD);
   }
   // Attach detector to scoring volume
   // fLuciteHodoBar_log->SetSensitiveDetector(fhodoscopeDet);//fSimulationManager->fHodoscopeDetector);
   if(fSimulationManager->fSimulateHodoscopeOptics)fLuciteHodoPMTphotocathode_log->SetSensitiveDetector ( HodoscopePMTSD );


   ///////////////////////////////////////////////////
   //  Visualizations
   // check out http://www.colorschemer.com/online.html   
   G4VisAttributes* hodoCont_log_attr = new G4VisAttributes ( G4Colour::Black()/*G4Colour ( 204.0/255.0, 204.0/255.0, 51.0/255.0 )*/ );
   hodoCont_log_attr->SetForceWireframe(true);
   hodoCont_log_attr->SetVisibility(false);
   hodoCont_log_attr->SetDaughtersInvisible(false);
   hodoscopeContainerBox_log->SetVisAttributes ( hodoCont_log_attr );
   //hodoscopeContainerBox_log->SetVisAttributes ( G4VisAttributes::Invisible );// use this to set to invisible

   G4VisAttributes* hodoCont2_log_attr = new G4VisAttributes ( G4Colour::Black()/*G4Colour ( 204.0/255.0, 204.0/255.0, 51.0/255.0 )*/ );
   hodoCont2_log_attr->SetForceWireframe(true);
   hodoCont2_log_attr->SetVisibility(false);
   hodoCont2_log_attr->SetDaughtersInvisible(false);
   hodoscope_log->SetVisAttributes ( hodoCont2_log_attr );
   //hodoscope_log->SetVisAttributes ( G4VisAttributes::Invisible );// use this to set to invisible

   G4VisAttributes* hodoBar_log_attr = new G4VisAttributes ( G4Colour::Black()/*G4Colour ( 228.0/255.0, 228.0/255.0, 149.0/255.0 )*/ );
   hodoBar_log_attr->SetForceSolid(true);
   //hodoBar_log_attr->SetVisibility(true);
   //hodoBar_log_attr->SetDaughtersInvisible(false);
   fLuciteHodoBar_log->SetVisAttributes ( hodoBar_log_attr );

   // check out http://www.colorschemer.com/online.html   
   G4VisAttributes* hodoPMT_log_attr = new G4VisAttributes ( G4Colour::Black()/*G4Colour ( 51.0/255.0, 51.0/255.0, 204.0/255.0) */);
   hodoPMT_log_attr->SetForceSolid(true);
   hodoPMT_log_attr->SetVisibility(true);
   fLuciteHodoPMTphotocathode_log->SetVisAttributes ( hodoPMT_log_attr );
/*
   G4VisAttributes* lucAttributes2 = new G4VisAttributes ( G4Colour ( 0.2,0.,0.5,0.5 ) );
   lucAttributes2->SetVisibility(false);
   lucAttributes2->SetDaughtersInvisible(true);
   hodoscopeContainerBox_log->SetVisAttributes(lucAttributes2   );

   G4VisAttributes* lucAttributes = new G4VisAttributes ( G4Colour ( 0.2,0.,0.5,0.5 ) );
   lucAttributes->SetVisibility(false);
   lucAttributes->SetDaughtersInvisible(true);
   hodoscope_log->SetVisAttributes( lucAttributes );

   G4VisAttributes* lucBarAttributes = new G4VisAttributes ( G4Colour ( 0.5,0.1,0.2 ) );
   fLuciteHodoBar_log->SetVisAttributes( lucBarAttributes );
//    hodoscopeContainerBox_log->SetVisAttributes ( G4VisAttributes::Invisible );
*/
}

//___________________________________________________________________
void BETADetectorConstruction::ConstructBIGCAL()
{
   //BigCal

   BIGCALGeometryCalculator * BCgeo = BIGCALGeometryCalculator::GetCalculator();

   G4double bigcalFace = BCgeo->bigcalFace*cm;// 3.45*m; // from target
   G4double bigcalFaceRelative = bigcalFace - ( DetectorLength/2.0+rTarget );

   // Sensitive detector (for all of bigcal)
   G4SDManager* manager = G4SDManager::GetSDMpointer();
   G4VSensitiveDetector* fBigcalSD =
       new BETAG4BigcalSD ( "BIGCAL" );

   // pointers used 
   G4VSolid* cellSolid=0;
   G4VPVParameterisation* cellParam;
   G4VSolid* calorimeterSolidBottom;
   G4VSolid* cellSolidBottom ;
   G4VPVParameterisation* cellParamBottom;


   // RCS Section (TOP)
   if(!calorimeterTop_log) {
   G4VSolid* calorimeterSolidTop = new G4Box ( "RCS_BOX", // Name
                                               BCgeo->rcsXSize*cm/2.0,                   // y half length
                                               BCgeo->rcsYSize*cm/2.0,                   // x half length
                                               BCgeo->rcsCellZSize*cm/2.0 ) ;            // z half length

   calorimeterTop_log =
      new G4LogicalVolume ( calorimeterSolidTop,     // Solid
                            Air,                    // Material
                            "RCS_BOX_Logical" ); // Name
   cellSolid = new G4Box ( "Cell_Solid", // Name
                                     4.02167*cm/2.0,         // x half length
                                     4.02167*cm/2.0,         // y half length
                                     40.*cm/2.0 );     // z half length

   cellLogical
   = new G4LogicalVolume ( cellSolid,     // Solid
                           LeadGlass,             // Material
                           "Cell_Logical" ); // Name

   cellParam = new BETARCSCellParameterisation();

   /*uncomment for placement */
   new G4PVParameterised ( "Cell_Physical",  // Name
                           cellLogical,        // Logical volume
                           calorimeterTop_log, // Mother volume
                           kXAxis,             // Axis
                           720,                // Number of replicas
                           cellParam );        // Parameterisation

//cellLogical->SetSensitiveDetector ( fSimulationManager->fBigcalDetector );
//    G4VSensitiveDetector* BIGCALRCS =
//       new BETARCSCalorimeter ( "BIGCALRCS" );

// Register detector with manager
   manager->AddNewDetector ( fBigcalSD );
// Attach detector to scoring volume
   cellLogical->SetSensitiveDetector ( fBigcalSD );
   }

   if(usingBigcal && calorimeterTop_phys==0) {
   calorimeterTop_phys = new G4PVPlacement ( 0,G4ThreeVector ( 
      BCgeo->rcsProtXSeparation*cm,BCgeo->bcVerticalOffset*cm+BCgeo->rcsYSize*cm/2.0,
                                         bigcalFaceRelative+40.*cm/2.0 ), calorimeterTop_log,     // Logical volume
                       "RCS_BOX_Physical",     // Name
                       BETADetector_log,             // Mother volume
                       false,                      // Unused boolean
                       0 );                        // Copy number
  }




// Protvino
   if(!calorimeterBottom_log) {


   calorimeterSolidBottom = new G4Box ( "Protvino_BOX", // Name
                                                  BCgeo->protXSize*cm/2.0,                // y half length
                                                  BCgeo->protYSize*cm/2.0,                // x half length
                                                  BCgeo->protCellZSize*cm/2.0 ) ;

   calorimeterBottom_log =
      new G4LogicalVolume ( calorimeterSolidBottom,     // Solid
                            Air,                   // Material
                            "Protvino_BOX_Logical" ); // Name



   cellSolidBottom = new G4Box ( "Cell_Solid_bottom", // Name
                                           3.8*cm/2.0,         // x half length
                                           3.8*cm/2.0,         // y half length
                                           45.*cm/2.0 );     // z half length

   cellLogicalBottom
   = new G4LogicalVolume ( cellSolidBottom,     // Solid
                           LeadGlass,             // Material
                           "Cell_Logical" ); // Name

   cellParamBottom = new BETAProtvinoCellParameterisation();

// uncomment for placement
   new G4PVParameterised ( "Cell_Physical",  // Name
                           cellLogicalBottom,        // Logical volume
                           calorimeterBottom_log, // Mother volume
                           kXAxis,             // Axis
                           1024,                // Number of replicas
                           cellParamBottom );        // Parameterisation
 //  cellLogicalBottom->SetSensitiveDetector ( fSimulationManager->fBigcalDetector );

//    G4VSensitiveDetector* BIGCALProtvino =
//       new BETAProtvinoCalorimeter ( "BIGCALBottom" );
//    // Register detector with manager
//    manager->AddNewDetector ( BIGCALProtvino );
//    // Attach detector to scoring volume
//    cellLogicalBottom->SetSensitiveDetector ( BIGCALProtvino );
//     G4VSensitiveDetector* fBigcalSDprot =
//        new BETAG4BigcalSD ( "PROT" );
//    // Register detector with manager
//    manager->AddNewDetector ( fBigcalSDprot );
   // Attach detector to scoring volume
   cellLogicalBottom->SetSensitiveDetector ( fBigcalSD );
   }  

   if(usingBigcal && calorimeterBottom_phys==0) {
   calorimeterBottom_phys = new G4PVPlacement ( 0,G4ThreeVector (BCgeo->rcsProtXSeparation*cm,BCgeo->bcVerticalOffset*cm-BCgeo->protYSize*cm/2.0,
                                         bigcalFaceRelative+45.*cm/2.0 ), calorimeterBottom_log,    // Logical volume
                       "Protvino_BOX_Physical",     // Name
                       BETADetector_log,             // Mother volume
                       false,                      // Unused boolean
                       0 );                        // Copy number
   }

// OPTICS

   G4OpticalSurface* bigcalReflecitveSurface = new G4OpticalSurface ( "BigcalLeadGlassOpticalSurface" );
     bigcalReflecitveSurface->SetModel ( unified );
     bigcalReflecitveSurface->SetType ( dielectric_dielectric );
     bigcalReflecitveSurface->SetFinish ( groundbackpainted);
//      forwardTrackerSurface->SetFinish ( polishedfrontpainted);

   G4OpticalSurface* bigcalNonReflectiveSurface = new G4OpticalSurface ( "BigcalLeadGlassOpticalSurface" );
     bigcalNonReflectiveSurface->SetModel ( unified );
     bigcalNonReflectiveSurface->SetType ( dielectric_metal);
     bigcalNonReflectiveSurface->SetFinish ( groundbackpainted);

if(fSimulationManager->fSimulateTrackerOptics) {
   new G4LogicalSkinSurface ( "protLeadGassSurface",  cellLogicalBottom, bigcalNonReflectiveSurface );
   new G4LogicalSkinSurface ( "rcsLeadGassSurface", cellLogical, bigcalNonReflectiveSurface );
} else {
   new G4LogicalSkinSurface ( "protLeadGassSurface",  cellLogicalBottom, bigcalNonReflectiveSurface );
   new G4LogicalSkinSurface ( "rcsLeadGassSurface", cellLogical, bigcalNonReflectiveSurface );
}

   G4VisAttributes* BIGCALAttributes = new G4VisAttributes (/* G4Colour::Black()*/G4Colour ( 0.0,0.5,0.5,0.5 ) );
/*   BIGCALAttributes->SetForceSolid ( false );*/
   BIGCALAttributes->SetForceWireframe( true );
   BIGCALAttributes->SetDaughtersInvisible(false  );
   calorimeterTop_log->SetVisAttributes(BIGCALAttributes);
   calorimeterBottom_log->SetVisAttributes(BIGCALAttributes);
// Make cells invisible!
//    cellLogical->SetVisAttributes ( G4VisAttributes::Invisible );
//    cellLogicalBottom->SetVisAttributes (G4VisAttributes::Invisible );
// Make containers invisible
  G4VisAttributes* BIGCALRcsProtAttributes = new G4VisAttributes (/*G4Colour::Black()*/ G4Colour ( 0.2,0.6,0.0,0.25 )  );
   calorimeterTop_log->SetVisAttributes ( BIGCALRcsProtAttributes );
   calorimeterBottom_log->SetVisAttributes ( BIGCALRcsProtAttributes);

  G4VisAttributes* BIGCALCellAttributes = new G4VisAttributes ( /*G4Colour::Black()*/G4Colour ( 0.2,0.6,0.0,0.25 )  );
  //BIGCALCellAttributes->SetForceSolid ( true );
  //BIGCALCellAttributes->SetForceWireframe( true );
//  cellLogical->SetVisAttributes(BIGCALCellAttributes );
  cellLogical->SetVisAttributes(G4VisAttributes::Invisible );
//  cellLogicalBottom->SetVisAttributes(BIGCALCellAttributes );
  cellLogicalBottom->SetVisAttributes (G4VisAttributes::Invisible );
}

//___________________________________________________________________
void BETADetectorConstruction::ConstructCherenkov()
{
// THICKNESS
   G4double frontWindowThickness = 2.0*0.002*2.54*cm;
// Target at is at (0,0,0)
// Target vacuum radius
// OVC radius = 46*cm
   G4double targetSnoutGap = 5*cm;
   G4double detectorFaceDistance = 55*cm;//55*cm;
   G4double tankVolume =0;



// Tank box dimensions
   G4double zTank = ( 25.6875 ) *2.54*cm;
   G4double yTank = 75*2.54*cm;
   G4double xTank = ( 43.4375 ) *2.54*cm;
   tankVolume = zTank*xTank*yTank+tankVolume;
// The angle the pmt face plane makes with the z axis
   G4double pmtPlaneAngle = 45*pi/180;
   G4double pmtAngle = 20*pi/180;
// Tank Front trapezdoid piece
   G4double tankFrontSide = 12.5*2.54*cm;
   G4double zTankFront  = tankFrontSide*std::cos ( pmtPlaneAngle );
   G4double xTankFrontBase = xTank;
   G4double yTankFrontBase = yTank;
   G4double xTankFrontEnd = xTank-2.*tankFrontSide*std::sin ( pmtPlaneAngle );
   G4double yTankFrontEnd = yTank;
// approx tank snout and front  volume...
   tankVolume = ( xTankFrontBase+xTankFrontEnd ) /2 * ( yTankFrontBase+yTankFrontEnd ) /2 *zTankFront+tankVolume;
// Snout
   G4double ySnoutEnd = 12.5*2.54*cm;
   G4double xSnoutEnd =9.0*2.54*cm;
// angle between snout-top plane and verticle (spherical coordinate theta)
   G4double snoutTheta = 120 *pi/180;
// angle between snout-side and z axis
   G4double snoutPhi = 80 *pi/180;
   G4double snoutSide = 37*2.54*cm;
//    G4double zSnout  = fabs ( snoutSide*std::sin ( snoutTheta ) *std::sin ( snoutPhi ) );
//    G4double ySnoutBase =ySnoutEnd+fabs ( 2*snoutSide*std::cos ( snoutTheta ) );
//    G4double xSnoutBase = xSnoutEnd +fabs ( 2*snoutSide*std::sin ( snoutTheta ) *std::cos ( snoutPhi ) );

// NEW snout
   G4double snoutThetaNEW = 30.0*pi/180.0;
   G4double snoutPhiNEW = 10.0*pi/180.0;
   G4double zSnout  = fabs ( snoutSide*std::cos ( snoutThetaNEW )  );
   G4double ySnoutBase =ySnoutEnd+fabs ( 2.0*snoutSide*std::sin ( snoutThetaNEW ) *std::cos ( snoutPhiNEW ) );
   G4double xSnoutBase = xSnoutEnd +fabs ( 2.0*snoutSide*std::sin ( snoutThetaNEW ) *std::sin ( snoutPhiNEW ) );

   tankVolume = ( xSnoutBase+xSnoutEnd ) /2 * ( ySnoutBase+ySnoutEnd ) /2 * zSnout + tankVolume;


// OLD SNOUT
// Snout
//    G4double xSnoutEnd = 12.5*2.54*cm;
//    G4double ySnoutEnd =9*2.54*cm;
// // angle between snout-top plane and verticle (spherical coordinate theta)
//    G4double snoutTheta = 120 *pi/180;
// angle between snout-side and z axis
//    G4double snoutPhi = 80 *pi/180;
//    G4double snoutSide = 37*2.54*cm;
//    G4double zSnout  = fabs ( snoutSide*std::sin ( snoutTheta ) *std::sin ( snoutPhi ) );
//    G4double xSnoutBase =xSnoutEnd+fabs ( 2*snoutSide*std::cos ( snoutTheta ) );
//    G4double ySnoutBase = ySnoutEnd +fabs ( 2*snoutSide*std::sin ( snoutTheta ) *std::cos ( snoutPhi ) );



// PMT Mount
   G4double PMTmountLength = ( 10.0 ) *2.54*cm, //0.5 is thickness of backplate
                             // discrepency of 1"(either 10 or 11?)
                             // Also here we can add the possibility of
                             // the 1.5" spacer
                             PMTmountOD = 6.625*2.54*cm,
PMTmountID = 5.75*2.54*cm,
PMTmountRadius = PMTmountOD/2.0,
PMTmountCenterLength = PMTmountLength - PMTmountRadius*std::tan ( pmtAngle ),
PMTmountXsize = 7.0*2.54*cm,
PMTmountYsize = 7.0*2.54*cm,
PMTmountBackPlateThickness = 0.5*2.54*cm;
// Panels
  G4double yBackPanel = 74.5*2.54*cm,
  xBackPanel = 43.0*2.54*cm,
  BackPanelThickness = 0.032*2.54*cm,
  ySidePanel = 74.0*2.54*cm,
  xSidePanel = 25.0*2.54*cm,
  SidePanelThickness = 0.1875*2.54*cm,
  
  yBevelPanel = 73.0*2.54*cm,
  xBevelPanel = 10.5*2.54*cm,
  BevelPanelThickness = 0.1875*2.54*cm,

  yPMTmountPanel = 73.0*2.54*cm,
  xPMTmountPanel = 12.1*2.54*cm,
  PMTmountThickness = 0.5*2.54*cm,
  PMTpanelThickness = PMTmountThickness;

  G4double 
  yTopPanelBox = 24.25*2.54*cm,
  zTopPanelBox=yTopPanelBox,
  xTopPanelBox = ( 41.0+7.0/16.0 ) *2.54*cm,
  TopPanelThickness = 0.25*2.54*cm,
  zTopPanelTrd = ( 32.0-24.25 ) *2.54*cm,
  endTopPanelTrd = 24.0*2.54*cm,

  yBottomPanelBox = 25.0*2.54*cm,
  zBottomPanelBox=yBottomPanelBox,
  xBottomPanelBox = ( 42.0+7.0/16.0 ) *2.54*cm,
  BottomPanelThickness = 0.25*2.54*cm,
  zBottomPanelTrd = ( 32.0-25.0+13.0/16.0 ) *2.54*cm,
  endBottomPanelTrd = ( 24.0+9.0/16.0 ) *2.54*cm;

///////////////////////////
// END PARAMETERS
////////////////////////////

// Detector Positioning

   G4double Extra = 3*m;
// ==============
// The Snout
// ==============
   G4double snoutCenter = rTarget+targetSnoutGap+zSnout/2;
   G4Trd * snout = new  G4Trd ( "Snout",xSnoutEnd/2, xSnoutBase/2, ySnoutEnd/2, ySnoutBase/2, zSnout/2 );
//  G4LogicalVolume * snout_log = new G4LogicalVolume(snout, NitrogenGas,"snout_log",0,0,0);
//  G4VPhysicalVolume * snout_phys = new G4PVPlacement(0,G4ThreeVector(0,0,snoutCenter), snout_log , "snout_phys",  expHall_log, false,0);

// ==============
// The tank front
// ==============
// This will be unioned with other cylinders for pmt mounts
   G4double tankFrontCenter =rTarget+targetSnoutGap+zSnout+zTankFront/2 ;
   G4Trd * tankFront = new  G4Trd ( "Front", xTankFrontEnd/2, xTankFrontBase/2, yTankFrontEnd/2, yTankFrontBase/2, zTankFront/2 );
   G4Tubs * tankPMTfar = new G4Tubs ( "PMTfar", 0, PMTmountRadius, PMTmountCenterLength,0,2*pi );
//G4Tubs * PMTmount = new G4Tubs("PMTfar", 0, PMTmountRadius, PMTmountLength,0,2*pi);
   G4Tubs * tankPMTnear = new G4Tubs ( "PMTfar", 0, PMTmountRadius, PMTmountCenterLength,0,2*pi );
   G4Box * removebox = new  G4Box ( "intersection box", 2*PMTmountCenterLength, 2*PMTmountCenterLength, 2*PMTmountCenterLength );

   G4RotationMatrix* oldCoordRot = new G4RotationMatrix;
     oldCoordRot->rotateZ ( -pi/2.0);

   G4RotationMatrix* noRot = new G4RotationMatrix;
   G4RotationMatrix* xRotNear = new G4RotationMatrix;
     xRotNear->rotateY ( pmtPlaneAngle-pmtAngle );
   G4RotationMatrix* xRotFar = new G4RotationMatrix;
     xRotFar->rotateY ( pmtPlaneAngle+pmtAngle );
   G4RotationMatrix* PMTfarROT = new G4RotationMatrix;
     PMTfarROT->rotateY ( +pmtAngle );
   G4RotationMatrix* PMTnearROT = new G4RotationMatrix;
     PMTnearROT->rotateY ( -pmtAngle );

   G4ThreeVector ZTrans ( 0, 0, -1.8*PMTmountCenterLength );

   G4ThreeVector pmt4Trans ( 
     ( xTankFrontEnd/2 + ( xTankFrontBase/2 - xTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ),
     PMTmountYsize/2,
     -PMTpanelThickness*std::sin ( pmtPlaneAngle ) 
   );

   G4ThreeVector pmt5Trans (  
     ( xTankFrontEnd/2 + ( xTankFrontBase/2 - xTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ),
     -PMTmountYsize/2,
     -PMTpanelThickness*std::sin ( pmtPlaneAngle ) 
   );

   G4ThreeVector pmt3Trans ( 
     ( xTankFrontEnd/2 + ( xTankFrontBase/2 - xTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ),
     -PMTmountYsize/2-PMTmountYsize,
     -PMTpanelThickness*std::sin ( pmtPlaneAngle ) 
   );

   G4ThreeVector pmt6Trans (  
     ( xTankFrontEnd/2 + ( xTankFrontBase/2 - xTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ),
     PMTmountYsize/2+PMTmountYsize,
     -PMTpanelThickness*std::sin ( pmtPlaneAngle ) 
   );
   G4ThreeVector pmt2Trans ( 
     ( xTankFrontEnd/2 + ( xTankFrontBase/2 - xTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ),
     -PMTmountYsize/2-2.0*PMTmountYsize,
     -PMTpanelThickness*std::sin ( pmtPlaneAngle ) 
   );

   G4ThreeVector pmt7Trans (  
     ( xTankFrontEnd/2 + ( xTankFrontBase/2 - xTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ),
     PMTmountYsize/2+2.0*PMTmountYsize,
     -PMTpanelThickness*std::sin ( pmtPlaneAngle ) 
   );
   G4ThreeVector pmt1Trans ( 
     ( xTankFrontEnd/2 + ( xTankFrontBase/2 - xTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ),
     -PMTmountYsize/2-3.0*PMTmountYsize,
     -PMTpanelThickness*std::sin ( pmtPlaneAngle ) 
   );

   G4ThreeVector pmt8Trans (  
     ( xTankFrontEnd/2 + ( xTankFrontBase/2 - xTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ),
     PMTmountYsize/2+3.0*PMTmountYsize,
     -PMTpanelThickness*std::sin ( pmtPlaneAngle ) 
   );
//    G4ThreeVector pmt3Trans ( PMTmountXsize/2+PMTmountXsize, - ( yTankFrontEnd/2 + ( yTankFrontBase/2 - yTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ), -PMTpanelThickness*std::sin ( pmtPlaneAngle ) );
// 
//    G4ThreeVector pmt6Trans ( -PMTmountXsize/2-PMTmountXsize, - ( yTankFrontEnd/2 + ( yTankFrontBase/2 - yTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ), -PMTpanelThickness*std::sin ( pmtPlaneAngle ) );

//    G4ThreeVector pmt2Trans ( PMTmountXsize/2 +2*PMTmountXsize, - ( yTankFrontEnd/2 + ( yTankFrontBase/2 - yTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ), -PMTpanelThickness*std::sin ( pmtPlaneAngle ) );
// 
//    G4ThreeVector pmt7Trans ( -PMTmountXsize/2-2*PMTmountXsize, - ( yTankFrontEnd/2 + ( yTankFrontBase/2 - yTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ), -PMTpanelThickness*std::sin ( pmtPlaneAngle ) );
// 
//    G4ThreeVector pmt1Trans ( PMTmountXsize/2+3*PMTmountXsize, - ( yTankFrontEnd/2 + ( yTankFrontBase/2 - yTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ), -PMTpanelThickness*std::sin ( pmtPlaneAngle ) );
// 
//    G4ThreeVector pmt8Trans ( -PMTmountXsize/2-3*PMTmountXsize, - ( yTankFrontEnd/2 + ( yTankFrontBase/2 - yTankFrontEnd/2 ) /2 +PMTpanelThickness*std::sin ( pmtPlaneAngle ) ), -PMTpanelThickness*std::sin ( pmtPlaneAngle ) );

   G4IntersectionSolid* PMTmountFar = new G4IntersectionSolid ( "pmt-removed unwanted union",tankPMTfar, removebox,PMTfarROT,ZTrans );
   G4IntersectionSolid* PMTmountNear = new G4IntersectionSolid ( "pmt-removed unwanted union",tankPMTnear, removebox,PMTnearROT,ZTrans );

//G4LogicalVolume * int_log = new G4LogicalVolume(PMTmount, NitrogenGas,"tankFront_log",0,0,0);
//  G4VPhysicalVolume * int_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0), int_log , "tankFront_phys",  expHall_log, false,0);



   G4UnionSolid* TANK =
      new G4UnionSolid ( "tankfront 1PMT", tankFront, PMTmountNear, xRotNear, pmt1Trans );
   TANK =
      new G4UnionSolid ( "tankfront 2PMT", TANK, PMTmountFar, xRotFar, pmt2Trans );
   TANK =
      new G4UnionSolid ( "tankfront 3PMT", TANK, PMTmountNear, xRotNear, pmt3Trans );
   TANK =
      new G4UnionSolid ( "tankfront 4PMT", TANK, PMTmountFar, xRotFar, pmt4Trans );
   TANK =
      new G4UnionSolid ( "tankfront 5PMT", TANK, PMTmountFar, xRotFar, pmt5Trans );
   TANK =
      new G4UnionSolid ( "tankfront 6PMT", TANK, PMTmountNear, xRotNear, pmt6Trans );
   TANK =
      new G4UnionSolid ( "tankfront 7PMT", TANK, PMTmountFar, xRotFar, pmt7Trans );
   TANK =
      new G4UnionSolid ( "tankfront 8PMT", TANK, PMTmountNear, xRotNear, pmt8Trans );

   G4ThreeVector snoutTrans ( 0,0,-zTankFront/2-zSnout/2 );

   TANK =
      new G4UnionSolid ( "totaltank minus box", TANK,snout, noRot, snoutTrans );

// ==============
// The tank
// ==============
// Extra is the space for the hodoscope and big cal
   G4double tankCenterDetector = DetectorLength/2 -zTank/2-1*frontWindowThickness -Extra;
   G4double tankCenter = rTarget+targetSnoutGap+zSnout+zTankFront+zTank/2;

   G4Box * tankbox = new  G4Box ( "tank", xTank/2, yTank/2, zTank/2 );
   G4ThreeVector snoutTrans2 ( 0,0,-zTank/2-zTankFront/2 );

   TANK=
      new G4UnionSolid ( "Total Tank", tankbox, TANK,noRot, snoutTrans2 );


// Add volumes for side panels
// in the PMTmount plane


   G4RotationMatrix* yRot90deg = new G4RotationMatrix;
   yRot90deg->rotateY ( pi/2 );
   G4RotationMatrix* xRot90deg = new G4RotationMatrix;
   xRot90deg->rotateX ( pi/2 );

   G4Box * TopPanelBox = new  G4Box ( "top panel", xTopPanelBox/2, TopPanelThickness/2 , zTopPanelBox/2);
   G4Trd * TopPanelTrd = new  G4Trd ( "top panel trd",  endTopPanelTrd/2, xTopPanelBox/2,TopPanelThickness/2, TopPanelThickness/2, zTopPanelTrd/2 );

   G4ThreeVector TopPanelBoxTrans ( 0, yTank/2+TopPanelThickness/2,0 );
   G4ThreeVector TopPanelTrdTrans ( 0, yTank/2+TopPanelThickness/2,-zTopPanelTrd/2-zTopPanelBox/2 );


//    TANK =
//       new G4UnionSolid ( "topPanelBox", TANK, TopPanelBox, noRot, TopPanelBoxTrans );
//    TANK =
//       new G4UnionSolid ( "topPanelTrd", TANK, TopPanelTrd, noRot, TopPanelTrdTrans );

   G4Box * BottomPanelBox = new  G4Box ( "bottom panel", xBottomPanelBox/2, BottomPanelThickness/2, zBottomPanelBox/2 );
   G4Trd * BottomPanelTrd = new  G4Trd ( "Bottom panel trd",  endBottomPanelTrd/2, yBottomPanelBox/2, BottomPanelThickness/2, BottomPanelThickness/2, zBottomPanelTrd/2 );


   G4ThreeVector BottomPanelBoxTrans ( 0, -yTank/2-BottomPanelThickness/2,0 );
   G4ThreeVector BottomPanelTrdTrans ( 0,-yTank/2-BottomPanelThickness/2,-zBottomPanelTrd/2-zBottomPanelBox/2 );

//    TANK =
//       new G4UnionSolid ( "bottomPanelBox", TANK, BottomPanelBox, noRot, BottomPanelBoxTrans );
//    TANK =
//       new G4UnionSolid ( "bottomPanelTrd", TANK, BottomPanelTrd, noRot, BottomPanelTrdTrans );



   G4Box * SidePanelBox = new  G4Box ( "side panel", xSidePanel/2, ySidePanel/2, SidePanelThickness/2 );

   G4ThreeVector SidePanelTrans ( xTank/2+SidePanelThickness/2,0,0 );

//    TANK =
//       new G4UnionSolid ( "tank_side_panel", TANK, SidePanelBox, yRot90deg, SidePanelTrans );
//    TANK =
//       new G4UnionSolid ( "tank_side_panel2", TANK, SidePanelBox, yRot90deg, -SidePanelTrans );

// Bevel Panel
   G4Box * BevelPanelBox = new  G4Box ( "bevel_panel", xBevelPanel/2, yBevelPanel/2, BevelPanelThickness/2 );

   G4ThreeVector BevelPanelTrans ( - ( xTankFrontEnd/2 +xTankFrontBase/2 ) /2-BevelPanelThickness/2*std::sin ( pmtPlaneAngle ) , 0,
                                   -zTank/2-zTankFront/2-BevelPanelThickness/2*std::cos ( pmtPlaneAngle ) );

   G4RotationMatrix *rot45degy = new G4RotationMatrix;
   rot45degy->rotateY ( -pi/4 );

//    TANK =
//       new G4UnionSolid ( "tank_bevel_panel", TANK, BevelPanelBox, rot45degy, BevelPanelTrans );

// PMT Panel
   G4Box * PMTPanelBox = new  G4Box ( "PMT_panel", xPMTmountPanel/2, yPMTmountPanel/2, PMTmountThickness/2 );

   G4ThreeVector PMTmountPanelTrans ( 
     ( xTankFrontEnd/2 +xTankFrontBase/2 ) /2+PMTmountThickness/2*std::sin ( pmtPlaneAngle ) ,
   0*cm,
                                      -zTank/2-zTankFront/2 -PMTmountThickness/2*std::cos ( pmtPlaneAngle ) );
    G4RotationMatrix *rot45degy2 = new G4RotationMatrix;
   rot45degy2->rotateY ( pi/4 );

//   TANK =
//     new G4UnionSolid("Cherenkov_Tank", TANK, PMTPanelBox, rot45degy2, PMTmountPanelTrans);


//////////////////////////////////////////////////////////////
//             Physical Placements
   G4double cherenkovFaceToCenter = zTank/2+zSnout+zTankFront;
   G4double TankPositionInDetPackage = -1.0*DetectorLength/2.0+ 5.0*cm+cherenkovFaceToCenter;

// Container Box 
   G4double cherenkovContainerZLength = zTank+zTankFront+zSnout+2.0*cm; // 2cm buffer
   G4double cherenkovContainerXLength = xTank+2.0*cm;// 2cm buffer
   G4double cherenkovContainerYLength = yTank+2.0*cm;// 2cm buffer

   G4double cherenkovPositionInContainer= -1.0*cherenkovContainerZLength/2.0+cherenkovFaceToCenter;
   G4double cherenkovContainerPosition= -1.0*DetectorLength/2.0+ 5.0*cm+cherenkovFaceToCenter;

   G4Box * cherenkovContainerBox = new  G4Box ( "cherenkovContainerBox", cherenkovContainerXLength/2.0, cherenkovContainerYLength/2.0, cherenkovContainerZLength/2.0 );

   cherenkovContainer_log = new G4LogicalVolume ( cherenkovContainerBox, Air,"cherenkovContainter_log",0,0,0 );
//    G4VPhysicalVolume * cherenkovContainer_phys = new G4PVPlacement ( 0,G4ThreeVector ( 0,0,cherenkovPositionInContainer ), tank_log , "tank_phys",  BETADetector_log, false,0 );

   if(fSimulationManager->fSimulateCherenkovOptics) {
     tank_log = new G4LogicalVolume ( TANK, NitrogenGas,"tank_log",0,0,0 );
   } else {
     tank_log = new G4LogicalVolume ( TANK, NitrogenGas_NoOptics,"tank_log",0,0,0 );
   }
   cherenkovTank_phys = new G4PVPlacement ( 0,G4ThreeVector ( 0,0,TankPositionInDetPackage ), tank_log , "tank_phys",  BETADetector_log, false,0 );

   G4Box * tedlarFront = new  G4Box ( "tedlarFrontBox", xSnoutEnd/2, ySnoutEnd/2, frontWindowThickness );
   G4Box * tedlarBack = new  G4Box ( "tedlarBackBox", xTank/2, yTank/2, frontWindowThickness );

   fCerFrontWindow_log = new G4LogicalVolume ( tedlarFront, Aluminum,"4milFrontWindow",0,0,0 );
   G4VPhysicalVolume * tedlarFront_phys = new G4PVPlacement ( 0,G4ThreeVector ( 0,0,TankPositionInDetPackage-zTank/2-zTankFront-zSnout-frontWindowThickness/2 ), fCerFrontWindow_log , "4milFrontWindow_phys",  BETADetector_log, false,0 );

   fCerBackWindow_log = new G4LogicalVolume ( tedlarBack, Aluminum,"tedlarBack",0,0,0 );
   G4VPhysicalVolume * tedlarBack_phys = new G4PVPlacement ( 0,G4ThreeVector ( 0,0,TankPositionInDetPackage+zTank/2+frontWindowThickness/2 ), fCerBackWindow_log, "tedlarBack_phys",  BETADetector_log, false,0 );

/////////////////////////////////////////////
////// ALUMINUM Pieces within TANK      /////
/////////////////////////////////////////////
//PMT panel plus the aluminum cans
   G4Tubs * PMTIDcylinder = new G4Tubs ( "PMTfar", 0, PMTmountID/2, PMTmountCenterLength,0,2*pi );
   G4Tubs * PMTODcylinder = new G4Tubs ( "PMTfar", 0, PMTmountOD/2, PMTmountCenterLength,0,2*pi );

//   G4IntersectionSolid* PMTmountFar = new G4IntersectionSolid("pmt-removed unwanted union",tankPMTfar, removebox,PMTfarROT,ZTrans);

//G4SubtractionSolid * PMTnear = new G4Subtraction("PMT removed

   G4ThreeVector PMTcanBackThickness ( 0, 0, PMTmountBackPlateThickness );
   G4ThreeVector zerotrans ( 0, 0,0 );

// NEED TO FIX NUMBERING to convention
   G4ThreeVector pmt1T ( 0 , -PMTmountYsize/2, 0 );
   G4ThreeVector pmt2T ( 0 , PMTmountYsize/2,0  );
   G4ThreeVector pmt3T ( 0 , -PMTmountYsize/2-PMTmountYsize, 0 );
   G4ThreeVector pmt4T ( 0 , PMTmountYsize/2+PMTmountYsize, 0 );
   G4ThreeVector pmt5T ( 0 , -PMTmountYsize/2-2*PMTmountYsize, 0 );
   G4ThreeVector pmt6T ( 0 , PMTmountYsize/2+2*PMTmountYsize, 0);
   G4ThreeVector pmt7T ( 0 , -PMTmountYsize/2-3*PMTmountYsize, 0 );
   G4ThreeVector pmt8T ( 0 , PMTmountYsize/2+3*PMTmountYsize,0  );


//    G4ThreeVector ZTrans(0, 0, -PMTmountLength+PMTmountRadius*std::tan(pmtAngle));
   /*  G4SubtractionSolid* PMTcanFar =
       new G4SubtractionSolid("PMT ODcylinder - IDcylinder", PMTmountFar, PMTIDcylinder,noRot,PMTcanBackThickness);
     G4SubtractionSolid* PMTcanNear =
       new G4SubtractionSolid("PMT ODcylinder - IDcylinder", PMTmountNear, PMTIDcylinder,noRot,PMTcanBackThickness);
   */

//Go DO ALL 8 Cans then Remove the middles

   G4UnionSolid*
   PMTmountAlCans = new G4UnionSolid ( "Pmt plate plus cans", PMTPanelBox, PMTmountNear, PMTfarROT, pmt1T );
   PMTmountAlCans = new G4UnionSolid ( "Pmt plate plus cans", PMTmountAlCans, PMTmountFar, PMTfarROT, pmt2T );
   PMTmountAlCans = new G4UnionSolid ( "Pmt plate plus cans", PMTmountAlCans, PMTmountFar, PMTnearROT, pmt3T );
   PMTmountAlCans = new G4UnionSolid ( "Pmt plate plus cans", PMTmountAlCans, PMTmountNear, PMTnearROT, pmt4T );
   PMTmountAlCans = new G4UnionSolid ( "Pmt plate plus cans", PMTmountAlCans, PMTmountNear, PMTfarROT, pmt5T );
   PMTmountAlCans = new G4UnionSolid ( "Pmt plate plus cans", PMTmountAlCans, PMTmountFar, PMTfarROT, pmt6T );
   PMTmountAlCans = new G4UnionSolid ( "Pmt plate plus cans", PMTmountAlCans, PMTmountFar, PMTnearROT, pmt7T );
   PMTmountAlCans = new G4UnionSolid ( "Pmt plate plus cans", PMTmountAlCans, PMTmountNear, PMTnearROT, pmt8T );

   G4ThreeVector pmt1Tbp (0 ,-PMTmountXsize/2,  PMTmountBackPlateThickness );
   G4ThreeVector pmt2Tbp (0 , PMTmountXsize/2, PMTmountBackPlateThickness );
   G4ThreeVector pmt3Tbp (0 , -PMTmountXsize/2-PMTmountXsize,  PMTmountBackPlateThickness );
   G4ThreeVector pmt4Tbp (0 , PMTmountXsize/2+PMTmountXsize,  PMTmountBackPlateThickness );
   G4ThreeVector pmt5Tbp (0 , -PMTmountXsize/2-2*PMTmountXsize,  PMTmountBackPlateThickness );
   G4ThreeVector pmt6Tbp (0 , PMTmountXsize/2+2*PMTmountXsize,  PMTmountBackPlateThickness );
   G4ThreeVector pmt7Tbp (0 , -PMTmountXsize/2-3*PMTmountXsize,  PMTmountBackPlateThickness );
   G4ThreeVector pmt8Tbp (0 , PMTmountXsize/2+3*PMTmountXsize, PMTmountBackPlateThickness );


   G4SubtractionSolid*
   pmtMountAlPanel = new G4SubtractionSolid ( "panel+pmtOD-pmtID1", PMTmountAlCans,PMTIDcylinder,PMTfarROT,pmt1Tbp );
   pmtMountAlPanel = new G4SubtractionSolid ( "panel+pmtOD-pmtID2", PMTmountAlCans,PMTIDcylinder,PMTfarROT,pmt2Tbp );
   pmtMountAlPanel = new G4SubtractionSolid ( "panel+pmtOD-pmtID3", PMTmountAlCans,PMTIDcylinder,PMTnearROT,pmt3Tbp );
   pmtMountAlPanel = new G4SubtractionSolid ( "panel+pmtOD-pmtID4", PMTmountAlCans,PMTIDcylinder,PMTnearROT,pmt4Tbp );
   pmtMountAlPanel = new G4SubtractionSolid ( "panel+pmtOD-pmtID5", PMTmountAlCans,PMTIDcylinder,PMTfarROT,pmt5Tbp );
   pmtMountAlPanel = new G4SubtractionSolid ( "panel+pmtOD-pmtID6", PMTmountAlCans,PMTIDcylinder,PMTfarROT,pmt6Tbp );
   pmtMountAlPanel = new G4SubtractionSolid ( "panel+pmtOD-pmtID7", PMTmountAlCans,PMTIDcylinder,PMTnearROT,pmt7Tbp );
   pmtMountAlPanel = new G4SubtractionSolid ( "panel+pmtOD-pmtID8", PMTmountAlCans,PMTIDcylinder,PMTnearROT,pmt8Tbp );

   G4Box * PMTremoveExtra = new  G4Box ( "PMT panel- remove", xPMTmountPanel+4*cm, yPMTmountPanel+4*cm, 1.5*PMTmountLength );
   G4ThreeVector removeExtra ( 0,0 , -1.5*PMTmountLength+PMTpanelThickness/2 );

   G4IntersectionSolid *  PMTAlCans =
      new G4IntersectionSolid ( "remove extra", pmtMountAlPanel, PMTremoveExtra,noRot,removeExtra );

   G4RotationMatrix pmtPanelRotMatrix;
   pmtPanelRotMatrix.rotateY ( -pi/4 );

// aluminum can placement
   AlCans_log = new G4LogicalVolume ( PMTAlCans, Aluminum,"AlCans_log",0,0,0 );

   G4VPhysicalVolume * AlCans_phys = new G4PVPlacement ( 
     G4Transform3D ( pmtPanelRotMatrix,
       G4ThreeVector (
         ( ( xTankFrontEnd/2 +xTankFrontBase/2 ) /2+PMTmountThickness/2*std::sin ( pmtPlaneAngle ) ) ,
         0,
         -zTank/2-zTankFront/2 -PMTmountThickness/2*std::sin ( pmtPlaneAngle ) )
     )
     , AlCans_log , "tankFront_phys",  tank_log, false,0 );


// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ALUMINUM FRAME
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
   G4RotationMatrix * rot90deg= new G4RotationMatrix;
   rot90deg->rotateX ( pi/2 );
   G4RotationMatrix * rotneg90deg= new G4RotationMatrix;
   rotneg90deg->rotateX ( -pi/2 );

   G4RotationMatrix *rot45deg= new G4RotationMatrix;
   rot45deg->rotateX ( pi/4 );
   G4RotationMatrix *rotneg45deg= new G4RotationMatrix;
   rotneg45deg->rotateX ( -pi/4 );
   G4RotationMatrix *rotZ180deg= new G4RotationMatrix;
   rotZ180deg->rotateX ( pi );

   G4RotationMatrix  Zrotneg90deg;
   Zrotneg90deg.rotateZ ( -pi/2 );

   G4double epsilon = 0.05*cm;
   G4Box * CTH6Outside = new  G4Box ( "CTH6 outside", 2.54*cm-epsilon, 2.54*cm-epsilon, zTank/2-epsilon );
   G4Box * CTH7Outside = new  G4Box ( "CTH6 outside", 2.54*cm, 2.54*cm, xTank/2 );
   G4Box * CTH5Outside = new  G4Box ( "CTH6 outside", 2.54*cm, 2.54*cm, 12.5*2.54*cm/2 );
   G4Box * CTH1Outside = new  G4Box ( "CTH6 outside", 2.54*cm-epsilon, 2.54*cm-epsilon, xTankFrontEnd/2-epsilon );

   G4Box * CTH6Inside = new  G4Box ( "CTH6 Inside", 0.75*2.54*cm-epsilon, 0.75*2.54*cm-epsilon, zTank/2-epsilon-0.25*cm );
   G4Box * CTH7Inside = new  G4Box ( "CTH6 Inside", 0.75*2.54*cm, 0.75*2.54*cm, xTank/2-0.25*cm );
   G4Box * CTH5Inside = new  G4Box ( "CTH6 Inside", 0.75*2.54*cm, 0.75*2.54*cm, 12.5*2.54*cm/2-0.25*cm );
   G4Box * CTH1Inside = new  G4Box ( "CTH6 Inside", 0.75*2.54*cm-epsilon, 0.75*2.54*cm-epsilon, xTankFrontEnd/2-epsilon-0.25*cm );

   G4ThreeVector cth61trans ( 0,xTank/2-2.54*cm,zTank/2-2.54*cm );
   G4ThreeVector cth62trans ( 0,xTank-2*2.54*cm,0 );
   G4ThreeVector cth5trans ( 0,xTank-12.5*2.54*cm*std::cos ( pi/4 ) /2-2.54*cm* ( 1+std::cos ( pi/4 ) ),-zTank/2 -12.5*cm*2.54*std::cos ( pi/4 ) /2+2.54*cm*std::cos ( pi/4 ) );

   G4UnionSolid * FrameTopOutside = new G4UnionSolid ( "cth", CTH6Outside, CTH7Outside, rot90deg,cth61trans );

   FrameTopOutside = new G4UnionSolid ( "cth", FrameTopOutside, CTH6Outside,0,cth62trans );

   FrameTopOutside = new G4UnionSolid ( "cth", FrameTopOutside, CTH5Outside, rot45deg,cth5trans );

   FrameTopOutside = new G4UnionSolid ( "cth", FrameTopOutside, CTH1Outside, rotneg90deg, G4ThreeVector ( 0, 12.5*cm*2.54*std::cos ( pi/4 ) +24.75*2.54*cm/2-2.54*cm/2,-zTank/2 -12.5*cm*2.54*std::cos ( pi/4 ) +2.54*cm ) );

   FrameTopOutside = new G4UnionSolid ( "cth", FrameTopOutside, CTH5Outside, rotneg45deg, G4ThreeVector ( 0, 12.5*cm*2.54*std::cos ( pi/4 ) /2+2.54*cm* ( std::cos ( pi/4 )-1 ),-zTank/2 -12.5*cm*2.54*std::cos ( pi/4 ) /2+2.54*cm*std::cos ( pi/4 ) ) );

   G4UnionSolid * FrameTopInside = new G4UnionSolid ( "cth", CTH6Inside, CTH7Inside, rot90deg,cth61trans );

   FrameTopInside = new G4UnionSolid ( "cth", FrameTopInside, CTH6Inside,0,cth62trans );

   FrameTopInside = new G4UnionSolid ( "cth", FrameTopInside, CTH5Inside, rot45deg,cth5trans );

   FrameTopInside = new G4UnionSolid ( "cth", FrameTopInside, CTH1Inside, rotneg90deg, G4ThreeVector ( 0, 12.5*cm*2.54*std::cos ( pi/4 ) +24.75*2.54*cm/2-2.54*cm/2,-zTank/2 -12.5*cm*2.54*std::cos ( pi/4 ) +2.54*cm ) );

   FrameTopInside = new G4UnionSolid ( "cth", FrameTopInside, CTH5Inside, rotneg45deg, G4ThreeVector ( 0, 12.5*cm*2.54*std::cos ( pi/4 ) /2+2.54*cm* ( std::cos ( pi/4 )-1 ),-zTank/2 -12.5*cm*2.54*std::cos ( pi/4 ) /2+2.54*cm*std::cos ( pi/4 ) ) );

   G4SubtractionSolid * FrameTop = new G4SubtractionSolid ( "Frame top", FrameTopOutside, FrameTopInside );

   fCerFrameTopBottom_log = new G4LogicalVolume ( FrameTop, Aluminum,"Frame-top-bottom",0,0,0 );
   G4VPhysicalVolume * test_phys = new G4PVPlacement ( G4Transform3D ( Zrotneg90deg,G4ThreeVector ( -xTank/2+2.54*cm,yTank/2-2.54*cm,0 )), fCerFrameTopBottom_log , "tankFrametop_phys",  tank_log, false,0 );
   G4VPhysicalVolume * test_phys2 = new G4PVPlacement ( G4Transform3D ( Zrotneg90deg, G4ThreeVector ( -xTank/2+2.54*cm,-yTank/2+2.54*cm,0 )), fCerFrameTopBottom_log , "tankFramebot_phys",  tank_log, false,0 );
// ~~~~~~~~~~~~~~~~~~~~~~~~
// END OF TANK CONSTRUCTION
// ~~~~~~~~~~~~~~~~~~~~~~~~




////////////////////////////////////////////////
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Spherical Mirrors (cut into rectangles)
// ---------------------------------------
////////////////////////////////////////////////

   /*
     ----------------------------
     5 Spherical mirrors (Set A):
     ----------------------------
       horiz size:    35.5 cm
             radius:  92.0 cm
       vert  size:    36.5 cm
             radius:  92.0 cm

     -------------------------------------------------------
     5 Toroidal mirrors (Set B) (sketch attached)
     -------------------------------------------------------
       horiz size:              43.0 cm
         toroid defining axis:  25.8 cm
       vert size:               36.5 cm
        toroid "tube" axis:     85.8 cm
   (Each set includes 4 mirrors plus 1 spare.)
   */
// ( above coordinates XYZ)->(below coordinates xyz)
// z -> X-150-zTank/2 , y -> -Z , x -> -Y
// mirrorx2 are the nearMirrors, mirrorx1 are the far mirrors
// This is due to the Brad's drawings using a length of 150cm while in Ed K.'s the tank is 167.49 long.
   G4double correction = 13.0*cm;
   G4double correctionFAR = 17.49*cm;
// Second attempt at PMT and Mirror Positions
   G4double TESTCorrection = 0.0*cm;
   G4double alength = zTank/2 + zSnout+ zTankFront;
   G4double zfix21 = 5.0*cos ( beta2 ) *cm, yfix21 = -5.0*sin ( beta2 ) *cm;
   G4ThreeVector 
   mirror11 (  -17.6499996*cm ,54.9375*cm,132*cm-150*cm  +correction +TESTCorrection),
   mirror21 (  -17.6499996*cm,17.8125*cm,  130.0*cm-150*cm+correction  +TESTCorrection),
   mirror31 (  -17.6499996*cm,-18.3125*cm,131.0*cm-150*cm+correction  +TESTCorrection),
   mirror41 (  -17.6499996*cm,-54.9375*cm, 132.0*cm-150*cm+correction  +TESTCorrection),

   mirror12 (  17.6499996*cm,54.4375*cm,150*cm-150*cm+correction +TESTCorrection ),
   mirror22 (  17.6499996*cm,17.8125*cm,150*cm-150*cm+correction +TESTCorrection ),
   mirror32 (  17.6499996*cm,-18.3125*cm,150.0*cm-150*cm+correction +TESTCorrection ),
   mirror42 (  17.6499996*cm,-54.9375*cm, 150.0*cm-150*cm+correction +TESTCorrection );

//The numbering above is kinda backwards
//toroidal
   mirror2Trans = mirror11;
   mirror4Trans = mirror21;
   mirror6Trans = mirror31;
   mirror8Trans = mirror41;
//spherical
   mirror1Trans = mirror12;
   mirror3Trans = mirror22;
   mirror5Trans = mirror32;
   mirror7Trans = mirror42;


// PMT positions
/////////////// ADJUSTMENTS to far pmt positions /////////////////////
   G4double xcorrection2 = 0.0*cm;
   G4double ycorrection2 = 0.0*cm;
   G4double zcorrection2 = 0.0*cm;

   G4double xcorrection4 = 0.0*cm;
   G4double ycorrection4 = 0.0*cm;
   G4double zcorrection4 = 0.0*cm;

   G4double xcorrection6 = 0.0*cm;
   G4double ycorrection6 = 0.0*cm;
   G4double zcorrection6 = 0.0*cm;

   G4double xcorrection8 = 0.0*cm;
   G4double ycorrection8 = 0.0*cm;
   G4double zcorrection8 = 0.0*cm;


   G4double xcorrection = 0.0*cm;//-2.0*cm; // to compensate for magnetic field
   G4double ycorrection = 0*cm;
   G4double zcorrection = 0*cm;

   G4ThreeVector
   pmt11 ( 40.0*cm +ycorrection2,45.0*cm+xcorrection, 98.*cm-150*cm+correction+zcorrection2 ),
   pmt21 ( 40.0*cm +ycorrection4, 9.0*cm+xcorrection, 98.0*cm-150*cm+correction+zcorrection4 ),
   pmt31 ( 40.0*cm +ycorrection6, -9.0*cm+xcorrection,98.0*cm-150*cm+correction+zcorrection6 ),
   pmt41 ( 40.0*cm +ycorrection8, -45.0*cm+xcorrection,98.0*cm-150*cm+correction+zcorrection8 ),
   pmt12 ( 40.0*cm +ycorrection,63.0*cm+xcorrection,98.0*cm-150*cm+correction+zcorrection ),
   pmt22 ( 40.0*cm+ycorrection, 27.0*cm+xcorrection,98.0*cm-150*cm+correction+zcorrection ),
   pmt32 ( 40.0*cm+ycorrection, -27.0*cm+xcorrection, 98.0*cm-150*cm+correction+zcorrection ),
   pmt42 ( 40.0*cm+ycorrection,-63.0*cm+xcorrection, 98.0*cm-150*cm+correction+zcorrection );



// PMT angles
   G4double   pmtalpha2 = -8*pi/180,//-16.2925549*pi/180,
pmtbeta2 = -59.4693527*pi/180,
pmtalpha4 = -8*pi/180,//-15.3970566*pi/180,
pmtbeta4 = -60.9664955*pi/180,
pmtalpha6 = 8*pi/180,//15.7589064*pi/180,
pmtbeta6 = -30*pi/180,//-60.212326*pi/180,
pmtalpha8 = 8*pi/180,//16.2925549*pi/180,
pmtbeta8= -59.4693527*pi/180,
pmtalpha1 =  9.350*pi/180,
pmtbeta1 =  -23.2584057*pi/180,
pmtalpha3 = 10.2121353*pi/180,
pmtbeta3 =  -23.6647282*pi/180,
pmtalpha5 = -9.48465538*pi/180,
pmtbeta5 =  -23.2584057*pi/180,
pmtalpha7 = -8.8134222*pi/180,
pmtbeta7 =  -23.2584057*pi/180;
//
// Toroidal mirror is ellipsoid4. -9.
//
G4double nearMirrorRadius = 92.*cm,
farToroidalTubeRadius = 85.8*cm,// 85.8
farToroidalAxisRadius = 25.8*cm,// 25.8*cm
farMirrorRadius = farToroidalTubeRadius + farToroidalAxisRadius,
mirrorThickness = 3.0*mm,
yNearMirrorBox = 36.5*cm,
xNearMirrorBox = 35.5*cm,
yFarMirrorBox = 36.5*cm,
xFarMirrorBox = 43.0*cm;
G4double yzsemiaxis = 86.0*cm,
xsemiaxis = 97.0*cm;
G4double farMirrorAngle = 20* pi/180;
 G4double nearMirrorAngle = 10*pi/180;




   G4Sphere * nearMirrorSphere = new G4Sphere ( "SphericalMirrornear", nearMirrorRadius, nearMirrorRadius+mirrorThickness,0,360*deg,0,90*deg );

   G4Box* nearMirrorBox = new G4Box ( "nearMirrorBox", xNearMirrorBox/2, yNearMirrorBox/2,nearMirrorRadius+2*mirrorThickness );

   G4IntersectionSolid * nearMirrorGlass = new G4IntersectionSolid ( "3mm thick glass spherical mirror near", nearMirrorBox, nearMirrorSphere,0,G4ThreeVector ( 0,0,-nearMirrorRadius ) );


//// Temporary Far Spherical Mirror
   G4Sphere * farMirrorSphere = new G4Sphere ( "SphericalMirrorfar", farMirrorRadius, farMirrorRadius+mirrorThickness,0,360*deg,0,90*deg );

   G4Box* farMirrorBox = new G4Box ( "farMirrorBox", xFarMirrorBox/2, yFarMirrorBox/2,farMirrorRadius+2*mirrorThickness );

   G4IntersectionSolid * farMirrorGlass = new G4IntersectionSolid ( "3mm thick glass spherical mirror far", farMirrorBox, farMirrorSphere,0,G4ThreeVector ( 0,0,-farMirrorRadius ) );

   G4Sphere * OUTSIDEsphere = new G4Sphere ( "SphericalMirrorfar", 0, farToroidalAxisRadius+farToroidalTubeRadius+mirrorThickness,0,360*deg,0,180*deg );


/// Ellipsoid
//
   G4Ellipsoid * ellipsoidMirrorOutside = new G4Ellipsoid ( "farmirror Ellipsoid outside",
                                                            xsemiaxis+mirrorThickness,
                                                            yzsemiaxis+mirrorThickness ,
                                                            yzsemiaxis+mirrorThickness,
                                                            15*cm,
                                                            farToroidalAxisRadius+ farToroidalTubeRadius+mirrorThickness );

   G4Ellipsoid * ellipsoidMirrorInside = new G4Ellipsoid ( "farmirror Ellipsoid inside",
                                                           xsemiaxis,

                                                           yzsemiaxis,
                                                           yzsemiaxis );


   G4SubtractionSolid * shell  = new G4SubtractionSolid ( "Outside - Inside Ellipsoid",ellipsoidMirrorOutside ,ellipsoidMirrorInside );

   G4Box* farMirrorBox2 = new G4Box ( "nearMirrorBox", xFarMirrorBox/2, yFarMirrorBox/2, ( farToroidalAxisRadius+mirrorThickness ) /2 );

   G4IntersectionSolid * farMirrorGlassElliptical = new G4IntersectionSolid ( "3mm glass elliptical mirror", farMirrorBox2, shell,0,G4ThreeVector ( 0,0,-yzsemiaxis ) );

//G4IntersectionSolid * farMirrorGlass = new G4IntersectionSolid("3mm glass elliptical mirror", farMirrorBox1, EllipsoidShell,toroidRot,G4ThreeVector(0,0,-farToroidalAxisRadius-farToroidalTubeRadius));
//Look at Toroidal mirror
   G4LogicalVolume * toroid_log = new G4LogicalVolume ( shell, Glass,"farMirrorBox_log",0,0,0 );
//  G4VPhysicalVolume * toroid_phys = new G4PVPlacement(0,G4ThreeVector(0,0,-2*m), toroid_log , "toroid phys",  expHall_log, false,0);

//Y-Z focal point coordinates w/ origin at the centertorus  radius of the tank
   G4double yFocalpoint =  zTankFront/2+zTank/2;
   G4double zFocalpoint = yTank/2-fabs ( tankFrontSide*std::cos ( pmtPlaneAngle ) /2 );

   setMirrorAngles ( 1,alpha1,beta1 );
   setMirrorAngles ( 2,alpha2,beta2 );
   setMirrorAngles ( 3,alpha3 ,beta3);
   setMirrorAngles ( 4,alpha4 ,beta4);
   setMirrorAngles ( 5,alpha5 ,beta5);
   setMirrorAngles ( 6,alpha6 ,beta6);
   setMirrorAngles ( 7,alpha7,beta7);
   setMirrorAngles ( 8,alpha8,beta8 );

   pmtRM1.rotateX ( pmtalpha1 );
   pmtRM1.rotateY ( pmtbeta1 );

   pmtRM2.rotateX ( pmtalpha2 );
   pmtRM2.rotateY ( pmtbeta2 );

   pmtRM3.rotateX ( pmtalpha3 );
   pmtRM3.rotateY ( pmtbeta3 );

   pmtRM4.rotateX ( pmtalpha4 );
   pmtRM4.rotateY ( pmtbeta4 );


   pmtRM5.rotateX ( pmtalpha5 );
   pmtRM5.rotateY ( pmtbeta5 );


   pmtRM6.rotateX ( pmtalpha6 );
   pmtRM6.rotateY ( pmtbeta6 );


   pmtRM8.rotateX ( pmtalpha8 );
   pmtRM8.rotateY ( pmtbeta8 );


   pmtRM7.rotateX ( pmtalpha7 );
   pmtRM7.rotateY ( pmtbeta7 );


/// Gas Cherenkov spherical mirror Logical Volume
   fCerSphericalMirror_log = new G4LogicalVolume ( nearMirrorGlass, Glass,"nearMirrorBox_log",0,0,0 );
/// Gas Cherenkov elliptical approximation to a toroidal mirror Logical Volume
   fCerToroidalMirror_log = new G4LogicalVolume ( farMirrorGlassElliptical, Glass,"farMirrorBox_log",0,0,0 );

/// Physical Volumes - MIRRORS
   MirrorGlass_phys1 = new G4PVPlacement ( G4Transform3D ( RM1,mirror12 ), fCerSphericalMirror_log , "Near - Mirror",  tank_log, false,1 );

   MirrorGlass_phys2 = new G4PVPlacement ( G4Transform3D ( RM2,mirror11 ), fCerToroidalMirror_log , "Far - Mirror",  tank_log, false,2 );

   MirrorGlass_phys3 = new G4PVPlacement ( G4Transform3D ( RM3,mirror22 ), fCerSphericalMirror_log , "Near - Mirror",  tank_log, false,3 );

   MirrorGlass_phys4 = new G4PVPlacement ( G4Transform3D ( RM4,mirror21 ), fCerToroidalMirror_log , "Far - Mirror",  tank_log, false,4 );

   MirrorGlass_phys5 = new G4PVPlacement ( G4Transform3D ( RM5,mirror32 ), fCerSphericalMirror_log , "Near - Mirror",  tank_log, false,5 );

   MirrorGlass_phys6 = new G4PVPlacement ( G4Transform3D ( RM6,mirror31 ), fCerToroidalMirror_log , "Far - Mirror",  tank_log, false,6 );

   MirrorGlass_phys7 = new G4PVPlacement ( G4Transform3D ( RM7,mirror42 ), fCerSphericalMirror_log , "Near - Mirror",  tank_log, false,7 );

   MirrorGlass_phys8 = new G4PVPlacement ( G4Transform3D ( RM8,mirror41 ), fCerToroidalMirror_log , "Far - Mirror",  tank_log, false,8 );


//
// temp counting box
  
//
//   G4Box * somevolume = new G4Box ( "someBox", xPMTmountPanel/2, yPMTmountPanel/2, 1*cm );
//   G4LogicalVolume * somevolume_log = new G4LogicalVolume ( somevolume, NitrogenGas, "somevolume_log", 0,0,0 );
//  G4VPhysicalVolume * somevolume_phys = new G4PVPlacement(G4Transform3D(ROTATE45DEG, G4ThreeVector(0, -yTankFrontEnd/2 , -zTank/2  )), somevolume_log,  "somevolume_phys", tank_log, false,0);

/// scoring       

  G4double tubeDiameter=4.0*2.54*cm;
  G4double tubeLength=15.0*2.54*cm;

 G4double  MUtubeID=3.01*2.54*cm;
 G4double  MUtubeLength=14.0*2.54*cm;
 G4double  MUtubeThickness=4.0*mm;

  G4double PMTtubeDiameter=3.0*2.54*cm;
  G4double PMTtubeLength=3.0*2.54*cm;

  G4Tubs * PMTcontainer =  new G4Tubs("PMTcontainer", 0., tubeDiameter/2.0, tubeLength/2.0, 0., 7.);
  G4Tubs * PMT =  new G4Tubs("PMT", 0., PMTtubeDiameter/2.0, PMTtubeLength/2.0, 0., 7.);
  G4Tubs * MU =  new G4Tubs("MU", 0., MUtubeID/2.0, tubeLength, 0., 7.);
  G4Tubs * MU2 =  new G4Tubs("MU2", 0., (MUtubeID+MUtubeThickness)/2.0, tubeLength/2.0, 0., 7.);
          G4RotationMatrix unitrm;

 //   fit perfectly with the real can. Do it for the cavity too.
   G4VSolid* MUPipe =
      new G4SubtractionSolid("MUPipe", MU2, MU,G4Transform3D(unitrm,G4ThreeVector(0.,0.,0.)));
/*,
                             G4Transform3D(,
                                           G4ThreeVector(0,0,-4.8433/2*m)));*/
  container_log = new G4LogicalVolume(PMTcontainer, NitrogenGas, "PipeCav1");
  MUPipe_log = new G4LogicalVolume(MUPipe, ((G4NistManager*)G4NistManager::Instance())->FindOrBuildMaterial ( "G4_Fe" ), "MUPipe_log");
   new G4PVPlacement(G4Transform3D(unitrm,G4ThreeVector()),MUPipe_log, "PMT+mumetal",container_log, false, 0);

  BETASimulationManager::GetInstance()->InitScoring();

   G4VSensitiveDetector* aPMTModel =
      new BETAG4PMTArray ( "GasCherenkov" );
   
   G4Tubs * pmtFace = new G4Tubs ( "PMTFACE",0,3*2.54*cm/2, 0.05*cm,0,360*deg );

   pmtFace_log =//container_log;
    new G4LogicalVolume ( pmtFace, NitrogenGas, "pmtFace_log", 0,0,0 );

   G4VPhysicalVolume * pmtFace1_phys = new G4PVPlacement ( G4Transform3D ( pmtRM1,pmt12 ), pmtFace_log,  "pmtFace_phys", tank_log, false,7 );
   G4VPhysicalVolume * pmtFace2_phys = new G4PVPlacement ( G4Transform3D ( pmtRM2,pmt11 ), pmtFace_log,  "pmtFace_phys", tank_log, false,8 );
   G4VPhysicalVolume * pmtFace3_phys = new G4PVPlacement ( G4Transform3D ( pmtRM3,pmt22 ), pmtFace_log,  "pmtFace_phys", tank_log, false,5 );
   G4VPhysicalVolume * pmtFace4_phys = new G4PVPlacement ( G4Transform3D ( pmtRM4,pmt21 ), pmtFace_log,  "pmtFace_phys", tank_log, false,6 );
   G4VPhysicalVolume * pmtFace5_phys = new G4PVPlacement ( G4Transform3D ( pmtRM5,pmt32 ), pmtFace_log,  "pmtFace_phys", tank_log, false,3 );
   G4VPhysicalVolume * pmtFace6_phys = new G4PVPlacement ( G4Transform3D ( pmtRM6,pmt31 ), pmtFace_log,  "pmtFace_phys", tank_log, false,4 );
   G4VPhysicalVolume * pmtFace7_phys = new G4PVPlacement ( G4Transform3D ( pmtRM7,pmt42 ), pmtFace_log,  "pmtFace_phys", tank_log, false,1 );
   G4VPhysicalVolume * pmtFace8_phys = new G4PVPlacement ( G4Transform3D ( pmtRM8,pmt41 ), pmtFace_log,  "pmtFace_phys", tank_log, false,2 );

////////////////////////// Print out Magnetic field at pmt face ////////////////
   G4ThreeVector localPosition = pmtFace4_phys->GetTranslation();
//G4ThreeVector fieldVector =
//    std::cout << "\n\nPMT#4 : POSITION (" << localPosition.x()/cm << ", " << localPosition.y()/cm << ", " <<localPosition.z()/cm << ") and Field \n\n" ;
//     G4ThreeVector localPosition = theTouchable->GetHistory()->
//                                   GetTopTransform().TransformPoint(worldPosition);
/////////////////////////////////////////////////
// Pmt Detector Faces
/////////////////////////////////////////////////
   // new for the detector part
   // Create a new sensitive detector named "MyDetector"
   G4SDManager* manager = G4SDManager::GetSDMpointer();

//    G4VSensitiveDetector* CherenkovTank =
//       new BETAPMT ( "PMT" );

   // Register detector with manager
//   manager->AddNewDetector ( CherenkovTank );
   pmtFace_log->SetSensitiveDetector (aPMTModel );

   manager->AddNewDetector ( aPMTModel );

   // Attach detector to scoring volume
 //  pmtFace_log->SetSensitiveDetector(fSimulationManager->fCherenkovDetector);
// CherenkovTank );

/// Also doing mirrors as a detector, but this is only for determining the optical efficiency of the
/// mirror/pmt combination.
   G4VSensitiveDetector* mirrorDetectorFake =
      new BETAMirror ( "Mirrors" );
   // Register detector with manager
   manager->AddNewDetector ( mirrorDetectorFake );
   // Attach detector to scoring volume
   fCerSphericalMirror_log->SetSensitiveDetector ( mirrorDetectorFake );
   fCerToroidalMirror_log->SetSensitiveDetector ( mirrorDetectorFake );
// OPTICS
   const G4int num = 2;
   G4double Ephoton[num] = {1.0*eV, 10.0*eV};

   G4double RefractiveIndex[num] = {1.00029, 1.00048};
   G4double SpecularLobe[num]    = {0.3, 0.3};
   G4double SpecularSpike[num]   = {0.2, 0.2};
   G4double Backscatter[num]     = {0.2, 0.2};
   G4double Reflectivity[num] = {0.3, 0.5};
   G4double Efficiency[num]   = {1.0, 1.0};

   //----- Optical surface of PMT 
   G4OpticalSurface* OpPMTSurface = new G4OpticalSurface ( "PMTSurface" );
   OpPMTSurface->SetModel ( unified );
   OpPMTSurface->SetType ( dielectric_dielectric );
   OpPMTSurface->SetFinish ( ground );

   G4LogicalSkinSurface * pmtFaceSurface = new G4LogicalSkinSurface ( "PmtFace surface", pmtFace_log, OpPMTSurface );

   G4double RefractiveIndexPMTFace[num] = {1.48, 1.48};
   G4double QuartzReflectivity[num] = {0,0};//{0.034, 0.034};

   G4MaterialPropertiesTable* PMTFaceTable = new G4MaterialPropertiesTable();

   PMTFaceTable->AddProperty ( "RINDEX",                Ephoton, RefractiveIndexPMTFace, num );
   PMTFaceTable->AddProperty ( "SPECULARLOBECONSTANT",  Ephoton, SpecularLobe,    num );
   PMTFaceTable->AddProperty ( "SPECULARSPIKECONSTANT", Ephoton, SpecularSpike,   num );
   PMTFaceTable->AddProperty ( "BACKSCATTERCONSTANT",   Ephoton, Backscatter,     num );
   PMTFaceTable->AddProperty ( "REFLECTIVITY", Ephoton, QuartzReflectivity, num );
   PMTFaceTable->AddProperty ( "EFFICIENCY",   Ephoton, Efficiency,   num );

   OpPMTSurface->SetMaterialPropertiesTable ( PMTFaceTable );

   // Mirror Surface - MIRROR
   G4double MSpecularLobe[num]    = {0.0, 0.0};
   G4double MSpecularSpike[num]   = {0.0, 0.0};
   G4double MBackscatter[num]     = {0.0, 0.0};
   G4double MEfficiency[num]   = {1.0, 1.0};
   G4double coatingReflectivity[num] = {0.85, 0.84};

   G4OpticalSurface* OpMirrorSurface = new G4OpticalSurface ( "SilverSurface" );
   OpMirrorSurface->SetType ( dielectric_metal );
   OpMirrorSurface->SetFinish ( polished );
   OpMirrorSurface->SetModel ( unified );

   G4MaterialPropertiesTable* reflectiveCoatingSurfacePTable = new G4MaterialPropertiesTable();

   reflectiveCoatingSurfacePTable->AddProperty ( "REFLECTIVITY", Ephoton, coatingReflectivity, num );
   reflectiveCoatingSurfacePTable->AddProperty ( "SPECULARLOBECONSTANT",  Ephoton, MSpecularLobe,    num );
   reflectiveCoatingSurfacePTable->AddProperty ( "SPECULARSPIKECONSTANT", Ephoton, MSpecularSpike,   num );
   reflectiveCoatingSurfacePTable->AddProperty ( "BACKSCATTERCONSTANT",   Ephoton, MBackscatter,     num );
   OpMirrorSurface->SetMaterialPropertiesTable ( reflectiveCoatingSurfacePTable );

   G4LogicalSkinSurface * nearMirrorSurface = new G4LogicalSkinSurface ( "silversurface", fCerSphericalMirror_log, OpMirrorSurface );
// Workaround: use border surface instead of skin for elliptical mirrors
//  G4LogicalSkinSurface * farMirrorSurface = new G4LogicalSkinSurface("silversurface", fCerToroidalMirror_log, OpMirrorSurface);
// elliptical mirror work around for far elliptical mirrors
   G4LogicalBorderSurface * farMirrorSurfaceTEMP2 = new G4LogicalBorderSurface ( "farmirror1", cherenkovTank_phys, MirrorGlass_phys2, OpMirrorSurface );
   G4LogicalBorderSurface * farMirrorSurfaceTEMP4 = new G4LogicalBorderSurface ( "farmirror3", cherenkovTank_phys, MirrorGlass_phys4, OpMirrorSurface );
   G4LogicalBorderSurface * farMirrorSurfaceTEMP6 = new G4LogicalBorderSurface ( "farmirror6", cherenkovTank_phys, MirrorGlass_phys6, OpMirrorSurface );
   G4LogicalBorderSurface * farMirrorSurfaceTEMP8 = new G4LogicalBorderSurface ( "farmirror7", cherenkovTank_phys, MirrorGlass_phys8, OpMirrorSurface );

   G4OpticalSurface* OpTankFlatBlackSurface = new G4OpticalSurface ( "FlatBlackSurface" );
   OpTankFlatBlackSurface->SetModel ( unified );
   OpTankFlatBlackSurface->SetType ( dielectric_metal );
   OpTankFlatBlackSurface->SetFinish ( groundbackpainted );

   G4LogicalSkinSurface * flatBalckSurface = new G4LogicalSkinSurface ( "TankFlatBlackSurface", tank_log, OpTankFlatBlackSurface );

   ///////////////////////////////////////////////////
   //  Visualizations
   G4VisAttributes* cerWindowAttrib = new G4VisAttributes ( G4Colour ( 0.0,0.5,0.5, 0.1 ) );
   cerWindowAttrib->SetForceWireframe ( true );

   /// Back window
   fCerBackWindow_log->SetVisAttributes( cerWindowAttrib );
   /// Front window
   fCerFrontWindow_log->SetVisAttributes( cerWindowAttrib );

   G4VisAttributes* cerSquareTubeFrameAttrib = new G4VisAttributes ( G4Colour ( 0.0,0.5,0.5 ) );
   cerSquareTubeFrameAttrib->SetForceWireframe( true );
   fCerFrameTopBottom_log->SetVisAttributes(cerSquareTubeFrameAttrib);

   /// Tank volume
   G4VisAttributes* tankAttributes = new G4VisAttributes ( G4Colour ( 0.0,0.5,0.5, 0.1 ) );
   //tankAttributes->SetForceWireframe ( true );
   tankAttributes->SetForceSolid ( true );
   tankAttributes->SetDaughtersInvisible( false );
   tankAttributes->SetVisibility( false );
   tank_log->SetVisAttributes ( tankAttributes );
   //tank_log->SetVisAttributes ( G4VisAttributes::Invisible );

   /// Aluminum PMT cans
   AlCans_log->SetVisAttributes(G4VisAttributes::Invisible);
   // AlCans_log->SetForceWireframe ( true );

   G4VisAttributes* mirrorAttributes = new G4VisAttributes ( G4Colour ( 0.80,0.40,1.0 ) );
   //mirrorAttributes->SetForceWireframe(true);
   mirrorAttributes->SetForceSolid(true);
   mirrorAttributes->SetVisibility( true );
   fCerSphericalMirror_log->SetVisAttributes(mirrorAttributes);
   fCerToroidalMirror_log->SetVisAttributes(mirrorAttributes);

}

//___________________________________________________________________
void BETADetectorConstruction::ConstructFakePlane()
{
   if(usingFakePlaneAtBigcal) {

      // The following is for a fake detector just before bigcal
      G4Box* fakePlane_box = new G4Box ( "PlaneBeforeBigcal",1.4*m/2.0,2.5*m/2.0 , 0.10*mm );
      fBigCalFakePlane_log = 
           new G4LogicalVolume ( fakePlane_box,Air,"PlaneBeforeBigcal_log",0,0,0 );
      BIGCALGeometryCalculator * BCgeo =  BIGCALGeometryCalculator::GetCalculator();
//rTarget
      G4double bigcalFace = BCgeo->bigcalFace*cm;// 3.45*m; // from target
      G4double bigcalFaceRelative = bigcalFace - ( DetectorLength/2.0+rTarget );
      G4VPhysicalVolume* fakePlane_phys = new G4PVPlacement ( 0,G4ThreeVector ( 0,0,bigcalFaceRelative-1.0*mm ) ,fBigCalFakePlane_log,"PlaneBeforeBigcal_phys",BETADetector_log,false,0 );
//      SetupScoring(fakePlane_log);
   G4SDManager* manager = G4SDManager::GetSDMpointer();
   BETAFakePlane* fakePlane =
      new BETAFakePlane("BIGCALPlane");
   fakePlane->SetSensitiveVolume(fakePlane_phys);

   // Register detector with manager
   manager->AddNewDetector((G4VSensitiveDetector*)fakePlane);
   // Attach detector to scoring volume
   fBigCalFakePlane_log->SetSensitiveDetector((G4VSensitiveDetector*)fakePlane);
   fBigCalFakePlane_log->SetVisAttributes(G4VisAttributes::Invisible);

//      planeBehindTracker_log->SetVisAttributes(G4VisAttributes::Invisible);
   }

// Fake Plane at tracker
   if(usingFakePlaneAtForwardTracker) {
      G4Box* trakerFakePlane_box = new G4Box ( "trackerFakePlane",0.5*m/2.0,0.35*m/2.0 , 0.10*mm );
      fTrackerFakePlane_log = 
           new G4LogicalVolume ( trakerFakePlane_box,Air,"trackerFakePlane_log",0,0,0 );
   
      G4VPhysicalVolume* trackerFakePlane_phys = new G4PVPlacement ( 0,G4ThreeVector ( 0,0,-1.0*DetectorLength/2.0+ 0.20*mm ) ,fTrackerFakePlane_log,"trackerFakePlane_phys",BETADetector_log,false,0 );
     SetupScoring(fTrackerFakePlane_log);
   G4SDManager* manager = G4SDManager::GetSDMpointer();
   BETAFakePlane* trackerFakePlaneDetector =
      new BETAFakePlane("ForwardTrackerPlane");
   // Register detector with manager
   trackerFakePlaneDetector->SetSensitiveVolume(trackerFakePlane_phys);
   manager->AddNewDetector((G4VSensitiveDetector*)trackerFakePlaneDetector);
   // Attach detector to scoring volume
   fTrackerFakePlane_log->SetSensitiveDetector((G4VSensitiveDetector*)trackerFakePlaneDetector);
   fTrackerFakePlane_log->SetVisAttributes(G4VisAttributes::Invisible);
}

}
//___________________________________________________________________

void BETADetectorConstruction::ConstructHeliumBag() {

   G4double fHorizSize = 4.67*2.54*cm;
   G4double fVertSize  = 7.97*2.54*cm +0.13*2.54*cm;
   G4double fLength  = 71.97*2.54*cm;
   G4double fWindowThickness  = 0.016*2.54*cm;
   G4RotationMatrix zeroRot;
   G4double fTopDistaceFromBeamline = 1.5*2.54*cm;
   G4double small_dist = 0.01*mm;
   fHeBagExtenderBox     = new G4Box("HeBagExtenderBox", fHorizSize/2.0+small_dist, fVertSize/2.0+small_dist,fLength/2.0+small_dist);
   fHeBagExtenderBox_log = new G4LogicalVolume(fHeBagExtenderBox, HeGas, "HeBagExtenderBox_log");
   new G4PVPlacement(0,
        G4ThreeVector(0,-fVertSize/2.0+fTopDistaceFromBeamline,22.0*2.54*cm+fLength/2.0),
        fHeBagExtenderBox_log, "HeBagExtenderBox_phys", expHall_log, false, 0);

// Windows front and sides 
   fHeBagExtenderFrontWindow = new G4Box("HeBagExtenderFrontWindow", 
      fHorizSize/2.0-2.0*fWindowThickness, // subtracting size from front window to not hit sides
      fVertSize/2.0-2.0*fWindowThickness, // subtracting size from front window to not hit sides
      fWindowThickness/2.0);
   fHeBagExtenderFrontWindow_log = new G4LogicalVolume(fHeBagExtenderFrontWindow, Al, "HeBagExtenderFrontWindow_log");
   new G4PVPlacement(0,
        G4ThreeVector( 0, 0, -fLength/2.0+fWindowThickness/2.0),
        fHeBagExtenderFrontWindow_log, "HeBagExtenderBox_log", fHeBagExtenderBox_log, false, 0);

   fHeBagExtenderHorizWindow = new G4Box("HeBagExtenderHorizWindow", 
      fHorizSize/2.0, 
      fWindowThickness/2.0, 
      fLength/2.0);
   fHeBagExtenderHorizWindow_log = new G4LogicalVolume(fHeBagExtenderHorizWindow, Al, "HeBagExtenderHorizWindow_log");
   new G4PVPlacement(0,
        G4ThreeVector(0.0,fVertSize/2.0-fWindowThickness/2.0,0.0),
        fHeBagExtenderHorizWindow_log, "HeBagExtenderHorizWindow1_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(0.0,-1.0*fVertSize/2.0+fWindowThickness/2.0,0.0),
        fHeBagExtenderHorizWindow_log, "HeBagExtenderHorizWindow2_phys", fHeBagExtenderBox_log, false, 0);

   fHeBagExtenderVertWindow = new G4Box("HeBagExtenderVertWindow", 
      fWindowThickness/2.0, 
      fVertSize/2.0, 
      fLength/2.0);
   fHeBagExtenderVertWindow_log = new G4LogicalVolume(fHeBagExtenderVertWindow, Al, "HeBagExtenderVertWindow_log");
   new G4PVPlacement(0,
        G4ThreeVector(fHorizSize/2.0-fWindowThickness/2.0,0.0,0.0),
        fHeBagExtenderVertWindow_log, "HeBagExtenderVertWindow1_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(-1.0*fHorizSize/2.0+fWindowThickness/2.0,0.0,0.0),
        fHeBagExtenderVertWindow_log, "HeBagExtenderVertWindow2_phys", fHeBagExtenderBox_log, false, 0);


// Angles used as longitudinal support
// Creat the angle pieces, 4 of them (easier than rotating)
   fHeBagExtenderAngleBox1     = new G4Box("HeBagExtenderAngleBox1", 1.0*2.54*cm/2.0, 1.0*2.54*cm/2.0,69.98*2.54*cm/2.0);
   fHeBagExtenderAngleBox2     = new G4Box("HeBagExtenderAngleBox2", 1.0*2.54*cm/2.0, 1.0*2.54*cm/2.0,69.98*2.54*cm/2.0+small_dist);
   fHeBagExtenderAngle1 = 
      new G4SubtractionSolid("HeBagExtenderAngle1", 
                             fHeBagExtenderAngleBox1, 
                             fHeBagExtenderAngleBox2, 
                             G4Transform3D(zeroRot, G4ThreeVector(-0.13*2.54*cm,-0.13*2.54*cm,0)));
   fHeBagExtenderAngle2 = 
      new G4SubtractionSolid("HeBagExtenderAngle2", 
                             fHeBagExtenderAngleBox1, 
                             fHeBagExtenderAngleBox2, 
                             G4Transform3D(zeroRot, G4ThreeVector(0.13*2.54*cm,-0.13*2.54*cm,0)));
   fHeBagExtenderAngle3 = 
      new G4SubtractionSolid("HeBagExtenderAngle3", 
                             fHeBagExtenderAngleBox1, 
                             fHeBagExtenderAngleBox2, 
                             G4Transform3D(zeroRot, G4ThreeVector(0.13*2.54*cm,0.13*2.54*cm,0)));
   fHeBagExtenderAngle4 = 
      new G4SubtractionSolid("HeBagExtenderAngle4", 
                             fHeBagExtenderAngleBox1, 
                             fHeBagExtenderAngleBox2, 
                             G4Transform3D(zeroRot, G4ThreeVector(-0.13*2.54*cm,0.13*2.54*cm,0)));
   fHeBagExtenderAngle1_log = new G4LogicalVolume(fHeBagExtenderAngle1, Al, "HeBagExtenderAngle1_log");
   fHeBagExtenderAngle2_log = new G4LogicalVolume(fHeBagExtenderAngle2, Al, "HeBagExtenderAngle2_log");
   fHeBagExtenderAngle3_log = new G4LogicalVolume(fHeBagExtenderAngle3, Al, "HeBagExtenderAngle3_log");
   fHeBagExtenderAngle4_log = new G4LogicalVolume(fHeBagExtenderAngle4, Al, "HeBagExtenderAngle4_log");
/* 1 is in first quadrant 2 goes CCW */

   new G4PVPlacement(0,
        G4ThreeVector(fHorizSize/2.0-fWindowThickness-1.0*2.54*cm/2.0,fVertSize/2.0-fWindowThickness-1.0*2.54*cm/2.0,0.0),
        fHeBagExtenderAngle1_log, "HeBagExtenderAngle1_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(-1.0*fHorizSize/2.0+fWindowThickness+1.0*2.54*cm/2.0,fVertSize/2.0-fWindowThickness-1.0*2.54*cm/2.0,0.0),
        fHeBagExtenderAngle2_log, "HeBagExtenderAngle2_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(-1.0*fHorizSize/2.0+fWindowThickness+1.0*2.54*cm/2.0,-1.0*fVertSize/2.0+fWindowThickness+1.0*2.54*cm/2.0,0.0),
        fHeBagExtenderAngle3_log, "HeBagExtenderAngle3_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(fHorizSize/2.0-fWindowThickness-1.0*2.54*cm/2.0,-1.0*fVertSize/2.0+fWindowThickness+1.0*2.54*cm/2.0,0.0),
        fHeBagExtenderAngle4_log, "HeBagExtenderAngle4_phys", fHeBagExtenderBox_log, false, 0);

// vertical supports
   fHeBagExtenderVertSupport   = new G4Box("HeBagExtenderVertSupport", 0.13*2.54*cm/2.0, 7.72*2.54*cm/2.0, 1.0*2.54*cm/2.0);
   fHeBagExtenderVertSupport_log = new G4LogicalVolume(fHeBagExtenderVertSupport, Al, "HeBagExtenderVertSupport_log");
   new G4PVPlacement(0,
        G4ThreeVector(fHorizSize/2.0-0.13*2.54*cm/2.0-fWindowThickness,0.0,fLength/2.0-1.0*2.54*cm/2.0),
        fHeBagExtenderVertSupport_log, "HeBagExtenderVertSupport1_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(-1.0*fHorizSize/2.0+0.13*2.54*cm/2.0+fWindowThickness,0.0,fLength/2.0-1.0*2.54*cm/2.0),
        fHeBagExtenderVertSupport_log, "HeBagExtenderVertSupport2_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(fHorizSize/2.0-0.13*2.54*cm/2.0-fWindowThickness,0.0,-1.0*fLength/2.0+1.0*2.54*cm/2.0),
        fHeBagExtenderVertSupport_log, "HeBagExtenderVertSupport3_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(-1.0*fHorizSize/2.0+0.13*2.54*cm/2.0+fWindowThickness,0.0,-1.0*fLength/2.0+1.0*2.54*cm/2.0),
        fHeBagExtenderVertSupport_log, "HeBagExtenderVertSupport4_phys", fHeBagExtenderBox_log, false, 0);

// horizontal supports
   fHeBagExtenderHorizSupport   = new G4Box("HeBagExtenderHorizSupport",  4.67*2.54*cm/2.0, 0.13*2.54*cm/2.0, 1.0*2.54*cm/2.0);
   fHeBagExtenderHorizSupport_log = new G4LogicalVolume(fHeBagExtenderHorizSupport, Al, "HeBagExtenderHorizSupport_log");
   new G4PVPlacement(0,
        G4ThreeVector(0.0,fVertSize/2.0-0.13*2.54*cm/2.0-fWindowThickness,fLength/2.0-1.0*2.54*cm/2.0),
        fHeBagExtenderHorizSupport_log, "HeBagExtenderHorizSupport1_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(0.0,-1.0*fVertSize/2.0+0.13*2.54*cm/2.0+fWindowThickness,fLength/2.0-1.0*2.54*cm/2.0),
        fHeBagExtenderHorizSupport_log, "HeBagExtenderHorizSupport2_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(0.0,fVertSize/2.0-0.13*2.54*cm/2.0-fWindowThickness,-1.0*fLength/2.0+2.54*cm/2.0),
        fHeBagExtenderHorizSupport_log, "HeBagExtenderHorizSupport3_phys", fHeBagExtenderBox_log, false, 0);
   new G4PVPlacement(0,
        G4ThreeVector(0.0,-1.0*fVertSize/2.0+0.13*2.54*cm/2.0+fWindowThickness,-1.0*fLength/2.0+2.54*cm/2.0),
        fHeBagExtenderHorizSupport_log, "HeBagExtenderHorizSupport4_phys", fHeBagExtenderBox_log, false, 0);
/*
   fHeBagExtenderHorizSupport   = new G4Box("HeBagExtenderHorizSupport", 0.13*2.54*cm/2.0, 7.72*2.54*cm/2.0,1.0*2.54*cm/2.0);
   fHeBagExtenderHorizSupport_log = new G4LogicalVolume(fHeBagExtenderHorizSupport, Al, "HeBagExtenderHorizSupport_log");  */

// Visualization
   G4VisAttributes* fHeBagExtenderWallsVisAtt= new G4VisAttributes ( G4Colour ( 0.1,0.0,0.8,0.4 ) );
   fHeBagExtenderWallsVisAtt->SetVisibility ( true );
   fHeBagExtenderWallsVisAtt->SetForceSolid ( true );

   fHeBagExtenderFrontWindow_log->SetVisAttributes ( fHeBagExtenderWallsVisAtt );
   fHeBagExtenderVertWindow_log->SetVisAttributes ( fHeBagExtenderWallsVisAtt );
   fHeBagExtenderHorizWindow_log->SetVisAttributes ( fHeBagExtenderWallsVisAtt );

   G4VisAttributes* fHeBagExtenderAngleVisAtt= new G4VisAttributes ( G4Colour ( 0.,0.4,0. ) );
   fHeBagExtenderAngleVisAtt->SetVisibility ( true );
/*   fHeBagExtenderAngleVisAtt->SetForceSolid ( false );*/
   fHeBagExtenderAngle1_log->SetVisAttributes ( fHeBagExtenderAngleVisAtt );
   fHeBagExtenderAngle2_log->SetVisAttributes ( fHeBagExtenderAngleVisAtt );
   fHeBagExtenderAngle3_log->SetVisAttributes ( fHeBagExtenderAngleVisAtt );
   fHeBagExtenderAngle4_log->SetVisAttributes ( fHeBagExtenderAngleVisAtt );

}
//___________________________________________________________________

void BETADetectorConstruction::ConstructBETA()
{
   G4double displacementBackwards = 1.0*cm;
   G4RotationMatrix detectorRot;
   detectorRot.rotateY ( DetectorAngle ); //
   //detectorRot.rotateZ ( PI ); //

/**
 * This is the box which holds all BETA detectors the 
 * face of which is placed at 50cm (rTarget) from the target
 */
   G4Box* BETADetector_box = new G4Box ( "BETAdetectorbox", DetectorWidth/2.0, DetectorWidth/2.0, DetectorLength/2.0 );
   BETADetector_log = new G4LogicalVolume ( BETADetector_box, Air, "BETAdetector",0,0,0 );
   G4VPhysicalVolume* BETADetector_phys = new G4PVPlacement(
     G4Transform3D(detectorRot, G4ThreeVector(
                            (rTarget+DetectorLength/2.0+displacementBackwards)*std::sin(DetectorAngle),
                            0.0*m,
                            (rTarget+DetectorLength/2.0+displacementBackwards)*std::cos(DetectorAngle))), 
                            BETADetector_log, "BETADetectorphys",expHall_log,false,0 );


// This must come before associating logical volumes with scorers
   fSimulationManager->InitScoring();
   fSimulationManager->DefineScoringFilters();


   if (usingFakePlaneAtBigcal || usingFakePlaneAtForwardTracker) ConstructFakePlane();
   if (usingForwardTracker)  ConstructForwardTracker();
   if (usingGasCherenkov)  ConstructCherenkov();
   if (usingLuciteHodoscope)  ConstructHodoscope();
   if (usingBigcal)   ConstructBIGCAL();

   expHall_log->SetVisAttributes ( G4VisAttributes::Invisible );
   BETADetector_log->SetVisAttributes ( G4VisAttributes::Invisible );

}
//___________________________________________________________________

void BETADetectorConstruction::SetupScoring(G4LogicalVolume * scoringVolume) {




}

//___________________________________________________________________
void BETADetectorConstruction::ConstructTarget()
{

}

//___________________________________________________________________
/** \brief Returns the physical world.
 *
 * Coordinate systems: 
 * 
 * Hall C/HMS (x is down, y is pointing away from HMS, z is pointing down the beampipe) Origin at target.
 * 
 * BigCal (x is horizontal and positive towards larger scattering angles, assuming we are on the opposite
 * side of the beam as HMS, y is up , z is along a
 * ray at $\theta=40$ degrees and $\phi=90$ degrees in the HMS coordinates) 
 * The origin is at the face of bigcal.
 */
G4VPhysicalVolume *
BETADetectorConstruction::Construct()
{
   constructed = 1;
   
   DefineMaterials();

// The experimental Hall
   G4Box* expHall_box = new G4Box ( "World",expHall_x,expHall_y,expHall_z );
   expHall_log = new G4LogicalVolume ( expHall_box,/*Air*/Vacuum,"World",0,0,0 );
   expHall_phys = new G4PVPlacement ( 0,G4ThreeVector(),expHall_log,"World",0,false,0 );

// FLOOR ( only for looks )
   G4Box* Floor = new G4Box ( "floor",1.0*mm,expHall_y,expHall_z );
   G4LogicalVolume* floor_log = new G4LogicalVolume ( Floor,Aluminum,"floor log",0,0,0 );
// G4VPhysicalVolume* floor_phys = new G4PVPlacement ( 0,G4ThreeVector ( -1.0*4.*m,0,0 ),floor_log,"floor phys",expHall_log,false,0 );

// beam pipe
   G4Tubs * beamPipe = new G4Tubs ( "Beam Pipe", 2.0*cm, ( 2.+0.25 )*cm, 2.0*m,0,2.0*pi );
   G4LogicalVolume* beamPipe_log = new G4LogicalVolume ( beamPipe,Aluminum,"target",0,0,0 );
 /*  G4VPhysicalVolume* beamPipe_phys = new G4PVPlacement(0,G4ThreeVector(0,0,-2*m),beamPipe_log,"Beam Pipe",expHall_log,false,0);
  */ /*  G4VPhysicalVolume* beamPipe2_phys = new G4PVPlacement(0,G4ThreeVector(2*(2.+0.3)*2.54*cm,0,-2*m),beamPipe_log,"Beam Pipe top",expHall_log,false,0);
   */

   ConstructTarget();
   // From Justin Wright:
   //    Construct everything in the target can.
   //Also constructs the target magnet and nosepiece.
   ConstructTCan();
   G4cout<<"Tcan";
   ConstructN2Shield();
//   G4cout<<"N2shield";
//   ConstructBeamPipe();

   //G4cout<<"Helium Bag";
   //ConstructHeliumBag();

   G4Element* H  = new G4Element ( "Hydrogen","H" ,1., 1.01*g/mole );
   G4Element* N  = new G4Element ( "Nitrogen","N" , 7.,  14.01*g/mole );
   G4Material* NH3solid =
      new G4Material ( "SolidAmmonia", 0.9*g/cm3, 2 );
   NH3solid->AddElement ( H, 3 );
   NH3solid->AddElement ( N, 1 );

// TARGET
   G4Box* target_box = new G4Box ( "target",3*cm,3*cm, 3*cm );
   G4LogicalVolume* target_log = new G4LogicalVolume ( target_box,NH3solid,"target",0,0,0 );
// G4VPhysicalVolume* target_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),target_log,"Target",expHall_log,false,0);



   /* Conventions
   Here we will use z as the beam axis and assume that our
   target is at (0,0,0). We will choose the x axis to be the vertical in
   the lab.
   */

   ConstructBETA();
// Lead Shield
   G4VSolid* leadShield = new G4Box ( "Pb_sheild_BOX", // Name
                                      (72.0*2.54*cm/2.0 ),             // x half length
                                      ( 2.54*2.0 ) *cm/2.0,             // y half length
                                      ( 2.54*2.0 ) *cm/2.0);            // z half length


   G4LogicalVolume* leadShield_log =
      new G4LogicalVolume ( leadShield,     // Solid
                            Lead,                    // Material
                            "BIGCAL_sheild" ); // Name

   G4double shieldDistance = 5*cm;

   /*  new G4PVPlacement(0,G4ThreeVector(0*cm,-14.0*2.54*cm,-66.5*2.54*cm), leadShield_log,         // Logical volume
                        "PMT_sheild",     // Name
                        BETADetector_log,             // Mother volume
                        false,                      // Unused boolean
                        0);
   */

///////////////////////////////////////////////////
//   MAGNETIC FIELD
///////////////////////////////////////////////////
/*   static G4bool fieldIsInitialized = false;
   if ( !fieldIsInitialized )
   {*/
      ConstructMagneticField();
//       BETAField * myField = new BETAField();
//       myField->fUVAMagnet->SetPolarizationAngle (messenger->fTargetAngle ); 
//       G4FieldManager* fieldMgr
//       = G4TransportationManager::GetTransportationManager()->GetFieldManager();
//       fieldMgr->SetDetectorField ( myField );
//       fieldMgr->CreateChordFinder ( myField );
//       fieldMgr->GetChordFinder()->SetDeltaChord ( 1.0e-1*cm ); // miss distance
//       G4double minEps= 1.0e-5;  //   Minimum & value for smallest steps
//       G4double maxEps= 1.0e-4;  //   Maximum & value for largest steps
// 
//       fieldMgr->SetMinimumEpsilonStep( minEps );
//       fieldMgr->SetMaximumEpsilonStep( maxEps );
//       fieldMgr->SetDeltaOneStep( 0.5e-1 * mm );  // 0.5 micrometer
// 
//       G4TransportationManager* tmanager = G4TransportationManager::GetTransportationManager();
//       tmanager->GetPropagatorInField()->SetLargestAcceptableStep(20.*m);
// 
//       expHall_log ->SetFieldManager ( fieldMgr, true );
// //    localFieldMgr->CreateChordFinder(myField);
//       fieldIsInitialized = true;

      /*
      double pos[4] = {9.0*cm,-113.0*cm,113.0*cm,0};
      double B[3];
      myField->GetFieldValue(pos,B);
      //G4cout << " \n FIELD (" << B[0]/tesla*10000.0 << " ," << B[1]/tesla*10000.0 << " ," << B[2]/tesla*10000.0 << ") \n\n " ;
      int i,j;
      G4cout << " \n Units are cm and Gauss " ;
      G4cout << " \n x(vert)  y(horz)  z(beam)   Bx   By   Bz  " ;
      for(i=0;i<10;i++){
      for(j=0;j<10;j++){
      pos[0] = 0.0*cm;
      pos[1] = -1.0*i*10.0*cm -70.0*cm;
      pos[2] = j*10.0*cm;
      myField->GetFieldValue(pos,B);
      G4cout << "\n " << pos[0]/cm << " "  << pos[1]/cm << " " << pos[2]/cm << " " << B[0]/tesla*10000.0 << " " << B[1]/tesla*10000.0 << " " << B[2]/tesla*10000.0 ;
      }
      G4cout << " \n " ;
      }
      G4cout << " \n " ;
      */
//   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Visualizations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   /*
    expHall_log->SetVisAttributes ( G4VisAttributes::Invisible );
    BETADetector_log->SetVisAttributes ( G4VisAttributes::Invisible );
    hodoscopeContainerBox_log->SetVisAttributes ( G4VisAttributes::Invisible );

    G4VisAttributes* tankAttributes = new G4VisAttributes ( G4Colour ( 0.0,0.5,0.5,0.5 ) );
    tankAttributes->SetForceWireframe ( true );
    tankAttributes->SetDaughtersInvisible ( false );
    tank_log->SetVisAttributes ( tankAttributes );
    tank_log->SetVisAttributes ( G4VisAttributes::Invisible );
   // AlCans_log->SetVisAttributes(G4VisAttributes::Invisible);

    G4VisAttributes* BIGCALAttributes = new G4VisAttributes ( G4Colour ( 0.1,0.5,0.0,0.5 ) );
   //  BIGCALAttributes->SetDaughtersInvisible(false);
   //   calorimeterTop_log->SetVisAttributes(BIGCALAttributes);
   //   calorimeterBottom_log->SetVisAttributes(BIGCALAttributes);
    calorimeterTop_log->SetVisAttributes ( G4VisAttributes::Invisible );
    calorimeterBottom_log->SetVisAttributes ( G4VisAttributes::Invisible );

    G4VisAttributes* BIGCALCellAttributes = new G4VisAttributes ( G4Colour ( 0.1,0.5,0.0,0.5 ) );
    BIGCALCellAttributes->SetForceSolid ( true );
   //  cellLogical->SetVisAttributes(G4VisAttributes::Invisible);
   //  cellLogicalBottom->SetVisAttributes(G4VisAttributes::Invisible);
    cellLogical->SetVisAttributes ( BIGCALCellAttributes );
    cellLogicalBottom->SetVisAttributes ( BIGCALCellAttributes );



    G4VisAttributes* hodoscopeAttributes = new G4VisAttributes ( G4Colour ( 0.5,0.0,0.7,0.5 ) );
    G4VisAttributes* hodoscopeBoxAttributes = new G4VisAttributes ( G4Colour ( 0.5,0.0,0.7,0.5 ) );
    hodoscopeBoxAttributes->SetForceWireframe ( true );
    hodoscopeAttributes->SetForceSolid ( true );
   //  hodoscope_log->SetVisAttributes(hodoscopeAttributes);
    hodoscope_log->SetVisAttributes ( hodoscopeBoxAttributes );
    fLuciteHodoBar_log->SetVisAttributes ( hodoscopeAttributes );

    G4VisAttributes* tedlarAttributes = new G4VisAttributes ( G4Colour::Blue() );
    tedlarAttributes->SetForceSolid ( true );
    fCerFrontWindow_log->SetVisAttributes ( tedlarAttributes );
    tedlarBack_log->SetVisAttributes ( tedlarAttributes );

    G4VisAttributes* mirrorAttributes = new G4VisAttributes ( G4Colour::Yellow() );
    m
   irrorAttributes->SetVisibility ( true );
    fCerSphericalMirror_log->SetVisAttributes ( mirrorAttributes );
    fCerToroidalMirror_log->SetVisAttributes ( mirrorAttributes );

   // AlCans_log->SetForceSolid(true);

    G4VisAttributes* floorAttributes = new G4VisAttributes ( G4Colour::Grey() );
    floorAttributes->SetForceSolid ( true );
    beamPipe_log->SetVisAttributes ( floorAttributes );
    floor_log->SetVisAttributes ( floorAttributes );
   */

//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ------------- Surfaces --------------
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////
// Generate & Add Material Properties Table attached to the optical surfaces
/////////////////////////////////////////////////////////////////////////////
   const G4int num = 2;
   G4double Ephoton[num] = {1.0*eV, 10.0*eV};

/////////////////////////////////////
// Nitrogen Radiator Surface - TANK
/////////////////////////////////////

   G4OpticalSurface* OpNitrogenSurface = new G4OpticalSurface ( "NitrogenSurface" );
   OpNitrogenSurface->SetType ( dielectric_dielectric );
   OpNitrogenSurface->SetFinish ( ground );
   OpNitrogenSurface->SetModel ( unified );

   G4LogicalSkinSurface* NitrogenSurface =
      new G4LogicalSkinSurface ( "NitrogenSurface", tank_log, OpNitrogenSurface );

// OpticalNitrogenSurface properties

   G4double RefractiveIndex[num] = {1.00029, 1.00048};
   G4double SpecularLobe[num]    = {0.3, 0.3};
   G4double SpecularSpike[num]   = {0.2, 0.2};
   G4double Backscatter[num]     = {0.2, 0.2};
   G4double Reflectivity[num] = {0.3, 0.5};
   G4double Efficiency[num]   = {1.0, 1.0};

   G4MaterialPropertiesTable* nitrogenSurfacePTable = new G4MaterialPropertiesTable();

   nitrogenSurfacePTable->AddProperty ( "RINDEX",                Ephoton, RefractiveIndex, num );
   nitrogenSurfacePTable->AddProperty ( "SPECULARLOBECONSTANT",  Ephoton, SpecularLobe,    num );
   nitrogenSurfacePTable->AddProperty ( "SPECULARSPIKECONSTANT", Ephoton, SpecularSpike,   num );
   nitrogenSurfacePTable->AddProperty ( "BACKSCATTERCONSTANT",   Ephoton, Backscatter,     num );
   nitrogenSurfacePTable->AddProperty ( "REFLECTIVITY",        Ephoton, Reflectivity,    num );
//  nitrogenSurfacePTable->AddProperty("EFFICIENCY",          Ephoton, Efficiency,      num);

   OpNitrogenSurface->SetMaterialPropertiesTable ( nitrogenSurfacePTable );
   /*
     if(AirSurface->GetLogicalVolume() == bubbleAir_log) G4cout << "Equal" << G4endl;
     ((G4OpticalSurface*)
     (AirSurface->GetSurface(bubbleAir_log)->GetSurfaceProperty()))->DumpInfo();
   */

//////////////////////////////
// target surface - TARGET
//////////////////////////////
   G4OpticalSurface* OpTargetSurface = new G4OpticalSurface ( "TargetSurface" );
   OpTargetSurface->SetType ( dielectric_dielectric );
   OpTargetSurface->SetFinish ( polished );
   OpTargetSurface->SetModel ( unified );

   G4LogicalSkinSurface* TargetnSurface =
      new G4LogicalSkinSurface ( "TargetSurface", target_log, OpTargetSurface );

   G4double RefractiveIndexT[num] = {1.0, 1.0};

   G4MaterialPropertiesTable* targetSurfacePTable = new G4MaterialPropertiesTable();

   targetSurfacePTable->AddProperty ( "RINDEX",                Ephoton, RefractiveIndexT, num );
   targetSurfacePTable->AddProperty ( "SPECULARLOBECONSTANT",  Ephoton, SpecularLobe,    num );
   targetSurfacePTable->AddProperty ( "SPECULARSPIKECONSTANT", Ephoton, SpecularSpike,   num );
   targetSurfacePTable->AddProperty ( "BACKSCATTERCONSTANT",   Ephoton, Backscatter,     num );
   targetSurfacePTable->AddProperty ( "REFLECTIVITY", Ephoton, Reflectivity, num );
   targetSurfacePTable->AddProperty ( "EFFICIENCY",   Ephoton, Efficiency,   num );

   OpTargetSurface->SetMaterialPropertiesTable ( targetSurfacePTable );




///////////////// VGM ////////////////////////
   // Export geometry in Root and save it in a file
   // Import Geant4 geometry to VGM
   //

//             Geant4GM::Factory g4Factory;
    //         g4Factory.SetDebug(1);
//             g4Factory.Import(expHall_phys);
     // Export VGM geometry to Root
// 
//  XmlVGM::GDMLExporter xmlExporter2(&g4Factory);
//  xmlExporter2.SetFileName("XML_test.xml");
//  xmlExporter2.GenerateXMLGeometry();
// 
//      // Export VGM geometry to Root
//      //
//              RootGM::Factory rtFactory;
   //           rtFactory.SetDebug(1);
//              g4Factory.Export(&rtFactory);
//              gGeoManager->CloseGeometry();
//              gGeoManager->Export("SANEGeometry.root");

//    
///////////////////////////////////////////////////////
// END OF OPTICAL SURFACES
///////////////////////////////////////////////////////
// Print material properties to file
   /*
    std::ofstream outfile("materialTable.dat", std::ios_base::out);
   outfile <<  *(G4Material::GetMaterialTable())
   outfile.close();
    std::ofstream outfile("materialTable.dat", std::ios_base::out);
   */
//always return the physical World

   SetVisAtt();

   return expHall_phys;
}
//___________________________________________________________________

void BETADetectorConstruction::ConstructMagneticField() {

      if(!fMagneticField) fMagneticField = new BETAField();
      fMagneticField->fUVAMagnet->SetPolarizationAngle (messenger->fTargetAngle ); 
      G4FieldManager* fieldMgr
         = G4TransportationManager::GetTransportationManager()->GetFieldManager();

      fieldMgr->SetDetectorField ( fMagneticField );
      fieldMgr->CreateChordFinder ( fMagneticField );

      G4double miss_distance = 0.00010*mm;
      G4double delta_intersection = 0.5e-3 * mm;
      G4double delta_one_step = 0.5e-3 * mm;
      G4double minEps= 1.0e-5;  //   Minimum & value for smallest steps
      G4double maxEps= 1.0e-4;  //   Maximum & value for largest steps

// the “miss distance”
// It is a measure of the error in whether the approximate track intersects a volume.
// It is quite expensive in CPU performance to set too small “miss distance”.
      fieldMgr->GetChordFinder()->SetDeltaChord ( miss_distance ); // miss distance

// The “delta intersection” parameter is the accuracy to which an intersection with a volume boundary is calculated. This parameter is especially important because it is used to limit a bias that our algorithm (for boundary crossing in a field) exhibits. The intersection point is always on the 'inside' of the curve. By setting a value for this parameter that is much smaller than some acceptable error, the user can limit the effect of this bias.


      fieldMgr->SetDeltaIntersection(delta_intersection);

// The “delta one step” parameter is the accuracy for the endpoint of 'ordinary' integration steps, those which do not intersect a volume boundary. This parameter is a limit on the estimation error of the endpoint of each physics step.
      fieldMgr->SetDeltaOneStep( delta_one_step );  // 0.5 micrometer


      fieldMgr->SetMinimumEpsilonStep( minEps );
      fieldMgr->SetMaximumEpsilonStep( maxEps );

      G4TransportationManager* tmanager = G4TransportationManager::GetTransportationManager();
//       tmanager->GetPropagatorInField()->SetLargestAcceptableStep(20.*m);

      expHall_log ->SetFieldManager ( fieldMgr, true );
//    localFieldMgr->CreateChordFinder(fMagneticField);
/*      fieldIsInitialized = true;*/
}
//______________________________________________________________________________

void BETADetectorConstruction::SetMaterialPropertiesTables()
{
// Lucite
   const int lucitedatapoints =  2;
   G4double LucitePhotonEnergy[lucitedatapoints] =//{1.57*eV, 1.59*eV};
      {0.2*eV,1.4*eV
/*,2.27*eV,2.89*eV,3.48013631*eV,
       3.84013632*eV,3.84013633*eV,3.84013634*eV,3.84013635*eV,
       3.84013636*eV,3.84013637*eV,4.2024*eV,6.186886*eV,10.0*eV*/
      };

   G4double LuciteRefractiveIndex[lucitedatapoints] =//{1.49,1.49};
      {1.49,1.49/*,1.49,1.49,1.49,1.49,1.49
       ,1.49,1.49,1.49,1.49,1.49,1.49,1.49*/
      };

   G4double LuciteAbsLength[lucitedatapoints] =//  {0.2611*m,0.021193*m};
      {0.2611*m, 0.2791*m/*, 0.2791*m, 0.26115*m,
       0.19275*m, 0.1928*m, 0.0941*m, 0.05397*m,
       0.032644*m, 0.02735*m, 0.0228*m, 0.02199*m,
       0.021193*m,0.021193*m*/
      };
    G4MaterialPropertiesTable* luciteMPT = new G4MaterialPropertiesTable();
    luciteMPT->AddProperty ( "RINDEX", LucitePhotonEnergy, LuciteRefractiveIndex, lucitedatapoints );
    luciteMPT->AddProperty ( "ABSLENGTH",    LucitePhotonEnergy, LuciteAbsLength,     lucitedatapoints );
    Lucite->SetMaterialPropertiesTable ( luciteMPT );

// LeadGlass
//    G4double LeadGlassPhotonEnergy[lucitedatapoints] =//{1.57*eV, 1.59*eV};
//       {0.2*eV,1.4*eV};
//    G4double LeadGlassRefractiveIndex[lucitedatapoints] =//{1.49,1.49};
//       {2.3,2.30 };
//    G4double LeadGlassAbsLength[lucitedatapoints] =//  {0.2611*m,0.021193*m};
//       {0.2611*m, 0.2791*m/*, 0.2791*m, 0.26115*m,
//        0.19275*m, 0.1928*m, 0.0941*m, 0.05397*m,
//        0.032644*m, 0.02735*m, 0.0228*m, 0.02199*m,
//        0.021193*m,0.021193*m*/
//       };
//     G4MaterialPropertiesTable* LeadGlassMPT = new G4MaterialPropertiesTable();
//     LeadGlassMPT->AddProperty ( "RINDEX", LeadGlassPhotonEnergy, LeadGlassRefractiveIndex, lucitedatapoints );
//     LeadGlassMPT->AddProperty ( "ABSLENGTH",    LeadGlassPhotonEnergy, LeadGlassAbsLength,     lucitedatapoints );
//     LeadGlass->SetMaterialPropertiesTable ( LeadGlassMPT );


   const G4int nEntries = 32;
   G4double PhotonEnergy[nEntries] =
      { 0.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
        2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
        2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
        2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
        2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
        3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
        3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
        3.760*eV, 3.877*eV, 4.002*eV, 20.0*eV
      };
// Water
   G4double waterRefractiveIndex[nEntries] =
      { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
        1.346,  1.3465, 1.347,  1.3475, 1.348,
        1.3485, 1.3492, 1.35,   1.3505, 1.351,
        1.3518, 1.3522, 1.3530, 1.3535, 1.354,
        1.3545, 1.355,  1.3555, 1.356,  1.3568,
        1.3572, 1.358,  1.3585, 1.359,  1.3595,
        1.36,   1.3608
      };

   G4double waterAbsorption[nEntries] =
      {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
       15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
       45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
       52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
       30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
       17.500*m, 14.500*m
      };

   G4double ScintilFast[nEntries] =
      { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00
      };
   G4double ScintilSlow[nEntries] =
      { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
        7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
        3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
        4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
        7.00, 6.00, 5.00, 4.00
      };

   G4MaterialPropertiesTable* waterMPT = new G4MaterialPropertiesTable();
   waterMPT->AddProperty ( "RINDEX",       PhotonEnergy, waterRefractiveIndex,nEntries );
   waterMPT->AddProperty ( "ABSLENGTH",    PhotonEnergy, waterAbsorption,     nEntries );
   waterMPT->AddProperty ( "FASTCOMPONENT",PhotonEnergy, ScintilFast,     nEntries );
   waterMPT->AddProperty ( "SLOWCOMPONENT",PhotonEnergy, ScintilSlow,     nEntries );
   waterMPT->AddConstProperty ( "SCINTILLATIONYIELD",50./MeV );
   waterMPT->AddConstProperty ( "RESOLUTIONSCALE",1.0 );
   waterMPT->AddConstProperty ( "FASTTIMECONSTANT", 1.*ns );
   waterMPT->AddConstProperty ( "SLOWTIMECONSTANT",10.*ns );
   waterMPT->AddConstProperty ( "YIELDRATIO",0.8 );

   Water->SetMaterialPropertiesTable ( waterMPT );

// Air
   G4double airIndexOfRefraction=1.000293;
   G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
/*   airMPT->AddConstProperty ( "RINDEX",airIndexOfRefraction );*/
   airMPT->AddConstProperty ( "ABSLENGTH",1000.0*m );
   Air->SetMaterialPropertiesTable ( airMPT );

//
// Vacuum
//
//    G4double vacuumRefractiveIndex[nEntries] =
//       { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
//         1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
//         1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
//         1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
//         1.00, 1.00, 1.00, 1.00
//       };

//   G4MaterialPropertiesTable* vacuumMPT = new G4MaterialPropertiesTable();
//   vacuumMPT->AddProperty ( "RINDEX", PhotonEnergy, vacuumRefractiveIndex, nEntries );
//   Vacuum->SetMaterialPropertiesTable ( myMPTV );



//
// Nitrogen Gas
//

   const G4int nEnergies = 32;

   G4double NitrogenPhotonEnergy[nEnergies] =
      {1.8*eV,2.*eV,2.2*eV,2.4000000000000004*eV,2.6*eV,2.8000000000000003*eV,3.*eV,3.2*eV,3.4000000000000004*eV,
       3.6*eV,3.8000000000000003*eV,4.*eV,4.2*eV,4.4*eV,
       4.6000000000000005*eV,4.800000000000001*eV,5.*eV,5.2*eV,5.4*eV,5.6000000000000005*eV,
       5.800000000000001*eV,6.*eV,6.2*eV,6.4*eV,6.6000000000000005*eV,
       6.800000000000001*eV,7.*eV,7.2*eV,7.4*eV,7.6000000000000005*eV,7.800000000000001*eV,8.2*eV
      };


   G4double NitrogenRefractiveIndexN[nEnergies] =
      {1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.00031, \
       1.00031, 1.00031, 1.00031, 1.00031, 1.00032, 1.00032, 1.00032, 1.00032, \
       1.00033, 1.00033, 1.00033, 1.00033, 1.00034, 1.00034, 1.00035, 1.00035, \
       1.00035, 1.00036, 1.00036, 1.00037, 1.00037, 1.00038, 1.00039
      };
   G4double AbsorptionN[nEntries] =
      {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
       15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
       45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
       52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
       30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
       17.500*m, 14.500*m
      };

   G4MaterialPropertiesTable* nitrogenMPT = new G4MaterialPropertiesTable();
   nitrogenMPT->AddProperty ( "RINDEX", NitrogenPhotonEnergy, NitrogenRefractiveIndexN, nEnergies );
   nitrogenMPT->AddConstProperty ( "ABSLENGTH",1000.0*m );
   NitrogenGas->SetMaterialPropertiesTable ( nitrogenMPT );

   //----- Tracker Scint Material Properties
   const G4int NUMENTRIESbc408 = 12;
   G4double PhotonEnergybc408[NUMENTRIESbc408] =
        { 3.44*eV, 3.26*eV, 3.1*eV, 3.02*eV, 2.95*eV,
          2.92*eV, 2.82*eV, 2.76*eV, 2.7*eV, 2.58*eV,
          2.38*eV, 2.08*eV };
   G4double RINDEX_Bc408[NUMENTRIESbc408] =
        { 1.58, 1.58, 1.58, 1.58, 1.58,
          1.58, 1.58, 1.58, 1.58, 1.58,
          1.58, 1.58 };
   G4double ABSORPTION_Bc408[NUMENTRIESbc408] =
        { 210*cm, 210*cm, 210*cm, 210*cm, 210*cm,
          210*cm, 210*cm, 210*cm, 210*cm, 210*cm,
          210*cm, 210*cm };
   G4double SCINTILLATION_Bc408[NUMENTRIESbc408] =
        { 0.04, 0.07, 0.20, 0.49, 0.84,
          1.00, 0.83, 0.55, 0.40, 0.17,
          0.03, 0 };
   G4MaterialPropertiesTable *Bc408_mt = new G4MaterialPropertiesTable();
   Bc408_mt->AddProperty("RINDEX", PhotonEnergybc408, RINDEX_Bc408, NUMENTRIESbc408);
   Bc408_mt->AddProperty("ABSLENGTH", PhotonEnergybc408, ABSORPTION_Bc408, NUMENTRIESbc408);
   Bc408_mt->AddProperty("FASTCOMPONENT", PhotonEnergybc408, SCINTILLATION_Bc408, NUMENTRIESbc408);
   Bc408_mt->AddConstProperty("SCINTILLATIONYIELD",500./MeV);
   Bc408_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
   Bc408_mt->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
   //Bc408_mt->AddConstProperty("SLOWTIMECONSTANT",1.*ns);
   Bc408_mt->AddConstProperty("YIELDRATIO",1.);
   //Bc408->SetMaterialPropertiesTable(Bc408_mt);
   TrackerScint->SetMaterialPropertiesTable ( Bc408_mt );
   /*G4MaterialPropertiesTable* trackerScintMPT = new G4MaterialPropertiesTable();
   trackerScintMPT->AddConstProperty ( "RINDEX", 1.59 );
   trackerScintMPT->AddConstProperty ( "ABSLENGTH",210.0*cm );
   TrackerScint->SetMaterialPropertiesTable ( trackerScintMPT );
   */

// Setting Glass==nitrogen!!!!!!!!!!!!!!! WRONGs
   Glass->SetMaterialPropertiesTable ( nitrogenMPT );

// Quartz
   G4double QuartzRefractiveIndexN[nEnergies] =
      { 1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,
        1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,
        1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,1.4585,
        1.4585,1.4585
      };
   G4MaterialPropertiesTable* quartzMPT = new G4MaterialPropertiesTable();
   quartzMPT->AddProperty ( "RINDEX", NitrogenPhotonEnergy, QuartzRefractiveIndexN, nEnergies );
   Quartz->SetMaterialPropertiesTable ( quartzMPT );
//  G4cout << *(G4Material::GetMaterialTable());  // print the list of materials

}
//___________________________________________________________________

void BETADetectorConstruction::DefineMaterials()
{

   G4NistManager* nistman = G4NistManager::Instance();
   //nistman->SetVerbose ( 1 );
   G4double a, z, density,fractionmass;
   G4int nelements,ncomponents,natoms;
   G4String symbol;
// define Elements
   G4Element* H  = new G4Element("Hydrogen",symbol="H",z= 1., a= 1.01*g/mole );
   G4Element* C  = new G4Element("Carbon" ,symbol="C",z= 6., a= 12.01*g/mole );
   G4Element* N  = new G4Element("Nitrogen",symbol="N",z= 7., a= 14.01*g/mole );
   G4Element* O  = new G4Element("Oxygen",symbol="O",z= 8., a= 16.00*g/mole );
   G4Element* Si = new G4Element("Silicon",symbol="Si",z= 14.,a= 28.09*g/mole );


   //----- Air
   Air = new G4Material ( "Air", density=1.29*mg/cm3, nelements=2 );
   Air->AddElement ( N, 70.*perCent );
   Air->AddElement ( O, 30.*perCent );

   //----- Aluminum
   Aluminum = nistman->FindOrBuildMaterial ( "G4_Al" );
   Al = Aluminum;

   //----- Lead 
   Pb =  new G4Material ( "Lead", z=82., a=207.19*g/mole, density=11.35*g/cm3 );

   //----- Liquid Helium-4
   LHe = new G4Material("LHe", z=2., a=4.002602*g/mole, density=0.1249*g/cm3 );

   //----- Copper
   Cu = new G4Material ( "Copper", z=29, a=63.55*g/mole, density=8.96*g/cm3 );

   //----- Berillium
   Be = new G4Material ( "Be", z =4., a=9.01218*g/mole, density=1.85*g/cm3 );

   //----- Niobium
   Nb = new G4Material ( "Niobium", z=41., z=93.*g/mole, density=8.56*g/cm3 );


   /// \todo add packing fraction information here
   //----- Polarized NH3
   //Let's say the target cell is half NH3 and half LHe. (packing fraction 0.5)
   G4Material* NH3solid =
      new G4Material("SolidAmmonia", density=.9*g/cm3, ncomponents=2 );
   NH3solid->AddElement ( H, natoms=3 );
   NH3solid->AddElement ( N, natoms=1 );

   TargetNH3 = new G4Material ( "TargetNH3", density = 0.5*g/cm3, ncomponents=2 );
   TargetNH3->AddMaterial ( NH3solid, fractionmass=88.*perCent );
   TargetNH3->AddMaterial ( LHe, fractionmass=12.*perCent );

   N2Gas = new G4Material ( "N2Gas", density=1.*g/cm3, ncomponents=1 );
   N2Gas->AddElement ( N, natoms = 2 );

   HeGas =  new G4Material ( "HeGas", z=2., a=4.002602*g/mole, density=(0.1786/1000.0)*g/cm3 );


   //----- Water
   G4Material* H2O =
      new G4Material ( "Water", density= 1.000*g/cm3, ncomponents=2 );
   H2O->AddElement ( H, natoms=2 );
   H2O->AddElement ( O, natoms=1 );
   // overwrite computed meanExcitationEnergy with ICRU recommended value
   H2O->GetIonisation()->SetMeanExcitationEnergy ( 75.0*eV );


   //----- G4_POLYVINYLIDENE_FLUORIDE (Plexiglass,Lucite,or whatever you want to call it)
   PVF = nistman->FindOrBuildMaterial ( "G4_POLYVINYLIDENE_FLUORIDE" );

   //----- Ammonia
   NH3 = nistman->FindOrBuildMaterial ( "G4_AMMONIA" );

   //----- G4_Pyrex_Glass
   Glass = nistman->FindOrBuildMaterial ( "G4_Pyrex_Glass" );

   //----- G4_GLASS_LEAD
   LeadGlass = nistman->FindOrBuildMaterial ( "G4_GLASS_LEAD" );

   //----- QUARTZ
   Quartz = nistman->FindOrBuildMaterial ( "G4_SILICON_DIOXIDE" );

   //----- Lead 
   Lead =   nistman->FindOrBuildMaterial ( "G4_Pb" );

   //----- G4_PLEXIGLASS
   Lucite = nistman->FindOrBuildMaterial ( "G4_PLEXIGLASS" );
   Lucite_NoOptics = new G4Material("Lucite_NoOptics",Lucite->GetDensity(),1 );
   Lucite_NoOptics->AddMaterial(Lucite,1.0);
   //nistman->FindOrBuildMaterial ( "G4_PLEXIGLASS" );

   //----- Bicron BC-408 (or similar) Plastic
   TrackerScint = nistman->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
   TrackerScint_NoOptics = new G4Material("TrackerScint_NoOptics",TrackerScint->GetDensity(),1 );
   TrackerScint_NoOptics->AddMaterial(TrackerScint,1.0);

   //----- Water
   Water = new G4Material ( "Water", density= 1.0*g/cm3, nelements=2 );
   Water->AddElement ( H, 2 );
   Water->AddElement ( O, 1 );

   //----- Nitrogen Gas, N2
   NitrogenGas = new G4Material ( "Nitrogen", z=7,  a=14.0067*g/mole, density=1250.6*g/m3 );
   NitrogenGas_NoOptics = new G4Material ( "Nitrogen", z=7,  a=14.0067*g/mole, density=1250.6*g/m3 );

   //----- Vacuum
   Vacuum = new G4Material ( "Vacuum", density= 1.e-5*g/cm3,
                             ncomponents=1, kStateGas, STP_Temperature,
                             2.e-2*bar );
   Vacuum->AddMaterial( Air, fractionmass=1. );

   //----- Silver
   Silver = new G4Material ( "Silver", z=47, a=107.8682*g/mole, density=10.49*g/cm3 );

   //----- CsI
   G4Element* Cs = new G4Element ( "Cesium", "Cs", z=55., a=132.9*g/mole );
   G4Element* I = new G4Element ( "Iodine", "I", z=53., a=126.9*g/mole );
   CsI = new G4Material ( "CsI", 4.51*g/cm3, 2 );
   CsI->AddElement ( I, .5 );
   CsI->AddElement ( Cs, .5 );

   SetMaterialPropertiesTables();

}

//___________________________________________________________________
void BETADetectorConstruction::lookAtField(G4String comp) {
  if(!fMagneticField) fMagneticField = new BETAField();
  if( fMagneticField )  {
   G4cout << "Looking at the radial and z components\n";
/*    TRint *theApp = new TRint("To Look at Magnetic Field", NULL,NULL);*/
//    (gROOT->GetApplication()) =  new TRint("ROOT example", NULL, NULL);

   fMagneticField->fUVAMagnet->LookAtFieldRadialComponent(50,2.0);
   fMagneticField->fUVAMagnet->LookAtFieldZComponent(50,2.0);
   fMagneticField->fUVAMagnet->LookAtFieldAbsoluteValue(50,2.0);
   // Init Intrinsics, build all windows, and enter event loop
//   gROOT->Reset();
   (gROOT->GetApplication())->Run(true);
  } else {
        G4cout << " Magnetic Field NOT Constructed Yet!" << G4endl;
  } 
}

//___________________________________________________________________
void BETADetectorConstruction::switchTargetField()
{
   if ( messenger->fTargetAngle == TMath::Pi() )
   {
      G4cout << "Target switching to TRANSVERSE field orientation." << G4endl;
      messenger->fTargetAngle = 80.0*TMath::Pi()/180.0;
   }
   else
   {
      G4cout << "Target switching to ANTIPARALLEL field orientation." << G4endl;
      messenger->fTargetAngle = TMath::Pi();
   }
  if(!fMagneticField) fMagneticField = new BETAField();

  if(fMagneticField) setTargetAngle (messenger->fTargetAngle );
   G4RunManager::GetRunManager()->GeometryHasBeenModified();

}

//___________________________________________________________________
void BETADetectorConstruction::setTargetAngle ( G4double angle )
{
  messenger->fTargetAngle = angle;
  PrintTargetAngle();
  if(!fMagneticField) fMagneticField = new BETAField();
  fMagnetRotationMatirx = G4RotationMatrix::IDENTITY;
  fMagnetRotationMatirx.rotateZ( messenger->fTargetAngle );
  if(fMagneticField) {
    fMagneticField->fUVAMagnet->SetPolarizationAngle (angle );
    fCerSphericalMirror_log->RemoveDaughter ( MirrorGlass_phys1 );
    delete physiMagnet;
    physiMagnet = new G4PVPlacement (G4Transform3D(fMagnetRotationMatirx, G4ThreeVector ( 0.,0.,-LN2dis )),
                                     logicMagnet, "Magnet", logicLN2Shield,
                                     false, 0 );
  }
   G4RunManager::GetRunManager()->GeometryHasBeenModified();

}
//___________________________________________________________________

double BETADetectorConstruction::getTargetAngle() {
  if(!fMagneticField) fMagneticField = new BETAField();
  return( fMagneticField->fUVAMagnet->fPolarizationAngle );
}

void BETADetectorConstruction::PrintTargetAngle() {
  if(!fMagneticField) fMagneticField = new BETAField();
  std::cout << " target angle is " << fMagneticField->fUVAMagnet->fPolarizationAngle*180.0/TMath::Pi() << " Degrees\n";
}

//___________________________________________________________________
void BETADetectorConstruction::setMirrorAngles ( int mirrorNumber, G4double alpha, G4double beta )
{
   if ( mirrorNumber ==1 )
   {
      RM1.rotateX ( alpha );
      RM1.rotateY (beta );
   }
   else if ( mirrorNumber==2 )
   {
      RM2.rotateX ( alpha );
      RM2.rotateY ( beta );
   }
   else if ( mirrorNumber==3 )
   {
      RM3.rotateX ( alpha );
      RM3.rotateY ( beta );
   }
   else if ( mirrorNumber==4 )
   {
      RM4.rotateX ( alpha );
      RM4.rotateY ( beta );
   }
   else if ( mirrorNumber==5 )
   {
      RM5.rotateX ( alpha );
      RM5.rotateY ( beta );
   }
   else if ( mirrorNumber==6 )
   {
      RM6.rotateX ( alpha );
      RM6.rotateY ( beta );
   }
   else if ( mirrorNumber==7 )
   {
      RM7.rotateX ( alpha );
      RM7.rotateY ( beta );
   }
   else if ( mirrorNumber==8 )
   {
      RM8.rotateX ( alpha );
      RM8.rotateY (beta );
   }

   if ( !constructed ) return ;


}

//___________________________________________________________________
void BETADetectorConstruction::rotateMirror ( int mirrorNumber, G4double alpha, G4double beta )
{
   G4RotationMatrix newRM;
   if ( mirrorNumber ==1 )
   {
      RM1 = newRM;
      RM1.rotateY ( alpha+alpha1 );
      RM1.rotateX ( beta+beta1 );
      fCerSphericalMirror_log->RemoveDaughter ( MirrorGlass_phys1 );
      delete MirrorGlass_phys1;
      MirrorGlass_phys1 = new G4PVPlacement ( G4Transform3D ( RM1,mirror1Trans ), fCerSphericalMirror_log , "Far - Mirror",  tank_log, false,1 );
   }
   else if ( mirrorNumber==2 )
   {
      RM2 = newRM;

      RM2.rotateY ( alpha+alpha2 );
      RM2.rotateX ( beta+beta2 );
      fCerToroidalMirror_log->RemoveDaughter ( MirrorGlass_phys2 );
      delete MirrorGlass_phys1;
      MirrorGlass_phys2 = new G4PVPlacement ( G4Transform3D ( RM2,mirror2Trans ), fCerToroidalMirror_log , "Far - Mirror",  tank_log, false,2 );
   }
   else if ( mirrorNumber==3 )
   {
      RM3 = newRM;

      RM3.rotateY ( alpha+alpha3 );
      RM3.rotateX ( beta+beta3 );
      fCerSphericalMirror_log->RemoveDaughter ( MirrorGlass_phys3 );
      delete MirrorGlass_phys3;
      MirrorGlass_phys3 = new G4PVPlacement ( G4Transform3D ( RM3,mirror3Trans ), fCerSphericalMirror_log , "Far - Mirror",  tank_log, false,3 );
   }
   else if ( mirrorNumber==4 )
   {
      RM4 = newRM;

      RM4.rotateY ( alpha+alpha4 );
      RM4.rotateX ( beta+beta4 );
      fCerToroidalMirror_log->RemoveDaughter ( MirrorGlass_phys4 );
      delete MirrorGlass_phys4;
      MirrorGlass_phys4 = new G4PVPlacement ( G4Transform3D ( RM4,mirror4Trans ), fCerToroidalMirror_log , "Far - Mirror",  tank_log, false,4 );
   }
   else if ( mirrorNumber==5 )
   {
      RM5 = newRM;

      RM5.rotateY ( alpha+alpha5 );
      RM5.rotateX ( beta+beta5 );
      fCerSphericalMirror_log->RemoveDaughter ( MirrorGlass_phys5 );
      delete MirrorGlass_phys5;
      MirrorGlass_phys5 = new G4PVPlacement ( G4Transform3D ( RM5,mirror5Trans ), fCerSphericalMirror_log , "Far - Mirror",  tank_log, false,5 );
   }
   else if ( mirrorNumber==6 )
   {
      RM6 = newRM;

      RM6.rotateY ( alpha+alpha6 );
      RM6.rotateX ( beta+beta6 );
      fCerToroidalMirror_log->RemoveDaughter ( MirrorGlass_phys6 );
      delete MirrorGlass_phys6;
      MirrorGlass_phys6 = new G4PVPlacement ( G4Transform3D ( RM6,mirror6Trans ), fCerToroidalMirror_log , "Far - Mirror",  tank_log, false,6 );
   }
   else if ( mirrorNumber==7 )
   {
      RM7 = newRM;

      RM7.rotateY ( alpha+alpha7 );
      RM7.rotateX ( beta+beta7 );
      fCerSphericalMirror_log->RemoveDaughter ( MirrorGlass_phys7 );
      delete MirrorGlass_phys7;
      MirrorGlass_phys7 = new G4PVPlacement ( G4Transform3D ( RM7,mirror7Trans ), fCerSphericalMirror_log , "Far - Mirror",  tank_log, false,7 );
   }
   else if ( mirrorNumber==8 )
   {
      RM8 = newRM;

      RM8.rotateY ( alpha+alpha8 );
      RM8.rotateX ( beta+beta8 );
      fCerToroidalMirror_log->RemoveDaughter ( MirrorGlass_phys8 );
      delete MirrorGlass_phys8;
      MirrorGlass_phys8 = new G4PVPlacement ( G4Transform3D ( RM8,mirror8Trans ), fCerToroidalMirror_log , "Far - Mirror",  tank_log, false,8 );
   }

   if ( !constructed ) return ;

   G4RunManager::GetRunManager()->GeometryHasBeenModified();

}

void BETADetectorConstruction::ToggleTargetMaterial( int state) {
   fTargetState = state;
   if(state == 1) {
      if(physiTCan)   delete physiTCan;
      physiTCan=0;
      if(physiLN2Can)   delete physiLN2Can;
      physiLN2Can=0;
      G4RotationMatrix targRot;
      targRot.rotateX ( -pi/2. );
      targRot.rotateY ( -pi/2. );
      physiTCan = new G4PVPlacement ( G4Transform3D ( targRot, G4ThreeVector ( 0.,0.,OVCdis ) ),
                                      logicTCan, "TrgtCanPhys", expHall_log, false, 0 );
      physiLN2Can = new G4PVPlacement ( G4Transform3D ( targRot, G4ThreeVector ( 0,0,0 ) ), logicLN2Can,
                                     "LN2SHCAN_phys", expHall_log, false, 0 );
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
  } else if(state == 0) {
      if(physiTCan)   delete physiTCan;
      physiTCan=0;
      if(physiLN2Can)   delete physiLN2Can;
      physiLN2Can=0;
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
}

void BETADetectorConstruction::ToggleTargetField( int state) {
  if(state == 1) {
    if(!fMagneticField) fMagneticField = new BETAField();
    if(fMagneticField) {
       fMagneticField->SetScaleFactor(1.0);
       G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
  } else if(state == 0) {
    if(!fMagneticField) fMagneticField = new BETAField();
    if(fMagneticField) {
       fMagneticField->SetScaleFactor(0.0);
       G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
  }
}


// From Justin Wright:
//___________________________________________________________________
//  Constructs the target can and everything in it.
//  Also calls the functions that construct the target magnet
//  and the nosepiece.
//  Last updated May 31, 2006 based on Shige's May schematic (non-final)
//___________________________________________________________________
void BETADetectorConstruction::ConstructTCan()
{
// ADDED to transform coordinates from Justins ( APPEARS TWICE . SEE BELOW )
   G4RotationMatrix targRot;
   targRot.rotateX ( -pi/2. );
   targRot.rotateY ( -pi/2. );


   //                             IMPORTANT
   //Because of the displacement, the can must be moved up by 'OVCdis' so that the
   //target is at the origin. The target MUST be at the origin because
   //the target field is not attached to the target volume. It is attached
   //to the world volume in which the target volume sits.

   //All constants for the OVC are defined at the start of this file.

   //The whole target can area (made of vacuum)
   G4VSolid* StartingTCan =
      new G4Tubs ( "TrgtCan", 0., OVCIR+OVCCthick, OVCCheight/2., 0., 7. );

   //The solid Aluminum part of the can, in which the windows will be placed.
   //Make it by subtracting a cylinder from a slightly larger cylinder.
   G4VSolid* Cylinder =
      new G4Tubs ( "WinCan", 0.*cm, OVCIR+OVCCthick, OVCCheight/2., 0., 7. );
   G4VSolid* Cavity =
      new G4Tubs ( "Cav", 0.*cm, OVCIR, ( OVCCheight-OVCCthick ) /2., 0., 7. );
   G4VSolid* solidHollowCan =
      new G4SubtractionSolid ( "WinCan", Cylinder, Cavity );

   //Subtract out a hole in the top of the OVC for the LN2 shield.
   //Assuming the LN2 shield wall is the same thickness as the OVC wall.
   G4VSolid* LN2dummy =

      new G4Tubs ( "dummy", 0.*cm, ( LN2IR+LN2Cthick ), LN2Cheight/2., 0, 7. );
   G4VSolid* solidDummyCan =
      new G4SubtractionSolid ( "WinCan", solidHollowCan, LN2dummy, 0,
                               G4ThreeVector ( 0, 0, LN2dis ) );

   G4VSolid* solidTCan = new G4SubtractionSolid ( "TrgtCan", StartingTCan, LN2dummy,
                                                  0, G4ThreeVector ( 0,0, LN2dis ) );
   logicTCan = new G4LogicalVolume ( solidTCan, Vacuum, "TrgtCanLog" );
   if(fTargetState) physiTCan = new G4PVPlacement ( G4Transform3D ( targRot, G4ThreeVector ( 0.,0.,OVCdis ) ),
                                   logicTCan, "TrgtCanPhys", expHall_log, false, 0 );

   //A space needs to be carved out of the 0.935 inch thick wall for the window
   //Fill it with vacuum and displace it -1.8542cm as per Shige's drawing.
   //This is for the window that will point to the HMS.
   //Use window thickness of .013cm Al for now. Will get actual thickness
   // from Shige later. Also assume window is flush with outer edge.
   G4VSolid* solidHMSSpace =
      new G4Tubs ( "HMSSpace", 0.*cm, OVCIR+OVCCthick, OVCWheight/2.,
                   -51.*deg, 60.*deg );
   G4VSolid* solidHMSWin =
      new G4Tubs ( "HMSWin", OVCIR+OVCCthick-OVCWthick, OVCIR+OVCCthick,
                   OVCWheight/2., -51.*deg, 60.*deg );

   //This is for the window that will point to the Beta Calorimeter.
   G4VSolid* solidBetaSpace =
      new G4Tubs ( "BetaSpace", 0.*cm, OVCIR+OVCCthick, OVCWheight/2.,
                   19.*deg, 32.*deg );
   G4VSolid* solidBetaWin =
      new G4Tubs ( "BetaWin", OVCIR+OVCCthick-OVCWthick, OVCIR+OVCCthick,
                   OVCWheight/2., 19.*deg, 32.*deg );

   //This is for the window that will be needed for the Semi_SANE exp.
   G4VSolid* solidSemiSpace =
      new G4Tubs ( "SemiSpace", 0.*cm, OVCIR+OVCCthick, OVCWheight/2.,
                   -108.*deg, 36.*deg );
   G4VSolid* solidSemiWin =
      new G4Tubs ( "SemiWin", OVCIR+OVCCthick-OVCWthick, OVCIR+OVCCthick,
                   OVCWheight/2., -108.*deg, 36.*deg );

   //This is for the window that will be needed for both RCS and Semi-SANE
   G4VSolid* solidRCSSpace =
      new G4Tubs ( "RCSSpace", 0.*cm, OVCIR+OVCCthick, OVCWheight/2.,
                   72.*deg, 36.*deg );
   G4VSolid* solidRCSWin =
      new G4Tubs ( "RCSWin", OVCIR+OVCCthick-OVCWthick, OVCIR+OVCCthick,
                   OVCWheight/2., 72.*deg, 36.*deg );

   //the rotation matrix for the beam space.
   G4RotationMatrix BeamWinRM;
   BeamWinRM.rotateY ( 90.*deg );

   //The cylinder that will be carved out of the target can at the beam
   // entrance. It will eventually be covered by the Be window.
   G4VSolid* solidBeamSpace =
      new G4Tubs ( "BeamSpace", 0., OVCBrad, 10.*cm, 0., 7. );
   G4VSolid* solidBeamCan =
      new G4Tubs ( "BeamWin", OVCIR+OVCCthick-OVCBthick, OVCIR+OVCCthick,
                   OVCCheight/2., 0., 7. );

   //Must systematically subtract all the window spaces out of the WinCan.
   G4VSolid* Step1 =
      new G4SubtractionSolid ( "Can_HMS", solidDummyCan, solidHMSSpace, 0,
                               G4ThreeVector ( 0.,0.,-OVCdis ) );
   G4VSolid* Step2 =
      new G4SubtractionSolid ( "Can_HMS_Beta", Step1, solidBetaSpace, 0,
                               G4ThreeVector ( 0.,0.,-OVCdis ) );
   G4VSolid* Step3 =
      new G4SubtractionSolid ( "Can_HMS_Beta_RCS", Step2, solidRCSSpace, 0,
                               G4ThreeVector ( 0.,0.,-OVCdis ) );
   G4VSolid* Step4 =
      new G4SubtractionSolid ( "Can_HMS_Beta_RCS_Semi", Step3 , solidSemiSpace, 0,
                               G4ThreeVector ( 0.,0.,-OVCdis ) );
   G4VSolid* Step5 =
      new G4SubtractionSolid ( "Can_AllSpaces", Step4, solidBeamSpace,
                               &BeamWinRM, G4ThreeVector ( -OVCIR, 0., -OVCdis ) );

   //Now must systematically add in all the Aluminum windows.
   G4VSolid* Step6 =
      new G4UnionSolid ( "Win_HMS", Step5, solidHMSWin, 0,
                         G4ThreeVector ( 0.,0.,-OVCdis ) );
   G4VSolid* Step7 =
      new G4UnionSolid ( "Win_HMS_Beta", Step6, solidBetaWin, 0,
                         G4ThreeVector ( 0.,0.,-OVCdis ) );
   G4VSolid* Step8 =
      new G4UnionSolid ( "Win_HMS_Beta_RCS", Step7, solidRCSWin, 0,
                         G4ThreeVector ( 0.,0.,-OVCdis ) );
   G4VSolid* solidWinCan =
      new G4UnionSolid ( "WinCan", Step8 , solidSemiWin, 0,
                         G4ThreeVector ( 0.,0.,-OVCdis ) );

   //solidWinCan is the final version of the target can minus the beam window.
   //The beam window must be a seperate object since it won't be made of Al.

   logicWinCan = new G4LogicalVolume ( solidWinCan, Al, "WinCan" );
   physiWinCan = new G4PVPlacement ( 0, G4ThreeVector(), logicWinCan, "WinCan",
                                     logicTCan, false, 0 );

   //Now we build the beam entrance window by intersecting a tube section
   // with the same cylinder used for the beam entrance space.
   G4VSolid* solidBeamWin =
      new G4IntersectionSolid ( "Intersect", solidBeamCan, solidBeamSpace,
                                &BeamWinRM, G4ThreeVector ( -OVCIR,0., -OVCdis ) );
   logicBeamWin = new G4LogicalVolume ( solidBeamWin, Be, "BeamWin" );
   physiBeamWin= new G4PVPlacement ( 0, G4ThreeVector(), logicBeamWin, "BeamWin",
                                     logicTCan, false, 0 );

//   // Radiator

/* G4RotationMatrix RMRad;
  RMRad.rotateY(PI/2.);

  G4double radiatorThickness = 0.5*((radPercent*CuRadiationLength)/100.);
  radX = -60.*cm - radiatorThickness;
  solidRad = new G4Tubs("Radiator",0., 4.*2.54*cm, radiatorThickness,0.,7.);
  logicRad = new G4LogicalVolume(solidRad, Cu, "Radiator");
  physiRad = new G4PVPlacement(G4Transform3D(RMRad,
          G4ThreeVector(radX,0.,0.)),
          logicRad, "Radiator", expHall_log, false, 0);

  //Radiator visualization attributes (red wire-frame)
  G4VisAttributes* radVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  radVisAtt->SetVisibility(true);
  radVisAtt->SetForceSolid(true);
  logicRad->SetVisAttributes(radVisAtt);
*/
   //The Visualization attributes of everything in the TCAN
   G4VisAttributes* WinCanVisAtt= new G4VisAttributes ( G4Colour ( 0.,0.,1. ) );
   WinCanVisAtt->SetVisibility ( true );
   WinCanVisAtt->SetForceSolid ( false );

// logicTCan->SetVisAttributes ( Invisible );
   logicWinCan->SetVisAttributes ( WinCanVisAtt );
// logicBeamWin->SetVisAttributes ( Invisible );

   logicTCan->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
   logicWinCan->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
   logicBeamWin->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
}

//___________________________________________________________________
void BETADetectorConstruction::ConstructN2Shield()
{
   //Similar to the target can itself, just a little smaller.
   //Updated June 1, 2006
   //Using 0.935 inch as the thickness of the wall. This is the same as the
   //   OVC, but is probably too thick. Will update when I get the final
   //   specs from Shige

   //All constants for the LN2 shield are defined at the start of this file.

   solidLN2Can = new G4Tubs ( "LN2SHCAN", 0.*cm, LN2IR+LN2Cthick, LN2Cheight/2.,
                              0., 7. );
   logicLN2Can = new G4LogicalVolume ( solidLN2Can, Vacuum, "LN2SHCAN" );
// ADDED to transform coordinates from Justins
   G4RotationMatrix targRot;
   targRot.rotateX ( -pi/2. );
   targRot.rotateY ( -pi/2. );

   if(fTargetState) physiLN2Can = new G4PVPlacement ( G4Transform3D ( targRot, G4ThreeVector ( 0,0,0 ) ), logicLN2Can,
                                     "LN2SHCAN_phys", expHall_log, false, 0 );

   //The solid Aluminum part of the shield, in which the windows will be placed.
   //Make the can by subtracting a cylinder from a slightly larger cylinder.
   G4VSolid* LNCylinder =
      new G4Tubs ( "LN2Can", 0.*cm, LN2IR+LN2Cthick, LN2Cheight/2., 0., 7. );
   G4VSolid* LNCavity = new G4Tubs ( "Cav", 0.*cm, LN2IR,
                                     ( LN2Cheight-LN2Cthick ) /2., 0., 7. );
   G4VSolid* solidDummySH =
      new G4SubtractionSolid ( "LN2Can", LNCylinder, LNCavity );

   //For now we'll assume the shield and its windows are similar to the
   //   target can. This can be ammended once Shige et. al. have finalized
   //   the window thicknesses. (Using .002 inch windows for now).
   G4VSolid* solidHMSSpace =
      new G4Tubs ( "HMSSpace", LN2IR, LN2IR+LN2Cthick, LN2Wheight/2.,
                   -52.*deg, 62.*deg );
   G4VSolid* solidHMSWin =
      new G4Tubs ( "HMSWin", LN2IR+LN2Cthick-LN2Wthick, LN2IR+LN2Cthick,
                   LN2Wheight/2., -52.*deg, 62.*deg );

   //This is for the window that will point to the Beta Calorimeter.
   G4VSolid* solidBetaSpace =
      new G4Tubs ( "BetaSpace", LN2IR, LN2IR+LN2Cthick, LN2Wheight/2.,
                   18.*deg, 34.*deg );
   G4VSolid* solidBetaWin =
      new G4Tubs ( "BetaWin", LN2IR+LN2Cthick-LN2Wthick, LN2IR+LN2Cthick,
                   LN2Wheight/2., 18.*deg, 34.*deg );

   //This is for the window that will be needed for the Semi_SANE exp.
   G4VSolid* solidSemiSpace =
      new G4Tubs ( "SemiSpace", LN2IR, LN2IR+LN2Cthick, LN2Wheight/2.,
                   -109.*deg, 38.*deg );
   G4VSolid* solidSemiWin =
      new G4Tubs ( "SemiWin", LN2IR+LN2Cthick-LN2Wthick, LN2IR+LN2Cthick,
                   LN2Wheight/2., -109.*deg, 38.*deg );

   //This is for the window that will be needed for both RCS and Semi-SANE
   G4VSolid* solidRCSSpace =
      new G4Tubs ( "RCSSpace", LN2IR, LN2IR+LN2Cthick, LN2Wheight/2.,
                   71.*deg, 38.*deg );
   G4VSolid* solidRCSWin =
      new G4Tubs ( "RCSWin", LN2IR+LN2Cthick-LN2Wthick, LN2IR+LN2Cthick,
                   LN2Wheight/2., 71.*deg, 38.*deg );

   //the rotation matrix for the beam space.
   G4RotationMatrix BeamWinRM;
   BeamWinRM.rotateY ( 90.*deg );

   //The cylinder that will be carved out of the LN2Shield at the beam
   // entrance. It will eventually be covered by a Be window...maybe?
   G4VSolid* solidBeamSpace =
      new G4Tubs ( "BeamSpace", 0., LN2Brad, 3.*cm, 0., 7. );
   G4VSolid* solidBeamCan =
      new G4Tubs ( "BeamWin", LN2IR+LN2Cthick-LN2Bthick, LN2IR+LN2Cthick,
                   LN2Cheight/2., 0., 7. );

   //Must systematically subtract all the window spaces out of the LN2Sh.
   G4VSolid* Step1 =
      new G4SubtractionSolid ( "Sh_HMS", solidDummySH, solidHMSSpace, 0,
                               G4ThreeVector ( 0.,0.,-LN2dis ) );
   G4VSolid* Step2 =
      new G4SubtractionSolid ( "Sh_HMS_Beta", Step1, solidBetaSpace, 0,
                               G4ThreeVector ( 0.,0.,-LN2dis ) );
   G4VSolid* Step3 =
      new G4SubtractionSolid ( "Sh_HMS_Beta_RCS", Step2, solidRCSSpace, 0,
                               G4ThreeVector ( 0.,0.,-LN2dis ) );
   G4VSolid* Step4 =
      new G4SubtractionSolid ( "Sh_HMS_Beta_RCS_Semi", Step3 , solidSemiSpace, 0,
                               G4ThreeVector ( 0.,0.,-LN2dis ) );
   G4VSolid* Step5 =
      new G4SubtractionSolid ( "Sh_AllSpaces", Step4, solidBeamSpace,
                               &BeamWinRM, G4ThreeVector ( -LN2IR,0., -LN2dis ) );

   //Now must systematically add in all the Aluminum windows.
   G4VSolid* Step6 =
      new G4UnionSolid ( "ShWin_HMS", Step5, solidHMSWin, 0,
                         G4ThreeVector ( 0.,0.,-LN2dis ) );
   G4VSolid* Step7 =
      new G4UnionSolid ( "ShWin_HMS_Beta", Step6, solidBetaWin, 0,
                         G4ThreeVector ( 0.,0.,-LN2dis ) );
   G4VSolid* Step8 =
      new G4UnionSolid ( "ShWin_HMS_Beta_RCS", Step7, solidRCSWin, 0,
                         G4ThreeVector ( 0.,0.,-LN2dis ) );
   G4VSolid* solidLN2Shield =
      new G4UnionSolid ( "ShWinCan", Step8 , solidSemiWin, 0,
                         G4ThreeVector ( 0.,0.,-LN2dis ) );

   //solidLN2Shield is the final version of the LN2 Shield minus the beam window.
   //The beam window must be a seperate object since it might not be made of Al.
   //Note that the LN2Shield is located in the World volume, not the TCan volume.

   logicLN2Shield = new G4LogicalVolume ( solidLN2Shield, Al, "LN2Shield" );
   physiLN2Shield = new G4PVPlacement ( 0, G4ThreeVector ( 0.,0.,LN2dis ),
                                        logicLN2Shield, "LN2Shield",
                                        logicLN2Can, false, 0 );

   //Now we build the beam entrance window by intersecting a tube section
   // with the same cylinder used for the beam entrance space.
   G4VSolid* solidLN2BeamWin =
      new G4IntersectionSolid ( "Intersect", solidBeamCan, solidBeamSpace,
                                &BeamWinRM, G4ThreeVector ( -LN2IR,0., -OVCdis ) );
   logicLN2BeamWin = new G4LogicalVolume ( solidLN2BeamWin, Be, "LN2BeamWin" );
   physiLN2BeamWin= new G4PVPlacement ( 0, G4ThreeVector(), logicLN2BeamWin,
                                        "LN2BeamWin", logicTCan, false, 0 );

   //The Visualization attributes of the LN2 Shield
   G4VisAttributes* LN2ShVisAtt= new G4VisAttributes ( G4Colour ( 2.,0.,1. ) );
   LN2ShVisAtt->SetForceSolid ( false );
   logicLN2Shield->SetVisAttributes ( LN2ShVisAtt );

  G4VisAttributes* LNBWVisAtt= new G4VisAttributes(G4Colour(.4,.7,.2));
//   LNBWVisAtt->SetForceSolid(true);
 logicLN2BeamWin->SetVisAttributes ( Invisible );
 logicLN2Can->SetVisAttributes ( LNBWVisAtt );

   logicLN2BeamWin->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
   logicLN2Can->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
   logicLN2Shield->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );

   // Radiator
//   G4RotationMatrix RMRad;
//   RMRad.rotateY(PI/2.);
// 
//   G4double radiatorThickness = 0.5*((radPercent*CuRadiationLength)/100.);
//   radX = -38.*cm - radiatorThickness;
//   solidRad = new G4Tubs("Radiator",0., 4.*2.54*cm, radiatorThickness,0.,7.*cm);
//   logicRad = new G4LogicalVolume(solidRad, Cu, "Radiator");
//   physiRad = new G4PVPlacement(G4Transform3D(RMRad,
//           G4ThreeVector(radX,0.,0.)),
//           logicRad, "Radiator", logicLN2Can, false, 0);
// 
//   logicRad->SetVisAttributes(AlVisAtt);

   ConstructNose();     //Constructs the Nose and its components
   ConstructMagnet();   //Constructs the magnet and its components

}

//___________________________________________________________________
// Constructs the nosepiece and everything in it
void BETADetectorConstruction::ConstructNose()
{
   //The Nose piece as a whole (made of LHe)
   //Displaced by -1.8542 cm in z direction because the windows in the target
   // can are also slightly off center

   HeIR = 4.*cm;
   HeShThick = .00381*cm;
   HeLength =LN2Cheight;

   ////////////////////////////////////////////////////////////////////////
   //This is also from the old target can, but...
   // we'll keep using it until we can find something more accurate.
   //The Liquid helium shield (4K) (Made of Liquid Al)
   solid4KSH = new G4Tubs ( "4KShield", HeIR, HeIR+HeShThick, HeLength/2., 0., 7. );
   logic4KSH = new G4LogicalVolume ( solid4KSH, Al, "4KShield" );
   physi4KSH = new G4PVPlacement ( 0, G4ThreeVector ( 0.,0.,LN2dis ), logic4KSH,
                                   "4KShield", logicLN2Can, false, 0 );

// //   solidInnerVac = new G4Tubs("InnerVac", 0., HeIR, HeLength/2., 0., 7.);
// //   logicInnerVac = new G4LogicalVolume(solidHelium, LHe, "Helium");
// //   physiInnerVac = new G4PVPlacement(0, G4ThreeVector(), logicHelium,
// //       "Helium", logicLN2Shield, false, 0);

   ///////////////////////////////////////////////////////////////////////


   solidNose = new G4Tubs ( "Nose", 0.*cm, ( NoseIR+NoseThick ), NoseLength/2.,
                            0., 7. );
   logicNose = new G4LogicalVolume ( solidNose, LHe, "Nose" );
   physiNose = new G4PVPlacement ( 0, G4ThreeVector ( 0.,0.,LN2dis ), logicNose, "Nose",
                                   logicLN2Can, false, 0 );

   //The Tail piece (outer shell of the Nose)
   solidTail = new G4Tubs ( "Tail", NoseIR, NoseIR+NoseThick, NoseLength/2., 0., 7. );
   logicTail = new G4LogicalVolume ( solidTail, Al, "Tail" );
   physiTail = new G4PVPlacement ( 0, G4ThreeVector(), logicTail,
                                   "Tail", logicNose, false, 0 );

   //The Cell contents
   //Keep it as one whole piece.
   G4RotationMatrix RMCell;
   RMCell.rotateY ( 3.*PI/2 );

   //Define the cell wall as a cylinder then add the cell as a slightly smaller
   //  daughter cylinder.
   solidCWall = new G4Tubs ( "CellWall", 0.*cm, CellOR+CellThick,
                             ( CellLength+CellThick ) /2., 0, 7. );
   logicCWall = new G4LogicalVolume ( solidCWall, Al, "CellWall" );
   physiCWall = new G4PVPlacement ( G4Transform3D ( RMCell,
                                                    G4ThreeVector ( 0.,0.,-LN2dis ) ),
                                    logicCWall, "CellWall", logicNose, false, 0 );

   solidCell = new G4Tubs ( "Cell", 0.*cm, CellOR, CellLength/2., 0., 7. );
   logicCell = new G4LogicalVolume ( solidCell, TargetNH3, "Cell" );
   physiCell = new G4PVPlacement ( 0, G4ThreeVector(), logicCell, "Cell",
                                   logicCWall, false, 0 );

/// \todo add packing fraction geometry
   //May not need to break it up. I can just randomize the starting point of the
   //   particles over the entire Cell. I would only need to break it up if I were
   //   also going to propagate the particles into the cell...
   //Break it up into 15 thin cylinders sitting side by side.
  int i;
  double x,y,z;

  solidCell = new G4Tubs("Cell", 0.*cm, 1.25*cm, 0.15*cm, 0., 7.);
    logicCell = new G4LogicalVolume(solidCell, TargetNH3, "Cell");

//   for(i=0;i<10;i++){
//     x = -1.35*cm + i*0.30*cm;
//     y = 0.;
//     z = 0.;
//     physiCell = new G4PVPlacement(G4Transform3D(RMCell, G4ThreeVector(x,y,z)),
//       logicCell, "Cell", logicNose, true, i);
//   }

   //I want to see the tail...
   G4VisAttributes* TailVisAtt= new G4VisAttributes ( G4Colour ( 1.,0.,0.5,0.2 ) );
   TailVisAtt->SetVisibility ( true );
/*   TailVisAtt->SetForceSolid ( true );*/
   logicTail->SetVisAttributes ( TailVisAtt );

   //Make the target cell wall visible
   G4VisAttributes* CWallVisAtt= new G4VisAttributes ( G4Colour ( 0.,0.4,0. ) );
   CWallVisAtt->SetVisibility ( true );
   CWallVisAtt->SetForceSolid ( false );
   logicCWall->SetVisAttributes ( CWallVisAtt );

  G4VisAttributes* CellVisAtt= new G4VisAttributes(G4Colour(.5,1.,.5,0.0));
  CellVisAtt->SetVisibility(true);
/*  CellVisAtt->SetForceSolid(true);*/
  logicCell->SetVisAttributes ( CellVisAtt );//CellVisAtt);
//    logicNose->SetVisAttributes ( AlVisAtt );
//    logic4KSH->SetVisAttributes ( AlVisAtt );

   logicTail->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
   logicCWall->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
   logicCell->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
   logicNose->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
   logic4KSH->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
}

//___________________________________________________________________
void BETADetectorConstruction::ConstructMagnet()
{
//messenger->fTargetAngle
   fMagnetRotationMatirx.rotateZ( messenger->fTargetAngle);

   //The Magnet as a whole (made of vacuum)
   G4VSolid* StarterMagnet =
      new G4Tubs ( "Magnet", ( 4.+.001905 ) *cm, 33.*cm, 25.*cm, 0., 7. );

   //Need to subtract out a volume for the Nosepiece
   G4VSolid* Dummy4KSH =
      new G4Tubs ( "Dummy", 0.*cm, ( HeIR+HeShThick ), NoseLength/2., 0., 7. );
   G4VSolid* solidMagnet =
      new G4SubtractionSolid ( "Magnet", StarterMagnet, Dummy4KSH, 0,
                               G4ThreeVector() );
   logicMagnet = new G4LogicalVolume ( solidMagnet, Vacuum, "Magnet" );
//This is the physical placement for the magnet volume, ie this has all the piece contained in it
   physiMagnet = new G4PVPlacement (G4Transform3D(fMagnetRotationMatirx, G4ThreeVector ( 0.,0.,-LN2dis )),
                                     logicMagnet, "Magnet", logicLN2Shield,
                                     false, 0 );

   //Set the rotation matrices for the two coils
   G4RotationMatrix RMCoilUp;
   G4RotationMatrix RMCoilDown;
   RMCoilUp.rotateY ( 3.*PI/2. );
   RMCoilDown.rotateY ( PI/2. );
   RMCoilDown.rotateZ ( MAGROT );
   RMCoilUp.rotateZ ( MAGROT );

   G4double rmin1, rmin2, rmax1, rmax2, DZ;

   DZ = 5.*cm;                               //half length of cone
   rmin1 = 10.*cm;                           //
   rmax1 = 5.*cm/tan ( 17*PI/180. );         // ~16.354 cm
   rmin2 = ( 5.*cm + 2*DZ ) *tan ( 48.5*PI/180. ); // ~16.954 cm
   rmax2 = rmin2 + rmax1 - rmin1;            // ~23.309 cm

   //The two magnet coils, identical, but with different locations and rotations
   solidCoil = new G4Cons ( "Coil", rmin1, rmax1, rmin2, rmax2, DZ, 0., 7. );
   logicCoil = new G4LogicalVolume ( solidCoil, Nb, "Coil" );
   physiCoilUp =
      new G4PVPlacement ( G4Transform3D ( RMCoilUp, G4ThreeVector ( -10.*cm,0.,0. ) ),
                          logicCoil, "CoilUp", logicMagnet, false, 0 );
   physiCoilDown =
      new G4PVPlacement ( G4Transform3D ( RMCoilDown, G4ThreeVector ( 10.*cm,0.,0. ) ),
                          logicCoil, "CoilDown", logicMagnet, false, 0 );

   //The four braces that connect the two magnet coils
   solidBrace1 = new G4Tubs ( "Brace1", rmin1, rmax1, 5.*cm,
                              25.*PI/180., 40.*PI/180. );
   logicBrace1 = new G4LogicalVolume ( solidBrace1, Nb, "Brace1" );
   physiBrace1 =
      new G4PVPlacement ( G4Transform3D ( RMCoilDown, G4ThreeVector() ),
                          logicBrace1, "Brace1", logicMagnet, false, 0 );

   solidBrace2 = new G4Tubs ( "Brace2", rmin1, rmax1, 5.*cm,
                              115.*PI/180., 40.*PI/180. );
   logicBrace2 = new G4LogicalVolume ( solidBrace2, Nb, "Brace2" );
   physiBrace2 =
      new G4PVPlacement ( G4Transform3D ( RMCoilDown, G4ThreeVector() ),
                          logicBrace2, "Brace2", logicMagnet, false, 0 );

   solidBrace3 = new G4Tubs ( "Brace3", rmin1, rmax1, 5.*cm,
                              205.*PI/180., 40.*PI/180. );
   logicBrace3 = new G4LogicalVolume ( solidBrace3, Nb, "Brace3" );
   physiBrace3 =
      new G4PVPlacement ( G4Transform3D ( RMCoilDown, G4ThreeVector() ),
                          logicBrace3, "Brace3", logicMagnet, false, 0 );

   solidBrace4 = new G4Tubs ( "Brace4", rmin1, rmax1, 5.*cm,
                              295.*PI/180., 40.*PI/180. );
   logicBrace4 = new G4LogicalVolume ( solidBrace4, Nb, "Brace4" );
   physiBrace4 =
      new G4PVPlacement ( G4Transform3D ( RMCoilDown, G4ThreeVector() ),
                          logicBrace4, "Brace4", logicMagnet, false, 0 );

   G4VisAttributes* CoilVisAtt= new G4VisAttributes ( G4Colour ( 0.5,0.5,0.5 ) );
   CoilVisAtt->SetVisibility ( true );
   CoilVisAtt->SetForceSolid ( true );
   logicCoil->SetVisAttributes ( CoilVisAtt );

   logicBrace1->SetVisAttributes ( CoilVisAtt );
   logicBrace2->SetVisAttributes ( CoilVisAtt );
   logicBrace3->SetVisAttributes ( CoilVisAtt );
   logicBrace4->SetVisAttributes ( CoilVisAtt );

   //Invisible and AlVisAtt are defined in ConstructAll()
   logicMagnet->SetVisAttributes ( AlVisAtt );

//    logicCoil->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
//    logicBrace1->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
//    logicBrace2->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
//    logicBrace3->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
//    logicBrace4->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
//    logicMagnet->SetUserLimits ( new G4UserLimits ( 100.*m,100.*m, 1.*s, ULimits ) );
}

//___________________________________________________________________
//Constructs the beampipes in Hall C (as near as I can figure them).
//These will be updated in the future once I get the actual dimensions.
//"0" refers to the pipe section closest to the Target Can (smaller).
//"1" refers to the pipe section farthest from the Target Can (larger).
void BETADetectorConstruction::ConstructBeamPipe()
{

   G4cout << "ConstructBeamPipe" << G4endl;

   G4float wall0, wall1, rad0, rad1;
//   wall0 = 1./8.*2.54*cm;
//   wall1 = wall0*2.;
//   rad0 = 4.*2.54*cm;
//   rad1 = 2.*rad0;

   wall0 = 1./8.*2.54*cm;
   wall1 = wall0*2.;
   rad0 = 5.08*cm;
   rad1 = 2.*rad0;

    G4RotationMatrix RM00;
   G4RotationMatrix RM180;
   RM180.rotateY(PI);

   G4RotationMatrix xRM90;
   xRM90.rotateX(PI/2.);
   G4RotationMatrix RM90;
   RM90.rotateY(PI/2.);
   G4RotationMatrix RM270;
   RM270.rotateY(3.*PI/2.);

   //Create a dummy target can and the beam pipe cylinder
   G4VSolid* FakeCan =
      new G4Tubs("Dummy", 0., OVCIR+OVCCthick, OVCCheight/2., 0., 7.);

   //Create a solid cylinder and cavity for the pipe.
   solidUpPipe0 = new G4Tubs("UpPipe", 0., rad0+wall0, 4.8433/2.*m, 0., 7.);
   solidUpPipe0Cav =  new G4Tubs("PipeCav", 0., rad0, 4.8433/2.*m, 0., 7.);

   //Subtract the fake can from the pipe cylinder so that the real pipe will
   //   fit perfectly with the real can. Do it for the cavity too.
   G4VSolid* UpPipe =
      new G4SubtractionSolid("UpPipe", solidUpPipe0, FakeCan,
                             G4Transform3D(xRM90,
                                           G4ThreeVector(0,0,-4.8433/2*m)));
   logicUpPipe0 = new G4LogicalVolume(UpPipe, Al, "UpPipe");
//    physiUpPipe0 = new G4PVPlacement(G4Transform3D(RM180,
//                                                   G4ThreeVector(0,0,-4.8433/2*m)),
//                                     logicUpPipe0, "UpPipe",
//                                     expHall_log, false, 0);

   G4VSolid* UpPipeCav =
      new G4SubtractionSolid("PipeCav1", solidUpPipe0Cav, FakeCan,
                             G4Transform3D(xRM90,
                                           G4ThreeVector(0,0,-4.8433/2*m)));
   logicUpPipe0Cav = new G4LogicalVolume(UpPipeCav, Vacuum, "PipeCav1");
   physiUpPipe0Cav = new G4PVPlacement(0,G4ThreeVector(),
                                       logicUpPipe0Cav, "PipeCav1",
                                       logicUpPipe0, false, 0);

   solidDownPipe0 = new G4Tubs("DownPipe", 0., rad0+wall0, 5.25/2.*m, 0.,7.);
   solidDownPipe0Cav = new G4Tubs("PipeCav", 0., rad0, 5.25/2.*m, 0., 7.);

   //Subtract the fake can from the pipe cylinder so that the real pipe will
   //   fit perfectly with the real can. Do the cavity too.
   G4VSolid* DownPipe =
      new G4SubtractionSolid("DownPipe", solidDownPipe0, FakeCan,
                             G4Transform3D(xRM90,
                                           G4ThreeVector(0,0,5.25/2.*m)));
   logicDownPipe0 = new G4LogicalVolume(DownPipe, Al, "DownPipe");
   physiDownPipe0 = new G4PVPlacement(G4Transform3D(RM180,
                                                    G4ThreeVector(0,0,5.25/2.*m)),
                                      logicDownPipe0, "DownPipe",
                                      expHall_log, false, 0);

   G4VSolid* DownPipeCav =
      new G4SubtractionSolid("PipeCav2", solidDownPipe0Cav, FakeCan,
                             G4Transform3D(xRM90,
                                           G4ThreeVector(0,0,5.25/2.*m)));
   logicDownPipe0Cav = new G4LogicalVolume(DownPipeCav, Vacuum, "PipeCav2");
   physiDownPipe0Cav = new G4PVPlacement(0,G4ThreeVector(),
                                         logicDownPipe0Cav, "PipeCav2",
                                         logicDownPipe0, false, 0);

//    G4RotationMatrix RMRad;
//    RMRad.rotateY(PI/2.);
// 
//    G4double radiatorThickness = 0.5*((radPercent*CuRadiationLength)/100.);
//    radX = -60.*cm - radiatorThickness;
//    solidRad = new G4Tubs("Radiator",0., 5.08*cm, radiatorThickness,0.,7.);
//    // 4.*2.54*cm, radiatorThickness,0.,7.);
//    logicRad = new G4LogicalVolume(solidRad, Cu, "Radiator");
//    physiRad = new G4PVPlacement(0, G4ThreeVector(0.,0.,-4.8433/2.*m-radX),
//                                 logicRad, "Radiator", logicUpPipe0Cav, false, 0);
// 
//    logicRad->SetVisAttributes(AlVisAtt);

   //Create the Flanges that connect the small beam pipes to the larger pipes.
  solidFlange = new G4Tubs("Flange", 0., rad1+wall1, .25*2.54*cm, 0., 7.);
  logicFlange = new G4LogicalVolume(solidFlange, Al, "Flange");
  physiUpFlange =
    new G4PVPlacement(G4Transform3D(RM00,
        G4ThreeVector(0,0,-4.8433*m-.25*2.54*cm)),
        logicFlange, "UpPipe", expHall_log, false, 0);
//   physiDownFlange =
//     new G4PVPlacement(G4Transform3D(RM00,
//         G4ThreeVector(0,0,5.25*m+.25*2.54*cm)),
//         logicFlange, "DownPipe", expHall_log, false, 0);

  //  Create the cavities for the flanges
  solidFlangeCav = new G4Tubs("FlangeCav", 0., rad0, .25*2.54*cm, 0., 7.);
  logicFlangeCav = new G4LogicalVolume(solidFlangeCav, Vacuum, "FlangeCav");
  physiUpFlangeCav =
    new G4PVPlacement(0, G4ThreeVector(0,0,0),
        logicFlangeCav, "PipeCav3", logicFlange, false, 0);
    physiDownFlangeCav =
      new G4PVPlacement(G4Transform3D(RM00,
          G4ThreeVector(0,0,0)),
          logicFlangeCav, "PipeCav3", logicFlange, false, 0);



    //Create the larger beampipes that lead to the accel. and the He Shroud
  solidUpPipe1 = new G4Tubs("UpPipe", 0., rad1+wall1, 13.8652/2.*m, 0, 7.);
  logicUpPipe1 = new G4LogicalVolume(solidUpPipe1, Al, "UpPipe");
//   physiUpPipe1 =
//     new G4PVPlacement(G4Transform3D(RM00,
//          G4ThreeVector(0,0,-4.8433*m-0.5*2.54*cm-13.8652/2.*m)),
//         logicUpPipe1, "UpPipe", expHall_log, false, 0);
  solidDownPipe1 = new G4Tubs("DownPipe", 0., rad1+wall1, 9.00/2.*m, 0, 7.);
  logicDownPipe1 = new G4LogicalVolume(solidDownPipe1, Al, "DownPipe");
//   physiDownPipe1 =
//     new G4PVPlacement(G4Transform3D(RM00,
//          G4ThreeVector(0,0,5.25*m+0.5*2.54*cm+9.0/2.*m)),
//         logicDownPipe1, "DownPipe", expHall_log, false, 0);

  // Create cavities for the larger pipes.

  solidUpPipe1Cav = new G4Tubs("PipeCav4", 0., rad1, 13.8652/2.*m, 0, 7.);
  logicUpPipe1Cav = new G4LogicalVolume(solidUpPipe1Cav, Vacuum, "PipeCav4");
  physiUpPipe1Cav =
    new G4PVPlacement(0,G4ThreeVector(0,0,0),
        logicUpPipe1Cav, "PipeCav4", logicUpPipe1, false, 0);
  solidDownPipe1Cav = new G4Tubs("PipeCav5", 0., rad1, 9.00/2.*m, 0, 7.);
  logicDownPipe1Cav =
    new G4LogicalVolume(solidDownPipe1Cav, Vacuum, "PipeCav5");
  physiDownPipe1Cav =
    new G4PVPlacement(0, G4ThreeVector(0,0,0),
        logicDownPipe1Cav, "PipeCav5", logicDownPipe1, false, 0);

//   As far as I'm concerned the He Shroud is the beam dump.
//   This is a dummy detector that won't interact, but will allow me
//    to detect all particles that hit the "dump"
  solidHeShroud = new G4Tubs("Shroud", 0, rad1+wall1, 1.*mm, 0., 7.);
  logicHeShroud = new G4LogicalVolume(solidHeShroud, Vacuum, "Shroud");
//   physiHeShroud =
//     new G4PVPlacement(G4Transform3D(RM00,
//      G4ThreeVector(0,0,14.25*m+0.5*2.54*cm+0.5*mm)),
//         logicHeShroud, "Shroud", expHall_log, false, 0);

   logicUpPipe0->SetUserLimits(new G4UserLimits(100.*m,100.*m, 1.*s, ULimits));
   logicDownPipe0->SetUserLimits(new G4UserLimits(100.*m,100.*m, 1.*s, ULimits));
   logicUpPipe0Cav->SetUserLimits(new G4UserLimits(100.*m,100.*m, 1.*s, ULimits));
   logicDownPipe0Cav->SetUserLimits(new G4UserLimits(100.*m,100.*m, 1.*s, ULimits));

   //Vis attributes for the pipes
   G4VisAttributes* PipeVisAtt = new G4VisAttributes(G4Colour(0.5,1.,0.5));
   PipeVisAtt->SetVisibility(true);
   PipeVisAtt->SetForceWireframe(true);
   logicUpPipe0->SetVisAttributes(PipeVisAtt);
   logicDownPipe0->SetVisAttributes(PipeVisAtt);

   //Vis attributes for the cavities
   G4VisAttributes* PipeCavVisAtt = new G4VisAttributes(G4Colour(0.5,.1,0.7));
   PipeCavVisAtt->SetVisibility(true);
   PipeCavVisAtt->SetForceWireframe(true);
   logicUpPipe0Cav->SetVisAttributes(PipeCavVisAtt);
   logicDownPipe0Cav->SetVisAttributes(PipeCavVisAtt);

   logicFlange->SetVisAttributes(PipeVisAtt);
  logicUpPipe1->SetVisAttributes(PipeVisAtt);
  logicDownPipe1->SetVisAttributes(PipeVisAtt);

  logicFlangeCav->SetVisAttributes(PipeCavVisAtt);
  logicUpPipe1Cav->SetVisAttributes(PipeCavVisAtt);
  logicDownPipe1Cav->SetVisAttributes(PipeCavVisAtt);

//   logicUpPipe1->SetUserLimits(new G4UserLimits(100.*m,100.*m, 1.*s, ULimits));
//   logicDownPipe1->SetUserLimits(new G4UserLimits(100.*m,100.*m, 1.*s, ULimits));
//   logicUpPipe1Cav->SetUserLimits(new G4UserLimits(100.*m,100.*m, 1.*s, ULimits));
//   logicDownPipe1Cav->SetUserLimits(new G4UserLimits(100.*m,100.*m, 1.*s, ULimits));
//   logicFlange->SetUserLimits(new G4UserLimits(100.*m,100.*m,1.*s,ULimits));
//   logicFlangeCav->SetUserLimits(new G4UserLimits(100.*m,100.*m,1.*s,ULimits));


   //Vis attributes for the He shroud
  G4VisAttributes* ShroudVisAtt = new G4VisAttributes(G4Colour(0.2,0.6,0.));
  ShroudVisAtt->SetVisibility(true);
  ShroudVisAtt->SetForceSolid(false);
  logicHeShroud->SetVisAttributes(ShroudVisAtt);



}

//___________________________________________________________________
