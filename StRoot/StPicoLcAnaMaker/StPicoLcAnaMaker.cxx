#include "StPicoLcAnaMaker.h"
//#include "StPicoHFMaker/StHFCuts.h"
#include <iostream>
#include <cmath>

ClassImp(StPicoLcAnaMaker)

using namespace std;

// _________________________________________________________
StPicoLcAnaMaker::StPicoLcAnaMaker(char const* name, StPicoDstMaker* picoMaker, int LcCharge, 
				   char const* outputBaseFileName,
				   char const* inputHFListHFtree) :
  StPicoHFMaker(name, picoMaker, outputBaseFileName, inputHFListHFtree),
  mDecayChannel(kChannel1), mRefmultCorrUtil(NULL), mLambdaCCharge(LcCharge), 
  mOutFileBaseName(outputBaseFileName){

  if (abs(LcCharge) != 1)
  {
    cerr << " StPicoLcAnaMaker::StPicoLcAnaMaker: Unexpected charge of LambdaC. You have entered: " << LcCharge << endl;
    return;
  }
  mPartCharge[StPicoCutsBase::kPion] = LcCharge;
  mPartCharge[StPicoCutsBase::kKaon] = -LcCharge;
  mPartCharge[StPicoCutsBase::kProton] = LcCharge;

  // constructor
}

// _________________________________________________________
StPicoLcAnaMaker::~StPicoLcAnaMaker() {
  // destructor
}

// _________________________________________________________
int StPicoLcAnaMaker::InitHF() {
  // -- INITIALIZE USER HISTOGRAMS ETC HERE -------------------
  //    add them to the output list mOutList which is automatically written

  // cout<<"InitHF1"<<endl;
  mOutFileBaseName = mOutFileBaseName.ReplaceAll(".root", "");
  histoInit(mOutFileBaseName, true); //for createQA()

  // -------------- USER VARIABLES -------------------------

  mRunNumber = 0;
  return kStOK;
}

// _________________________________________________________
void StPicoLcAnaMaker::ClearHF(Option_t *opt="") {
  return;
}

// _________________________________________________________
int StPicoLcAnaMaker::FinishHF() {
  if( isMakerMode() != StPicoHFMaker::kWrite )
    closeFile(); // for QA

  //	cout<<"FinishHF2"<<endl;
  return kStOK;
}

