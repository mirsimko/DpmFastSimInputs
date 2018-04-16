#ifndef StHFCUTS_H
#define StHFCUTS_H

/* **************************************************
 *  Cut class for HF analysis
 *  - Based on PicoCuts class 
 *
 *  Initial Authors:  
 *            Xin Dong        (xdong@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *          **Jochen Thaeder  (jmthader@lbl.gov)   
 *
 *  Contributing Authors
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Guannan Xie     (guannanxie@lbl.gov)
 *            Miroslav Simko  (msimko@bnl.gov)
 *
 *  ** Code Maintainer
 *
 * **************************************************
 */

#include "StPicoCutsBase/StPicoCutsBase.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StHFPair.h"
#include "StHFTriplet.h"

class StHFPair;
class StHFClosePair;
class StHFTriplet;
class StPicoTrack; //Vanek

class StHFCuts : public StPicoCutsBase
{
public:

  StHFCuts();
  StHFCuts(const Char_t *name);
  ~StHFCuts();

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  virtual void init() { initBase(); }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  bool isClosePair(StHFPair const & pair) const;
  bool isClosePair(StHFClosePair const & pair) const;

  bool isGoodSecondaryVertexPair(StHFPair const & pair) const;
  bool isGoodTertiaryVertexPair(StHFPair const & pair) const;
  bool isGoodSecondaryVertexTriplet(StHFTriplet const & triplet) const;

  //---------MY CUTS----------------------------------------------------------

  inline bool hasGoodNHitsFitMinHist(StPicoTrack const *track)	const;
  inline bool hasGoodNHitsFitnHitsMax(StPicoTrack const *track) const;
  inline bool hasGoodEta(StThreeVectorF const & trkMom) const;
  inline bool hasGoodNSigmaHist(StPicoTrack const *track, int hadrFlag) const;
  inline bool hasGoodTripletdV0Max(StHFTriplet const &triplet) const;
  inline bool hasGoodPtQA(StPicoTrack const *track) const;


  //--------------------------------------------------------------------------

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  // -- SETTER for CUTS
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  void setCutSecondaryPair(float dcaDaughtersMax, float decayLengthMin, float decayLengthMax, 
			   float cosThetaMin, float massMin, float massMax); 

  void setCutTertiaryPair(float dcaDaughtersMax, float decayLengthMin, float decayLengthMax, 
			  float cosThetaMin, float massMin, float massMax); 

  void setCutSecondaryTriplet(float dcaDaughters12Max, float dcaDaughters23Max, float dcaDaughters31Max, 
			      float decayLengthMin, float decayLengthMax, 
			      float cosThetaMin, float massMin, float massMax);

  void setCutSecondaryPairDcaToPvMax(float dcaToPvMax){ mSecondaryPairDcaToPvMax = dcaToPvMax; }

  void setCutTertiaryPairDcaToPvMax(float dcaToPvMax) { mTertiaryPairDcaToPvMax = dcaToPvMax; }

  void setCutPtQA(float PtQA) { mPtQA = PtQA; }

  //----MY SETTERS------------------------------------------------------------------------------------------

  void setCutNHitsFitMinHist(int i) {mNHitsFitMinHist = i;}
  void setCutEta(float eta) { mEta = eta; }
  void setCutTPCNSigmaHadronHist(float nSigHadr, int hadrFlag);
  void setCutTripletdV0Max(float dV0MaxSetCut) {mdV0MaxCut = dV0MaxSetCut;}

  void setCutDca(float dca) { mDcaQA = dca; }
  void setCutDcaXy(float dcaXy) { mDcaXyQA = dcaXy; }
  void setCutDcaZ(float dcaZ) { mDcaZQA = dcaZ; }
  //--------------------------------------------------------------------------------------------------------

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  // -- GETTER for single CUTS
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  const float&    cutSecondaryPairDcaDaughtersMax()       const;
  const float&    cutSecondaryPairDecayLengthMin()        const;
  const float&    cutSecondaryPairDecayLengthMax()        const;
  const float&    cutSecondaryPairCosThetaMin()           const;
  const float&    cutSecondaryPairMassMin()               const;
  const float&    cutSecondaryPairMassMax()               const;
  const float&    cutSecondaryPairDcaToPvMax()		  const;

