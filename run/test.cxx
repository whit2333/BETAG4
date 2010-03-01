{
// Add the top folder of my hierary to //root
TFolder *aliroot=gROOT->GetRootFolder()->AddFolder("aliroot",
"aliroot top level folders");
// Add the hierarchy to the list of browsables
gROOT->GetListOfBrowsables()->Add(aliroot,"aliroot");
// Create and add the constants folder
TFolder *constants=aliroot->AddFolder("Constants","Detector constants");
// Create and add the pdg folder to pdg
TFolder *pdg = constants->AddFolder("DatabasePDG","PDG database");
// Create and add the run folder
TFolder *run = aliroot->AddFolder("Run","Run dependent folders");
// Create and add the configuration folder to run
TFolder *configuration = run->AddFolder("Configuration","Run configuration");
// Create and add the run_mc folder
TFolder *run_mc = aliroot->AddFolder("RunMC","MonteCarlo run dependent folders");
// Create and add the configuration_mc folder to run_mc
TFolder *configuration_mc = run_mc->AddFolder("Configuration",
"MonteCarlo run configuration");
}
