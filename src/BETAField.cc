#include "BETAField.hh"
#include <iostream>  // I/O
#include <fstream>   // file I/O
#include <iomanip>   // format manipulation
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "TGraph2D.h"
#include "TGraph.h"
// ROOT //
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
#include "TStyle.h"
//#define CKOVFIELD_OFF 1
//#define fieldDataPoints 30800

double BilinearInterpolation(double * cornerDataPoints, double * corners, double x, double y)  ;


BETAField::BETAField() :polAngle ( pi*180.0/180. )
{


   BzFieldRaw = BzRpoint;
   BrFieldRaw = BrRpoint;

// Unfortunately, the data reads in the z's first,
// so it is BzFieldRaw[R index][Z index]
   for ( int g=0;g<221;g++ )
   {
      BzRpoint[g]=&RawBZ[141*g];
      BrRpoint[g]=&RawBR[141*g];

   }

   ReadDataFile(); /// New data file


}

BETAField::~BETAField()
{
   ;
}

void BETAField::ReadDataFile() {
// this function is designed to read in the csv extended field map data for the UVA target
   fieldDataPoint temp;
   ifstream input_file ;
   int i=0;
   int j = 0;
   double tempz, tempr, tempBz , tempBr;
   input_file.open ( "UVA-ExtendedFieldMap.csv" );

   if (!input_file) { "\n \n BREAK";
   }

   for (j=0;j<141;j++) {

      for (i=0;i<221;i++)
      {


         input_file >> tempz >> tempr >> tempBz >> tempBr ;
// fieldData.push_back(temp);

// G4cout << tempz <<" "<<tempr<<" "<< tempBz <<" "<< tempBr << G4endl;

         BzFieldRaw[i][j] = tempBz*tesla;
         BrFieldRaw[i][j] = tempBr*tesla;
// G4cout << i <<" "<< j <<  G4endl;




      }

   }



   input_file.close();



}
//


/// Z -> i, R -> j
double BilinearInterpolation(double * cornerDataPoints, double * corners, double x, double y) {

   double Q11 = cornerDataPoints[0];
   double Q21 = cornerDataPoints[1];
   double Q12 = cornerDataPoints[2];
   double Q22 = cornerDataPoints[3];
   double x1 = corners[0];
   double x2 = corners[1];
   double y1 = corners[2];
   double y2 = corners[3];

   double dxdy = (x2-x1)*(y2-y1);

   double result  = Q11*(x2-x)*(y2-y)/dxdy +
                    Q21*(x-x1)*(y2-y)/dxdy +
                    Q12*(x2-x)*(y-y1)/dxdy +
                    Q22*(x-x1)*(y-y1)/dxdy  ;

   return(result);
}