  const float&    cutTertiaryPairDcaDaughtersMax()        const;
  const float&    cutTertiaryPairDecayLengthMin()         const;
  const float&    cutTertiaryPairDecayLengthMax()         const;
  const float&    cutTertiaryPairCosThetaMin()            const;
  const float&    cutTertiaryPairMassMin()                const;
  const float&    cutTertiaryPairMassMax()                const;
  const float&    cutTertiaryPairDcaToPvMax()		  const;

  const float&    cutSecondaryTripletDcaDaughters12Max()  const;
  const float&    cutSecondaryTripletDcaDaughters23Max()  const;
  const float&    cutSecondaryTripletDcaDaughters31Max()  const;
  const float&    cutSecondaryTripletDecayLengthMin()     const;
  const float&    cutSecondaryTripletDecayLengthMax()     const;
  const float&    cutSecondaryTripletCosThetaMin()        const;
  const float&    cutSecondaryTripletMassMin()            const;
  const float&    cutSecondaryTripletMassMax()            const;

  //----MY GETTERS------------------------------------------------------------------------------------------

  const float&    cutEta() const;
  const float&    cutPt() const;

  const float&    cutDca() const;
  const float&    cutDcaXy() const;
  const float&    cutDcaZ() const;
  //--------------------------------------------------------------------------------------------------------

private:

  StHFCuts(StHFCuts const &);       
  StHFCuts& operator=(StHFCuts const &);

  //---MY VARIABLES------------------------

  int mNHitsFitMinHist;
  float mMinNHitsFitNHitsMaxRatio;
  float mEta;

  float mNSigPionHist;
  float mNSigKaonHist;
  float mNSigProtonHist;

  float mdV0MaxCut;

  float mPtQA;

  float mDcaQA;
  float mDcaXyQA;
  float mDcaZQA;

  //---------------------------------------

  // ------------------------------------------
  // -- Pair cuts for secondary pair
  // ------------------------------------------
  float mSecondaryPairDcaDaughtersMax;
  float mSecondaryPairDecayLengthMin; 
  float mSecondaryPairDecayLengthMax; 
  float mSecondaryPairCosThetaMin;
  float mSecondaryPairMassMin;
  float mSecondaryPairMassMax;
  float mSecondaryPairDcaToPvMax;

  // ------------------------------------------
  // -- Pair cuts tertiary pair
  // ------------------------------------------
  float mTertiaryPairDcaDaughtersMax;
  float mTertiaryPairDecayLengthMin; 
  float mTertiaryPairDecayLengthMax; 
  float mTertiaryPairCosThetaMin;
  float mTertiaryPairMassMin;
  float mTertiaryPairMassMax;
  float mTertiaryPairDcaToPvMax;

  // ------------------------------------------
  // -- Cuts of secondary triplet
  // ------------------------------------------
  float mSecondaryTripletDcaDaughters12Max;
  float mSecondaryTripletDcaDaughters23Max;
  float mSecondaryTripletDcaDaughters31Max;
  float mSecondaryTripletDecayLengthMin; 
  float mSecondaryTripletDecayLengthMax; 
  float mSecondaryTripletCosThetaMin;
  float mSecondaryTripletMassMin;
  float mSecondaryTripletMassMax;

  ClassDef(StHFCuts,1)
};

