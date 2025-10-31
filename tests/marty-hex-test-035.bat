@set TEST_NAME=%~n0
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

@rem set SUBTEST=traffic_lights

@call :DOTEST traffic_lights samples.traffic_lights.TrafficLightRoad
@call :DOTEST http           samples.http.HttpParser
@call :DOTEST trimble        protocols.trimble.Tsip
@goto EOF


:DOTEST
@set SUBTEST=%1
@set MACHINE=%2
%TEST_EXE% "%~dp0\..\_libs/umba_tokenizer/inc/umba/tokenizer/parsers/ufsm/samples/%SUBTEST%.ufsm" %MACHINE%    > %~dp0\%TEST_NAME%_%SUBTEST%_%MACHINE%_1.dot   2>%~dp0\%TEST_NAME%_%SUBTEST%_%MACHINE%_1.log
dot -Tpng -o%~dp0\%TEST_NAME%_%SUBTEST%_%MACHINE%_1.png %~dp0\%TEST_NAME%_%SUBTEST%_%MACHINE%_1.dot
%TEST_EXE% "%~dp0\..\_libs/umba_tokenizer/inc/umba/tokenizer/parsers/ufsm/samples/%SUBTEST%.ufsm" %MACHINE% -  > %~dp0\%TEST_NAME%_%SUBTEST%_%MACHINE%_2.dot   2>%~dp0\%TEST_NAME%_%SUBTEST%_%MACHINE%_2.log
dot -Tpng -o%~dp0\%TEST_NAME%_%SUBTEST%_%MACHINE%_2.png %~dp0\%TEST_NAME%_%SUBTEST%_%MACHINE%_2.dot
@exit /b

:EOF
@exit /b 0


:NOT_FOUND
@echo Test executable not found
