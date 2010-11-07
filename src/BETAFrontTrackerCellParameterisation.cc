// Whitney Armstrong 1/7/2008

#include "BETAFrontTrackerCellParameterisation.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4VTouchable.hh"
#include "G4LogicalBorderSurface.hh"

BETAFrontTrackerCellParameterisation::BETAFrontTrackerCellParameterisation(G4Material * aMaterial)
{
   G4double smallSeparation = 0.01*mm;
   theMaterial = aMaterial;
//    vertBar = new G4Box ( "verticalMeasurementBar", // Name
//                          3.*mm/2.0,         // x half length
//                          22.*cm/2.0,         // y half length
//                          3.*mm/2.0 );     // z half length
//    vertBarScore = new G4Box ( "verticalMeasurementBarScorer", // Name
//                          3.*mm/2.0,         // x half length
//                          smallSeparation,         // y half length
//                          3.*mm/2.0 );     // z half length
// 
//    horizBar = new G4Box ( "horizontalMeasurementBar", // Name
//                           40.*cm/2.0,         // x half length
//                           3.*mm/2.0,         // y half length
//                           3.*mm/2.0 );     // z half length
// 
//    horizBarScore = new G4Box ( "horizontalMeasurementBarScorer", // Name
//                           smallSeparation,         // x half length
//                           3.*mm/2.0,         // y half length
//                           3.*mm/2.0 );     // z half length
// // 266 bars total 3mmx3mm x (40 vert or 22 horz cm)
// // Going through  we have...
// // 73 bars horizontal then 66 vertical bars
// // followed by another 67 veritcal offset by 1.5mm
// 
//   horizBarScore_log = new G4LogicalVolume ( horizBarScore,Air,"horizBarScore_log",0,0,0 );
//   vertBarScore_log = new G4LogicalVolume ( vertBarScore,Air,"vertBarScore_log",0,0,0 );
// 

   // Initialise
   G4int i ( 0 );

   for ( i=0; i<73; i++ )
   {
      xCell.push_back ( 0.*cm );
      yCell.push_back ( ( 36-i ) *(3.*mm+smallSeparation )+ 1.5*mm+smallSeparation/2.0);
      zCell.push_back (  -3.*mm-smallSeparation ); // move forward
   }
   for ( i=0; i<133; i++ )
   {
      xCell.push_back ( ( 66-i ) *(3.*mm+smallSeparation ) + 1.5*mm+smallSeparation/2.0 );
      yCell.push_back ( 0.*cm );
      zCell.push_back ( 0.*cm );
   }

   for ( i=0; i<133; i++ )
   {
      xCell.push_back ( ( 66-i ) *(3.*mm+smallSeparation )  );
      yCell.push_back ( 0.*cm );
      zCell.push_back ( 3.*mm+smallSeparation ); // move backward
   }


}

BETAFrontTrackerCellParameterisation::~BETAFrontTrackerCellParameterisation() {}

void BETAFrontTrackerCellParameterisation::ComputeTransformation ( const G4int copyNo,G4VPhysicalVolume *physVol ) const
{
   physVol->SetTranslation ( G4ThreeVector ( xCell[copyNo],yCell[copyNo],zCell[copyNo] ) );
}

G4VSolid* BETAFrontTrackerCellParameterisation::ComputeSolid
( const G4int copyNo, G4VPhysicalVolume* physVol )
{
   if ( copyNo < 73 ) 
   {
     theSolid = horizBar;
   }
   else if ( copyNo < 72+132+133) 
   {
     theSolid = vertBar;
   }
   physVol->SetName ( "frontTrackerBar" );


   return theSolid;
}

G4Material* BETAFrontTrackerCellParameterisation::ComputeMaterial // material, sensitivity, visAtt
( const G4int copyNo, G4VPhysicalVolume* physVol,
  const G4VTouchable *parentTouch )
{

   // Attach detector to scoring volume
//   physVol->GetLogicalVolume()->SetSensitiveDetector ( HodoscopePMTs );
   G4Material* mat = theMaterial;

   G4OpticalSurface* forwardTrackerSurface = new G4OpticalSurface ( "TrackerBarOpticalSurface" );
     forwardTrackerSurface->SetModel ( unified );
     forwardTrackerSurface->SetType ( dielectric_dielectric );
     forwardTrackerSurface->SetFinish ( polishedfrontpainted);

   G4OpticalSurface* scoringSurface = new G4OpticalSurface ( "scoringTrackerBarOpticalSurface" );
     scoringSurface->SetModel ( unified );
     scoringSurface->SetType ( dielectric_metal);
     scoringSurface->SetFinish ( groundbackpainted);
//   new G4LogicalSkinSurface ( "BorderTrackerBar", physVol->GetLogicalVolume(), forwardTrackerSurface );
   new G4LogicalBorderSurface ( "BorderTrackerBar", physVol, motherPhysicalVolume, forwardTrackerSurface );
//   G4LogicalSkinSurface * trackerBarSkinSurface = new G4LogicalSkinSurface ( "TrackerBarsurface", trackerDummyCell_log, forwardTrackerSurface );
   for(int k=0;k<6;k++) 
     new G4LogicalBorderSurface ( "BorderTrackerScoring",  scoringPhysicalVolumes[k],physVol, scoringSurface );

   return mat;
}