inline void StHFCuts::setCutSecondaryPair(float dcaDaughtersMax, float decayLengthMin, float decayLengthMax, 
					  float cosThetaMin, float massMin, float massMax)  {
  mSecondaryPairDcaDaughtersMax = dcaDaughtersMax;
  mSecondaryPairDecayLengthMin = decayLengthMin; mSecondaryPairDecayLengthMax = decayLengthMax;
  mSecondaryPairCosThetaMin = cosThetaMin;
  mSecondaryPairMassMin = massMin; mSecondaryPairMassMax = massMax; 
}

inline void StHFCuts::setCutTertiaryPair(float dcaDaughtersMax, float decayLengthMin, float decayLengthMax, 
					 float cosThetaMin, float massMin, float massMax)  {
  mTertiaryPairDcaDaughtersMax = dcaDaughtersMax;
  mTertiaryPairDecayLengthMin = decayLengthMin; mTertiaryPairDecayLengthMax = decayLengthMax;
  mTertiaryPairCosThetaMin = cosThetaMin;
  mTertiaryPairMassMin = massMin; mTertiaryPairMassMax = massMax; 
}
  
inline void StHFCuts::setCutSecondaryTriplet(float dcaDaughters12Max, float dcaDaughters23Max, float dcaDaughters31Max, 
					     float decayLengthMin, float decayLengthMax, 
					     float cosThetaMin, float massMin, float massMax)  {
  // setting up the triplet
  mSecondaryTripletDcaDaughters12Max = dcaDaughters12Max; mSecondaryTripletDcaDaughters23Max = dcaDaughters23Max; 
  mSecondaryTripletDcaDaughters31Max = dcaDaughters31Max; 
  mSecondaryTripletDecayLengthMin = decayLengthMin; mSecondaryTripletDecayLengthMax = decayLengthMax; 
  mSecondaryTripletCosThetaMin = cosThetaMin;
  mSecondaryTripletMassMin = massMin; mSecondaryTripletMassMax = massMax; 

  // setting up the doublet
  mSecondaryPairDcaDaughtersMax = mSecondaryTripletDcaDaughters12Max;
  mSecondaryPairDecayLengthMin = mSecondaryTripletDecayLengthMin;
  mSecondaryPairDcaDaughtersMax = mSecondaryTripletDecayLengthMax;
}

// _________________________________________________________
inline void StHFCuts::setCutTPCNSigmaHadronHist(float nSigHadr, int hadrFlag) {
  switch(hadrFlag)
  {
    case StPicoCutsBase::kPion: mNSigPionHist = nSigHadr; break;
    case StPicoCutsBase::kKaon: mNSigKaonHist = nSigHadr; break
    case StPicoCutsBase::kProton: mNSigProtonHist = nSigHadr; break;
    default: std::cerr << "StHFCuts::setCutTPCNSigmaHadronHist: unexpected particle: exitting" << std::endl; return;
  }
}
// _________________________________________________________

inline const float&    StHFCuts::cutSecondaryPairDcaDaughtersMax()       const { return mSecondaryPairDcaDaughtersMax; }
inline const float&    StHFCuts::cutSecondaryPairDecayLengthMin()        const { return mSecondaryPairDecayLengthMin; }
inline const float&    StHFCuts::cutSecondaryPairDecayLengthMax()        const { return mSecondaryPairDecayLengthMax; }
inline const float&    StHFCuts::cutSecondaryPairCosThetaMin()           const { return mSecondaryPairCosThetaMin; }
inline const float&    StHFCuts::cutSecondaryPairMassMin()               const { return mSecondaryPairMassMin; }
inline const float&    StHFCuts::cutSecondaryPairMassMax()               const { return mSecondaryPairMassMax; }
inline const float&    StHFCuts::cutSecondaryPairDcaToPvMax()            const { return mSecondaryPairDcaToPvMax; }

