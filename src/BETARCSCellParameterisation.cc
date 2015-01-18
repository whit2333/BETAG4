#include "BETARCSCellParameterisation.hh"

#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "BIGCALGeometryCalculator.h"


//______________________________________________________________________________
BETARCSCellParameterisation::BETARCSCellParameterisation() {

   BIGCALGeometryCalculator * BCgeo = BIGCALGeometryCalculator::GetCalculator();
   double size = 4.02167 ;

   // Initialise
   G4int i ( 0 );

   for ( i=0; i<720; i++ )
   {
      G4int column = i % BCgeo->rcsNx +1 ;
      G4int row = i / BCgeo->rcsNx +1;
      yCell.push_back (BCgeo->GetRCSCellY(row)*cm );
      xCell.push_back (BCgeo->GetRCSCellX(column)*cm );
   }
}
//______________________________________________________________________________
BETARCSCellParameterisation::~BETARCSCellParameterisation(){
}
//______________________________________________________________________________
void BETARCSCellParameterisation::ComputeTransformation ( const G4int copyNo,G4VPhysicalVolume *physVol ) const
{
   int blockNumber = copyNo+1024;
   physVol->SetTranslation ( G4ThreeVector ( xCell[copyNo],yCell[copyNo],0. ) );
   physVol->SetCopyNo(blockNumber);

   for(int i=0; i< physVol->GetLogicalVolume()->GetNoDaughters();i++){ 
      physVol->GetLogicalVolume()->GetDaughter(i)->SetCopyNo(blockNumber);
   }

}
//______________________________________________________________________________

