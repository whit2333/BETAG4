// Whitney Armstrong 1/7/2008

#include "BETAFrontTrackerCellParameterisation.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

BETAFrontTrackerCellParameterisation::BETAFrontTrackerCellParameterisation(G4Material * aMaterial)
{

   theMaterial = aMaterial;
   vertBar = new G4Box ( "verticalMeasurementBar", // Name
                         3.*mm/2.0,         // x half length
                         22.*cm/2.0,         // y half length
                         3.*mm/2.0 );     // z half length


   horizBar = new G4Box ( "horizontalMeasurementBar", // Name
                          40.*cm/2.0,         // x half length
                          3.*mm/2.0,         // y half length
                          3.*mm/2.0 );     // z half length
// 266 bars total 3mmx3mm x (40 vert or 22 horz cm)
// Going through  we have...
// 73 bars horizontal then 66 vertical bars
// followed by another 67 veritcal offset by 1.5mm
   G4double smallSeparation = 0.01*mm;
   // Initialise
   G4int i ( 0 );

   for ( i=0; i<73; i++ )
   {
      xCell.push_back ( 0.*cm );
      yCell.push_back ( ( 36-i ) *3.*mm+ 1.5*mm +smallSeparation );
      zCell.push_back (  -3.*mm-smallSeparation ); // move forward
   }
   for ( i=0; i<133; i++ )
   {
      xCell.push_back ( ( 66-i ) *3.*mm + 1.5*mm+smallSeparation );
      yCell.push_back ( 0.*cm );
      zCell.push_back ( 0.*cm );
   }

   for ( i=0; i<133; i++ )
   {
      xCell.push_back ( ( 66-i ) *3.*mm  +smallSeparation );
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
   if ( copyNo < 73 ) theSolid = horizBar;
   else theSolid = vertBar;

   physVol->SetName ( "frontTrackerBar" );


   return theSolid;
}
