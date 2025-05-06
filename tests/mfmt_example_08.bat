@set TEST_NAME=mfmt_example_08
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"
%TEST_EXE%   > %TEST_NAME%_1.txt 2>%TEST_NAME%_1.log
%TEST_EXE% 2 > %TEST_NAME%_2.txt 2>%TEST_NAME%_2.log

@rem copy /Y %TEST_NAME%_1.txt "%~dp0\..\_libs\marty_format\md_\float_test_locale_output.txt"

@exit /B 0
:NOT_FOUND
@echo Test executable not found
