#include <stdio.h>
#include <math.h>
#include <iostream.h>  // I/O 
#include <fstream.h>   // file I/O
#include <iomanip.h>   // format manipulation

void GetFieldValue(double x,double y, double z, double *** BF);

int main() {
const int N = 61;
double zposition,rposition,absfield;

    double *** BF;
    double Bz;
    double rmax_sq;
    double zmax;
    int numPoints;
    double * * Field;

double cm = 0.01, m= 1, tesla = 1;

double * * R[N];
BF = &R[0];
double * locations[N*N];
double zandrValue[2*N*N];
 for(int g=0;g<61;g++) {
	R[g]=&locations[61*g];
		}

  	ifstream input_file ;
	input_file.open("fielddata.csv");
  for(int i = 0;i < N*N;i++) {
		input_file >>zposition>>rposition>> zandrValue[2*i]>>zandrValue[2*i+1]>>absfield;
		locations[i] = &zandrValue[2*i];

				}
	input_file.close();

  rmax_sq = sqrt(50.0*cm);
  zmax = 100.0*cm;

  GetFieldValue(0,0,1,BF);
return(0);

}

void GetFieldValue(double x,double y, double z, double *** BF) {

double Bfield[3];
double cm = 0.01, m= 1, tesla = 1;
double Point[3] = {x,y,z};
int RR = sqrt(Point[0]*Point[0]+Point[1]*Point[1])/(10*cm),
      ZZ = Point[2]/(10*cm);
double phi;
double rdistance = sqrt(Point[0]*Point[0]+Point[1]*Point[1]);
if(y == 0) {
phi = 0;
} else {
phi =  asin(Point[1]/rdistance);
}


  if((Point[2] <= 6*m )&& (Point[2]>=0) && rdistance <= 6*m)
  { 

    Bfield[2] = BF[RR][ZZ][0] * tesla;

    Bfield[0] =  cos(phi) * BF[RR][ZZ][1] *tesla;
printf("\nTEST\n");
    Bfield[1] =  sin(phi) * BF[RR][ZZ][1] *tesla;

  }
  else
  { 
    Bfield[2] = 0.0; 
    Bfield[1] = 0.0; 
    Bfield[0] = 0.0; 
}
printf("field x = %lf, y = %lf, z = %lf ",Bfield[0],Bfield[1],Bfield[2]);

}

