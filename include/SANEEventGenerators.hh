#ifndef SANEEventGenerators_HH
#define SANEEventGenerators_HH 1
#include "Randomize.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "InSANEInclusivePhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"

/**
 */ 
class BETAG4EventGenerator   {
public:

   int fNumberOfGeneratedParticles;

   InSANEPhaseSpaceVariable * varTheta;
   InSANEPhaseSpaceVariable * varPhi;
   InSANEPhaseSpaceVariable * varEnergy;
   InSANEPhaseSpaceVariable * var;

   double fThetaMax;
   double fEnergyMax;
   double fPhiMax;
   double fEnergyMin;
   double fThetaMin;
   double fPhiMin;
   InSANEInclusivePhaseSpace * fPhaseSpace;
   InSANEPhaseSpaceSampler * fEventSampler;
   InSANEInclusiveDiffXSec * fDiffXSec;
public:

   void Print() {
      std::cout << " ==== BETAG4EventGenerator ==== \n";
      fPhaseSpace->Print();
      fDiffXSec->Print();
      fEventSampler->Print();
      std::cout << " BETAG4EventGenerator member variables: ";
      std::cout << "   Energy: " << fEnergyMin << " - " << fEnergyMax << "\n";
      std::cout << "    Theta: " << fThetaMin << " - " << fThetaMax<< "\n";
      std::cout << "      Phi: " << fPhiMin << " - " << fPhiMax<< "\n";
   }

   void SetThetaMax(double val) { 
     fThetaMax = val;
     varTheta->SetVariableMaxima(val);
   }
   void SetThetaMin(double val) { 
     fThetaMin = val;
     varTheta->SetVariableMinima(val);
   }
   void SetPhiMax(double val) { 
     fPhiMax = val;
     varPhi->SetVariableMaxima(val);
   }
   void SetPhiMin(double val) { 
     fPhiMin = val;
     varPhi->SetVariableMinima(val);
   }
   void SetEnergyMax(double val) { 
     fEnergyMax = val;
     varEnergy->SetVariableMaxima(val);
   }
   void SetEnergyMin(double val) { 
     fEnergyMin = val;
     varEnergy->SetVariableMinima(val);
   }
   BETAG4EventGenerator();

   virtual ~BETAG4EventGenerator() {
      if(fPhaseSpace) delete fPhaseSpace;
   }

   virtual double * GenerateEvent() {
      return(fEventSampler->GenerateEvent());
   }

   virtual void * Refresh() {
      Print();
      if(fDiffXSec) fDiffXSec->Refresh();
      if(fEventSampler)fEventSampler->Refresh(fDiffXSec);
      Print();
   } 


};


class BigcalCenterEventGenerator : public BETAG4EventGenerator   {

public:
   BigcalCenterEventGenerator(){ };
   virtual ~BigcalCenterEventGenerator() {
   }
};



class SANEInclusiveDISEventGenerator : public BETAG4EventGenerator  {
public:
   SANEInclusiveDISEventGenerator() ;

   virtual ~SANEInclusiveDISEventGenerator() {
   }

};



#endif

