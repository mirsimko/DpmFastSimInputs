
/* **************************************************
 *   Run StPicoHFMyAnaMaker in different modes
 * --------------------------------------------------
 * run as :
 *  root -l -b -q StRoot/macros/loadSharedHFLibraries.C StRoot/macros/runPicoHFMyAnaMaker.C++
 *   or
 *  root -l -b -q StRoot/macros/runPicoHFMyAnaMaker.C
 *
 * --------------------------------------------------
 *  - Different modes to use the  class
 *    - StPicoHFMaker::kAnalyze - don't write candidate trees, just fill histograms
 *        inputFile : fileList of PicoDst files or single picoDst file
 *        outputFile: baseName for outfile
 *    - StPicoHFMaker::kWrite   - write candidate trees
 *        inputFile : path to single picoDist file
 *        outputFile: baseName for outfile
 *    - StPicoHFMaker::kRead    - read candidate trees and fill histograms
 *        inputFile : fileList of PicoDst files
 *        outputFile: baseName for outfile
 *
 * --------------------------------------------------
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)
 *
 * **************************************************
 */

#ifndef __CINT__
#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"

#include "StMaker.h"
#include "StChain.h"

#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoHFMaker/StPicoHFEvent.h"
#include "StPicoHFMaker/StHFCuts.h"

#include "macros/loadSharedHFLibraries.C"

#include <iostream>
#include <ctime>
#include <cstdio>

#include "StPicoLcAnaMaker/StPicoLcAnaMaker.h" //kvapil

#include "StRefMultCorr/StRefMultCorr.h"
#include "StRefMultCorr/CentralityMaker.h"

using namespace std;

#else
class StChain;
#endif

StChain *chain;

