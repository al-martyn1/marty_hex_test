@set TEST_NAME=%~n0
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

@rem echo Run tests

@set CMN=-s -t -a
@rem -t

@set TEST_OPTS=-p -S
@rem -M -C -N -U -L -S
@rem -N

@rem -A  CA  doConstantAbsorption          
@rem -M  MA  doMultiAry                    
@rem -C  CP  doCollapseParentheses         
@rem -N  PN  doPromoteNegations            
@rem -U  RU  doRemoveUnnecessaryParentheses
@rem -L  CS  doCollapseSameVars            
@rem -P  AP  doAddParentheses
@rem -G  CN  doConvertNegativeOperands
@rem -S  SM  doSimplify                    

@set F1="(h&g&f&e)|q|p|o|(a1 or !(!(b1&c))|a1) & not !((a2||b2|a2)) | c&!true | e&!(((r|t|!(!y&u&!u))|q|e)&h&g|f)"

@set ARGS00=
@set ARGS01=-A
@set ARGS02=-A -M
@set ARGS03=-A -M -C
@set ARGS04=-A -M -C -N
@set ARGS05=-A -M -C -N -C
@set ARGS06=-A -M -C -N -C -R
@set ARGS07=-A -M -C -N -C -R -U
@set ARGS08=-A -M -C -N -C -R -U -L
@set ARGS09=-A -M -C -N -C -R -U -L -A
@set ARGS10=-A -M -C -N -C -R -U -L -A -G
@set ARGS11=-A -M -C -N -C -R -U -L -A -G -M
@set ARGS12=-A -M -C -N -C -R -U -L -A -G -M -U
@set ARGS13=-A -M -C -N -C -R -U -L -A -G -M -U -N
@set ARGS14=-A -M -C -N -C -R -U -L -A -G -M -U -N -G
@set ARGS15=-A -M -C -N -C -R -U -L -A -G -M -U -N -G -O
@set ARGS16=-A -M -C -N -C -R -U -L -A -G -M -U -N -G -O

@set ARGSFIN=-S


%TEST_EXE% %CMN% %ARGS00%  %F1%  2>%TEST_NAME%_01_00.log  >%TEST_NAME%_01_00.gv
dot -Tsvg                        -o%TEST_NAME%_01_00.svg   %TEST_NAME%_01_00.gv
dot -Tpng                        -o%TEST_NAME%_01_00.png   %TEST_NAME%_01_00.gv

%TEST_EXE% %CMN% %ARGS01%  %F1%  2>%TEST_NAME%_01_01.log  >%TEST_NAME%_01_01.gv
dot -Tsvg                        -o%TEST_NAME%_01_01.svg   %TEST_NAME%_01_01.gv
dot -Tpng                        -o%TEST_NAME%_01_01.png   %TEST_NAME%_01_01.gv

%TEST_EXE% %CMN% %ARGS02%  %F1%  2>%TEST_NAME%_01_02.log  >%TEST_NAME%_01_02.gv
dot -Tsvg                        -o%TEST_NAME%_01_02.svg   %TEST_NAME%_01_02.gv
dot -Tpng                        -o%TEST_NAME%_01_02.png   %TEST_NAME%_01_02.gv

%TEST_EXE% %CMN% %ARGS03%  %F1%  2>%TEST_NAME%_01_03.log  >%TEST_NAME%_01_03.gv
dot -Tsvg                        -o%TEST_NAME%_01_03.svg   %TEST_NAME%_01_03.gv
dot -Tpng                        -o%TEST_NAME%_01_03.png   %TEST_NAME%_01_03.gv

%TEST_EXE% %CMN% %ARGS04%  %F1%  2>%TEST_NAME%_01_04.log  >%TEST_NAME%_01_04.gv
dot -Tsvg                        -o%TEST_NAME%_01_04.svg   %TEST_NAME%_01_04.gv
dot -Tpng                        -o%TEST_NAME%_01_04.png   %TEST_NAME%_01_04.gv

%TEST_EXE% %CMN% %ARGS05%  %F1%  2>%TEST_NAME%_01_05.log  >%TEST_NAME%_01_05.gv
dot -Tsvg                        -o%TEST_NAME%_01_05.svg   %TEST_NAME%_01_05.gv
dot -Tpng                        -o%TEST_NAME%_01_05.png   %TEST_NAME%_01_05.gv

