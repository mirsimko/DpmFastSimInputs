#include <limits>

#include "StHFCuts.h"

#include "StHFPair.h"
#include "StHFTriplet.h"

#include "StPicoEvent/StPicoTrack.h" //Vanek

ClassImp(StHFCuts)

// _________________________________________________________
StHFCuts::StHFCuts() : StPicoCutsBase("HFCutsBase"), 
  mNHitsFitMinHist(std::numeric_limits<int>::lowest()),
  mEta(std::numeric_limits<float>::max()),
  mNSigPionHist(std::numeric_limits<float>::max()),
  mNSigKaonHist(std::numeric_limits<float>::max()),
  mNSigProtonHist(std::numeric_limits<float>::max()),
  mdV0MaxCut(std::numeric_limits<float>::max()),
  mPtQA(std::numeric_limits<float>::lowest()),
  mDcaQA(std::numeric_limits<float>::lowest()),
  mDcaXyQA(std::numeric_limits<float>::lowest()),
  mDcaZQA(std::numeric_limits<float>::lowest()),

  mSecondaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mSecondaryPairDecayLengthMin(std::numeric_limits<float>::lowest()), mSecondaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryPairCosThetaMin(std::numeric_limits<float>::lowest()), 
  mSecondaryPairMassMin(std::numeric_limits<float>::lowest()), mSecondaryPairMassMax(std::numeric_limits<float>::max()), 
  mSecondaryPairDcaToPvMax(std::numeric_limits<float>::max()),

  mTertiaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mTertiaryPairDecayLengthMin(std::numeric_limits<float>::lowest()), mTertiaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mTertiaryPairCosThetaMin(std::numeric_limits<float>::lowest()), 
  mTertiaryPairMassMin(std::numeric_limits<float>::lowest()), mTertiaryPairMassMax(std::numeric_limits<float>::max()), 
  mTertiaryPairDcaToPvMax(std::numeric_limits<float>::max()),

  mSecondaryTripletDcaDaughters12Max(std::numeric_limits<float>::max()), mSecondaryTripletDcaDaughters23Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDcaDaughters31Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDecayLengthMin(std::numeric_limits<float>::lowest()), mSecondaryTripletDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryTripletCosThetaMin(std::numeric_limits<float>::lowest()), 
  mSecondaryTripletMassMin(std::numeric_limits<float>::lowest()), mSecondaryTripletMassMax(std::numeric_limits<float>::max()) {
    // -- default constructor
}

// _________________________________________________________
StHFCuts::StHFCuts(const Char_t *name) : StPicoCutsBase(name), 
  mNHitsFitMinHist(std::numeric_limits<int>::lowest()),
  mEta(std::numeric_limits<float>::max()),
  mNSigPionHist(std::numeric_limits<float>::max()),
  mNSigKaonHist(std::numeric_limits<float>::max()),
  mNSigProtonHist(std::numeric_limits<float>::max()),
  mdV0MaxCut(std::numeric_limits<float>::max()),
  mPtQA(std::numeric_limits<float>::lowest()),
  mDcaQA(std::numeric_limits<float>::lowest()),
  mDcaXyQA(std::numeric_limits<float>::lowest()),
  mDcaZQA(std::numeric_limits<float>::lowest()),

  mSecondaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mSecondaryPairDecayLengthMin(std::numeric_limits<float>::lowest()), mSecondaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryPairCosThetaMin(std::numeric_limits<float>::lowest()), 
  mSecondaryPairMassMin(std::numeric_limits<float>::lowest()), mSecondaryPairMassMax(std::numeric_limits<float>::max()), 
  mSecondaryPairDcaToPvMax(std::numeric_limits<float>::max()),

  mTertiaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mTertiaryPairDecayLengthMin(std::numeric_limits<float>::lowest()), mTertiaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mTertiaryPairCosThetaMin(std::numeric_limits<float>::lowest()), 
  mTertiaryPairMassMin(std::numeric_limits<float>::lowest()), mTertiaryPairMassMax(std::numeric_limits<float>::max()), 
  mTertiaryPairDcaToPvMax(std::numeric_limits<float>::max()),

  mSecondaryTripletDcaDaughters12Max(std::numeric_limits<float>::max()), mSecondaryTripletDcaDaughters23Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDcaDaughters31Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDecayLengthMin(std::numeric_limits<float>::lowest()), mSecondaryTripletDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryTripletCosThetaMin(std::numeric_limits<float>::lowest()), 
  mSecondaryTripletMassMin(std::numeric_limits<float>::lowest()), mSecondaryTripletMassMax(std::numeric_limits<float>::max()) {
  // -- constructor
}

// _________________________________________________________
StHFCuts::~StHFCuts() { 
  // destructor
}

// =======================================================================