// _________________________________________________________
int StPicoLcAnaMaker::MakeHF() {
  // -- process event
  //    ADD YOUR PROCESSING CODE HERE
  //    ... it is usefull to use the methods below
  //     - createCandidates()
  //     - analyzeCandidates()
//	cout<<"start"<<endl;
  std::clock_t start1 = std::clock();//kvapil
  if (isMakerMode() != StPicoHFMaker::kAnalyze) {
    std::cerr << "wrong version of the code. Please run in the " << StPicoHFMaker::kAnalyze << " mode" << std::endl;
  }
  else if (isMakerMode() == StPicoHFMaker::kAnalyze) {
    //createCandidates();
    //analyzeCandidates();
    createQA();
  }

  return kStOK;
}
//_________________________________________________________
int StPicoLcAnaMaker::createQA(){
  mRefmultCorrUtil->init(mPicoDst->event()->runId());
  if (!mRefmultCorrUtil){
    LOG_WARN << " No mGRefMultCorrUtil! Skip! " << endl;
    return kStWarn;
  }
  if (mRefmultCorrUtil->isBadRun(mPicoDst->event()->runId())) 
    return kStOK;

  mRefmultCorrUtil->initEvent(mPicoDst->event()->grefMult(), mPrimVtx.z(), mPicoDst->event()->ZDCx()) ;

  int const centrality = mRefmultCorrUtil->getCentralityBin9();
  const double reweight = mRefmultCorrUtil->getWeight();
  const double refmultCor = mRefmultCorrUtil->getRefMultCorr();
  //mHists->addCent(refmultCor, centrality, reweight, pVtx.z());
  unsigned int nTracks = mPicoDst->numberOfTracks();

  for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
  {
    StPicoTrack const* trk = mPicoDst->track(iTrack);
    if (!trk) continue;

    StPhysicalHelixD helix = trk->helix(); //SL16d
    helix.moveOrigin(helix.pathLength(mPrimVtx));
    float dca = float(helix.geometricSignedDistance(mPrimVtx));
    StThreeVectorF momentum = trk->gMom(mPrimVtx, mPicoDst->event()->bField());

    // if (!isGoodQaTrack(trk, momentum, dca)) continue; pt, nhits, pseudorap
    if (fabs(dca) < mHFCuts->cutDca()) continue;
    if (!(mHFCuts->hasGoodPtQA(trk))) continue;
    if (!(mHFCuts->hasGoodNHitsFitMinHist(trk))) continue;
    if (!(mHFCuts->hasGoodNHitsFitnHitsMax(trk))) continue; //nHitsFit/nHitsMax
    if (!(mHFCuts->hasGoodEta(momentum))) continue;

    StThreeVectorF dcaPoint = helix.at(helix.pathLength(mPrimVtx.x(), mPrimVtx.y()));
    float dcaZ = dcaPoint.z() - mPrimVtx.z();
    double dcaXy = helix.geometricSignedDistance(mPrimVtx.x(), mPrimVtx.y());

    bool tpcPion = false;
    bool tpcKaon = false;
    bool tpcProton = false;
    const int charge = trk->charge();
    if(mHFCuts->hasGoodNSigmaHist(trk, StPicoCutsBase::kPion) && charge == mPartCharge[StPicoCutsBase::kPion]) tpcPion = true;
    if(mHFCuts->hasGoodNSigmaHist(trk, StPicoCutsBase::kKaon) && charge == mPartCharge[StPicoCutsBase::kKaon]) tpcKaon = true;
    if(mHFCuts->hasGoodNSigmaHist(trk, StPicoCutsBase::kProton) && charge == mPartCharge[StPicoCutsBase::kProton]) tpcProton = true;
    //float hBeta = mHFCuts->getTofBetaBase(trk); //SL16d
    float hBeta = mHFCuts->getTofBetaBase(trk); //SL16j, Vanek
    bool hTofAvailable = !isnan(hBeta) && hBeta > 0;


    bool tofPion = false, tofKaon = false, tofProton = false;
    if(mHFCuts->isTOFHadron(trk, hBeta, StHFCuts::kPion, mPrimVtx))
      tofPion = true;
    if(mHFCuts->isTOFHadron(trk, hBeta, StHFCuts::kKaon, mPrimVtx))
      tofKaon = true;
    if(mHFCuts->isTOFHadron(trk, hBeta, StHFCuts::kProton, mPrimVtx))
      tofProton = true;

    bool goodPion = tpcPion && tofPion; //do not want TOF for HFT matching and resolution determination
    bool goodKaon = tpcKaon && tofKaon;
    bool goodProton = tpcProton && tofProton;

    if (trk  && fabs(dca) < mHFCuts->cutDca() && trk->isHFTTrack() && (goodPion || goodKaon || goodProton)){
      addDcaPtCent(dca, dcaXy, dcaZ, goodPion, goodKaon, goodProton, momentum.perp(), centrality, momentum.pseudoRapidity(), momentum.phi(), mPrimVtx.z()); //add Dca distribution
    }

    if (trk  && fabs(dca) < mHFCuts->cutDca() && (goodPion || goodKaon || goodProton)){
      //std::cout<<"1: "<<goodPion<<" "<< goodKaon<<" "<<  goodProton<<" "<<  momentum.perp()<<" "<<  centrality<<" "<<  momentum.pseudoRapidity()<<" "<<  momentum.phi()<<" "<<  mPrimVtx.z()<<std::endl;
      addTpcDenom1(goodPion, goodKaon, goodProton, momentum.perp(), centrality, momentum.pseudoRapidity(), momentum.phi(), mPrimVtx.z(), hTofAvailable); //Dca cut on 1.5cm, add Tpc Denominator
    }
    //new version, Vanek 03/10/18
    if (trk && fabs(dca) < mHFCuts->cutDca() && trk->isHFTTrack() && (goodPion || goodKaon || goodProton)){
      addHFTNumer1(goodPion, goodKaon, goodProton, momentum.perp(), centrality,  momentum.pseudoRapidity(), momentum.phi(), mPrimVtx.z(), hTofAvailable); //Dca cut on 1.5cm, add HFT Numerator
    }
  } // .. end tracks loop
  return 0;
}

