@echo off

set listNiveauIntro=000 001 002 003 004 005

(for %%a in (%listNiveauIntro%) do (
   echo Niveau : L_%%a
   call PlayLocalMatchInput.bat %%a
))

set listNiveauAstar=010 011 012 013

(for %%a in (%listNiveauAstar%) do (
   echo Niveau : L_%%a
   call PlayLocalMatchInput.bat %%a
))

set listNiveauVision=020 021 022 023 024

(for %%a in (%listNiveauVision%) do (
   echo Niveau : L_%%a
   call PlayLocalMatchInput.bat %%a
))

set listNiveauWindows=030 031 032

(for %%a in (%listNiveauWindows%) do (
   echo Niveau : L_%%a
   call PlayLocalMatchInput.bat %%a
))

set listNiveauMazes=040

(for %%a in (%listNiveauMazes%) do (
   echo Niveau : L_%%a
   call PlayLocalMatchInput.bat %%a
))