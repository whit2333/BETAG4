void BetaAnalysis() {

   gROOT->Reset();

	TFile *outfile = new
   TFile("data/a.BETA.1.root","RECREATE","Example");
//  c1 = new TCanvas("c1","The HSUM example",200,10,600,400);
//  c1->SetGrid();
//   Connect file generated in $ROOTSYS/test
   TFile *  f = new TFile("data/BETA.1.root");
	TFile *hfile = new
   TFile("data/a.BETA.1.root","RECREATE","Example");
	f->ls();
if(f) {
//   Read Tree named "T" in memory. Tree pointer is assigned the same name
   TTree *T = (TTree*)f->Get("Detectors;1");
    BigCalPos = new TH2F("BIGCAL - positions"," ",32,0,32,56,0,56);

//   TBrowser bro;

//   Start main loop on all events
   Double_t calpointer[1024];

   Int_t nevent = T->GetEntries();
   Int_t nb = 0;

//TBranch * branch = T->GetBranch(i)
	Int_t gg = 0;
for(Int_t i = 0; i<nevent; i++) {
   T->SetBranchAddress("BigCaltop", calpointer);
   T->GetEntry(i);


	for(gg =0;gg<720;gg++) {
		BigCalPos->Fill( gg%30 +1 , gg/30 +32, calpointer[gg]);
//		std::cout << "Test   " << gg << " " <<  calpointer[gg]<< std::endl;
			
	}	
   T->SetBranchAddress("BigCalbottom", &calpointer);
   T->GetEntry(i);

	for(gg =0;gg<1024;gg++) {
		BigCalPos->Fill( gg%32 , gg/32, calpointer[gg]);
//		std::cout << "Test   " << gg << " " <<  calpointer[gg]<< std::endl;	
	}
}	

/*
//  Stop timer and print results
   timer.Stop();
   Float_t mbytes = 0.000001*nb;
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
   printf("You read %f Mbytes/Realtime seconds\n",mbytes/rtime);
   printf("You read %f Mbytes/Cputime seconds\n",mbytes/ctime);
   printf("%d events and %d bytes read.\n",nevent,nb);
*/

   if(T) T->StartViewer();
   c1->Update();
//   T->StartViewer();
}

}
