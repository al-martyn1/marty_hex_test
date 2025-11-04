@set ACC=al-martyn1
@set BASE=github.com

@if "%1"=="SSH"  goto SETUP_SSH
@if "%1"=="GIT"  goto SETUP_SSH
@if "%1"=="HTTP" goto SETUP_HTTP

@rem Default is HTTPS mode

:SETUP_HTTPS
@rem По https 
set PREFIX=https://%BASE%/%ACC%
goto DO_CLONE

:SETUP_HTTP
@rem По https 
set PREFIX=http://%BASE%/%ACC%
goto DO_CLONE

:SETUP_SSH
set PREFIX=git@%BASE%:%ACC%

:DO_CLONE
git clone --recurse-submodules %PREFIX%/umba_mm_mod_encodings.git   %~dp0\encoding
git clone --recurse-submodules %PREFIX%/marty_bigint.git            %~dp0\marty_bigint
git clone --recurse-submodules %PREFIX%/marty_containers.git        %~dp0\marty_containers
git clone --recurse-submodules %PREFIX%/marty_cpp.git               %~dp0\marty_cpp
git clone --recurse-submodules %PREFIX%/marty_crc.git               %~dp0\marty_crc
git clone --recurse-submodules %PREFIX%/marty_csv.git               %~dp0\marty_csv
git clone --recurse-submodules %PREFIX%/marty_decimal.git           %~dp0\marty_decimal
git clone --recurse-submodules %PREFIX%/marty_dot.git               %~dp0\marty_dot
git clone --recurse-submodules %PREFIX%/marty_expressions.git       %~dp0\marty_expressions
git clone --recurse-submodules %PREFIX%/marty_format.git            %~dp0\marty_format
git clone --recurse-submodules %PREFIX%/marty_gdl.git               %~dp0\marty_gdl
git clone --recurse-submodules %PREFIX%/marty_hex.git               %~dp0\marty_hex
git clone --recurse-submodules %PREFIX%/marty_mem.git               %~dp0\marty_mem
git clone --recurse-submodules %PREFIX%/marty_svg.git               %~dp0\marty_svg
@rem git clone --recurse-submodules %PREFIX%/marty_pugixml.git           %~dp0\marty_pugixml
@rem git clone --recurse-submodules %PREFIX%/extern_pugixml.git          %~dp0\pugixml
@rem git clone --recurse-submodules %PREFIX%/marty_tr.git                %~dp0\marty_tr
git clone --recurse-submodules %PREFIX%/marty_utf.git               %~dp0\marty_utf
@rem git clone --recurse-submodules %PREFIX%/marty_yaml_toml_json.git    %~dp0\marty_yaml_toml_json
@rem git clone --recurse-submodules %PREFIX%/forks-nlohmann-json.git     %~dp0\nlohmann
git clone --recurse-submodules %PREFIX%/umba_mm_mod_sfmt.git        %~dp0\sfmt
git clone --recurse-submodules %PREFIX%/umba_mm_mod_umba.git        %~dp0\umba
git clone --recurse-submodules %PREFIX%/umba_tokenizer.git          %~dp0\umba_tokenizer
@rem git clone --recurse-submodules %PREFIX%/forks-jbeder-yaml-cpp.git   %~dp0\yaml-cpp
