/////////////////////////////////////////////////////////////////////
//
//   DetectorConstruction
//   Big Electron Telescope Array ( BETA )
//
//   The detector package consists of a forward tracker, Gas 
//   Cherenkov, Lucite Hodoscope and Calorimeter (BIGCAL)
//
//   author Whitney Armstrong (whit@temple.edu)
//
/////////////////////////////////////////////////////////////////////
#ifndef BETADetectorConstruction_h
#define BETADetectorConstruction_h 1
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4PVParameterised.hh"
#include "G4Sphere.hh"
#include "G4VisAttributes.hh"
#include "G4Ellipsoid.hh"
#include "G4ChordFinder.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Torus.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSSphereSurfaceCurrent.hh"
#include "G4SDManager.hh"
#include "G4PSFlatSurfaceCurrent.hh"
#include "G4SDParticleWithEnergyFilter.hh"
#include "G4Trd.hh"
#include "BETAField.hh"
#include "BETASimulationManager.hh"
#include "BETASimulationMessenger.hh"

class BETASimulationManager;


/** BETAG4 Detector construction class
 *
 *   Detector construction class which builds all geometries and returns the world volume 
 *   following the standard GEANT4 way...
 *
 *   This class makes use of a MySQL database to grab all the detector geometry information. 
 *   Currently the database is named SANE and the geometry tables used are :
 *   <ul>
 *   <li> cherenkov_positioning_parameters
 *   <li> lucite_geometry_parameters
 *   <li> bigcal_parameters
 *   <li> tracker_geometry_parameters (NOT IMPLEMENTED YET)
 *   <li> target_material_runlist 
 *   <li> target_geometry_parameters
 *   </ul>
 *
 *   The idea is to use the standard geometry constructions for each detector for both simulation (here) and
 *   for reconstruction. For this we make use of the VGM framework which allows for easy conversion from one
 *   geometry model to another (eg ROOT->GEANT4 or XML->ROOT etc...)
 *
 * \ingroup Detectors
 */
class BETADetectorConstruction : public G4VUserDetectorConstruction {
   private:

   protected:

   public:
      friend class BETASimulationManager;
   public:
      BETAField * fMagneticField;

   public :
      BETADetectorConstruction();
      ~BETADetectorConstruction();

      G4bool             constructed;
      G4VPhysicalVolume* expHall_phys;

      G4LogicalVolume* expHall_log;

      /** @name Big Electron Telescope Array Construction
       *  Constructs the BETA Detector Package.
       *  We construct the BETADetectors in a box for which y is vertical and x is horizontal.
       *  These are the Bigcal Coordinates.
       *  @{
       */
      void ConstructBETA();
      G4LogicalVolume* BETADetector_log;

      G4double fBETADistance; ///< Distance from origin to the front face of the BETA bounding box
      G4double DetectorAngle ;
      G4double DetectorLength ;
      G4double DetectorWidth  ;
      G4double rTarget; 
      G4double rCherenkov;
      G4double rTracker;
      G4double rHodoscope;
      G4double rBigcal;

      //@}

      /** 
       *
       */
      bool usingTargetCup;
      bool usingTargetOVC;
      bool usingFakePlaneAtBigcal; 
      bool usingFakePlaneAtForwardTracker;





      /** @name BigCal construction.
       *  @{
       */
      void ConstructBIGCAL();
      bool usingBigcal;
      G4LogicalVolume *   calorimeterTop_log;
      G4VPhysicalVolume * calorimeterTop_phys;
      G4LogicalVolume *   calorimeterBottom_log;
      G4VPhysicalVolume * calorimeterBottom_phys;
      G4LogicalVolume*    cellLogicalBottom;
      G4LogicalVolume*    cellLogical;
      //@}

      /** @name Cherenkov Construction.
       *  @{
       */
      void ConstructCherenkov();
      bool usingGasCherenkov;
      G4VPhysicalVolume * cherenkovTank_phys;
      G4LogicalVolume*   tank_log; 
      G4LogicalVolume * cherenkovContainer_log;
      G4LogicalVolume * fCerFrontWindow_log;
      G4LogicalVolume * fCerBackWindow_log;
      G4LogicalVolume * AlCans_log;
      G4LogicalVolume * fCerFrameTopBottom_log;
      G4LogicalVolume * fCerToroidalMirror_log;
      G4LogicalVolume * fCerSphericalMirror_log;
      G4LogicalVolume * container_log;
      G4LogicalVolume * MUPipe_log;
      G4LogicalVolume * pmtFace_log;
      //@}

