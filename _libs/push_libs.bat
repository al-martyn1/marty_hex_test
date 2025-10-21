@rem echo off
@setlocal enabledelayedexpansion
@for /r "%~dp0." %%i in (.) do @(
    @if exist "%%i\.git" (
        @echo Pushing %%~nxi
        @pushd "%%i"
        git push
        @popd
    )
)
