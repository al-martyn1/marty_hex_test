@echo off
setlocal enabledelayedexpansion

@set ACC=al-martyn1
@set BASE=github.com

@if "%1"=="SSH"  goto SETUP_SSH
@if "%1"=="GIT"  goto SETUP_SSH
@if "%1"=="HTTP" goto SETUP_HTTP

@rem Default is HTTPS mode

:SETUP_HTTPS
@rem По https 
set PREFIX=https://%BASE%/%ACC%
goto DO_CLONE

:SETUP_HTTP
@rem По https 
set PREFIX=http://%BASE%/%ACC%
goto DO_CLONE

:SETUP_SSH
set PREFIX=git@%BASE%:%ACC%

:DO_CLONE
@set "GIT_OPTS=--recurse-submodules"
@set "MAIN_REPO=%PREFIX%"
@set "INPUT_FILE=%~dp0\libs.list"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    @echo.
    @set first=%%A
    @if /i "!first:~0,1!"=="#" (
      @echo Skip %%A %%B
    ) else (
      @if "%%B"=="" (
          call :process_single "%%A"
      ) else (
          call :process_double "%%A" "%%B"
      )
    )
)
@goto :eof

:process_single
echo git clone %GIT_OPTS% %MAIN_REPO%/%~1.git %~1
git clone %GIT_OPTS% %MAIN_REPO%/%~1.git %~1
@exit /B 0

:process_double
@set "value=%~2"
@if /i "!value:~0,8!"=="https://" (
  echo git clone %GIT_OPTS% %value%.git %~1
  git clone %GIT_OPTS% %value%.git %~1
) else if /i "!value:~0,7!"=="http://" (
  echo git clone %GIT_OPTS% %value%.git %~1
  git clone %GIT_OPTS% %value%.git %~1
) else if /i "!value:~0,4!"=="git@" (
  echo git clone %GIT_OPTS% %value%.git %~1
  git clone %GIT_OPTS% %value%.git %~1
) else (
  echo git clone %GIT_OPTS% %MAIN_REPO%/%value%.git %~1
  git clone %GIT_OPTS% %MAIN_REPO%/%value%.git %~1
)

@exit /B 0
