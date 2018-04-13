#!/bin/bash

toMerge="$1"

# -- change for your folder
baseFolder=/gpfs/mnt/gpfs01/star/pwg/msimko/LambdaC/LcFastSimInputs

# ###############################################
# -- Do not change below
# ###############################################

if [ -d "$toMerge" ]; then
  pushd "$toMerge" >> /dev/null
  # we want the physical address of the directory to merge
  directoryToMerge=`pwd -P`
  popd >> /dev/null
else
  echo The argument must be a directory
  exit 1
fi

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
for folder in $folders ; do
  if [ ! -d ${baseFolder}/${folder} ]; then
    echo "${folder} does not exist in ${baseFolder}"
    exit 1
  else
    if cp -rfL ${baseFolder}/${folder} .  ; then
      echo Done copying $folder
    else
      echo $folder could not be copied
      exit 1
    fi
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
if [ ! -f ${xmlFile} ]; then
  echo "${xmlFile} does not exist in ${baseFolder}"
  exit 1
fi
echo "ok"

# -----------------------------------------------

if [ -e LocalLibraries.zip ]; then
  rm LocalLibraries.zip
fi 

if [ -d LocalLibraries.package ]; then
  rm -rf LocalLibraries.package
fi

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

star-submit-template -template "$xmlFile" -entities basePath="$baseFolder",prodId="$productionId",dirToMerge="$directoryToMerge",runMacro="$runMacro"

popd >> /dev/null
