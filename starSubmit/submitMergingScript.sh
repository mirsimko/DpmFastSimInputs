#!/bin/bash

directoryToMerge="$1"

# -- change for your folder
baseFolder=/gpfs/mnt/gpfs01/star/pwg/msimko/LambdaC/LcFastSimInputs

# ###############################################
# -- Do not change below
# ###############################################

if [ -d "$directoryToMerge" ]; then
  pushd "$directoryToMerge" >> /dev/null
  # we want the physical address of the directory to merge
  directoryToMerge=`pwd -P`
  popd >> /dev/null
else
  echo The argument must be a directory
  exit 1
fi

productionId=`date +%F_%H-%M`

mkdir ${baseFolder}/jobs/merge${productionId}
mkdir ${baseFolder}/production/merge${productionId}

pushd ${baseFolder}/production/merge${productionId} >> /dev/null
mkdir report err log list csh

# -- check for prerequisits and copy folders
set folders="starSubmit"

echo "Checking prerequisits folders ...  "
for folder in $folders ; do
  if [ ! -d ${baseFolder}/${folder} ]; then
    echo "${folder} does not exist in ${baseFolder}"
    exit 1
  else
    cp -rfL ${baseFolder}/${folder} .
  fi
done
echo "ok"

# -- check and copy the run macro
runMacro="mergeHere.sh"

echo "Checking the run macro ...  "
if [ ! -f ${baseFolder}/${runMacro} ]; then
  echo "${runMacro} does not exist in ${baseFolder}"
  exit 1
else
  cp -rfL ${baseFolder}/${runMacro} .
fi
echo "ok"

# -- check and copy the xml file
xmlFile="starSubmit/submitMergingScript.xml"

echo "Checking the xml file ...  "
if [ ! -f ${baseFolder}/${xmlFile} ]; then
  echo "${xmlFile} does not exist in ${baseFolder}"
  exit 1
else
  cp -rfL ${baseFolder}/${xmlFile} .
fi
echo "ok"

# -----------------------------------------------

if [ -e LocalLibraries.zip ]; then
  rm LocalLibraries.zip
fi 

if [ -d LocalLibraries.package ]; then
  rm -rf LocalLibraries.package
fi

star-submit-template -template "$xmlFile" -entities basePath="$baseFolder",prodId="$productionId",dirToMerge="$directoryToMerge",runMacro="$runMacro"

popd >> /dev/null
