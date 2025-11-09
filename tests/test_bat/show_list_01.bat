@echo off
setlocal enabledelayedexpansion

@set "MAIN_REPO=https://al-martyn1"
@set "INPUT_FILE=%~dp0\libs.list"
@set "GIT_OPTS=--recurse-submodules"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    @set first=%%A
    @if /i "!first:~0,1!"=="#" (
      @echo skip %%A %%B
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
@rem echo Обрабатываю одно значение: %1 с префиксом %MAIN_REPO%
@echo git clone %GIT_OPTS% %MAIN_REPO%/%~1.git %~1
@rem goto :eof
@exit /B 0

:process_double
@rem echo Обрабатываю два значения: %1 и %2
@set "value=%~2"
@if /i "!value:~0,8!"=="https://" (
  @echo git clone %GIT_OPTS% %value%.git %~1
) else if /i "!value:~0,7!"=="http://" (
  @echo git clone %GIT_OPTS% %value%.git %~1
) else if /i "!value:~0,4!"=="git@" (
  @echo git clone %GIT_OPTS% %value%.git %~1
) else (
  @echo git clone %GIT_OPTS% %MAIN_REPO%/%value%.git %~1
)

@rem goto :eof
@exit /B 0