// _________________________________________________________
bool StPicoLcAnaMaker::isHadron(StPicoTrack const * const trk, int pidFlag) const {
  // -- good hadron
  StThreeVectorF t = trk->pMom();
  if (fabs(t.pseudoRapidity()) > 1.) return false; //pridano fabs 1212
  //if (!mHFCuts->isHybridTOFHadron(trk, mHFCuts->getTofBetaBase(trk), StHFCuts::kPion) ) return false; //SL16d
  if (!mHFCuts->isHybridTOFHadron(trk, mHFCuts->getTofBetaBase(trk), pidFlag, mPrimVtx) ) return false; //SL16j, Vanek
  if (!mHFCuts->cutMinDcaToPrimVertex(trk, pidFlag)) return false;
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, pidFlag) && trk->charge() == mPartCharge[pidFlag]);
}

// _________________________________________________________
bool StPicoLcAnaMaker::isPion(StPicoTrack const * const trk) const {
  // -- good pion
  return isHadron(trk, StPicoCutsBase::kPion);
}

// _________________________________________________________
bool StPicoLcAnaMaker::isKaon(StPicoTrack const * const trk) const {
  // -- good kaon
  return isHadron(trk, StPicoCutsBase::kKaon);
}

// _________________________________________________________
bool StPicoLcAnaMaker::isProton(StPicoTrack const * const trk) const {
  // -- good proton
  return isHadron(trk, StPicoCutsBase::kProton);
}

// _________________________________________________________
double StPicoLcAnaMaker::DCA(StPicoTrack const * const trk, StThreeVectorF const & vtx) const {
  // -- particle DCA
  StPhysicalHelixD helix = trk->helix();
  helix.moveOrigin(helix.pathLength(vtx));
  return ((helix.origin() - vtx).mag());
}


//-----------------------------------------------------------------------------

