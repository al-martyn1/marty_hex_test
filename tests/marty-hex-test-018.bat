@set TEST_NAME=marty-hex-test-018
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"
%TEST_EXE% > %TEST_NAME%.txt 2>&1
@rem %TEST_NAME%.log


@exit /B 0
:NOT_FOUND
@echo Test executable not found
