@set TEST_NAME=marty-hex-test-002
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set TEST_EXE=%FOUND_EXE%
%TEST_EXE% "%~dp0\layouts\01.txt" > %TEST_NAME%-01.log 2>&1
%TEST_EXE% "%~dp0\layouts\02.txt" > %TEST_NAME%-02.log 2>&1
%TEST_EXE% "%~dp0\layouts\03.txt" > %TEST_NAME%-03.log 2>&1
%TEST_EXE% "%~dp0\layouts\04.txt" > %TEST_NAME%-04.log 2>&1
%TEST_EXE% "%~dp0\layouts\05.txt" > %TEST_NAME%-05.log 2>&1
%TEST_EXE% "%~dp0\layouts\06.txt" > %TEST_NAME%-06.log 2>&1
