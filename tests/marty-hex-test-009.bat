@set TEST_NAME=marty-hex-test-009
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set TEST_EXE=%FOUND_EXE%
%TEST_EXE% "%~dp0\layouts\22.txt" "%~dp0\layouts\22_values.txt" > %TEST_NAME%-22.log 2>&1