// _________________________________________________________
bool StHFCuts::isClosePair(StHFPair const & pair) const {
  // -- check for a pair which is close in dca w/o mass constraint,
  //    using secondary vertex cuts
  return ( std::cos(pair.pointingAngle()) > mSecondaryPairCosThetaMin &&
	   pair.decayLength() > mSecondaryPairDecayLengthMin && pair.decayLength() < mSecondaryPairDecayLengthMax &&
	   pair.dcaDaughters() < mSecondaryPairDcaDaughtersMax);
}

// _________________________________________________________
bool StHFCuts::isGoodSecondaryVertexPair(StHFPair const & pair) const {
  // -- check for good secondary vertex pair

  return ( pair.m() > mSecondaryPairMassMin && pair.m() < mSecondaryPairMassMax &&
	   std::cos(pair.pointingAngle()) > mSecondaryPairCosThetaMin &&
	   pair.decayLength() > mSecondaryPairDecayLengthMin && pair.decayLength() < mSecondaryPairDecayLengthMax &&
	   pair.dcaDaughters() < mSecondaryPairDcaDaughtersMax &&
	   pair.DcaToPrimaryVertex() < mSecondaryPairDcaToPvMax);
}

// _________________________________________________________
bool StHFCuts::isGoodTertiaryVertexPair(StHFPair const & pair) const {
  // -- check for good tertiary vertex pair

  return ( pair.m() > mTertiaryPairMassMin && pair.m() < mTertiaryPairMassMax &&
	   std::cos(pair.pointingAngle()) > mTertiaryPairCosThetaMin &&
	   pair.decayLength() > mTertiaryPairDecayLengthMin && pair.decayLength() < mTertiaryPairDecayLengthMax &&
	   pair.dcaDaughters() < mTertiaryPairDcaDaughtersMax &&
	   pair.DcaToPrimaryVertex() < mTertiaryPairDcaToPvMax);
}

// _________________________________________________________
bool StHFCuts::isGoodSecondaryVertexTriplet(StHFTriplet const & triplet) const {
  // -- check for good secondary vertex triplet

  return ( triplet.m() > mSecondaryTripletMassMin && triplet.m() < mSecondaryTripletMassMax &&
	   std::cos(triplet.pointingAngle()) > mSecondaryTripletCosThetaMin &&
	   triplet.decayLength() > mSecondaryTripletDecayLengthMin && triplet.decayLength() < mSecondaryTripletDecayLengthMax &&
	   triplet.dcaDaughters12() < mSecondaryTripletDcaDaughters12Max &&
	   triplet.dcaDaughters23() < mSecondaryTripletDcaDaughters23Max &&
	   triplet.dcaDaughters31() < mSecondaryTripletDcaDaughters31Max);
}

// _________________________________________________________
inline bool StHFCuts::hasGoodNHitsFitMinHist(StPicoTrack const *track) const {
  return ( track->nHitsFit()>=mNHitsFitMinHist );
}

// _________________________________________________________
inline bool StHFCuts::hasGoodEta(StThreeVectorF const & trkMom) const {
  return ( fabs(trkMom.pseudoRapidity()) <= mEta );
}

// _________________________________________________________
inline void StHFCuts::setCutTPCNSigmaHadronHist(float nSigHadr, int hadrFlag) {
  // msimko: "awefull needs changing"
  switch(hadrFlag)
  {
    case StPicoCutsBase::kPion: mNSigPionHist = nSigHadr;
    case StPicoCutsBase::kKaon: mNSigKaonHist = nSigHadr;
    case StPicoCutsBase::kProton: mNSigProtonHist = nSigHadr;
    default: std::cerr << "StHFCuts::setCutTPCNSigmaHadronHist: unexpected particle: exitting" << std::endl; return;
  }


}

// _________________________________________________________
bool StHFCuts::hasGoodNSigmaHist(StPicoTrack const *track, int hadrFlag) const {
  switch(hadrFlag)
  {
    case StPicoCutsBase::kPion: return (fabs(track->nSigmaPion()) < mNSigPionHist );
    case StPicoCutsBase::kKaon: return (fabs(track->nSigmaKaon()) < mNSigKaonHist );
    case StPicoCutsBase::kProton: return (fabs(track->nSigmaProton()) < mNSigProtonHist);
    default: std::cerr << "StHFCuts::hasGoodNSigmaHist: unexpected particle: exitting" << std::endl; return 0;
  }

}

// _________________________________________________________
bool StHFCuts::hasGoodTripletdV0Max(StHFTriplet const &triplet) const {
  return( triplet.dV0Max() > mdV0MaxCut );
}

// _________________________________________________________
bool StHFCuts::hasGoodPtQA(StPicoTrack const *track) const {
  return ( track->gPt() > mPtQA  );
}

