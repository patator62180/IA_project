@echo off

set mapname=
set mapname=%1
if "%mapname%" == "" (
	echo Veuillez saisir le niveau que vous souhaitez faire :
	set /p mapname=L_%1
)

call PlayLocalMatch.bat .\build-solution-x64\Debug L_%mapname%
