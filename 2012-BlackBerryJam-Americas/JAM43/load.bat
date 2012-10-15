@echo off
REM This script can be used to sideload a BAR file
REM Dependencies: BlackBerry 10 WebWorks SDK
REM   Change the sdk path to be that of the WebWorks SDK installed on your machine:

SET sdk=C:\Program Files\Research In Motion\BlackBerry 10 WebWorks SDK 1.0.2.9\dependencies\tools\binx

IF "%1" == "" GOTO MISSINGPARAMS
IF "%2" == "" GOTO MISSINGPARAMS
IF "%3" == "" GOTO MISSINGPARAMS

"%sdk%\blackberry-deploy.bat" -package %1 -device %2 -installApp -password %3
GOTO END

:MISSINGPARAMS
ECHO Missing parameters.  Correct usage: load.bat [BAR file] [device ip] [device password]

:END