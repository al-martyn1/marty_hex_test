call :DO_GIT_JOB %1 %~dp0\encoding
call :DO_GIT_JOB %1 %~dp0\marty_bigint
call :DO_GIT_JOB %1 %~dp0\marty_cpp
call :DO_GIT_JOB %1 %~dp0\marty_crc
call :DO_GIT_JOB %1 %~dp0\marty_csv
call :DO_GIT_JOB %1 %~dp0\marty_decimal
call :DO_GIT_JOB %1 %~dp0\marty_expressions
call :DO_GIT_JOB %1 %~dp0\marty_format
call :DO_GIT_JOB %1 %~dp0\marty_hex
call :DO_GIT_JOB %1 %~dp0\marty_mem
call :DO_GIT_JOB %1 %~dp0\marty_svg
@rem call :DO_GIT_JOB %1 %~dp0\marty_pugixml
@rem call :DO_GIT_JOB %1 %~dp0\pugixml
@rem call :DO_GIT_JOB %1 %~dp0\marty_tr
call :DO_GIT_JOB %1 %~dp0\marty_utf
@rem call :DO_GIT_JOB %1 %~dp0\marty_yaml_toml_json
@rem call :DO_GIT_JOB %1 %~dp0\nlohmann
call :DO_GIT_JOB %1 %~dp0\sfmt
call :DO_GIT_JOB %1 %~dp0\umba
call :DO_GIT_JOB %1 %~dp0\umba_tokenizer
@rem call :DO_GIT_JOB %1 %~dp0\yaml-cpp

@exit /B

:DO_GIT_JOB
@echo %1'ing %2
@cd %2
@git %1
@cd ..
@echo.
@exit /B
