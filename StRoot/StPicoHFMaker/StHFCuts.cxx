#include <limits>

#include "StHFCuts.h"

#include "StHFPair.h"
#include "StHFClosePair.h"
#include "StHFTriplet.h"

#include "StPicoDstMaker/StPicoTrack.h" //Vanek

ClassImp(StHFCuts)

// _________________________________________________________
StHFCuts::StHFCuts() : StPicoCutsBase("HFCutsBase"), 
  mNHitsFitMinHist(std::numeric_limits<int>::lowest()),
  mMinNHitsFitNHitsMaxRatio(std::numeric_limits<float>::lowest()),
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
  mMinNHitsFitNHitsMaxRatio(std::numeric_limits<float>::lowest()),
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
bool StHFCuts::isClosePair(StHFClosePair const & pair) const {
  // -- check for a pair which is close in dca w/o mass constraint works with the faster StHFClosePair
  return (pair.dcaDaughters() < mSecondaryPairDcaDaughtersMax);
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
