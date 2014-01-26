#ifndef UVAMagneticField_H
#define UVAMagneticField_H 1
#include "UVAMagneticField.hh"
#include "globals.hh"
#include "G4MagneticField.hh"
#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TNetFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include "TH1F.h"
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRint.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "UVAOxfordMagneticField.h"

//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_spline.h>
#include <vector>

#define fieldDataPoints 31161

/**
 *  Concrete class implements the UVA Polarized Ammonia target magnetic field
 */
class UVAMagneticField : public BETAField
{
  public:
    UVAMagneticField();
    ~UVAMagneticField();


//UVAOxfordMagneticField * fUVAMagnet;

/**
 * Fills the array Bfield[0-2], (xyz) components, given the spacetime point, Point
 */
  //virtual void GetFieldValue( const  double Point[4], double *Bfield );

/**
 * Creates a plot of the field component given in the argument
 * component can be "z","Z","r", or "R"
 */
    void LookAtField(char * component);


/**
 * 
 */
//    void TestInterpolation();


/**
 * 
 */
    void ReadDataFile();

/**
 * 
 */
    void setPolarizationAngle(G4double angle) {polAngle = angle;}

/**
 * 
 */
    void switchPolarization();

    G4double polAngle;

  private:

  struct fieldDataPoint 
  {
   G4double z,r,Bz,Br;
  };

double RawBZ[fieldDataPoints];
double RawBR[fieldDataPoints];
double * BzRpoint[221];
double * BrRpoint[221];

	vector<fieldDataPoint> fieldData;

    G4double ** BzFieldRaw;
    G4double ** BrFieldRaw;

    G4double *** BF;
//     G4double * locations[fieldDataPoints*fieldDataPoints];
//     G4double zandrValue[2*fieldDataPoints*fieldDataPoints];
//     G4double * * R[fieldDataPoints];



    G4int numPoints;
    G4double* * Field;
};

#endif
