#include "PMTresponse.hh"
#include <iostream.h>  
#include <fstream.h>  
#include <iomanip.h>  
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/Randomize.h"
#include "CLHEP/Random/RandBinomial.h"
#include "CLHEP/config/iostream.h"

PMTresponse::PMTresponse()
{
	
	output_file.open("binomial.dat");
}

PMTresponse::~PMTresponse()
{
output_file.close();
}

void PMTresponse::GetNumberPhotoelectrons()
{
  int g;
  int a = 20;
  double b = 0.2;          
   
for(g=0;g<50;g++){
	output_file << RandBinomial::shoot(a,b)  << endl;
		}

	

}

int
main() {

PMTresponse  qwert;
qwert.GetNumberPhotoelectrons();

return(0);
}
