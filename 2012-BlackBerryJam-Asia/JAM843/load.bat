@echo off
REM This script can be used to sideload a BAR file
REM Dependencies: BlackBerry 10 WebWorks SDK
REM   Change the SDK variable to match the path of the WebWorks SDK installed on your machine:

SET SDK=C:\Program Files\Research In Motion\BlackBerry 10 WebWorks SDK 1.0.3.8\dependencies\tools\bin

IF "%1" == "" GOTO MISSINGPARAMS
IF "%2" == "" GOTO MISSINGPARAMS

"%SDK%\blackberry-deploy.bat" -package app\device\wic.bar -device %1 -installApp -password %2
GOTO END

:MISSINGPARAMS
ECHO Missing parameters.  Correct usage: load_win.bat [device ip] [device password]

:END