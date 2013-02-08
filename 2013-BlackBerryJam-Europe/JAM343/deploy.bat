@echo OFF
CLS

REM --------------------------------------------------------------------------------
REM Variables used by this script:
REM
IF "%1" == "" GOTO MISSINGPARAMS
IF "%2" == "" GOTO MISSINGPARAMS
SET deviceip=%1
SET devicepwd=%2
SET tmpfolder=C:\Lab\JAM343\app

REM --------------------------------------------------------------------------------
REM Dependancies check - ensure that the following dependancies are in place:
REM
IF NOT EXIST %tmpfolder% GOTO NOTEMPFOLDER

REM --------------------------------------------------------------------------------
REM Deploy the BAR to a target device using using BlackBerry 10 WebWorks SDK
REM
ECHO Starting deployment.
ECHO.
ECHO Package name: %tmpfolder%\device\WIC.bar
ECHO.
ECHO Device IP address:  %deviceip%
ECHO.
ECHO Device password:  ****
ECHO.
ECHO ...
"c:\program files (x86)\Research In Motion\BlackBerry 10 WebWorks SDK 1.0.4.7\dependencies\tools\bin\blackberry-deploy" -installApp -launchApp -device %deviceip% -password %devicepwd% -package %tmpfolder%\device\WIC.bar

ECHO.
GOTO DEPLOYDONE

:DEPLOYDONE
GOTO SUCCESS

REM --------------------------------------------------------------------------------
REM Display any errors that may have occurred
REM
:MISSINGPARAMS
ECHO ** ERROR **
ECHO.
ECHO Missing parameters!
ECHO.
ECHO Correct usage: deploy.bat [device IP address] [device password]
GOTO END
:NOTEMPFOLDER
ECHO ** ERROR **
ECHO.
ECHO Required file sample_application.bar not found in output directory.
ECHO.
ECHO Make sure to specify "%tmpfolder%" as the output folder when packaging and signing the application.
GOTO END

:END