      /** @name Forwart Tracker Construction.
       *  @{
       */
      void ConstructForwardTracker();
      bool usingForwardTracker; 
      G4LogicalVolume*   tracker_log; 
      G4VPhysicalVolume* tracker_phys;

      G4LogicalVolume*   fTrackerY1Bar_log  ;
      G4LogicalVolume*   fTrackerY2Bar_log  ;
      G4LogicalVolume*   fTrackerX1Bar_log  ;
      G4LogicalVolume*   fTrackerY1BarScore_log  ;
      G4LogicalVolume*   fTrackerY2BarScore_log  ;
      G4LogicalVolume*   fTrackerX1BarScore_log  ;

      G4LogicalVolume*   fTrackerG10Frame_log ;

      G4LogicalVolume*   trackerY1_log;
      G4LogicalVolume*   trackerY2_log;
      G4LogicalVolume*   trackerX1_log;
      G4VSensitiveDetector* frontTrackerSD;

      G4LogicalVolume*   fTrackerHorizBar_log  ; /// \deprecated
      G4LogicalVolume*   fTrackerVertBar_log ; /// \deprecated
      G4LogicalVolume*   fTrackerHorizBarScore_log  ; /// \deprecated
      G4LogicalVolume*   fTrackerVertBarScore_log ; /// \deprecated
      //@}

      /** @name Lucite Hodoscope Construction
       *  @{
       */
      void ConstructHodoscope();
      bool usingLuciteHodoscope;
      G4LogicalVolume*   hodoscopeContainerBox_log; 
      G4VPhysicalVolume* hodoscopeContainerBox_phys;
      G4LogicalVolume*   fLuciteHodoPMTphotocathode_log;
      G4LogicalVolume*   fLuciteHodoPMTinquotes_log;
      G4LogicalVolume*   fLuciteHodoBar_log;
      G4VSensitiveDetector* HodoscopePMTSD;

      //@}

      /** @name Construct "fake" scoring planes
       *  @{
       */
      void ConstructFakePlane();
      G4LogicalVolume * fBigCalFakePlane_log;
      G4LogicalVolume * fTrackerFakePlane_log;
      //@}

      /** @name Target Magnetic Field
       *  @{
       */
      void ConstructMagneticField();

      //@}


      /** @name Polarized Ammonia Target Construction
       *  From Wright's code which constructs the UVA NH3 Target
       *  @{
       */
      /** From Wright's code which constructs the UVA NH3 Target. */
      void ConstructTarget();

      /**Constructs the Target Can.*/
      void ConstructTCan();

      /**Constructs the Liquid Nitrogen Shield.*/
      void ConstructN2Shield();

      /**Constructs the Oxford Magnet geometry.*/
      void ConstructMagnet();

      /**Constructs the Target Nose.*/
      void ConstructNose();


      double GetTargetAngle();
      void   SetTargetAngle(G4double);
      void   SwitchTargetField();           ///< swaps between 180 and 80 degrees
      void   PrintTargetAngle();

      /**
      */
      G4RotationMatrix fMagnetRotationMatirx;


      /// Copied from Justin Wright's code:

      G4double CuRadiationLength;
      G4double radPercent;          // Enter as percentage of radiation length
      G4double radX;

      //define some constants 
      G4double OVCIR;          //inner radius of can
      G4double OVCCthick;      //thickness of can (0.935 inch Al)
      G4double OVCCheight;     //length of can along axis
      G4double OVCWthick;      //window thickness (0.013 inch)
      G4double OVCWheight;     //height of window along can axis
      G4double OVCBrad;        //beam window radius (2.00 inches)
      G4double OVCBthick;      //beam window thickness (0.02 inch Be)
      G4double OVCdis;         //The displacement of the OVC windows
      //    in the Z direction

      G4double LN2IR;          //inner radius of can
      G4double LN2Cthick;      //thickness of can (0.935 inch Al)
      G4double LN2Cheight;     //length of can along axis (30.5 inches)
      G4double LN2Wthick;      //window thickness (0.002 inch)
      G4double LN2Wheight;     //height of window along can axis
      G4double LN2Brad;        //beam window radius (2.25 inches)
      G4double LN2Bthick;      //beam window thickness (0.013 inch Be)
      G4double LN2dis;         //vertical displacement of windows and 
      //target from center of can.