void BETAField::GetFieldValue ( const double Point[4],double *Bfield ) const
{

//G4cout << " X=" << Point[0]/cm << " Y=" << Point[1]/cm << " Z=" << Point[2]/cm<< G4endl;

   G4ThreeVector thePositionVector ( Point[0], Point[1], Point[2] );
// x = Point[0] , y = Point[1], z = Point[2] , t = Point[3]
// These are the cylindrical Coordintates phi and r
   G4double phi = thePositionVector.phi();
//G4cout << "PHI " << phi << G4endl;
   double R = sqrt(Point[0]*Point[0]+Point[1]*Point[1]);



////// POSITIVE Z
   if ( ( thePositionVector.z() < 4.40*m ) && (thePositionVector.z()>= 0*m ) && R <= 2.80*m )
   {

      int ii = floor(thePositionVector.z()/(cm*2));
      int jj = floor(R/(cm*2));

//  G4cout << "jj=" << jj<< " ii =" << ii<< G4endl;

      double corners[4] = {ii*2, (ii+1)*2, jj*2,(jj+1)*2};

// get interpolated Bz value
      double cornerDataPoints[4] = {BzFieldRaw[ii][jj], BzFieldRaw[ii+1][jj], BzFieldRaw[ii][jj+1], BzFieldRaw[ii+1][jj+1] };
      double interpBz = BilinearInterpolation( &cornerDataPoints[0], &corners[0], thePositionVector.z()/cm , sqrt(Point[0]*Point[0]+Point[1]*Point[1])/cm);

// get interpoolated Br value
      double cornerDataPoints2[4] = {BrFieldRaw[ii][jj], BrFieldRaw[ii+1][jj], BrFieldRaw[ii][jj+1], BrFieldRaw[ii+1][jj+1] };
      double interpBr = BilinearInterpolation( &cornerDataPoints2[0], &corners[0], thePositionVector.z()/cm, sqrt(Point[0]*Point[0]+Point[1]*Point[1])/cm );


      G4ThreeVector theField (interpBr*cos(phi),interpBr*sin(phi),interpBz );
//  theField.setRhoPhiZ(interpBr,phi, interpBz );
      theField.rotateX ( polAngle );
      Bfield[0] = theField.x();
      Bfield[1] = theField.y();
      Bfield[2] = theField.z();

   }
/// / NEGATIVE Z
   else if ( ( thePositionVector.z() < 0*m ) && (thePositionVector.z() > -4.40*m ) && R <= 2.80*m )
   {

      int ii = floor(fabs(thePositionVector.z()/(cm*2)));
      int jj = floor(R/(cm*2));


//  G4cout << "ii=" << ii<< " jj =" << jj<< G4endl;

      double corners[4] = {ii*2, (ii+1)*2, jj*2,(jj+1)*2};


// get interpolated Bz value
      double cornerDataPoints[4] = {BzFieldRaw[ii][jj], BzFieldRaw[ii+1][jj], BzFieldRaw[ii][jj+1], BzFieldRaw[ii+1][jj+1] };
      double interpBz = BilinearInterpolation( &cornerDataPoints[0], &corners[0], fabs(thePositionVector.z()/cm) , sqrt(Point[0]*Point[0]+Point[1]*Point[1])/cm);

// get interpoolated Br value
      double cornerDataPoints2[4] = {BrFieldRaw[ii][jj], BrFieldRaw[ii+1][jj], BrFieldRaw[ii][jj+1], BrFieldRaw[ii+1][jj+1] };
      double interpBr = -1.0*BilinearInterpolation( &cornerDataPoints2[0], &corners[0], fabs(thePositionVector.z()/cm), sqrt(Point[0]*Point[0]+Point[1]*Point[1])/cm );


      G4ThreeVector theField (interpBr*cos(phi), interpBr*sin(phi), interpBz );
//  theField.setRhoPhiZ(interpBr,phi, interpBz );
      theField.rotateX ( polAngle );
      Bfield[0] = theField.x();
      Bfield[1] = theField.y();
      Bfield[2] = theField.z();



   }
   else
   {
      Bfield[2] = 0.0;
      Bfield[1] = 0.0;
      Bfield[0] = 0.0;
   }

// TURN FIELD OFF - UNCOMMENT
//   Bfield[2] = 0.0;
//   Bfield[1] = 0.0;
//   Bfield[0] = 0.0;
//printf("field x = %lf, y = %lf, z = %lf ",Bfield[0],Bfield[1],Bfield[2]);

}

void BETAField::switchPolarization() {
   if ( polAngle==pi )
   {
      G4cout << "Target switching to TRANSVERSE field orientation." << G4endl;
      polAngle= ( 80.*pi/180. );
   }
   else
   {
      G4cout << "Target switching to PARALLEL field orientation." << G4endl;
      polAngle= pi;
   }
}


void BETAField::TestInterpolation() {

   Double_t y[10];
   Double_t x[10];

   TGraph * test = new TGraph();
   TCanvas *c = new TCanvas("c", "sine test",200,10,400,400);
   for (int t = 0;t<10;t++) {

      y[t] = 0;//TMath::Sin(t);
      x[t] = t;
   }
   TGraph * graph = new TGraph(10,x,y);

}