void StPicoLcAnaMaker::histoInit(TString fileBaseName, bool fillQaHists){
  TString m_ParticleName[m_nParticles] = {"Pion", "Kaon", "Proton"};

  float m_EtaEdgeDca[m_nEtasDca+1] = {-1.0, -0.6, -0.2, 0.2, 0.6, 1.0}; //replace bottom!!!
  float m_PhiEdgeDca[m_nPhisDca + 1] = {-3.14159, -2.80359, -2.17527, -1.54696, -0.918637, -0.290319, 0.338, 0.966319, 1.59464, 2.22296, 2.85127, 3.14159};
  float m_VzEdgeDca[m_nVzsDca + 1] = { -6.0, -3.0, 0, 3.0, 6.0};//replace bottom!!!
  float m_CentEdgeDca[m_nCentsDca + 1] = { -0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5};
  float m_PtEdgeDca[m_nPtsDca + 1] = {0.3, 0.4, 0.5, 0.6,  0.7, 0.8, 0.9, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.5, 4.0, 6.0, 12.0};
  float m_EtaEdgeRatio[m_nEtasRatio + 1] = { -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4 , 0.6, 0.8, 1.0}; //replace bottom!!!
  float m_PhiEdgeRatio[m_nPhisRatio + 1] = { -3.14159, -2.80359, -2.17527, -1.54696, -0.918637, -0.290319, 0.338, 0.966319, 1.59464, 2.22296, 2.85127, 3.14159};//replace bottom!!!
  float m_VzEdgeRatio[m_nVzsRatio + 1] = { -6.0, -4.0, -2.0, 0, 2.0, 4.0, 6.0};//replace bottom!!!
  float m_CentEdgeRatio[m_nCentsRatio + 1] = { -1.5, -0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5};
  float m_PtEdgeRatio[m_nPtsRatio + 1] =
  {
    0.3, 0.4, 0.5, 0.6 , 0.7 , 0.8 , 0.9 ,
    1. , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 ,
    2. , 2.2 , 2.4 , 2.6 , 2.8 , 3.0 ,
    3.4 , 3.8 , 4.2 , 4.6 , 5.0 ,  5.5 ,
    6. , 6.5 , 7.0 , 8.0 , 9.0 , 10. , 11,  12.0
  };
  float m_DcaEdgeDca[m_nDcasDca + 1] =
  {
    -1 , -0.96 , -0.92 , -0.88 , -0.84 , -0.8 , -0.76 , -0.72 , -0.68 , -0.64 , -0.6 , -0.56 , -0.52 , -0.48 , -0.44 , -0.4 , -0.36 , -0.32 , -0.28 , -0.24 , -0.2 , -0.16 , -0.12 ,  -0.08,
    -0.078 , -0.075 , -0.072 , -0.069 , -0.066 , -0.063 , -0.06 , -0.057 , -0.054 , -0.051 , -0.048 , -0.045 , -0.042 , -0.039 , -0.036 , -0.033 , -0.03 , -0.027 , -0.024 , -0.021 , -0.018 , -0.015 , -0.012 ,
    -0.01 , -0.0096 , -0.0092 , -0.0088 , -0.0084 , -0.008 , -0.0076 , -0.0072 , -0.0068 , -0.0064 , -0.006 , -0.0056 , -0.0052 , -0.0048 , -0.0044 , -0.004 , -0.0036 , -0.0032 , -0.0028 , -0.0024 , -0.002 , -0.0016 , -0.0012 , -0.0008 , -0.0004 , 0 , 0.0004 , 0.0008 , 0.0012 , 0.0016 , 0.002 , 0.0024 , 0.0028 , 0.0032 , 0.0036 , 0.004 , 0.0044 , 0.0048 , 0.0052 , 0.0056 , 0.006 , 0.0064 , 0.0068 , 0.0072 , 0.0076 , 0.008 , 0.0084 , 0.0088 , 0.0092 , 0.0096 , 0.01 ,
    0.012 , 0.015 , 0.018 , 0.021 , 0.024 , 0.027 , 0.03 , 0.033 , 0.036 , 0.039 , 0.042 , 0.045 , 0.048 , 0.051 , 0.054 , 0.057 , 0.06 , 0.063 , 0.066 , 0.069 , 0.072 , 0.075 , 0.078 ,
    0.08 , 0.12 , 0.16 , 0.2 , 0.24 , 0.28 , 0.32 , 0.36 , 0.4 , 0.44 , 0.48 , 0.52 , 0.56 , 0.6 , 0.64 , 0.68 , 0.72 , 0.76 , 0.8 , 0.84 , 0.88 , 0.92 , 0.96 , 1
  };


  mFillQaHists = fillQaHists;

  TH1::SetDefaultSumw2();
  if (!mFillQaHists) return;

  mh1Cent         = new TH1F("mh1Cent", "EventsVsCentrality;cent;Counts", 10, -1.5, 8.5);
  mh1CentWg         = new TH1F("mh1CentWg", "EventsVsCentrality;cent;Counts", 10, -1.5, 8.5);
  mh1gRefmultCor  = new TH1F("mh1gRefmultCor", "gRefmultCor;gRefmult;Counts", 700, 0, 700);
  mh1gRefmultCorWg  = new TH1F("mh1gRefmultCorWg", "gRefmultCorWg;gRefmultCorWg;Counts", 700, 0, 700);
  mh2CentVz         = new TH2F("mh2CentVz", "CentralityVsVz;cent;Vz", 10, -1.5, 8.5, 200, -10, 10);
  mh2CentVzWg = new TH2F("mh2CentVzWg", "CentralityVsVzWg;cent;Vz", 10, -1.5, 8.5, 200, -10, 10);

  //Add some HFT ratio plots
  mh2Tpc1PtCent  = new TH2F("mh2Tpc1PtCent", "Tpc tacks;p_{T}(GeV/c);cent", 120, 0, 12, 10, -1.5, 8.5); //Dca 1.5cm
  mh2HFT1PtCent  = new TH2F("mh2HFT1PtCent", "HFT tacks;p_{T}(GeV/c);cent", 120, 0, 12, 10, -1.5, 8.5); //Dca 1.5cm
  mh2Tpc1PhiVz  = new TH2F("mh2Tpc1PhiVz", "Tpc tacks;#Phi;Vz", 100, -3.1415, 3.1415, 20, -10, 10); //Dca 1.5cm
  mh2HFT1PhiVz  = new TH2F("mh2HFT1PhiVz", "HFT tacks;#Phi;Vz", 100, -3.1415, 3.1415, 20, -10, 10); //Dca 1.5cm

  for (int iParticle = 0; iParticle < m_nParticles; ++iParticle){
    mh1Tpc1Pt[iParticle] = new TH1D(Form("mh1Tpc1Pt_%d", iParticle), "mh1Tpc1Pt_" + m_ParticleName[iParticle], 120, 0, 12);
    mh1HFT1Pt[iParticle] = new TH1D(Form("mh1HFT1Pt_%d", iParticle), "mh1HFT1Pt_" + m_ParticleName[iParticle], 120, 0, 12);
    mh2TOF1PtCent[iParticle] = new TH2F(Form("mh2TOF1PtCent_%d", iParticle), "mh2TOF1PtCent_" + m_ParticleName[iParticle], 120, 0, 12, 10, -1.5, 8.5);
    mh2TOF1HFTPtCent[iParticle] = new TH2F(Form("mh2TOF1HFTPtCent_%d", iParticle), "mh2TOF1HFTPtCent_" + m_ParticleName[iParticle], 120, 0, 12, 10, -1.5, 8.5);

    for (int iEta = 0; iEta < m_nEtasRatio; ++iEta){
      for (int iVz = 0; iVz < m_nVzsRatio; ++iVz){
	for (int iPhi = 0; iPhi < m_nPhisRatio; ++iPhi){

	  mh2Tpc1PtCentPartEtaVzPhi[iParticle][iEta][iVz][iPhi]  = new TH2F(Form("mh2Tpc1PtCentPartEtaVzPhi_%d_%d_%d_%d", iParticle, iEta, iVz, iPhi), "mh2Tpc1PtCent_"+m_ParticleName[iParticle]+Form("_Eta%2.1f_Vz%2.1f_Phi%2.1f;p_{T}(GeV/c);cent", m_EtaEdgeRatio[iEta], m_VzEdgeRatio[iVz], m_PhiEdgeRatio[iPhi]), m_nPtsRatio, m_PtEdgeRatio, m_nCentsRatio, m_CentEdgeRatio); //Dca 1.cm
	  mh2HFT1PtCentPartEtaVzPhi[iParticle][iEta][iVz][iPhi]  = new TH2F(Form("mh2HFT1PtCentPartEtaVzPhi_%d_%d_%d_%d", iParticle, iEta, iVz, iPhi), "mh2HFT1PtCent_"+m_ParticleName[iParticle]+Form("_Eta%2.1f_Vz%2.1f_Phi%2.1f;p_{T}(GeV/c);cent", m_EtaEdgeRatio[iEta], m_VzEdgeRatio[iVz], m_PhiEdgeRatio[iPhi]), m_nPtsRatio, m_PtEdgeRatio, m_nCentsRatio, m_CentEdgeRatio); //Dca 1.cm
	}
      }
    }
  }

  // Add some Dca, resolution
  for (int iParticle = 0; iParticle < m_nParticles; iParticle++){
    for (int iEta = 0; iEta < m_nEtasDca; iEta++){
      for (int iVz = 0; iVz < m_nVzsDca; iVz++){
	for (int iCent = 0; iCent < m_nCentsDca; iCent++){

	  mh3DcaXyZPtCentPartEtaVzPhi[iParticle][iEta][iVz][iCent]  = new TH3F(Form("mh3DcaXyZPtCentPartEtaVzPhi_%d_%d_%d_%d", iParticle, iEta, iVz, iCent),"mh3DcaXyZPt_"+m_ParticleName[iParticle]+Form("_Eta%2.1f_Vz%2.1f_Cent%2.1f;p_{T}(GeV/c);DcaXy(cm);DcaZ(cm)", m_EtaEdgeDca[iEta], m_VzEdgeDca[iVz], m_CentEdgeDca[iCent]), m_nPtsDca, m_PtEdgeDca, m_nDcasDca, m_DcaEdgeDca, m_nDcasDca, m_DcaEdgeDca); //Dca 1.cm
	}
      }
    }
  }

  mh3DcaPtCent  = new TH3F("mh3DcaPtCent", "mh3DcaPtCent;p_{T}(GeV/c);cent;Dca(cm)", 120, 0, 12, 10, -1.5, 8.5, 1000, -1, 1); //Dca 1.cm
  mh3DcaXyPtCent  = new TH3F("mh3DcaXyPtCent", "mh3DcaXyPtCent;p_{T}(GeV/c);cent;DcaXy(cm)", 120, 0, 12, 10, -1.5, 8.5, 1000, -1, 1); //Dca 1.cm
  mh3DcaZPtCent  = new TH3F("mh3DcaZPtCent", "mh3DcaZPtCent;p_{T}(GeV/c);cent;DcaZ(cm)", 120, 0, 12, 10, -1.5, 8.5, 1000, -1, 1); //Dca 1.cm

}