      G4double HeIR;           //4K He shield inner radius
      G4double HeShThick;      //4K He shield thickness
      G4double HeLength;       //4K He shield length (same as LN2 shield)
      G4double NoseIR;         //Inner radius of the Nose
      G4double NoseThick;      //Thickness of the Nosepiece
      G4double NoseLength;     //Length of the nosepiece (same as LN2Shield)
      G4double CellOR;         //OuterRadius of the cellwall
      G4double CellLength;     //Axial length of the cell wall
      G4double CellThick;      //Thickness of the cell wall

      //-----------The target can------------------------------------
      G4LogicalVolume*   logicTCan;    //pointer to the Target Can as a whole
      G4VPhysicalVolume* physiTCan;

      G4Tubs*            solidRad;     //pointer to the solid radiator
      G4LogicalVolume*   logicRad;
      G4VPhysicalVolume* physiRad;

      G4LogicalVolume*   logicWinCan;
      G4VPhysicalVolume* physiWinCan;
      G4LogicalVolume*   logicBeamWin;
      G4VPhysicalVolume* physiBeamWin;

      //----------The Beam Pipes---------------------------------------
      G4Tubs*            solidUpPipe0;     //Pointers to the narrow pipe
      G4LogicalVolume*   logicUpPipe0;     // (upstream)
      G4VPhysicalVolume* physiUpPipe0;

      G4Tubs*            solidUpPipe0Cav;  //Pointers to the narrow pipe
      G4LogicalVolume*   logicUpPipe0Cav;  // cavity (upstream)
      G4VPhysicalVolume* physiUpPipe0Cav;

      G4Tubs*            solidUpPipe1;     //Pointers to the wider pipe
      G4LogicalVolume*   logicUpPipe1;     // (upstream)
      G4VPhysicalVolume* physiUpPipe1;

      G4Tubs*            solidUpPipe1Cav;  //Pointers to the wider pipe
      G4LogicalVolume*   logicUpPipe1Cav;  // cavity (upstream)
      G4VPhysicalVolume* physiUpPipe1Cav;

      G4Tubs*            solidDownPipe0;   //Pointers to the narrow pipe
      G4LogicalVolume*   logicDownPipe0;   // (downstream)
      G4VPhysicalVolume* physiDownPipe0;

      G4Tubs*            solidDownPipe0Cav;  //Pointers to the narrow pipe
      G4LogicalVolume*   logicDownPipe0Cav;  // cavity (downstream)
      G4VPhysicalVolume* physiDownPipe0Cav;

      G4Tubs*            solidDownPipe1;   //Pointers to the wider pipe
      G4LogicalVolume*   logicDownPipe1;   // (downstream)
      G4VPhysicalVolume* physiDownPipe1;

      G4Tubs*            solidDownPipe1Cav;  //Pointers to the wider pipe
      G4LogicalVolume*   logicDownPipe1Cav;  // cavity (downstream)
      G4VPhysicalVolume* physiDownPipe1Cav;

      G4Tubs*            solidFlange;      //Pointers to the flanges
      G4LogicalVolume*   logicFlange;
      G4VPhysicalVolume* physiUpFlange;
      G4VPhysicalVolume* physiDownFlange;

      G4Tubs*            solidFlangeCav;   //Pointers to the cavity
      G4LogicalVolume*   logicFlangeCav;   //   in the flanges
      G4VPhysicalVolume* physiUpFlangeCav;
      G4VPhysicalVolume* physiDownFlangeCav;

      G4Tubs*            solidHeShroud;    //Pointers to the He Shroud
      G4LogicalVolume*   logicHeShroud;  
      G4VPhysicalVolume* physiHeShroud;

      //----------The LN2 shield----------------------------------------
      G4LogicalVolume*   logicLN2Shield;
      G4VPhysicalVolume* physiLN2Shield;
      G4LogicalVolume*   logicLN2BeamWin;
      G4VPhysicalVolume* physiLN2BeamWin;

      //------Part of old target can... NOT new target can------------------------
      //    G4Tubs*            solidTWin;   //Pointer to the Target Window
      //    G4LogicalVolume*   logicTWin;
      //    G4VPhysicalVolume* physiTWin;

      G4Tubs*            solidLN2Can;   //Pointer to the LN2 shield can as a whole
      G4LogicalVolume*   logicLN2Can;
      G4VPhysicalVolume* physiLN2Can;

      G4Tubs*            solid4KSH;   //Pointer to the 4 K shield (LHe)
      G4LogicalVolume*   logic4KSH;
      G4VPhysicalVolume* physi4KSH; 

      G4Tubs*            solidHelium; //Pointer to the Helium inside the 4Kshield
      G4LogicalVolume*   logicHelium;
      G4VPhysicalVolume* physiHelium;

