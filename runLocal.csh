#!/bin/tcsh

starver SL16d

root4star -l -b -q StRoot/macros/loadSharedHFLibraries.C 'runPicoLcAnaMaker.C+("jobs/2018-04-14_16-02/list/sched53B6783E7F0235F33482255AC3FA0587_57.list","outputQA_new_test_00",0,"picoLists/BadRunList_MB.list","picoHFtree","/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/physics2/P16id",0, -1)'
