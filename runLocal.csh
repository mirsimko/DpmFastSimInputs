#!/bin/tcsh

starver SL16j
# starver SL18a

root4star -l -b -q StRoot/macros/loadSharedHFLibraries.C 'runPicoLcAnaMaker.C+("test_pico.list","outputQA_new_test_00",0,"BadRunList_MB.list","picoHFtree","root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/AuAu_200_production_2016/ReversedFullField/P16ij/2016",0, -1)'
# root4star -l -b -q StRoot/macros/loadSharedHFLibraries.C 'runPicoLcAnaMaker.C+("jobs/2018-04-14_16-02/list/sched53B6783E7F0235F33482255AC3FA0587_57.list","outputQA_new_test_00",0,"picoLists/BadRunList_MB.list","picoHFtree","/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/physics2/P16id",0, -1)'
