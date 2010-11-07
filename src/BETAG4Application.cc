/** 
 * \mainpage 
 *  BETAG4 is a simulation designed for the Spin Asymmetries of the Nucleon Experiment (SANE).
 * 
 *   author Whitney Armstrong (whit@temple.edu)
 * 
 */
//
//
//-----------------------------------------------------------

/*! \page intro Introduction
This page introduces the user to the topic.
Now you can proceed to the \ref advanced "advanced section".  
*/
#include "BETAG4Application.hh"
#include "Ex01MCStack.h"
#include "Ex01DetectorConstructionOld.h"

#include <TROOT.h>
#include <Riostream.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TLorentzVector.h>
#include <TArrayD.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TGeoMaterial.h>

/// \cond CLASSIMP
ClassImp(Ex01MCApplication)
/// \endcond



/// \cond CLASSIMP
ClassImp(Ex01MCApplication)
/// \endcond

//_____________________________________________________________________________
Ex01MCApplication::Ex01MCApplication(const char *name, const char *title) 
  : TVirtualMCApplication(name,title),
    fStack(0),
    fMagField(0),
    fImedAr(0),
    fImedAl(0),
    fImedPb(0),
    fOldGeometry(kFALSE)
{
/// Standard constructor
/// \param name   The MC application name 
/// \param title  The MC application description

  // create a user stack
  fStack = new Ex01MCStack(100);  
  
  // create magnetic field (with zero value)
  fMagField = new TGeoUniformMagField();
}

//_____________________________________________________________________________
Ex01MCApplication::Ex01MCApplication()
  : TVirtualMCApplication(),
    fStack(0),
    fMagField(0),
    fImedAr(0),
    fImedAl(0),
    fImedPb(0),
    fOldGeometry(kFALSE)
{    
/// Standard constructor
/// \param name   The MC application name 
/// \param title  The MC application description

  // create a user stack
  fStack = new Ex01MCStack(100);  
  
  // create magnetic field (with zero value)
  fMagField = new TGeoUniformMagField();

}

//_____________________________________________________________________________
Ex01MCApplication::~Ex01MCApplication() 
{
/// Destructor  

  delete fStack;
  delete fMagField;
  delete gMC;
  gMC = 0;
}


//_____________________________________________________________________________
void Ex01MCApplication::ConstructMaterials()
{
/// Construct materials using TGeo modeller

}





//_____________________________________________________________________________
void Ex01MCApplication::ConstructVolumes()
{
/// Contruct volumes using TGeo modeller



  // close geometry
//  gGeoManager->CloseGeometry();
    
  // notify VMC about Root geometry
//  gMC->SetRootGeometry();
}

//
// public methods
//

//_____________________________________________________________________________
void Ex01MCApplication::InitMC(const char* setup)
{    
/// Initialize MC.
/// The selection of the concrete MC is done in the macro.
/// \param setup The name of the configuration macro 

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
  gMC->Init();
  gMC->BuildPhysics();  
}

//__________________________________________________________________________
void Ex01MCApplication::RunMC(Int_t nofEvents)
{    
/// Run MC.
/// \param nofEvents Number of events to be processed

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishRun()
{    
/// Finish MC run.
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructGeometry()
{    
/// Construct geometry using TGeo functions or
/// TVirtualMC functions (if oldGeometry is selected)
  
  // Cannot use Root geometry if not supported with 
  // selected MC
  if ( !fOldGeometry && ! gMC->IsRootGeometrySupported() ) {
    cerr << "Selected MC does not support TGeo geometry"<< endl;
    cerr << "Exiting program"<< endl;
    exit(1);
  } 

  if ( ! fOldGeometry ) {
    cout << "Geometry will be defined via TGeo" << endl;
    ConstructMaterials();  
    ConstructVolumes(); 
  }
  else {   
    cout << "Geometry will be defined via VMC" << endl;
    Ex01DetectorConstructionOld detConstructionOld;
    detConstructionOld.ConstructMaterials();  
    detConstructionOld.ConstructVolumes(); 
  }  
}

//_____________________________________________________________________________
void Ex01MCApplication::InitGeometry()
{    
/// Initialize geometry.
  
  fImedAr = gMC->MediumId("ArgonGas");
  fImedAl = gMC->MediumId("Aluminium");
  fImedPb = gMC->MediumId("Lead");
}


//_____________________________________________________________________________
void Ex01MCApplication::GeneratePrimaries()
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
  
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // Geantino
 Int_t pdg  = 0;
 
 // Polarization
 Double_t polx = 0.; 
 Double_t poly = 0.; 
 Double_t polz = 0.; 

 // Position
 Double_t vx  = -200.; 
 Double_t vy  = 0.; 
 Double_t vz  = 0.; 
 Double_t tof = 0.;

 // Momentum 
 Double_t px, py, pz, e;
 px = 1.; 
 py = 0.; 
 pz = 0.; 
 e  = 1.;

 // Add particle to stack 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly
, polz, 
                  kPPrimary, ntr, 1., 0);

 // Change direction and add particle to stack 
 px = 1.; 
 py = 0.1; 
 pz = 0.; 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly
, polz,
                  kPPrimary, ntr, 1., 0);

 // Change direction and add particle to stack 
 px = 1.; 
 py = 0.; 
 pz = 0.1; 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly
, polz,
                  kPPrimary, ntr, 1., 0);
}


