{
   gROOT->Reset();
   TFile * rootfile = new TFile("data/beta.run.4416.root","READ","Simulation data");
   TTree * T = (TTree*)rootfile.Get("Detectors");
   
TCut pmt3("BETAPE[2]>1");

   T->Draw("BETAPE[2]", pmt3);

T->StartViewer();
}
