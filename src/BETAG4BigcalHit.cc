#include "BETAG4BigcalHit.hh"
#include "BETAPrimaryGeneratorAction.hh"
#include "G4AttDef.hh"
#include "G4AttDefStore.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
// HandsOn5: Draw box
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4ParticleGun.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "BIGCALGeometryCalculator.h"

G4Allocator<BETAG4BigcalHit> BETAG4BigcalHitAllocator;
//______________________________________________________________________________
BETAG4BigcalHit::BETAG4BigcalHit ( G4int id )
      :fCellID ( id ),
      fDepositedEnergy ( 0 ),
      fType(0),
      fPosition(),
      fRotation(),
      pLogicalVolume( 0 ),
      fNHits( 0 ),
      fTimingHit( false ) {
}
//______________________________________________________________________________
BETAG4BigcalHit::~BETAG4BigcalHit() {
}
//______________________________________________________________________________
void BETAG4BigcalHit::Draw() {

   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

   if(pVVisManager) {
      if( fType==0 && (fDepositedEnergy>0.)) {
         BIGCALGeometryCalculator * bcgeo = BIGCALGeometryCalculator::GetCalculator();

         // HandsOn5: Draw a box with depth propotional to the energy deposition
         G4double scale = 2000;//BETAPrimaryGeneratorAction::Gun()->GetParticleEnergy();
         G4double depth = (50.*cm)*fDepositedEnergy/scale;

         // Back face of box is flat against front face of calorimeter cell
         //G4ThreeVector myPos(pos.X(), fPosition.y(), fPosition.z() + depth);
         TVector3 pos = bcgeo->GetBlockFacePosition(fCellID);
         TVector3 hitVec(0.0,0.0,depth/cm);
         bcgeo->RotateFromBCCoords(hitVec);
         pos += hitVec;
         G4ThreeVector pos1(pos.X()*cm,pos.Y()*cm,pos.Z()*cm);
         //G4Transform3D trans(G4RotationMatrix::IDENTITY, pos1);
         G4Transform3D trans(fRotation.inverse(), pos1);

         // Magenta with transparency
         G4VisAttributes attribs;
         G4Colour colour(1., 0., 1., 0.6);
         attribs.SetColour(colour);
         attribs.SetForceSolid(true);

         G4Box box("MyBox", 3.8/2.*cm, 3.8/2.*cm, depth);

         pVVisManager->Draw(box, attribs, trans);
      }
      if( fTimingHit==true && fType==2 && fDepositedEnergy>0.) {
         BIGCALGeometryCalculator * bcgeo = BIGCALGeometryCalculator::GetCalculator();

         // HandsOn5: Draw a box with depth propotional to the energy deposition
         G4double scale = 2000;//BETAPrimaryGeneratorAction::Gun()->GetParticleEnergy();
         G4double depth = (10.*cm)*fDepositedEnergy/scale;

         std::cout << fCellID << " : " << fDepositedEnergy << " MeV" << std::endl;
         // Back face of box is flat against front face of calorimeter cell
         //G4ThreeVector myPos(pos.X(), fPosition.y(), fPosition.z() + depth);
         TVector3 pos = bcgeo->GetSumOf64Position(fCellID);
         bcgeo->RotateFromBCCoords(pos);
         TVector3 hitVec(0.0,0.0,45.0+depth/cm);
         bcgeo->RotateFromBCCoords(hitVec);
         pos += hitVec;
         G4ThreeVector pos1(pos.X()*cm,pos.Y()*cm,pos.Z()*cm);
         //G4Transform3D trans(G4RotationMatrix::IDENTITY, pos1);
         G4Transform3D trans(fRotation.inverse(), pos1);

         // Magenta with transparency
         G4VisAttributes attribs;
         G4Colour colour(1., 1., 0., 0.3);
         attribs.SetColour(colour);
         attribs.SetForceSolid(true);

         G4Box box("MyBox", 16.0*3.8/2.*cm, 4.0*3.8/2.*cm, depth);

         pVVisManager->Draw(box, attribs, trans);
      }
   }
}
//______________________________________________________________________________
const std::map<G4String,G4AttDef>* BETAG4BigcalHit::GetAttDefs() const {
   G4bool isNew;
   std::cout << " GETAttDefs " << std::endl;
   std::map<G4String,G4AttDef>* store
   = G4AttDefStore::GetInstance ( "BETAG4BigcalHit",isNew );

   if ( isNew )
   {
      G4String HitType ( "HitType" );
      ( *store ) [HitType] = G4AttDef ( HitType,"Hit Type", "Bookkeeping", "", "G4String" );

      G4String ID ( "ID" );
      ( *store ) [ID] = G4AttDef ( ID, "ID", "Bookkeeping", "", "G4int" );

      G4String Column ( "Column" );
      ( *store ) [Column] = G4AttDef ( Column, "Column ID", "Bookkeeping", "", "G4int" );

      G4String Row ( "Row" );
      ( *store ) [Row] = G4AttDef ( Row, "Row ID", "Bookkeeping", "", "G4int" );

      G4String Energy ( "Energy" );
      ( *store ) [Energy] = G4AttDef ( Energy, "Energy Deposited", "Physics",
                                       "G4BestUnit", "G4double" );

      G4String Pos ( "Pos" );
      ( *store ) [Pos] = G4AttDef ( Pos, "Position", "Physics", "G4BestUnit", "G4ThreeVector" );

      G4String LVol ( "LVol" );
      ( *store ) [LVol] = G4AttDef ( LVol, "Logical Volume", "Bookkeeping", "", "G4String" );
   }

   return store;
}
//______________________________________________________________________________
std::vector<G4AttValue>* BETAG4BigcalHit::CreateAttValues() const {
   std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

   values->push_back ( G4AttValue ( "HitType", "ProtvinoCalorimeterHit", "" ) );

   values->push_back
   ( G4AttValue ( "ID", G4UIcommand::ConvertToString ( fCellID ), "" ) );

   values->push_back ( G4AttValue ( "Column", " ", "" ) );

   values->push_back ( G4AttValue ( "Row", " ", "" ) );

   values->push_back ( G4AttValue ( "Energy", G4BestUnit ( fDepositedEnergy, "Energy" ), "" ) );

   values->push_back ( G4AttValue ( "Pos", G4BestUnit ( fPosition,"Length" ), "" ) );

   if ( pLogicalVolume ) values->push_back ( G4AttValue ( "LVol", pLogicalVolume->GetName(), "" ) );
   else values->push_back ( G4AttValue ( "LVol", " ", "" ) );

   return values;
}
//______________________________________________________________________________
void BETAG4BigcalHit::Print() {

   G4cout << "  Cell[" << fCellID << "] " << fDepositedEnergy/MeV << " (MeV)" << G4endl;
}
//______________________________________________________________________________