      //----------The NosePiece and Target Cell--------------------------
      G4Tubs*            solidNose;   //Pointer to the whole Nose
      G4LogicalVolume*   logicNose;
      G4VPhysicalVolume* physiNose;

      G4Tubs*            solidTail;   //Pointer to the shell of the Nose
      G4LogicalVolume*   logicTail;
      G4VPhysicalVolume* physiTail;

      G4Tubs*            solidCell;   //Pointer to the Cell contents
      G4LogicalVolume*   logicCell;
      G4VPhysicalVolume* physiCell;

      G4Tubs*            solidCWall;
      G4LogicalVolume*   logicCWall;   //Pointer to the Cell's container
      G4VPhysicalVolume* physiCWall;

      //-----------The target magnet-----------------------------------
      G4LogicalVolume*   logicMagnet;    //Pointer to the Magnet as a whole
      G4VPhysicalVolume* physiMagnet;

      //NOTE: CoilUp is the upstream coil. CoilDown is the downstream coil.
      G4Cons*            solidCoil;      //pointer to the magnet coils
      G4LogicalVolume*   logicCoil;
      G4VPhysicalVolume* physiCoilUp;   
      G4VPhysicalVolume* physiCoilDown;

      G4Tubs*            solidBrace1;    //Pointer to the target magnet braces
      G4LogicalVolume*   logicBrace1;
      G4VPhysicalVolume* physiBrace1;

      G4Tubs*            solidBrace2;    
      G4LogicalVolume*   logicBrace2;
      G4VPhysicalVolume* physiBrace2; 

      G4Tubs*            solidBrace3;   
      G4LogicalVolume*   logicBrace3;
      G4VPhysicalVolume* physiBrace3;

      G4Tubs*            solidBrace4;    
      G4LogicalVolume*   logicBrace4;
      G4VPhysicalVolume* physiBrace4;

      //@}

      /** @name Beamline components
       *  @{
       */
      /**Constructs the Beam pipes up sstream and down stream*/
      void ConstructBeamPipe();

      /** Constructs the Helium Bag and Helium Bag extension used for SANE.
       * The Box has dimensions (ignoring the flange) 4.67 x 7.97 x 71.97 in^3
       */
      void ConstructHeliumBag();

      G4Box*            fHeBagExtenderBox;
      G4LogicalVolume*   fHeBagExtenderBox_log;
      G4VPhysicalVolume* fHeBagExtenderBox_phys;

      G4Box*            fHeBagExtenderAngleBox1;
      G4Box*            fHeBagExtenderAngleBox2;
      G4VSolid*   fHeBagExtenderAngle1;
      G4VSolid*   fHeBagExtenderAngle2;
      G4VSolid*   fHeBagExtenderAngle3;
      G4VSolid*   fHeBagExtenderAngle4;
      G4LogicalVolume*   fHeBagExtenderAngle1_log;
      G4LogicalVolume*   fHeBagExtenderAngle2_log;
      G4LogicalVolume*   fHeBagExtenderAngle3_log;
      G4LogicalVolume*   fHeBagExtenderAngle4_log;

      G4Box*   fHeBagExtenderHorizSupport;
      G4Box*   fHeBagExtenderVertSupport;
      G4LogicalVolume*   fHeBagExtenderHorizSupport_log;
      G4LogicalVolume*   fHeBagExtenderVertSupport_log;

      G4Box*            fHeBagExtenderHorizWindow;
      G4Box*            fHeBagExtenderVertWindow;
      G4Box*            fHeBagExtenderFrontWindow;
      G4LogicalVolume*   fHeBagExtenderHorizWindow_log;
      G4LogicalVolume*   fHeBagExtenderVertWindow_log;
      G4LogicalVolume*   fHeBagExtenderFrontWindow_log;
      //@}

      /**
       * Called from ConstructBETA
       * Should get the simulation manager and names and corresponding pointers to scoring volumes.
       */
      void SetupScoring(G4LogicalVolume * scoringVolume);

      /** Mandatory GEANT4 user hook which initiates all geometry constructions
      */
      G4VPhysicalVolume* Construct();


      /**
       * For User Interface
       */
      void setMirrorAngles(int,G4double, G4double);

      /** turn on and off the target geometry and material */
      void ToggleTargetMaterial(int);

      /** turn on and off the target magnetic field*/
      void ToggleTargetField(int);

      /** For User Interface
      */
      void rotateMirror(int , G4double , G4double );

