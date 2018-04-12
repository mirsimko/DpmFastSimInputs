#!/bin/tcsh

starver SL16j
# starver SL18a

root4star -l -b -q StRoot/macros/loadSharedHFLibraries.C 'runPicoLcAnaMaker.C+("test_pico.list","outputQA_new_test_00",0,"BadRunList_MB.list","picoHFtree","root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/AuAu_200_production_2016/ReversedFullField/P16ij/2016",0, -1)'
