@if exist "%~dp0\..\.set_sln.bat" @call "%~dp0\..\.set_sln.bat"
@call %~dp0\..\.bat\.find-exe-in-out.bat %SLN%
@echo FOUND_EXE: %FOUND_EXE%
@set TEST_EXE=%FOUND_EXE%