//-----------------------------------------------------------------------
void StPicoLcAnaMaker::addTpcDenom1(bool IsPion, bool IsKaon, bool IsProton, float pt, int centrality, float Eta, float Phi, float Vz, bool isTOF){
  int EtaIndex = getEtaIndexRatio(Eta);
  int PhiIndex = getPhiIndexRatio(Phi);
  int VzIndex = getVzIndexRatio(Vz);
  if(EtaIndex == -1) return;
  if(PhiIndex == -1) return;
  if(VzIndex == -1) return;
  //std::cout<<"2: "<<IsPion<<" "<<IsKaon<<" "<<IsProton<<" "<<pt<<" "<<centrality<<" "<<Eta<<" "<<Phi<<" "<<Vz<<" "<<EtaIndex<<" "<<PhiIndex<<" "<<VzIndex<<std::endl;

  for (int i = 0; i < m_nParticles; ++i)
  {
    if (i == 0 && !IsPion)
      continue;
    if (i == 1 && !IsKaon)
      continue;
    if ( i== 2 && !IsProton)
      continue;
    if ( i > 2 )
    {
      cerr << "StPicoLcAnaMaker::addTpcDenom1: unexpected PID flag." << endl;
      throw;
    }

    mh1Tpc1Pt[i]->Fill(pt);
    mh2Tpc1PtCentPartEtaVzPhi[i][EtaIndex][VzIndex][PhiIndex]->Fill(pt, centrality);
    mh2Tpc1PtCentParticles[i]->Fill(pt, centrality);
    if(isTOF)
      mh2TOF1PtCent[i]->Fill(pt, centrality);
  }
  mh2Tpc1PtCent->Fill(pt, centrality);
  if (fabs(Eta) < mHFCuts->getEtaMax(StPicoCutsBase::kPion)  && pt > mHFCuts->getPtMin(StPicoCutsBase::kPion)) mh2Tpc1PhiVz->Fill(Phi, Vz); 
  // the eta and pt cuts are the same for all particles so, arbitrarily, pion was chosen
}
//-----------------------------------------------------------------------
void StPicoLcAnaMaker::addHFTNumer1(bool IsPion, bool IsKaon, bool IsProton, float pt, int centrality, float Eta, float Phi, float Vz, bool isTOF){
  int EtaIndex = getEtaIndexRatio(Eta);
  int PhiIndex = getPhiIndexRatio(Phi);
  int VzIndex = getVzIndexRatio(Vz);
  if(EtaIndex == -1) return;
  if(PhiIndex == -1) return;
  if(VzIndex == -1) return;

  // PID loop
  for (int i = 0; i < m_nParticles; ++i)
  {
    if (i == 0 && !IsPion)
      continue;
    if (i == 1 && !IsKaon)
      continue;
    if (i == 2 && !IsProton)
      continue;
    if ( i > 2 )
    {
      cerr << "StPicoLcAnaMaker::addTpcDenom1: unexpected PID flag." << endl;
      throw;
    }

    mh1HFT1Pt[i]->Fill(pt);
    mh2HFT1PtCentPartEtaVzPhi[i][EtaIndex][VzIndex][PhiIndex]->Fill(pt, centrality);
    if(isTOF)
      mh2TOF1HFTPtCent[i]->Fill(pt, centrality);
  }

  mh2HFT1PtCent->Fill(pt, centrality);
  if (fabs(Eta) < mHFCuts->getEtaMax(StPicoCutsBase::kPion)  && pt > mHFCuts->getPtMin(StPicoCutsBase::kPion)) 
    mh2HFT1PhiVz->Fill(Phi, Vz);
  // eta and pt cuts are the same for all particle species so, arbitrarily, pion was chosen
}
//---------------------------------------------------------------------
void StPicoLcAnaMaker::addDcaPtCent(float dca, float dcaXy, float dcaZ, bool IsPion, bool IsKaon, bool IsProton, float pt,  int centrality, float Eta, float Phi, float Vz){
  int EtaIndex = getEtaIndexDca(Eta);
  int VzIndex = getVzIndexDca(Vz);
  if(EtaIndex == -1) return;
  if(VzIndex == -1) return;

  if (centrality < 0) return; // remove bad centrality, only keep 9 centralities
  if (IsPion){
    mh3DcaXyZPtCentPartEtaVzPhi[0][EtaIndex][VzIndex][centrality]->Fill(pt, dcaXy, dcaZ);
  }
  if (IsKaon){
    mh3DcaXyZPtCentPartEtaVzPhi[1][EtaIndex][VzIndex][centrality]->Fill(pt, dcaXy, dcaZ);
  }
  if (IsProton){
    mh3DcaXyZPtCentPartEtaVzPhi[2][EtaIndex][VzIndex][centrality]->Fill(pt, dcaXy, dcaZ);
  }
  mh3DcaPtCent->Fill(pt, centrality, dca);
  mh3DcaXyPtCent->Fill(pt, centrality, dcaXy);
  mh3DcaZPtCent->Fill(pt, centrality, dcaZ);
}
//---------------------------------------------------------------------
int StPicoLcAnaMaker::getEtaIndexDca(float Eta){
  float EtaEdgeDca[m_nEtasDca+1] = {-1.0, -0.6, -0.2, 0.2, 0.6, 1.0};
  for (int i = 0; i < m_nEtasDca; i++){
    if ((Eta >= EtaEdgeDca[i]) && (Eta < EtaEdgeDca[i + 1]))
      return i;
  }
  //std::cout<<"SOMETHING WENT TERRIBRU WONG"<<std::endl;
  //return m_nEtasDca -1;
  return -1;
}

