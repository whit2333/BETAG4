#include "BETAMirrorHit.hh"
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


G4Allocator<BETAMirrorHit> BETAMirrorHitAllocator;

BETAMirrorHit::BETAMirrorHit()
      :mirrorNumber ( -1 )
      ,fPhotons ( 0 )
{
 ;
}

BETAMirrorHit::BETAMirrorHit ( G4int id )
      :mirrorNumber ( id )
      ,fPhotons ( 0 )
{
 ;
}

BETAMirrorHit::~BETAMirrorHit() {
 ;
}


void BETAMirrorHit::Draw()
{;
/*

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  if(pVVisManager && (fDepositedEnergy>0.)) {

    // HandsOn5: Draw a box with depth propotional to the energy deposition
        G4double scale = BETAPrimaryGeneratorAction::Gun()->GetParticleEnergy();
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

    G4Box box("MyBox", 5.*cm, 5.*cm, depth);

    pVVisManager->Draw(box, attribs, trans);
  }
*/
}


void BETAMirrorHit::Print()
{
   G4cout << "  Mirror[" << tubeNumber << "] " << fPhotons  << G4endl;
}


