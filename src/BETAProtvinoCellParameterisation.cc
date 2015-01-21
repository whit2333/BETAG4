#include "BETAProtvinoCellParameterisation.hh"

#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "BIGCALGeometryCalculator.h"

//______________________________________________________________________________
BETAProtvinoCellParameterisation::BETAProtvinoCellParameterisation() {

   BIGCALGeometryCalculator * BCgeo = BIGCALGeometryCalculator::GetCalculator();

   // Initialise
   // The protvino section has 32x32 blocks.
   for(int i=0; i<1024; i++ ) {
      G4int column = i % 32 + 1  ; // the plus one comes from the fortran index
      G4int row = i / 32  +1   ;
      yCell.push_back ( BCgeo->GetProtCellY(row)*cm );
      xCell.push_back ( BCgeo->GetProtCellX(column)*cm );

   }
}
//______________________________________________________________________________
BETAProtvinoCellParameterisation::~BETAProtvinoCellParameterisation() {
}
//______________________________________________________________________________
void BETAProtvinoCellParameterisation::ComputeTransformation ( const G4int copyNo,G4VPhysicalVolume *physVol ) const
{
   physVol->SetTranslation ( G4ThreeVector ( xCell[copyNo],yCell[copyNo],0. ) );
   physVol->SetCopyNo(copyNo);

   //for(int i=0; i< physVol->GetLogicalVolume()->GetNoDaughters();i++){ 
   //   physVol->GetLogicalVolume()->GetDaughter(i)->SetCopyNo(copyNo);
   //}

}
//______________________________________________________________________________