void BETAField::LookAtField(G4String component)  {
   //        TRint *theApp = new TRint("To Look at Magnetic Field", NULL,NULL);
   //(gROOT->GetApplication()) =  new TRint("ROOT example", NULL, NULL);
   // Init Intrinsics, build all windows, and enter event loop
//   gROOT->Reset();

   const int n = 51;
   Double_t Br[n*n], Bz[n*n], y[n*n], z[n*n];
   G4double point[4] = {0,0,0,0};
   G4double Bfield[3] = {0,0,0};
   G4double step = 5.0*cm;
//   TCanvas *c = new TCanvas("c","Graph2D example",1,1,700,600);

   TGraph *test = new TGraph();

   TH1F * afield = new TH1F ( "zfield", "zfield",50,0.,0.00001 );

//   TGraph2D *dt = new TGraph2D();
   TCanvas * c = new TCanvas("c1","Some Plots",200,10,700,900);



   Int_t N = 0;
   for (Int_t i=0; i<n; i++) {
      for (Int_t j=0; j<n; j++) {
         point[1] = (i-n/2)*step;
         point[2] = (j-n/2)*step;

         GetFieldValue (&point[0], &Bfield[0] );

         y[N] = point[1]/cm;
         z[N] = point[2]/cm;
         Bz[N] = Bfield[2]/tesla;
//        Br[N] = sqrt(Bfield[0]*Bfield[0] + Bfield[1]*Bfield[1])/tesla;
         Br[N] = Bfield[1];
         G4cout << " x " << point[0]/cm << " y " << point[1]/cm << " z " << point[2]/cm << G4endl;
         G4cout << " Bx " << Bfield[0]/tesla << " By " << Bfield[1]/tesla << " Bz " << Bfield[2]/tesla << G4endl;
         //test->SetPoint(N,(Double_t)point[2],(Double_t)Bfield[3]);
// afield->Fill(Bfield[3]);

//       dt->SetPoint(N,(Double_t)point[1],(Double_t)point[2],(Double_t)Bfield[3]);
         N++;
//  dt->Draw("surf1");
//   test->Draw();
//afield->Draw();
// c->Update();
      }
   }
   TGraph2D *dt;
   if (component == "Z" || component == "z") {
      dt = new TGraph2D(n*n,y,z,Bz);
      dt->SetTitle("Target Magnetic Field - z component");
   }
   else if (component == "R" || component == "r"){
      dt = new TGraph2D(n*n,y,z,Br);
      dt->SetTitle("Target Magnetic Field - r component");
   }else {
      G4cout << "Wrong argument. Assuming z component. "<< G4endl;
      dt = new TGraph2D(n*n,y,z,Bz);
      dt->SetTitle("Target Magnetic Field - z component");
   }

   dt->GetXaxis()->SetTitle("y [cm]");
   dt->GetYaxis()->SetTitle("z [cm]");
//  gStyle->SetPalette(1);
   dt->Draw("surf1");
//  test->Draw();
//afield->Draw();
   c->Update();
// int Ex =0;
// std::cout << "exit :" ;
// while(Ex ==0) std::cin >> Ex ;


   point[0] = 9*cm;
   point[1] = -51.58*2.54*cm;
   point[2] = 57.42*2.54*cm;

   G4cout << "Field at ("<< point[0]/cm << ", " << point[1]/cm<< ", " << point[2]/cm << ") " << G4endl;


   GetFieldValue (&point[0], &Bfield[0] );
   G4cout << "is ("<< Bfield[0]*10000.0/tesla << ", " << Bfield[1]*10000.0/tesla<< ", " << Bfield[2]*10000.0/tesla << ") " << G4endl;


   (gROOT->GetApplication())->Run(true);

//          return(0);
}
