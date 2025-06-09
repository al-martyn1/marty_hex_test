@set TEST_NAME=%~n0
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

%TEST_EXE% < 01.csv   > %TEST_NAME%_01.txt   2>%TEST_NAME%_01.log
%TEST_EXE% < 02.csv   > %TEST_NAME%_02.txt   2>%TEST_NAME%_02.log
%TEST_EXE% < 03.csv   > %TEST_NAME%_03.txt   2>%TEST_NAME%_03.log
%TEST_EXE% < 04.csv   > %TEST_NAME%_04.txt   2>%TEST_NAME%_04.log
%TEST_EXE% < 05.csv   > %TEST_NAME%_05.txt   2>%TEST_NAME%_05.log
%TEST_EXE% < 06.csv   > %TEST_NAME%_06.txt   2>%TEST_NAME%_06.log
%TEST_EXE% < 07.csv   > %TEST_NAME%_07.txt   2>%TEST_NAME%_07.log
%TEST_EXE% < 08.csv   > %TEST_NAME%_08.txt   2>%TEST_NAME%_08.log
%TEST_EXE% < 09.csv   > %TEST_NAME%_09.txt   2>%TEST_NAME%_09.log
%TEST_EXE% < 10.csv   > %TEST_NAME%_10.txt   2>%TEST_NAME%_10.log
%TEST_EXE% < 11.csv   > %TEST_NAME%_11.txt   2>%TEST_NAME%_11.log
%TEST_EXE% < 12.csv   > %TEST_NAME%_12.txt   2>%TEST_NAME%_12.log
%TEST_EXE% < 13.csv   > %TEST_NAME%_13.txt   2>%TEST_NAME%_13.log
%TEST_EXE% < 14.csv   > %TEST_NAME%_14.txt   2>%TEST_NAME%_14.log
%TEST_EXE% < 15.csv   > %TEST_NAME%_15.txt   2>%TEST_NAME%_15.log
%TEST_EXE% < 16.csv   > %TEST_NAME%_16.txt   2>%TEST_NAME%_16.log
%TEST_EXE% < 17.csv   > %TEST_NAME%_17.txt   2>%TEST_NAME%_17.log
%TEST_EXE% < 18.csv   > %TEST_NAME%_18.txt   2>%TEST_NAME%_18.log

%TEST_EXE% < excel_test01_dos.csv        > %TEST_NAME%_excel_test01_dos.txt        2>%TEST_NAME%_excel_test01_dos.log
%TEST_EXE% < excel_test01_mac.csv        > %TEST_NAME%_excel_test01_mac.txt        2>%TEST_NAME%_excel_test01_mac.log
%TEST_EXE% < excel_test01_semicolon.csv  > %TEST_NAME%_excel_test01_semicolon.txt  2>%TEST_NAME%_excel_test01_semicolon.log
%TEST_EXE% < excel_test01_tabs.csv       > %TEST_NAME%_excel_test01_tabs.txt       2>%TEST_NAME%_excel_test01_tabs.log

@rem %TEST_NAME%.log


@exit /B 0
:NOT_FOUND
@echo Test executable not found
