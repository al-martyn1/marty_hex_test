@set TEST_NAME=marty-hex-test-005
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set TEST_EXE=%FOUND_EXE%
%TEST_EXE% > %TEST_NAME%-01.log 2>&1
