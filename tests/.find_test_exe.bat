@set CONFIG=Debug
@if exist "%~dp0\.set_config.bat" @call "%~dp0\.set_config.bat"
@call %~dp0\..\.bat\find-exe-in-out.bat %1
