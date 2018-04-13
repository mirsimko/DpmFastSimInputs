#!/bin/bash

pushd .. >> /dev/null
echo '###############################'
echo Working in $PWD
echo '###############################'
mkdir StRoot
pushd StRoot >> /dev/null
ln -s ../LcFastSimInputs/StRoot/StPicoLcAnaMaker
ln -s ../LcFastSimInputs/StRoot/macros
popd >> /dev/null

echo
echo '###############################'
echo installing auau200GeVRun14and16
echo '###############################'
git clone git@github.com:mirsimko/auau200GeVRun14and16.git
pushd auau200GeVRun14and16 >> /dev/null
git checkout -t origin/run16   # creates branch run16 and starts following branch run16 in origin
popd >> /dev/null
pushd StRoot >> /dev/null
ln -s ../auau200GeVRun14and16/StRoot/StPicoCutsBase/
mkdir StPicoHFMaker
pushd StPicoHFMaker >> /dev/null
StPicoHFMakerDir="../../auau200GeVRun14and16/StRoot/StPicoHFMaker/"
for codeFile in "$StPicoHFMakerDir"*.cxx "$StPicoHFMakerDir"*.h; do
  ln -s "$codeFile" 
done
rm StHFCuts.cxx StHFCuts.h    # these will come from elsewhere
StHFCutsDir="../../LcFastSimInputs/StRoot/StPicoHFMaker/"
ln -s "$StHFCutsDir"StHFCuts.cxx
ln -s "$StHFCutsDir"StHFCuts.h
popd >> /dev/null
popd >> /dev/null

echo
echo '###############################'
echo installing refMultCorr
echo '###############################'
cp -r /gpfs/mnt/gpfs01/star/pwg/msimko/LambdaC/run16/StRefMultCorr
pushd StRoot >> /dev/null
ln -s ../StRefMultCorr
popd >> /dev/null


popd >> /dev/null

