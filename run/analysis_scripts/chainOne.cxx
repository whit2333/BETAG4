{
int i, j;
ifstream  inFile;
inFile.open("flist.temp");
if (!inFile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);   // call system to stop
}
string fname;

TChain chain("h9502"); // name of tree
while(inFile >> fname) {
  cout << fname << "\n";
  chain.Add(fname.c_str());
}

cout << "chain  pointer \n";

}