%TEST_EXE% %CMN% %ARGS06%  %F1%  2>%TEST_NAME%_01_06.log  >%TEST_NAME%_01_06.gv
dot -Tsvg                        -o%TEST_NAME%_01_06.svg   %TEST_NAME%_01_06.gv
dot -Tpng                        -o%TEST_NAME%_01_06.png   %TEST_NAME%_01_06.gv

%TEST_EXE% %CMN% %ARGS07%  %F1%  2>%TEST_NAME%_01_07.log  >%TEST_NAME%_01_07.gv
dot -Tsvg                        -o%TEST_NAME%_01_07.svg   %TEST_NAME%_01_07.gv
dot -Tpng                        -o%TEST_NAME%_01_07.png   %TEST_NAME%_01_07.gv

%TEST_EXE% %CMN% %ARGS08%  %F1%  2>%TEST_NAME%_01_08.log  >%TEST_NAME%_01_08.gv
dot -Tsvg                        -o%TEST_NAME%_01_08.svg   %TEST_NAME%_01_08.gv
dot -Tpng                        -o%TEST_NAME%_01_08.png   %TEST_NAME%_01_08.gv

%TEST_EXE% %CMN% %ARGS09%  %F1%  2>%TEST_NAME%_01_09.log  >%TEST_NAME%_01_09.gv
dot -Tsvg                        -o%TEST_NAME%_01_09.svg   %TEST_NAME%_01_09.gv
dot -Tpng                        -o%TEST_NAME%_01_09.png   %TEST_NAME%_01_09.gv

%TEST_EXE% %CMN% %ARGS10%  %F1%  2>%TEST_NAME%_01_10.log  >%TEST_NAME%_01_10.gv
dot -Tsvg                        -o%TEST_NAME%_01_10.svg   %TEST_NAME%_01_10.gv
dot -Tpng                        -o%TEST_NAME%_01_10.png   %TEST_NAME%_01_10.gv

%TEST_EXE% %CMN% %ARGS11%  %F1%  2>%TEST_NAME%_01_11.log  >%TEST_NAME%_01_11.gv
dot -Tsvg                        -o%TEST_NAME%_01_11.svg   %TEST_NAME%_01_11.gv
dot -Tpng                        -o%TEST_NAME%_01_11.png   %TEST_NAME%_01_11.gv

%TEST_EXE% %CMN% %ARGS12%  %F1%  2>%TEST_NAME%_01_12.log  >%TEST_NAME%_01_12.gv
dot -Tsvg                        -o%TEST_NAME%_01_12.svg   %TEST_NAME%_01_12.gv
dot -Tpng                        -o%TEST_NAME%_01_12.png   %TEST_NAME%_01_12.gv

%TEST_EXE% %CMN% %ARGS13%  %F1%  2>%TEST_NAME%_01_13.log  >%TEST_NAME%_01_13.gv
dot -Tsvg                        -o%TEST_NAME%_01_13.svg   %TEST_NAME%_01_13.gv
dot -Tpng                        -o%TEST_NAME%_01_13.png   %TEST_NAME%_01_13.gv

%TEST_EXE% %CMN% %ARGS14%  %F1%  2>%TEST_NAME%_01_14.log  >%TEST_NAME%_01_14.gv
dot -Tsvg                        -o%TEST_NAME%_01_14.svg   %TEST_NAME%_01_14.gv
dot -Tpng                        -o%TEST_NAME%_01_14.png   %TEST_NAME%_01_14.gv

%TEST_EXE% %CMN% %ARGS15%  %F1%  2>%TEST_NAME%_01_15.log  >%TEST_NAME%_01_15.gv
dot -Tsvg                        -o%TEST_NAME%_01_15.svg   %TEST_NAME%_01_15.gv
dot -Tpng                        -o%TEST_NAME%_01_15.png   %TEST_NAME%_01_15.gv

%TEST_EXE% %CMN% %ARGSFIN% %F1%  2>%TEST_NAME%_01_S.log   >%TEST_NAME%_01_S.gv
dot -Tsvg                        -o%TEST_NAME%_01_S.svg    %TEST_NAME%_01_S.gv
dot -Tpng                        -o%TEST_NAME%_01_S.png    %TEST_NAME%_01_S.gv



@exit /B 0
:NOT_FOUND
@echo Test executable not found
