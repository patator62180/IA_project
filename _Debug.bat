@echo off

if "%mapname%" == "" (
    echo Veuillez saisir le niveau aue vous souhaitez faire :
    set /p mapname=L_%1
)

PlayLocalMatch.bat .\build-solution-x64\Debug L_%mapname%