//---------------------------------------------------------------------
int StPicoLcAnaMaker::getVzIndexDca(float Vz){
  float VzEdgeDca[m_nVzsDca + 1] = { -6.0, -3.0, 0, 3.0, 6.0};
  for (int i = 0; i < m_nVzsDca; i++){
    if ((Vz >= VzEdgeDca[i]) && (Vz < VzEdgeDca[i + 1]))
      return i;
  }
  //std::cout<<"SOMETHING WENT TERRIBRU WONG"<<std::endl;
  //return m_nVzsDca - 1;
  return -1;
}
//---------------------------------------------------------------------
int StPicoLcAnaMaker::getEtaIndexRatio(float Eta){
  float EtaEdgeRatio[m_nEtasRatio + 1] = { -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4 , 0.6, 0.8, 1.0};
  for (int i = 0; i < m_nEtasRatio; i++){
    if ((Eta >= EtaEdgeRatio[i]) && (Eta < EtaEdgeRatio[i + 1]))
      return i;
  }
  //std::cout<<"SOMETHING WENT TERRIBRU WONG"<<std::endl;
  //return m_nEtasRatio - 1;
  return -1;
}
//---------------------------------------------------------------------
int StPicoLcAnaMaker::getPhiIndexRatio(float Phi){
  float PhiEdgeRatio[m_nPhisRatio + 1] = { -3.14159, -2.80359, -2.17527, -1.54696, -0.918637, -0.290319, 0.338, 0.966319, 1.59464, 2.22296, 2.85127, 3.14159};
  for (int i = 0; i < m_nPhisRatio; i++){
    if ((Phi >= PhiEdgeRatio[i]) && (Phi < PhiEdgeRatio[i + 1]))
      return i;
  }
  //std::cout<<"SOMETHING WENT TERRIBRU WONG"<<std::endl;
  // return m_nPhisRatio - 1;
  return -1;
}
//---------------------------------------------------------------------
int StPicoLcAnaMaker::getVzIndexRatio(float Vz){
  float VzEdgeRatio[m_nVzsRatio + 1] = { -6.0, -4.0, -2.0, 0, 2.0, 4.0, 6.0};
  for (int i = 0; i < m_nVzsRatio; i++) {
    if ((Vz >= VzEdgeRatio[i]) && (Vz < VzEdgeRatio[i + 1]))
      return i;
  }
  //std::cout<<"SOMETHING WENT TERRIBLY WRONG"<<std::endl;
  // return m_nVzsRatio - 1;
  return -1;
}

