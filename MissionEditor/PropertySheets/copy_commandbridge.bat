@echo off
setlocal enabledelayedexpansion

set "CONFIG=%~1"
set "PLATFORM=%~2"
set "OUTDIR=%~3"

if "%CONFIG%"=="" exit /b 0
if "%PLATFORM%"=="" set "PLATFORM=x64"

set "BRIDGE_CONFIG=Release"
echo %CONFIG% | findstr /i "debug" >nul && set "BRIDGE_CONFIG=Debug"

set "PROJECT_ROOT=%~dp0..\.."
set "SRC=%PROJECT_ROOT%\CommandBridge\%PLATFORM%\%BRIDGE_CONFIG%\CommandBridge.dll"
set "PDB=%PROJECT_ROOT%\CommandBridge\%PLATFORM%\%BRIDGE_CONFIG%\CommandBridge.pdb"
set "HEADER_SRC=%PROJECT_ROOT%\CommandBridge\CommandBridge.h"
set "HEADER_DST=%PROJECT_ROOT%\3rdParty\CommandBridge\CommandBridge.h"

if not exist "%SRC%" exit /b 0
if "%OUTDIR%"=="" set "OUTDIR=%PROJECT_ROOT%\dist\FinalRevenge"

copy /Y "%SRC%" "%OUTDIR%\" >nul 2>&1
if exist "%PDB%" copy /Y "%PDB%" "%OUTDIR%\" >nul 2>&1
if exist "%HEADER_SRC%" copy /Y "%HEADER_SRC%" "%HEADER_DST%" >nul 2>&1
