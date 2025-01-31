@set TEST_NAME=marty-hex-test-002
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set TEST_EXE=%FOUND_EXE%
%TEST_EXE% "%~dp0\hex\01.hex" > %TEST_NAME%-01.log 2>&1
%TEST_EXE% "%~dp0\hex\02.hex" > %TEST_NAME%-02.log 2>&1
%TEST_EXE% "%~dp0\hex\03.hex" > %TEST_NAME%-03.log 2>&1
%TEST_EXE% "%~dp0\hex\04.hex" > %TEST_NAME%-04.log 2>&1
%TEST_EXE% "%~dp0\hex\05.hex" > %TEST_NAME%-05.log 2>&1