inline const float&    StHFCuts::cutTertiaryPairDcaDaughtersMax()        const { return mTertiaryPairDcaDaughtersMax; }
inline const float&    StHFCuts::cutTertiaryPairDecayLengthMin()         const { return mTertiaryPairDecayLengthMin; }
inline const float&    StHFCuts::cutTertiaryPairDecayLengthMax()         const { return mTertiaryPairDecayLengthMax; }
inline const float&    StHFCuts::cutTertiaryPairCosThetaMin()            const { return mTertiaryPairCosThetaMin; }
inline const float&    StHFCuts::cutTertiaryPairMassMin()                const { return mTertiaryPairMassMin; }
inline const float&    StHFCuts::cutTertiaryPairMassMax()                const { return mTertiaryPairMassMax; }
inline const float&    StHFCuts::cutTertiaryPairDcaToPvMax()             const { return mTertiaryPairDcaToPvMax; }

inline const float&    StHFCuts::cutSecondaryTripletDcaDaughters12Max()  const { return mSecondaryTripletDcaDaughters12Max; }
inline const float&    StHFCuts::cutSecondaryTripletDcaDaughters23Max()  const { return mSecondaryTripletDcaDaughters23Max; }
inline const float&    StHFCuts::cutSecondaryTripletDcaDaughters31Max()  const { return mSecondaryTripletDcaDaughters31Max; }
inline const float&    StHFCuts::cutSecondaryTripletDecayLengthMin()     const { return mSecondaryTripletDecayLengthMin; }
inline const float&    StHFCuts::cutSecondaryTripletDecayLengthMax()     const { return mSecondaryTripletDecayLengthMax; }
inline const float&    StHFCuts::cutSecondaryTripletCosThetaMin()        const { return mSecondaryTripletCosThetaMin; }
inline const float&    StHFCuts::cutSecondaryTripletMassMin()            const { return mSecondaryTripletMassMin; }
inline const float&    StHFCuts::cutSecondaryTripletMassMax()            const { return mSecondaryTripletMassMax; }

inline const float&    StHFCuts::cutEta()				 const { return mEta; }
inline const float&    StHFCuts::cutPt()				 const { return mPtQA; }

inline const float&    StHFCuts::cutDca()				 const { return mDcaQA; }
inline const float&    StHFCuts::cutDcaXy()                              const { return mDcaXyQA; }
inline const float&    StHFCuts::cutDcaZ()                               const { return mDcaZQA; }


// _________________________________________________________
// inline cut functions
inline bool StHFCuts::hasGoodNHitsFitMinHist(StPicoTrack const *track) const {
  return ( track->nHitsFit()>=mNHitsFitMinHist );
}

// _________________________________________________________
inline bool StHFCuts::hasGoodNHitsFitnHitsMax(StPicoTrack const *track) const {
  return ( static_cast<float>(track->nHitsFit()) / static_cast<float>(track->nHitsMax()) ) >= mMinNHitsFitNHitsMaxRatio;
}

// _________________________________________________________
inline bool StHFCuts::hasGoodEta(StThreeVectorF const & trkMom) const {
  return ( fabs(trkMom.pseudoRapidity()) <= mEta );
}

// _________________________________________________________
inline bool StHFCuts::hasGoodNSigmaHist(StPicoTrack const *track, int hadrFlag) const {
  switch(hadrFlag)
  {
    case StPicoCutsBase::kPion: return (fabs(track->nSigmaPion()) < mNSigPionHist );
    case StPicoCutsBase::kKaon: return (fabs(track->nSigmaKaon()) < mNSigKaonHist );
    case StPicoCutsBase::kProton: return (fabs(track->nSigmaProton()) < mNSigProtonHist);
    default: std::cerr << "StHFCuts::hasGoodNSigmaHist: unexpected particle: exitting" << std::endl; return 0;
  }
}

// _________________________________________________________
inline bool StHFCuts::hasGoodTripletdV0Max(StHFTriplet const &triplet) const {
  return( triplet.dV0Max() > mdV0MaxCut );
}

// _________________________________________________________
inline bool StHFCuts::hasGoodPtQA(StPicoTrack const *track) const {
  return ( track->gPt() > mPtQA  );
}


#endif
