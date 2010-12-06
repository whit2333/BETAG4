#include "BETAHodoscopeCellParameterisation.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4SDManager.hh"
#include "BETAHodoscopePMT.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
BETAHodoscopeCellParameterisation::BETAHodoscopeCellParameterisation ( G4Material * aMaterial )
{
   theMaterial = aMaterial;

   rotate1 = &hodBarRotateRemove1;
   rotate2 = &hodBarRotateRemove2;
   double sideA = std::sqrt ( 2.*3.5*3.5 ) *cm/2. ;
   double senseThickness = 0.1*cm ;

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

   G4VSolid* hodoscope = new G4Box ( "Lucitebar-container", 45*2.54*cm/2., 45*2.54*cm/2.,6.*28.*cm/2. +10*cm );
   G4VSolid* hoBar= new G4Tubs ( "Lucitebar", 240.*cm, ( 240.+3.5 ) *cm,6.*cm/2. , -23.*pi/2.0/180.,23.*pi/180. );
   G4Box * hodoscopeRemoveBox = new G4Box ( "hodoscoperemovebox", 100.*cm,std::sqrt ( 2.*3.5*3.5 ) *cm/2.  ,100.*cm );

   hodoscopeBar = new G4SubtractionSolid ( "Frame top",hoBar , hodoscopeRemoveBox,G4Transform3D
                                           ( hodBarRotateRemove1,G4ThreeVector ( ( 240.+3.5 ) *cm*std::cos ( theta ), ( 240.+3.5 ) *cm*std::sin ( theta ),0*cm ) ) );


   hodoscopeBar = new G4SubtractionSolid ( "Frame top",hodoscopeBar, hodoscopeRemoveBox,
                                           G4Transform3D ( hodBarRotateRemove2, G4ThreeVector ( ( 240.+3.5 ) *cm*cos ( theta ),
                                                                                                -1.0* ( 240.+3.5 ) *cm*sin ( theta ),0*cm ) ) );
   hodoscopeBar_log =
      new G4LogicalVolume ( hodoscopeBar,     // Solid
                            theMaterial,                    // Material
                            "hodoscopeBar_log" ); // Name
   PMTinquotes = new G4Box ( "hodoscopePMTinquotes", std::sqrt ( 2.*3.5*3.5 ) *cm/2.,std::sqrt ( 2.*3.5*3.5 ) *cm/2.,6.*cm/2. );
   PMTphotocathode = new G4Box ( "hodoscopePMTinquotes", /*std::sqrt ( 2.*3.5*3.5 ) */std::sqrt ( 2.*3.5*3.5 ) *cm/2.,0.1*cm/2.,6.0*cm/2. );
   PMTinquotes_log =
      new G4LogicalVolume ( PMTinquotes,     // Solid
                            theMaterial,                    // Material
                            "PMTinquotes_log" ); // Name

   PMTphotocathode_log =
      new G4LogicalVolume ( PMTphotocathode,     // Solid
                            theMaterial,                    // Material
                            "PMTphotocathode_log" ); // Name
/*
      new G4PVPlacement ( 0,G4ThreeVector (0,0,0 ),PMTphotocathode_log,     // Logical volume
                       "hodoscope_sensitive",     // Name
                       PMTinquotes_log,false,0);*/
/////////////////////////////////////////////////
// Pmt Detector
/////////////////////////////////////////////////
   // new for the detector part
   // Create a new sensitive detector named "MyDetector"
   HodoscopePMTs =
      new BETAHodoscopePMT ( "hodoscopePMT" );
   // Get pointer to detector manager
   G4SDManager* manager = G4SDManager::GetSDMpointer();
   // Register detector with manager
   manager->AddNewDetector ( HodoscopePMTs );

}

BETAHodoscopeCellParameterisation::~BETAHodoscopeCellParameterisation() {}

void BETAHodoscopeCellParameterisation::ComputeTransformation ( const G4int copyNo,G4VPhysicalVolume *physVol ) const
{
// whats going on...
// the hodoscope bars are constructed of a piece of a tube (first 28)
// then added to the ends, rotated a bit are two boxes (last 56)
   physVol->SetTranslation ( G4ThreeVector ( xCell[copyNo],yCell[copyNo],zCell[copyNo] ) );
   if ( copyNo / 28 == 0 )
   {
      physVol->SetRotation ( 0 );
      physVol->SetName ( "hodoscope_curvedBar_phys" );
   }
   else if ( copyNo/28 == 1 )
   {
      physVol->SetRotation ( rotate2 );
      physVol->SetName ( "hodoscope_pmt1_phys" );

   }
   else if ( copyNo/28 == 2 )
   {
      physVol->SetRotation ( rotate1 );
      physVol->SetName ( "hodoscope_pmt1_phys" );

   }
   else if ( copyNo/28 == 3 )
   {
      physVol->SetRotation ( rotate2 );
      physVol->SetName ( "hodoscope_sense1_phys" );

   }
   else if ( copyNo/28 == 4 )
   {
      physVol->SetRotation ( rotate1 );
      physVol->SetName ( "hodoscope_sense2_phys ");

   }
//G4cout << "log name : " <<  physVol->GetLogicalVolume()->GetName()   << G4endl;
}

