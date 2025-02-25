@set TEST_NAME=marty-hex-test-002
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE=%FOUND_EXE%
%TEST_EXE% "%~dp0\layouts\01.txt" > %TEST_NAME%-01.log 2>&1
%TEST_EXE% "%~dp0\layouts\02.txt" > %TEST_NAME%-02.log 2>&1
%TEST_EXE% "%~dp0\layouts\03.txt" > %TEST_NAME%-03.log 2>&1
%TEST_EXE% "%~dp0\layouts\04.txt" > %TEST_NAME%-04.log 2>&1
%TEST_EXE% "%~dp0\layouts\05.txt" > %TEST_NAME%-05.log 2>&1
%TEST_EXE% "%~dp0\layouts\06.txt" > %TEST_NAME%-06.log 2>&1
%TEST_EXE% "%~dp0\layouts\07.txt" > %TEST_NAME%-07.log 2>&1
%TEST_EXE% "%~dp0\layouts\08.txt" > %TEST_NAME%-08.log 2>&1
%TEST_EXE% "%~dp0\layouts\09.txt" > %TEST_NAME%-09.log 2>&1
%TEST_EXE% "%~dp0\layouts\10.txt" > %TEST_NAME%-10.log 2>&1
%TEST_EXE% "%~dp0\layouts\11.txt" > %TEST_NAME%-11.log 2>&1
%TEST_EXE% "%~dp0\layouts\12.txt" > %TEST_NAME%-12.log 2>&1
%TEST_EXE% "%~dp0\layouts\13.txt" > %TEST_NAME%-13.log 2>&1
%TEST_EXE% "%~dp0\layouts\14.txt" > %TEST_NAME%-14.log 2>&1
%TEST_EXE% "%~dp0\layouts\15.txt" > %TEST_NAME%-15.log 2>&1
%TEST_EXE% "%~dp0\layouts\16.txt" > %TEST_NAME%-16.log 2>&1
%TEST_EXE% "%~dp0\layouts\17.txt" > %TEST_NAME%-17.log 2>&1
%TEST_EXE% "%~dp0\layouts\18.txt" > %TEST_NAME%-18.log 2>&1
%TEST_EXE% "%~dp0\layouts\19.txt" > %TEST_NAME%-19.log 2>&1
%TEST_EXE% "%~dp0\layouts\20.txt" > %TEST_NAME%-20.log 2>&1
%TEST_EXE% "%~dp0\layouts\21.txt" > %TEST_NAME%-21.log 2>&1
%TEST_EXE% "%~dp0\layouts\22.txt" > %TEST_NAME%-22.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\23.txt" > %TEST_NAME%-23.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\24.txt" > %TEST_NAME%-24.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\25.txt" > %TEST_NAME%-25.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\26.txt" > %TEST_NAME%-26.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\27.txt" > %TEST_NAME%-27.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\28.txt" > %TEST_NAME%-28.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\29.txt" > %TEST_NAME%-29.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\30.txt" > %TEST_NAME%-30.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\31.txt" > %TEST_NAME%-31.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\32.txt" > %TEST_NAME%-32.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\33.txt" > %TEST_NAME%-33.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\34.txt" > %TEST_NAME%-34.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\35.txt" > %TEST_NAME%-35.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\36.txt" > %TEST_NAME%-36.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\37.txt" > %TEST_NAME%-37.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\38.txt" > %TEST_NAME%-38.log 2>&1
@rem %TEST_EXE% "%~dp0\layouts\39.txt" > %TEST_NAME%-39.log 2>&1



exit /B 0
:NOT_FOUND
@echo Test executable not found
