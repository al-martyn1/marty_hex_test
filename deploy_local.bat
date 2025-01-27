@if exist "%~dp0\set_sln.bat"  @call "%~dp0\set_sln.bat"
@if exist "%~dp0\set_sln_exes.bat"  @call "%~dp0\set_sln_exes.bat"
@set DEPLOY_BINS=%SLN_EXES%
@if "%DEPLOY_BINS%"=="" @set DEPLOY_BINS=%SLN%
@rem Add extra bins here
@set DEPLOY_BINS=%DEPLOY_BINS%
@rem
@if "%DEPLOY_ROOT%"==""   @set DEPLOY_ROOT=%UMBA_TOOLS%
@rem
@if "%DEPLOY_ROOT%"=="" goto DEPLOY_ROOT_NOT_SET
@goto DEPLOY_ROOT_IS_SET
:DEPLOY_ROOT_NOT_SET
@echo DEPLOY_ROOT environment variable is not set, UMBA_TOOLS not set too
@exit /B 1
:DEPLOY_ROOT_IS_SET
@rem
@call "%~dp0.bat\deploy_impl.bat"
@rem
@rem Add extra deploy code here

%SLN% --help > help.txt