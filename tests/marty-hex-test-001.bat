@rem if exist "%~dp0\..\.set_sln.bat" @call "%~dp0\..\.set_sln.bat"
@rem call %~dp0\..\.bat\.find-exe-in-out.bat marty-hex-test-001
call "%~dp0\.find_test_exe.bat" marty-hex-test-001
@echo FOUND_EXE: %FOUND_EXE%
@set TEST_EXE=%FOUND_EXE%
%TEST_EXE% "%~dp0\hex\01.hex" > marty-hex-test-001-01.log 2>&1
%TEST_EXE% "%~dp0\hex\02.hex" > marty-hex-test-001-02.log 2>&1
%TEST_EXE% "%~dp0\hex\03.hex" > marty-hex-test-001-03.log 2>&1
%TEST_EXE% "%~dp0\hex\04.hex" > marty-hex-test-001-04.log 2>&1
%TEST_EXE% "%~dp0\hex\05.hex" > marty-hex-test-001-05.log 2>&1





