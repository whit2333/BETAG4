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
#include "BETAProtvinoCellParameterisation.hh"

#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "BIGCALGeometryCalculator.hh"

BETAProtvinoCellParameterisation::BETAProtvinoCellParameterisation()
{

   BIGCALGeometryCalculator * BCgeo = BIGCALGeometryCalculator::GetCalculator();


   // Initialise
   // The protvino section has 32x32 blocks.

   G4int i ( 0 );
   for ( i=0; i<1024; i++ )
   {
      G4int column = i % 32 + 1  ; // the plus one comes from the fortran index
      G4int row = i / 32  +1   ;
      xCell.push_back ( BCgeo->getProtCellY(row)*cm );
      yCell.push_back ( -BCgeo->getProtCellX(column)*cm );

   }
}

BETAProtvinoCellParameterisation::~BETAProtvinoCellParameterisation() {}

void BETAProtvinoCellParameterisation::ComputeTransformation ( const G4int copyNo,G4VPhysicalVolume *physVol ) const
{
   physVol->SetTranslation ( G4ThreeVector ( xCell[copyNo],yCell[copyNo],0. ) );
}