//_____________________________________________________________________________
void Ex01MCApplication::BeginEvent()
{    
/// User actions at beginning of event.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::BeginPrimary()
{    
/// User actions at beginning of a primary track.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::PreTrack()
{    
/// User actions at beginning of each track.
/// Print info message.

  cout << endl;
  cout << "Starting new track" << endl;
}

//_____________________________________________________________________________
void Ex01MCApplication::Stepping()
{    
/// User actions at each step.
/// Print track position, the current volume and current medium names.
  
  TLorentzVector position;
  gMC->TrackPosition(position);
  
  cout << "Track " 
       << position.X() << " " << position.Y() << " " << position.Z() 
       << "  in " <<  gMC->CurrentVolName() << "  ";
       
  if (gMC->CurrentMedium() == fImedAr) cout <<  "ArgonGas";      
  if (gMC->CurrentMedium() == fImedAl) cout <<  "Aluminium";      
  if (gMC->CurrentMedium() == fImedPb) cout <<  "Lead";      

  cout << endl;
}


//_____________________________________________________________________________
void Ex01MCApplication::PostTrack()
{    
/// User actions after finishing of each track
/// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishPrimary()
{    
/// User actions after finishing of a primary track.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishEvent()
{    
/// User actions after finishing of an event
/// Nothing to be done this example
} 

//_____________________________________________________________________________
void Ex01MCApplication::TestVMCGeometryGetters()
{
/// Test (new) TVirtualMC functions:
/// GetTransform(), GetShape(), GetMaterial(), GetMedium() 

  // Get transformation of 10th layer
  //
  TString volPath = "/EXPH_1/CALB_1/LAYB_9";
  TGeoHMatrix matrix;
  Bool_t result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  }
  else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;
 
  volPath = "/EXPH_1/CALB_1/LAYB_100";
  result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  }
  else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;

  volPath = "/EXPH_1/CALB_1/LAYB_9";
  result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  }
  else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;
 // Get shape
  //
  volPath = "/EXPH_1/CALB_1/LAYB_9";
  TString shapeType;
  TArrayD par;
  result = gMC->GetShape(volPath, shapeType, par);
  if (result) {
    cout << "Shape for " << volPath.Data() << ": " << endl;
    cout << shapeType.Data() << "  parameters: ";
    for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
      cout << par.At(ipar) << ",  ";
    cout << endl;  
  }
  else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;

  // Get material
  //
  TString volName = "LAYB";
  TString matName;
  Int_t imat;
  Double_t a, z, density, radl, inter;
  TArrayD mpar;
  result = gMC->GetMaterial(volName, matName, imat, a, z, density,
                            radl, inter, mpar);
  if (result) {
    cout << "Material for " << volName.Data() << " volume: " << endl;
    cout << matName.Data() << "  " << imat 
         << "  Aeff = " << a 
         << "  Zeff = " << z 
         << "  density = " << density 
         << "  radl = " << radl
         << "  inter = " << inter 
         << endl;
    if ( mpar.GetSize() > 0 ) {
      cout  << " User defined parameters: ";
      for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
        cout << mpar.At(ipar) << ",  ";
      cout << endl; 
    }   
  }
  else {
    cerr << "Volume " << volName.Data() << " not found" << endl;
  }
  cout << endl;

 // Get medium
  //
  TString medName;
  Int_t imed, nmat, isvol, ifield;
  Double_t fieldm, tmaxfd, stemax, deemax, epsil, stmin;
  result = gMC->GetMedium(volName,
                          medName, imed, nmat, isvol, ifield,
                          fieldm, tmaxfd, stemax, deemax, epsil, stmin, mpar);
  if (result) {
    cout << "Medium for " << volName.Data() << " volume: " << endl;
    cout << medName.Data() << "  " << imed 
         << "  nmat = " << nmat
         << "  isvol = " << isvol 
         << "  ifield = " << ifield 
         << "  fieldm = " << fieldm
         << "  tmaxfd = " << tmaxfd 
         << "  stemax = " << stemax
         << "  deemax = " << deemax
         << "  epsil = " << epsil
         << "  stmin = " << stmin
         << endl;
    if ( mpar.GetSize() > 0 ) {
      cout  << " User defined parameters: ";
      for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
        cout << mpar.At(ipar) << ",  ";
      cout << endl; 
    }   
  }
  else {
    cerr << "Volume " << volName.Data() << " not found" << endl;
  }
  cout << endl;

  // Test getters non-existing position/volume
  //
  
  // Transformation
  volPath = "/EXPH_1/CALB_1/LAYB_100";
  result = gMC->GetTransformation(volPath, matrix);
  cout << "GetTransformation: Volume path " << volPath.Data(); 
  if (!result) 
    cout << " not found" << endl;
  else 
    cout << " found" << endl;
  // Shape
  result = gMC->GetShape(volPath, shapeType, par);
  cout << "GetShape: Volume path " << volPath.Data(); 
  if (!result) 
    cout << " not found" << endl;
  else 
    cout << " found" << endl;

  // Material
  volName = "XYZ";
  result = gMC->GetMaterial(volName, matName, imat, a, z, density,
                            radl, inter, mpar);
  cout << "GetMaterial: Volume name " << volName.Data(); 
  if (!result) 
    cout << " not found" << endl;
  else 
    cout << " found" << endl;

  // Medium
  result = gMC->GetMedium(volName,
                          medName, imed, nmat, isvol, ifield,
                          fieldm, tmaxfd, stemax, deemax, epsil, stmin, mpar);
  cout << "GetMedium: Volume name " << volName.Data(); 
  if (!result) 
    cout << " not found" << endl;
  else 
    cout << " found" << endl;
 }  







