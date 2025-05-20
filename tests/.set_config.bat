@if "%CONFIG%"=="" goto CONFIG_NOT_SET
@goto CONFIG_IS_SET
:CONFIG_NOT_SET
@set CONFIG=Debug
@rem set CONFIG=Release
:CONFIG_IS_SET