      void lookAtField(G4String);

      int fTargetState;

      /** For which detectors are constructed
      */
      BETASimulationManager * fSimulationManager;

   private:

      G4float ULimits;

      /*     void SetFrontThickness (G4String);     
             void SetBackThickness(G4String);     
             void SetFrontMaterial (G4double);     
             void SetBackMaterial(G4double);
             */

   public: 
      /** Assigns material property tables to G4Materials
       *  Called from DefineMaterials
       *  Important differences are between, say, Lucite and Lucite_NoOptics.
       *  The former has an index of refraction associated with it, which thus 
       *  triggers the GEANT4 kernel to produce Optical Photon processes.
       */
      void SetMaterialPropertiesTables();

      /**
       *  Defines All Detector Materials
       */
      void DefineMaterials();

      /*	void ConstructAluminumFrame();
                void ConstructMirrors();
                void ConstructPMTs();
                void Construct*/

   public:

      /** @name Vis Attributes
       *  For controlling visualiztion.
       * @{
       */
      G4VisAttributes*   AlVisAtt;
      G4VisAttributes*   Invisible;
      G4VisAttributes*   LeadVis;
      G4VisAttributes*   Opaque;

      G4VisAttributes*   fPrimaryColorLineVisAtt;
      G4VisAttributes*   fComplementaryColorLineVisAtt;
      G4VisAttributes*   fSecondaryColorALineVisAtt;
      G4VisAttributes*   fSecondaryColorBLineVisAtt;

      G4VisAttributes*   fInvisibleVisAtt;
      G4VisAttributes*   fBlackLineVisAtt;
      G4bool             fIsBlackLineVis;

      void               ConstructVisAtt();
      void               SetVisAtt();

      //@}




      G4Material*        NitrogenGas_NoOptics;
      G4Material*        Lucite_NoOptics;
      G4Material*        LeadGlass_NoOptics;
      G4Material*        HeGas;
      G4Material*        LHe;
      G4Material*        TargetNH3;
      G4Material*        Pb;
      G4Material*        Nb;
      G4Material*        N2Gas;
      G4Material*        NitrogenGas;
      G4Material*        Aluminum;
      G4Material*        Al;
      G4Material*        H;
      G4Material*        G10;
      G4Material*        Cu;
      G4Material*        Be;
      G4Material*        Water;
      G4Material*        Vacuum;
      G4Material*        Air;
      G4Material*        Glass;
      G4Material*	       Quartz;
      G4Material*        Silver;
      G4Material*        CsI;
      G4Material*        PVF;
      G4Material*        NH3;
      G4Material*        Lead;
      G4Material*        LeadGlass;
      G4Material*        Lucite;
      G4Material        *TrackerScint_NoOptics;
      G4Material        *TrackerScint;	   

      G4double expHall_x;
      G4double expHall_y;
      G4double expHall_z;

      G4double tank_x;
      G4double tank_y;
      G4double tank_z;

      // Cherenkov Members
      // Mirror memebers
   private:

      G4VPhysicalVolume * MirrorGlass_phys1;
      G4VPhysicalVolume * MirrorGlass_phys2;
      G4VPhysicalVolume * MirrorGlass_phys3;
      G4VPhysicalVolume * MirrorGlass_phys4;
      G4VPhysicalVolume * MirrorGlass_phys5;
      G4VPhysicalVolume * MirrorGlass_phys6;
      G4VPhysicalVolume * MirrorGlass_phys7;
      G4VPhysicalVolume * MirrorGlass_phys8;
      G4ThreeVector mirror1Trans;
      G4ThreeVector mirror2Trans;
      G4ThreeVector mirror3Trans;
      G4ThreeVector mirror4Trans;
      G4ThreeVector mirror5Trans;
      G4ThreeVector mirror6Trans;
      G4ThreeVector mirror7Trans;
      G4ThreeVector mirror8Trans;


      G4double alpha1,
               alpha2,
               alpha3,
               alpha4,
               alpha5,
               alpha6,
               alpha7,
               alpha8,
               beta1,
               beta2,
               beta3,
               beta4,
               beta5,
               beta6,
               beta7,
               beta8;

      //The spherical mirror (near and far) rotation matrix
      G4RotationMatrix RM1,RM2,RM3,RM4,RM5,RM6,RM7,RM8,
                       pmtRM1,pmtRM2,pmtRM3,pmtRM4,pmtRM5,pmtRM6,pmtRM7,pmtRM8;



};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*BETADetectorConstruction_h*/
