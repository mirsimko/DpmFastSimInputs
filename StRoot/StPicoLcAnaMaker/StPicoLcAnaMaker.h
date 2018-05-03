#ifndef StPicoLcAnaMaker_h
#define StPicoLcAnaMaker_h

#include "StPicoHFMaker/StPicoHFMaker.h"
#include "TNtuple.h"
#include "StRefMultCorr/StRefMultCorr.h"
#include "TH2F.h"
//#include "StPicoDpmAnaHists.h"
#include <vector>

#include "TClonesArray.h"

#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"

#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"

#include "StPicoHFMaker/StPicoHFEvent.h"
#include "StPicoHFMaker/StHFCuts.h"
#include "StPicoHFMaker/StHFPair.h"
#include "StPicoHFMaker/StHFTriplet.h"
#include "StBTofUtil/tofPathLength.hh"

#include "phys_constants.h"

#include "TH1F.h"
#include "TH3F.h"


#include <ctime>

/* **************************************************
 *  Sample class fo HF picoDST analysis
 * --------------------------------------------------
 * 
 *  For more info look also in the .h files in StPicoHFMaker/
 *     StPicoHFMaker/StPicoHFMaker.h      <-- Base Class for analysis
 *     StPicoHFMaker/StPicoHFEvent.h      <-- Holds candidates for one event (written to Tree)
 *     StPicoHFMaker/StHFCuts.h           <-- Cuts, can be set in run macro
 *     StPicoHFMaker/StHFPair.h           <-- Holds a pair candidate of a two body decay
 *     StPicoHFMaker/StHFTriplet.h        <-- Holds a triplet of a three body decay
 *
 *  Usage:
 *   - Implement
 *        InitHF()
 *        MakeHF()
 *        ClearHF()
 *        FinishHF()
 *
 *  - Do not ovewrite Init, Make, Clear, Finish which are inhertited from StPicoHFMaker via StMaker 

 *  - Set StHFCuts class via setHFBaseCuts(...) in run macro
 *
 *  - Set use mode of StPicoHFMaker class  via setMakerMode(...)
 *     use enum of StPicoHFMaker::eMakerMode
 *      StPicoHFMaker::kAnalyze - don't write candidate trees, just fill histograms
 *      StPicoHFMaker::kWrite   - write candidate trees
 *      StPicoHFMaker::kRead    - read candidate trees and fill histograms
 *
 *  - Set decay mode of analysis via setDecayMode(...)
 *     use enum of StPicoHFEvent::eHFEventMode (see there for more info)
 *      StPicoHFEvent::kTwoParticleDecay,
 *      StPicoHFEvent::kThreeParticleDecay
 *      StPicoHFEvent::kTwoAndTwoParticleDecay
 *
 *  - Implement these track selection methods used to fill vectors for 'good' identified particles
 *      (methods from StHFCuts utility class can/should be used)
 *       isPion
 *       isKaon
 *       isProton
 *
 *  --------------------------------------------------
 *  
 *  Initial Authors:  
 *            Xin Dong        (xdong@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov) 
 * 
 *  ** Code Maintainer
 *
 * **************************************************
 */

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoHFEvent;

class StHFPair;
class StHFTriplet;
class StHFCuts;

class StRefMultCorr;

class StPicoLcAnaMaker : public StPicoHFMaker 
{
public:
  StPicoLcAnaMaker(char const* name, StPicoDstMaker* picoMaker, int LcCharge, 
		   char const* outputBaseFileName = "testPicoLcAnaMaker",  
		   char const* inputHFListHFtree = "");
  virtual ~StPicoLcAnaMaker();

  virtual int InitHF();
  virtual int MakeHF();
  virtual void  ClearHF(Option_t *opt);
  virtual int FinishHF();
  virtual double DCA(StPicoTrack const*, StThreeVectorF const &) const;
  int createQA();

  // -- ADOPT DECAY CHANNELS, if wished ------------------- 
  void setDecayChannel(unsigned int u) { mDecayChannel = u; }

  enum eDecayChannel {kChannel1, kChannel2, kChannel3};

  void setRefMutCorr(StRefMultCorr* gRefMultCorr) { mRefmultCorrUtil = gRefMultCorr; }
  StRefMultCorr* getRefMultCorr() { return mRefmultCorrUtil; }

  //  float getTofBeta(StPicoTrack const*,StThreeVectorF const& vtx) const;

  void histoInit(TString fileBaseName,bool fillQaHists=true);
  void addTpcDenom1(bool IsPion, bool IsKaon, bool IsProton, float pt, int centrality, float Eta, float Phi, float Vz, bool isTOF);
  void addHFTNumer1(bool IsPion, bool IsKaon, bool IsProton, float pt, int centrality, float Eta, float Phi, float Vz, bool isTOF);
  void addDcaPtCent(float dca, float dcaXy, float  dcaZ, bool IsPion, bool IsKaon, bool IsProton, float pt,  int centrality, float Eta, float Phi, float Vz);
  int getEtaIndexDca(float Eta) ;
  int getPhiIndexDca(float Phi) ;
  int getVzIndexDca(float Vz) ;
  int getEtaIndexRatio(float Eta) ;
  int getPhiIndexRatio(float Phi) ;
  int getVzIndexRatio(float Vz) ;
  void addCent(const double refmultCor, int centrality, const double reweight, const float vz);
  void closeFile();