void runPicoLcAnaMaker(const Char_t *inputFile="test.list", const Char_t *outputFile="outputBaseName",
		       const unsigned int makerMode = 0 /*kAnalyze*/,
		       const Char_t *badRunListFileName = "picoLists/picoList_bad_MB.list", const Char_t *treeName = "picoHFtree",
		       const Char_t *productionBasePath = "/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/physics2/P16id",
		       const unsigned int decayChannel = 0 /* kChannel0 */,
		       const int LcCharge = 1) {
  // -- Check STAR Library. Please set SL_version to the original star library used in the production
  //    from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL16d";
  string env_SL = getenv ("STAR");
  if (env_SL.find(SL_version)==string::npos) {
      cout<<"Environment Star Library does not match the requested library in runPicoHFMyAnaMaker.C. Exiting..."<<endl;
      exit(1);
  }

  //Int_t nEvents = 10000000;
  //Int_t nEvents = 1000;

#ifdef __CINT__
  gROOT->LoadMacro("loadSharedHFLibraries.C");
  loadSharedHFLibraries();
#endif

  const long numberOfEventsToRun = 1e9;
  if (numberOfEventsToRun < 1e7)
    cerr << "Warning: number of events < 1e7" << endl;

  chain = new StChain();

  // ========================================================================================
  //makerMode    = StPicoHFMaker::kAnalyze;
  // ========================================================================================

  cout << "Maker Mode    " << makerMode << endl;
  cout << "TreeName      " << treeName << endl;
  cout << "Decay Channel " << decayChannel << endl;

  TString sInputFile(inputFile);
  cout << "sInputFile: " << inputFile << endl;
  TString sInputListHF("");
  TString sProductionBasePath(productionBasePath);
  TString sTreeName(treeName);

  if (makerMode == StPicoHFMaker::kAnalyze) {
    if (!sInputFile.Contains(".list") && !sInputFile.Contains("picoDst.root")) {
      cout << "No input list or picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFMaker::kWrite) {
    if (!sInputFile.Contains("picoDst.root")) {
      cout << "No input picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFMaker::kRead) {
    if (!sInputFile.Contains(".list")) {
      cout << "No input list provided! Exiting..." << endl;
      exit(1);
    }

    // -- prepare filelist for picoDst from trees
    sInputListHF = sInputFile;
    sInputFile = "tmpPico.list";

    TString command = "sed 's|" + sTreeName + ".root|picoDst.root|g' " + sInputListHF + " > " + sInputFile;
    cout << "COMMAND : " << command << endl;
    gSystem->Exec(command.Data());

    command = "sed -i 's|^.*" + sTreeName + "|" + sProductionBasePath + "|g' " + sInputFile; // + " > " + sInputFile;
    cout << "COMMAND : " << command << endl;
    gSystem->Exec(command.Data());
  }
  else {
    cout << "Unknown makerMode! Exiting..." << endl;
    exit(1);
  }

  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0, sInputFile, "picoDstMaker");

  cout << "Starting: StPicoLcAnaMaker(\"picoLcAnaMaker\", picoDstMaker, " << LcCharge << ", " << outputFile << ", " << sInputListHF.Data() << ")" << endl;
  StPicoLcAnaMaker* picoLcAnaMaker = new StPicoLcAnaMaker("picoLcAnaMaker", picoDstMaker, LcCharge, outputFile, sInputListHF);
  picoLcAnaMaker->setMakerMode(makerMode);
  picoLcAnaMaker->setDecayChannel(StPicoLcAnaMaker::kChannel1);//kvapil
  picoLcAnaMaker->setTreeName(treeName);
  //picoLcAnaMaker->setMcMode(mcMode); commented kvapil

  StHFCuts* hfCuts = new StHFCuts("hfBaseCuts");
  picoLcAnaMaker->setHFBaseCuts(hfCuts);

  // ---------------------------------------------------
  // -- Set Base cuts for HF analysis

  // -- File name of bad run list
  hfCuts->setBadRunListFileName(badRunListFileName);

  // -- ADD USER CUTS HERE ----------------------------

  hfCuts->setCutVzMax(6.);
  hfCuts->setCutVzVpdVzMax(3.);
/* SL16d triggers */
  hfCuts->addTriggerId(450050);    // vpdmb-5-p-nobsmd-hlt
  hfCuts->addTriggerId(450060);    // vpdmb-5-p-nobsmd-hlt
  hfCuts->addTriggerId(450005);    // vpdmb-5-p-nobsmd
  hfCuts->addTriggerId(450015);    // vpdmb-5-p-nobsmd
  hfCuts->addTriggerId(450025);    // vpdmb-5-p-nobsmd

  hfCuts->setCutNHitsFitMin(15); //kvapil 20 to 15, for candidates
  hfCuts->setCutNHitsFitMinHist(20); //for histograms, Vanek
  hfCuts->setCutRequireHFT(true);

  hfCuts->setCutDca(1.5); //for QA, see createQA() in StPicoLcAnaMaker.cxx
  hfCuts->setCutDcaXy(1.); //used in Kvapil's version, not used in Vanek's version
  hfCuts->setCutDcaZ(1.);

  hfCuts->setCutDcaMin(0.009,StHFCuts::kPion); //federic 1aug2016
  //hfCuts->setCutDcaMin(0.01,StHFCuts::kKaon); //federic 1aug2016
  hfCuts->setCutDcaMin(0.007,StHFCuts::kKaon); //federic 3aug2016

  hfCuts->setCutNHitsFitnHitsMax(0.52);

  // ---------------------------------------------------

  // -- Channel0
  //picoHFMyAnaMaker->setDecayMode(StPicoHFEvent::kTwoParticleDecay);
  picoLcAnaMaker->setDecayMode(StPicoHFEvent::kThreeParticleDecay); //kvapil

  // -- ADD USER CUTS HERE ----------------------------

  hfCuts->setCutEta(1.);

  hfCuts->setCutTripletdV0Max(0.022);

  //-----------SECONDARY TRIPLET CUTS----------------------------
  float dcaDaughters12Max, dcaDaughters23Max, dcaDaughters31Max;
  float decayLengthMin, decayLengthMax;
  float cosThetaMin, massMin, massMax;

  dcaDaughters12Max = 0.009;
  dcaDaughters23Max = 0.009;
  dcaDaughters31Max = 0.009;

  decayLengthMin = 0.003;
  decayLengthMax = 0.2;

  cosThetaMin = 0.997;
  massMin = 1.7;
  massMax = 2.1;

  hfCuts->setCutSecondaryTriplet(dcaDaughters12Max, dcaDaughters23Max, dcaDaughters31Max,
				 decayLengthMin, decayLengthMax,
				 cosThetaMin, massMin, massMax);
  // --- Lomnitz cuts to remove noise from ghosting

  //Single track pt
  hfCuts->setCutPtRange(0.3,50.0,StHFCuts::kPion); //used in candidates analysis
  hfCuts->setCutPtRange(0.3,50.0,StHFCuts::kKaon); //changed to 0.3
  hfCuts->setCutPtRange(0.3,50.0,StHFCuts::kProton); //changed to 0.3

  hfCuts->setCutPtQA(0.3); //p_T used in createQA() in StPicoLcAnaMaker.cxx
  //TPC setters
  hfCuts->setCutTPCNSigmaPion(1.0); //possibly close as possible, e.g. 1.0
  hfCuts->setCutTPCNSigmaKaon(1.0); //all changed to 1.0
  hfCuts->setCutTPCNSigmaProton(1.0); //for QA and analysis

  hfCuts->setCutTPCNSigmaHadronHist(1.0, StHFCuts::kPion); //1 = pion, not used for QA
  hfCuts->setCutTPCNSigmaHadronHist(1.0, StHFCuts::kKaon); //2 = kaon
  hfCuts->setCutTPCNSigmaHadronHist(1.0, StHFCuts::kProton); //2 = kaon
  //TOF setters, need to set pt range as well
  hfCuts->setCutTOFDeltaOneOverBeta(0.02, StHFCuts::kKaon); //changed to 0.03 (same cut as used in analysis)
  hfCuts->setCutPtotRangeHybridTOF(0.3,50.0,StHFCuts::kKaon); //changed lower boundary from 0.5 to 0.3 (same cut as used in analysis)
  hfCuts->setCutTOFDeltaOneOverBeta(0.02, StHFCuts::kPion); //changed to 0.03 (same cut as used in analysis)
  hfCuts->setCutPtotRangeHybridTOF(0.3,50.0,StHFCuts::kPion); //changed lower boundary from 0.5 to 0.3 (same cut as used in analysis)
  hfCuts->setCutTOFDeltaOneOverBeta(0.02, StHFCuts::kProton); //changed to 0.03 (same cut as used in analysis)
  hfCuts->setCutPtotRangeHybridTOF(0.3,50.0,StHFCuts::kProton); //changed lower boundary from 0.5 to 0.3 (same cut as used in analysis)

  // set refmultCorr
  //  cout<<"test"<<endl;
  StRefMultCorr* grefmultCorrUtil = CentralityMaker::instance()->getgRefMultCorr_P16id(); //new StRefMultCorr, info about Run16, SL16j in the same file as for Run14, SL16d
  grefmultCorrUtil->setVzForWeight(6, -6.0, 6.0);
  grefmultCorrUtil->readScaleForWeight("StRoot/StRefMultCorr/macros/weight_grefmult_vpd30_vpd5_Run14.txt");
  picoLcAnaMaker->setRefMutCorr(grefmultCorrUtil);
  //cout<<"test2"<<endl;
  // ========================================================================================

  // ========================================================================================

  //clock_t start = clock(); // getting starting time
  chain->Init();
  cout << "chain->Init();" << endl;
  long nEvents = picoDstMaker->chain()->GetEntries();
  cout << " Total entries = " << nEvents << endl;
  //if(nEvents>total) nEvents = total;

  // -- take the smaller of the user-defined or all available events
  nEvents = nEvents < numberOfEventsToRun ? nEvents : numberOfEventsToRun; 

  for (Int_t i=0; i<nEvents; i++) {
    if(i%1000==0)
      cout << "Working on eventNumber " << i << endl;

    chain->Clear();
    int iret = chain->Make(i);

    if (iret) { cout << "Bad return code!" << iret << endl; break;}

    //total++;
  }

  cout << "****************************************** " << endl;
  cout << "Work done... now its time to close up shop!"<< endl;
  cout << "****************************************** " << endl;
  chain->Finish();
  //double duration = (double) (clock() - start) / (double) CLOCKS_PER_SEC;
  cout << "****************************************** " << endl;
  cout << "total number of events  " << nEvents << endl;
  cout << "****************************************** " << endl;
  // cout << "Time needed " << duration << " s" << endl;
  cout << "****************************************** " << endl;

  delete chain;

  // -- clean up if in read mode
  if (makerMode == StPicoHFMaker::kRead)
    gSystem->Exec(Form("rm -f %s", sInputFile.Data()));
}

