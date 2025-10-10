@set TEST_NAME=%~n0
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

%TEST_EXE% "%~dp0\..\_libs/umba_tokenizer/inc/umba/tokenizer/parsers/ufsm/samples/traffic_lights.ufsm"  > %~dp0\%TEST_NAME%_01.ufsm    2>%~dp0\%TEST_NAME%_01.log
%TEST_EXE% %~dp0\%TEST_NAME%_01.ufsm                                                                    > %~dp0\%TEST_NAME%_01a.ufsm   2>%~dp0\%TEST_NAME%_01a.log

%TEST_EXE% "%~dp0\..\_libs/umba_tokenizer/inc/umba/tokenizer/parsers/ufsm/samples/http.ufsm"            > %~dp0\%TEST_NAME%_02.ufsm    2>%~dp0\%TEST_NAME%_02.log
%TEST_EXE% %~dp0\%TEST_NAME%_02.ufsm                                                                    > %~dp0\%TEST_NAME%_02a.ufsm   2>%~dp0\%TEST_NAME%_02a.log

@rem %TEST_NAME%.log

@exit /B 0
:NOT_FOUND
@echo Test executable not found
