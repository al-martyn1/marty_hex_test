@rem echo off
@setlocal enabledelayedexpansion
@for /r "%~dp0." %%i in (.) do @(
    @if exist "%%i\.git" (
        @echo Updating %%~nxi
        @pushd "%%i"
        git pull
        git submodule update --progress --init --recursive --merge
        @popd
    )
)
