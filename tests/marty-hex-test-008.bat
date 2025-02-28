@set TEST_NAME=marty-hex-test-008
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

@rem https://microsin.net/adminstuff/windows/for-in-bat-files.html

@if not exist "%~dp0\hex_logs" @mkdir "%~dp0\hex_logs"

@echo Creating error log > %TEST_NAME%.log

@for %%I in ("%~dp0\hex\*.hex") do @(
    @rem echo %%I
    @rem echo ---------  %TEST_NAME%.log
    @%TEST_EXE% "%%I"  > "%~dp0\hex_logs\%%~nI.log" 2>>%TEST_NAME%.log
)



@exit /B 0
:NOT_FOUND
@echo Test executable not found
