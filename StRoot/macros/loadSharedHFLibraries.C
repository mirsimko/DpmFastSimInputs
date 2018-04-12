#include "TSystem.h"

#ifndef __CINT__
#include "StMuDSTMaker/COMMON/macros/loadSharedLibraries.C"
#endif

extern TSystem* gSystem;

void loadSharedHFLibraries() {

  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();

  gSystem->Load("StBTofUtil");
  gSystem->Load("StPicoDstMaker");
  gSystem->Load("StPicoEvent");
  gSystem->Load("StPicoCutsBase");
  gSystem->Load("StPicoHFMaker");
  gSystem->Load("StRefMultCorr");

  // -->>> ADD your own library/class HERE 
  gSystem->Load( "StPicoLcAnaMaker" );

  cout << " loading of shared HF libraries are done" << endl;
 }