  // virtual float getEta(int index){return m_EtaEdgeDca[index];};


  //  ClassDef(StPicoLcAnaMaker, 1)

protected:
  virtual bool isHadron(StPicoTrack const*, int pidFlag) const;
  virtual bool isPion(StPicoTrack const*) const;
  virtual bool isKaon(StPicoTrack const*) const;
  virtual bool isProton(StPicoTrack const*) const;

private:
  // -- private members --------------------------

  //Cuts consts: trick with enum-----
  enum cutsConsts{
    m_nParticles = 3,
    m_nEtasDca = 5,
    m_nPhisDca = 11,
    m_nVzsDca = 4,
    m_nCentsDca = 9,
    m_nPtsDca = 19,
    m_nEtasRatio = 10,
    m_nPhisRatio = 11,
    m_nVzsRatio = 6,
    m_nCentsRatio = 10,
    m_nPtsRatio = 36,
    m_nDcasDca = 144,
    m_ncharges = 2
  };
  //-----------------------------------

  unsigned int mDecayChannel;

  // -- ADD USER MEMBERS HERE ------------------- 
  //TNtuple *ntp_DMeson; //orig. Kvapil
  TTree *ntp_Dmeson; //Vanek

  StRefMultCorr* mRefmultCorrUtil;
  int mRunNumber;

  int mLambdaCCharge;
  int mPartCharge[StPicoCutsBase::kPicoPIDMax];

  TString mOutFileBaseName;

  bool mFillQaHists;
  TFile* mOutFile;

  TH1F* mh1Cent;
  TH1F* mh1CentWg;
  TH1F* mh1gRefmultCor;
  TH1F* mh1gRefmultCorWg;
  TH2F* mh2CentVz;
  TH2F* mh2CentVzWg;

  //HFT ratio QA
  TH2F* mh2Tpc1PtCent;
  TH2F* mh2Tpc1PhiVz;
  TH1D* mh1Tpc1Pt[m_nParticles];
  TH1D* mh1HFT1Pt[m_nParticles];
  TH2F* mh2TOF1PtCent[m_nParticles];
  TH2F* mh2TOF1HFTPtCent[m_nParticles];
  TH2F* mh2Tpc1PtCentParticles[m_nParticles];
  TH2F* mh2HFT1PtCent;
  TH2F* mh2HFT1PhiVz;
  TH2F* mh2Tpc1PtCentPartEtaVzPhi[m_nParticles][m_nEtasRatio][m_nVzsRatio][m_nPhisRatio];
  TH2F* mh2HFT1PtCentPartEtaVzPhi[m_nParticles][m_nEtasRatio][m_nVzsRatio][m_nPhisRatio];

  //HFT Dca
  // TH3F* mh3DcaXyZPtCentPartEtaVzPhi[m_nParticles][m_nEtasDca][m_nVzsDca][m_nCentsDca];

  TH3F* mh3DcaPtCent;
  TH3F* mh3DcaXyPtCent;
  TH3F* mh3DcaZPtCent;

  //---Variables for TTree---------------------------
  //Pion1
  int pi1_runId, pi1_eventId;
  float pi1_phi, pi1_eta, pi1_pt, pi1_dca, pi1_dedx, pi1_nSigma;
  int pi1_nHitFit, pi1_nHitdedx;
  float pi1_TOFinvbeta, pi1_betaBase;

  //Pion2
  int pi2_runId, pi2_eventId;
  float pi2_phi, pi2_eta, pi2_pt, pi2_dca, pi2_dedx, pi2_nSigma;
  int pi2_nHitFit, pi2_nHitdedx;
  float pi2_TOFinvbeta, pi2_betaBase;

  //Kaon
  int k_runId, k_eventId;
  float k_phi, k_eta, k_pt, k_dca, k_dedx, k_nSigma;
  int k_nHitFit, k_nHitdedx;
  float k_TOFinvbeta, k_betaBase;

  //dca, flag, prim. vertex, charge
  int mcharge;
  float mdcaMax;
  int flag;
  float primVz;

  //D meson
  float D_theta, D_decayL, D_phi, D_eta, D_pt, D_mass, D_dV0Max;

  //centrality, refMult
  float mcentrality, refmult, mrefmultcorr, mreweight;

  //-------------------------------------------------
  // -- ADD USER MEMBERS HERE -------------------

  ClassDef(StPicoLcAnaMaker, 1) //set to 1
};

#endif
