@set CONFIG=Debug
@rem set CONFIG=Release
@call %~dp0..\.bat\setup_out_root_paths.bat
@echo OUTROOTPATH=%OUTROOTPATH%
@set TEST_EXE="%OUTROOTPATH%\%CONFIG%\%1.exe"
