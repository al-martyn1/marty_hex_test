@set TEST_NAME=%~n0
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"
%TEST_EXE% "%~dp0\layouts\22.txt" > %TEST_NAME%-22.svg 2>%TEST_NAME%-22.log


@exit /B 0
:NOT_FOUND
@echo Test executable not found
