@set CONFIG=Debug
@if exist "%~dp0\.set_config.bat" @call "%~dp0\.set_config.bat"
@call "%~dp0..\.bat\setup_out_root_paths.bat"
@echo OUTROOTPATH=%OUTROOTPATH%
@set TEST_EXE="%OUTROOTPATH%\%CONFIG%\%1.exe"
