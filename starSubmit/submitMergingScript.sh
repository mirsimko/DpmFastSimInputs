#!/bin/bash

<<<<<<< HEAD
toMerge="$1"
=======
directoryToMerge="$1"
>>>>>>> 15cdc30... submission of merging script as a job

# -- change for your folder
baseFolder=/gpfs/mnt/gpfs01/star/pwg/msimko/LambdaC/LcFastSimInputs

# ###############################################
# -- Do not change below
# ###############################################

<<<<<<< HEAD
if [ -d "$toMerge" ]; then
  pushd "$toMerge" >> /dev/null
=======
if [ -d "$directoryToMerge" ]; then
  pushd "$directoryToMerge" >> /dev/null
>>>>>>> 15cdc30... submission of merging script as a job
  # we want the physical address of the directory to merge
  directoryToMerge=`pwd -P`
  popd >> /dev/null
else
  echo The argument must be a directory
  exit 1
fi

<<<<<<< HEAD
# productionId=`date +%F_%H-%M`
productionId=`echo $toMerge | sed 's"/"_"g'`

jobDir=${baseFolder}/jobs/merge${productionId}
mkdir ${baseFolder}/production/merge${productionId}
mkdir "$jobDir"

pushd "$jobDir" >> /dev/null

echo Setting the job in $PWD
mkdir report err log list csh

# -- check for prerequisits and copy folders
folders="starSubmit"

echo "Checking $folders  ...  "
=======
productionId=`date +%F_%H-%M`

mkdir ${baseFolder}/jobs/merge${productionId}
mkdir ${baseFolder}/production/merge${productionId}

pushd ${baseFolder}/production/merge${productionId} >> /dev/null
mkdir report err log list csh

# -- check for prerequisits and copy folders
set folders="starSubmit"

echo "Checking prerequisits folders ...  "
>>>>>>> 15cdc30... submission of merging script as a job
for folder in $folders ; do
  if [ ! -d ${baseFolder}/${folder} ]; then
    echo "${folder} does not exist in ${baseFolder}"
    exit 1
  else
<<<<<<< HEAD
    if cp -rfL ${baseFolder}/${folder} .  ; then
      echo Done copying $folder
    else
      echo $folder could not be copied
      exit 1
    fi
=======
    cp -rfL ${baseFolder}/${folder} .
>>>>>>> 15cdc30... submission of merging script as a job
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
<<<<<<< HEAD
if [ ! -f ${xmlFile} ]; then
  echo "${xmlFile} does not exist in ${baseFolder}"
  exit 1
=======
if [ ! -f ${baseFolder}/${xmlFile} ]; then
  echo "${xmlFile} does not exist in ${baseFolder}"
  exit 1
else
  cp -rfL ${baseFolder}/${xmlFile} .
>>>>>>> 15cdc30... submission of merging script as a job
fi
echo "ok"

# -----------------------------------------------

if [ -e LocalLibraries.zip ]; then
  rm LocalLibraries.zip
fi 

if [ -d LocalLibraries.package ]; then
  rm -rf LocalLibraries.package
fi

<<<<<<< HEAD
# -- Jochen's hack of template submission
# hackTemplate=submitPicoHFMaker_temp.xml 
#
# if [ -e submitPicoHFMaker_temp.xml  ]; then
#   rm submitPicoHFMaker_temp.xml 
# fi 
#
# echo '<?xml version="1.0" encoding="utf-8" ?>'		        > $hackTemplate
# echo '<\!DOCTYPE note ['                      		       >> $hackTemplate
# echo '<\!ENTITY basePath "'"$baseFolder"'">'                   >> $hackTemplate   
# echo '<\!ENTITY prodId "'"$productionId"'">'	               >> $hackTemplate       
# echo '<\!ENTITY dirToMerge "'"$directoryToMerge"'">'           >> $hackTemplate 
# echo '<\!ENTITY runMacro "'"$runMacro"'">'  		       >> $hackTemplate
# echo ']>'					       	       >> $hackTemplate
#
# tail -n +2 ${xmlFile} >> $hackTemplate
#
# csh << EOF
# star-submit -u ie $hackTemplate
# EOF

=======
>>>>>>> 15cdc30... submission of merging script as a job
star-submit-template -template "$xmlFile" -entities basePath="$baseFolder",prodId="$productionId",dirToMerge="$directoryToMerge",runMacro="$runMacro"

popd >> /dev/null
