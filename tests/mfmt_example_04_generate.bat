@set TEST_NAME=mfmt_example_04_generate
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"
%TEST_EXE% > %TEST_NAME%.txt 2>%TEST_NAME%.log
@rem %TEST_NAME%.log

copy /Y "%~dp0\mfmt_example_04_generate.txt" "%~dp0\..\_libs\marty_format\md_\examples\example_04.cpp"

@exit /B 0
:NOT_FOUND
@echo Test executable not found
