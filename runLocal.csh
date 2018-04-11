#!/bin/tcsh

starver SL16d

root4star -l -b -q 'runPicoLcAnaMaker.C("test_2.list","outputQA_new_test_00",0,"picoLists/BadRunList_MB.list","picoHFtree","/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/physics2/P16id",0, -1)'
