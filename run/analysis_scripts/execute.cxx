{
TFile * f = new TFile("../data/beta.run.23863.root");
TTree * tree = (TTree * ) gROOT->FindObject("Detectors");
//tree->MakeSelector("dump"); // Change this after executing so that you don't accidentally kill your work !
tree->Process("DQ1.C+");

}


