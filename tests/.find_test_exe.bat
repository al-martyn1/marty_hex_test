@if "%CONFIG%"=="" goto CONFIG_NOT_SET
@goto CONFIG_IS_SET
:CONFIG_NOT_SET
@set CONFIG=Debug
:CONFIG_IS_SET
@if exist "%~dp0\.set_config.bat" @call "%~dp0\.set_config.bat"
@call %~dp0\..\.bat\find-exe-in-out.bat %1
