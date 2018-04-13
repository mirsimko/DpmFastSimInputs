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
<<<<<<< HEAD
echo installing picoDst
echo '###############################'
git clone git@github.com:star-bnl/star-picoDst.git
pushd star-picoDst >> /dev/null
git checkout Pico16a
popd >> /dev/null
pushd StRoot >> /dev/null
ln -s ../star-picoDst/StPicoDstMaker
popd >> /dev/null

echo
echo '###############################'
=======
>>>>>>> 15cdc30... submission of merging script as a job
echo installing auau200GeVRun14and16
echo '###############################'
git clone git@github.com:mirsimko/auau200GeVRun14and16.git
pushd StRoot >> /dev/null
ln -s ../auau200GeVRun14and16/StRoot/StPicoCutsBase/
ln -s ../auau200GeVRun14and16/StRoot/StPicoPrescales/
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
cp -r /global/project/projectdirs/star/pwg/starhf/simkomir/LambdaC/StRefMultCorr .
pushd StRoot >> /dev/null
ln -s ../StRefMultCorr
popd >> /dev/null


popd >> /dev/null