void StPicoLcAnaMaker::addCent(const double refmultCor, int centrality, const double reweight, const float vz)
{
  mh1gRefmultCor->Fill(refmultCor);
  mh1gRefmultCorWg->Fill(refmultCor, reweight);
  mh1Cent->Fill(centrality);
  mh1CentWg->Fill(centrality, reweight);
  mh2CentVz->Fill(centrality, vz);
  mh2CentVzWg->Fill(centrality, vz, reweight);
}

//---------------------------------------------------------------------
void StPicoLcAnaMaker::closeFile()
{
  //mOutFile->cd(); //do not want second file this time
//	cout<<"CloseFile1"<<endl;
  mh1Cent->Write();
  mh1CentWg->Write();
  mh1gRefmultCor->Write();
  mh1gRefmultCorWg->Write();
  mh2CentVz->Write();
  mh2CentVzWg->Write();
//	cout<<"CloseFile2"<<endl;
  //HFT ratio QA
  mh2Tpc1PtCent->Write();
  mh2Tpc1PhiVz->Write();
  mh2HFT1PhiVz->Write();
  mh2HFT1PtCent->Write();
//	cout<<"CloseFile3"<<endl;
  //HFT DCA Ratio
  for (int iParticle = 0; iParticle < m_nParticles; iParticle++)
  {
    for (int iEta = 0; iEta < m_nEtasDca; iEta++)
    {
      for (int iVz = 0; iVz < m_nVzsDca; iVz++)
      {
	for (int iCent = 0; iCent < m_nCentsDca; iCent++)
	{
	  mh3DcaXyZPtCentPartEtaVzPhi[iParticle][iEta][iVz][iCent]->Write();
	}
      }
    }
  }
  // std::cout<<"tuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu"<<m_nParticles<<" "<<m_nEtasRatio<<std::endl;
//cout<<"CloseFile4"<<endl;
  for (int iParticle = 0; iParticle < m_nParticles; iParticle++)
  {
    mh1Tpc1Pt[iParticle]->Write();
    mh1HFT1Pt[iParticle]->Write();
    mh2TOF1HFTPtCent[iParticle]->Write();
    mh2TOF1PtCent[iParticle]->Write();
    for (int iEta = 0; iEta < m_nEtasRatio; iEta++)
    {
      for (int iVz = 0; iVz < m_nVzsRatio; iVz++)
      {
	for (int iPhi = 0; iPhi < m_nPhisRatio; iPhi++)
	{
	  mh2Tpc1PtCentPartEtaVzPhi[iParticle][iEta][iVz][iPhi]->Write();
	  mh2HFT1PtCentPartEtaVzPhi[iParticle][iEta][iVz][iPhi]->Write();
	}
      }
    }
  }

  mh3DcaPtCent->Write();
  mh3DcaXyPtCent->Write();
  mh3DcaZPtCent->Write();
}
