#include "SANEEventGenerators.hh"

/** Base Class for GEANT4 - InSANE interfacesd 
 *
 */
BETAG4EventGenerator::BETAG4EventGenerator() {
      fNumberOfGeneratedParticles = 1;
      fEnergyMin = 0.80;
      fEnergyMax = 4.7;
      fThetaMin  = 25.0;
      fThetaMax  = 55.0;
      fPhiMin    =-90.0;
      fPhiMax    = 90.0;
      fPhaseSpace = new InSANEInclusivePhaseSpace();

      // Add random variables ( 3 needed) to phase space
      varEnergy = new InSANEPhaseSpaceVariable();
      varEnergy->fVariableName="Energy"; 
      varEnergy->fVariable="E#prime"; // ROOT string latex
      varEnergy->SetVariableMinima(fEnergyMin); //GeV
      varEnergy->SetVariableMaxima(fEnergyMax); //GeV
      fPhaseSpace->AddVariable(varEnergy);

      varTheta = new InSANEPhaseSpaceVariable();
      varTheta->fVariableName="theta"; 
      varTheta->fVariable="#theta"; // ROOT string latex
      varTheta->SetVariableMinima(fThetaMin*TMath::Pi()/180.0); //
      varTheta->SetVariableMaxima(fThetaMax*TMath::Pi()/180.0); //
      fPhaseSpace->AddVariable(varTheta);

      varPhi = new InSANEPhaseSpaceVariable();
      varPhi->fVariableName="phi"; 
      varPhi->fVariable="#phi"; // ROOT string latex
      varPhi->SetVariableMinima(fPhiMin*TMath::Pi()/180.0); //
      varPhi->SetVariableMaxima(fPhiMax*TMath::Pi()/180.0); //
      fPhaseSpace->AddVariable(varPhi);

      // Create the differential cross section to be used
      fDiffXSec = new InSANEFlatInclusiveDiffXSec();
      // Set the cross section's phase space
      fDiffXSec->SetPhaseSpace(fPhaseSpace);
      // Create event sampler
      fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
      // Set the seed 
      fEventSampler->fRandomNumberGenerator->SetSeed((int)(G4UniformRand()*999999));

      Print();

}


SANEInclusiveDISEventGenerator::SANEInclusiveDISEventGenerator() {
//       fEnergyMin = 0.80;
//       fEnergyMax = 4.7;
//       fThetaMin  = 25.0;
//       fThetaMax  = 55.0;
//       fPhiMin    =-90.0;
//       fPhiMax    = 90.0;
//       fPhaseSpace = new InSANEInclusivePhaseSpace();
// 
//       // Add random variables ( 3 needed) to phase space
//       varEnergy = new InSANEPhaseSpaceVariable();
//       varEnergy->fVariableName="Energy"; 
//       varEnergy->fVariable="E#prime"; // ROOT string latex
//       varEnergy->SetVariableMinima(fEnergyMin); //GeV
//       varEnergy->SetVariableMaxima(fEnergyMax); //GeV
//       fPhaseSpace->AddVariable(varEnergy);
// 
//       varTheta = new InSANEPhaseSpaceVariable();
//       varTheta->fVariableName="theta"; 
//       varTheta->fVariable="#theta"; // ROOT string latex
//       varTheta->SetVariableMinima(fThetaMin*TMath::Pi()/180.0); //
//       varTheta->SetVariableMaxima(fThetaMax*TMath::Pi()/180.0); //
//       fPhaseSpace->AddVariable(varTheta);
// 
//       varPhi = new InSANEPhaseSpaceVariable();
//       varPhi->fVariableName="phi"; 
//       varPhi->fVariable="#phi"; // ROOT string latex
//       varPhi->SetVariableMinima(fPhiMin*TMath::Pi()/180.0); //
//       varPhi->SetVariableMaxima(fPhiMax*TMath::Pi()/180.0); //
//       fPhaseSpace->AddVariable(varPhi);
// 
// 
//       // Create the differential cross section to be used
//       fDiffXSec = new InSANEFlatInclusiveDiffXSec();
//       // Set the cross section's phase space
//       fDiffXSec->SetPhaseSpace(fPhaseSpace);
//       // Create event sampler
//       fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
//       // Set the seed 
//       fEventSampler->fRandomNumberGenerator->SetSeed((int)(G4UniformRand()*999999));
}

