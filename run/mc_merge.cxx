void mergeBETA(int start, int stop) {

gROOT->ProcessLine(".L BETAEvent.C+");
int num;
/*
ifstream in;
//ifstream 
in.open ( "run.txt" );
in >> num ;
in.close();
*/
int i, j,n;
n=stop-start;
int start = start;

//TFile * file = TFile::Open("data/all.root","RECREATE");

TChain chain("Detectors"); // name of tree

BETAEvent * event = new BETAEvent();
chain.SetBranchAddress("aBetaEvent",&event);

for(i=start;i<start+n;i++)
{
  chain.Add(Form("data/beta.run.%d.root",i));
}	

chain.Merge("data/all.root");
gROOT->pwd();
//file.Write(i);
//chain.StartViewer();
gROOT->ProcessLine(".q");
}
