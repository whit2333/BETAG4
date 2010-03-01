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
#include "BETARCSCellParameterisation.hh"

#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "BIGCALGeometryCalculator.hh"
// RCS cell = 4.
BETARCSCellParameterisation::BETARCSCellParameterisation()
{
   BIGCALGeometryCalculator * BCgeo = BIGCALGeometryCalculator::GetCalculator();
   double size = 4.02167 ;

   // Initialise
   G4int i ( 0 );

   for ( i=0; i<720; i++ )
   {
      G4int column = i % BCgeo->rcsNx +1 ;
      G4int row = i / BCgeo->rcsNx +1;
      xCell.push_back (BCgeo->getRCSCellY(row)*cm );
      yCell.push_back (-BCgeo->getRCSCellX(column)*cm );
   }
}

BETARCSCellParameterisation::~BETARCSCellParameterisation() {}

void BETARCSCellParameterisation::ComputeTransformation ( const G4int copyNo,G4VPhysicalVolume *physVol ) const
{
   physVol->SetTranslation ( G4ThreeVector ( xCell[copyNo],yCell[copyNo],0. ) );
}