G4VSolid* BETAHodoscopeCellParameterisation::ComputeSolid
( const G4int copyNo, G4VPhysicalVolume* physVol )
{

   if ( copyNo / 28 == 0 )
   {
      physVol->SetLogicalVolume(hodoscopeBar_log);
     theSolid = hodoscopeBar;   }
   else if ( copyNo/28 == 1 )
   {
      physVol->SetLogicalVolume(PMTinquotes_log);
     theSolid = PMTinquotes;

   }
   else if ( copyNo/28 == 2 )
   {
      physVol->SetLogicalVolume(PMTinquotes_log);
     theSolid = PMTinquotes;
  }
   else if ( copyNo/28 == 3 )
   {
      physVol->SetLogicalVolume(PMTinquotes_log);
     theSolid = PMTphotocathode;

   }
   else if ( copyNo/28 == 4 )
   {
      physVol->SetLogicalVolume(PMTinquotes_log);
     theSolid = PMTphotocathode;

   }

   return theSolid;
}


G4Material* BETAHodoscopeCellParameterisation::ComputeMaterial // material, sensitivity, visAtt
( const G4int copyNo, G4VPhysicalVolume* physVol,
  const G4VTouchable *parentTouch )
{
   G4Material* mat = theMaterial;

   G4OpticalSurface* scoringSurface = new G4OpticalSurface ( "scoringHodoscopeBarOpticalSurface" );
     scoringSurface->SetModel ( unified );
     scoringSurface->SetType ( dielectric_metal);
     scoringSurface->SetFinish ( groundbackpainted);

 /*  if ( copyNo / 28 == 0 )
   {
      physVol->SetLogicalVolume(hodoscopeBar_log);
     theSolid = hodoscopeBar;   }
   else if ( copyNo/28 == 1 )
   {
      physVol->SetLogicalVolume(PMTinquotes_log);
     theSolid = PMTinquotes;

   }
   else if ( copyNo/28 == 2 )
   {
      physVol->SetLogicalVolume(PMTinquotes_log);
     theSolid = PMTinquotes;
  }
   else*/ if ( copyNo/28 == 3 )
   {
       physVol->GetLogicalVolume()->SetSensitiveDetector ( HodoscopePMTs );   


   }
   else if ( copyNo/28 == 4 )
   {
       physVol->GetLogicalVolume()->SetSensitiveDetector ( HodoscopePMTs );   
}



/*      physVol->SetLogicalVolume(PMTinquotes_log);*/
//       physVol->SetName ( "hodoscope_pmt_phys" );
//      new G4LogicalSkinSurface( "BorderHodoscopeBar",physVol->GetLogicalVolume(), scoringSurface );
//       physVol->SetName ( "hodoscope_pmt_phys" );
//       new G4PVPlacement ( 0,G4ThreeVector (0,0,0 ),PMTphotocathode_log,     // Logical volume
//                        "hodoscope_sensitive",     // Name
//                        physVol->GetLogicalVolume(),false,copyNo);

   // Attach detector to scoring volume
   /*  if(copyNo == Ã¢ÂÂ¦)
     {
       mat = material1;
       physVol->GetLogicalVolume()->SetVisAttributes( att1 );
     }

     G4double LucitePhotonEnergy[2] =
               {3.9*eV,4.*eV};

     G4double LuciteRefractiveIndex[2] =
               {1.49,1.49};
   G4MaterialPropertiesTable* myMPTL = new G4MaterialPropertiesTable();
     myMPTL->AddProperty("RINDEX", LucitePhotonEnergy, LuciteRefractiveIndex, 2);
     mat->SetMaterialPropertiesTable(myMPTL);
   */

   return mat;
}

/*
G4Material* BETAHodoscopeCellParameterisation::ComputeMaterial(G4VPhysicalVolume *currentVol,
                                                           const G4int no_lev, 
                                                           const G4VTouchable *parentTouch) {
return(theMaterial);
}*/