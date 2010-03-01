//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id:$
//
// Jane Tinslay - adapted from A01 example
//
#include "BETAProtvinoCalorimeterHit.hh"
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

G4Allocator<BETAProtvinoCalorimeterHit> BETAProtvinoCalorimeterHitAllocator;

BETAProtvinoCalorimeterHit::BETAProtvinoCalorimeterHit()
      :fCellID ( -1 )
      ,fDepositedEnergy ( 0 )
      ,fPosition()
      ,fRotation()
      ,pLogicalVolume ( 0 )
{}

BETAProtvinoCalorimeterHit::BETAProtvinoCalorimeterHit ( G4int id )
      :fCellID ( id )
      ,fDepositedEnergy ( 0 )
      ,fPosition()
      ,fRotation()
      ,pLogicalVolume ( 0 )
{}

BETAProtvinoCalorimeterHit::~BETAProtvinoCalorimeterHit() {}

void BETAProtvinoCalorimeterHit::Draw()
{/*
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  if(pVVisManager && (fDepositedEnergy>0.)) {

    // HandsOn5: Draw a box with depth propotional to the energy deposition
        G4double scale = 2000;//BETAPrimaryGeneratorAction::Gun()->GetParticleEnergy();
    G4double depth = (50.*cm)*(fDepositedEnergy*MeV)/(scale*MeV);

    // Back face of box is flat against front face of calorimeter cell
    double z = fPosition.z()  + 25.*cm;
    G4ThreeVector myPos(fPosition.x(), fPosition.y(), z+depth);

    G4Transform3D trans(fRotation.inverse(), myPos);
    G4VisAttributes attribs;

    // Magenta with transparency
    G4Colour colour(1., 0., 1., 0.6);
    attribs.SetColour(colour);
    attribs.SetForceSolid(true);

    G4Box box("MyBox", 3.8/2.*cm, 3.8/2.*cm, depth);

    pVVisManager->Draw(box, attribs, trans);
  }
*/
}

const std::map<G4String,G4AttDef>* BETAProtvinoCalorimeterHit::GetAttDefs() const
{
   G4bool isNew;
   std::map<G4String,G4AttDef>* store
   = G4AttDefStore::GetInstance ( "BETAProtvinoCalorimeterHit",isNew );

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

std::vector<G4AttValue>* BETAProtvinoCalorimeterHit::CreateAttValues() const
{
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

void BETAProtvinoCalorimeterHit::Print()
{
   G4cout << "  Cell[" << fCellID << "] " << fDepositedEnergy/MeV << " (MeV)" << G4endl;
}


