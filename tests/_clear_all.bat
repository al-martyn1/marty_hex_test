if exist "%~dp0\*.txt"   del    "%~dp0\*.txt"
if exist "%~dp0\*.svg"   del /S "%~dp0\*.svg"
if exist "%~dp0\*.png"   del /S "%~dp0\*.png"
@rem if exist "%~dp0\*.csv"   del /S "%~dp0\*.csv"
if exist "%~dp0\*.gv"    del /S "%~dp0\*.gv"
if exist "%~dp0\*.ufsm"  del /S "%~dp0\*.ufsm"
@call _clear_logs.bat