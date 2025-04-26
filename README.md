# Шаблон Umba-проекта

- [Настройка проекта на базе данного шаблона](#user-content-настройка-проекта-на-базе-данного-шаблона)
  - [Настройка CMakeLists.txt](#user-content-настройка-cmakeliststxt)
    - [Название проекта](#user-content-название-проекта)
    - [Настройка библиотек](#user-content-настройка-библиотек)
    - [Добавление целей](#user-content-добавление-целей)
  - [Настройка запуска MSVC](#user-content-настройка-запуска-msvc)
- [Сборка и отладка проекта](#user-content-сборка-и-отладка-проекта)
  - [Извлечение библиотек](#user-content-извлечение-библиотек)
  - [Генерация сборочных скриптов и сборка](#user-content-генерация-сборочных-скриптов-и-сборка)
  - [Открытие проекта в IDE](#user-content-открытие-проекта-в-ide)
    - [Запуск VSCode](#user-content-запуск-vscode)
    - [Запуск MSVC](#user-content-запуск-msvc)


Для подключения и использования необходимых библиотек их нужно раскомментировать в файлах:

 - `_libs/_clone_libs.bat`
 - `_libs/_git-job-impl.bat`


# Настройка проекта на базе данного шаблона

## Настройка CMakeLists.txt

### Название проекта

В начале файла находим строку описания проекта (с директивой `project`):

**CMakeLists.txt:0**
```cmake
```

Заменяем название `umba-template` на название проекта.



### Настройка библиотек

Импортируем нужные библиотеки:

**CMakeLists.txt:23**
```cmake
# Import libraries here
add_subdirectory(${LIB_ROOT}/encoding)
add_subdirectory(${LIB_ROOT}/marty_cpp)
add_subdirectory(${LIB_ROOT}/marty_crc)
add_subdirectory(${LIB_ROOT}/marty_decimal)
add_subdirectory(${LIB_ROOT}/marty_hex)
add_subdirectory(${LIB_ROOT}/marty_mem)
add_subdirectory(${LIB_ROOT}/marty_svg)
add_subdirectory(${LIB_ROOT}/marty_utf)
add_subdirectory(${LIB_ROOT}/marty_format)
# add_subdirectory(${LIB_ROOT}/marty_pugixml)
# add_subdirectory(${LIB_ROOT}/marty_tr)
# add_subdirectory(${LIB_ROOT}/marty_yaml_toml_json)
# add_subdirectory(${LIB_ROOT}/nlohmann)    # https://json.nlohmann.me/integration/cmake/#external
# add_subdirectory(${LIB_ROOT}/pugixml)
add_subdirectory(${LIB_ROOT}/sfmt)
add_subdirectory(${LIB_ROOT}/umba)
add_subdirectory(${LIB_ROOT}/umba_tokenizer)
# add_subdirectory(${LIB_ROOT}/yaml-cpp)
```


Настраиваем списки библиотек для последующего использования:

**CMakeLists.txt:51**
```cmake
# Configure libraries here
set(COMMON_LIBS encoding::encoding sfmt::sfmt umba marty::cpp marty::crc marty::decimal marty::hex marty::mem marty::svg umba::tokenizer marty::utf marty::format)
# set(PUGIXML_LIB pugixml::pugixml marty_pugixml::marty_pugixml)
# set(JSON_YAML_LIB nlohmann_json::nlohmann_json yaml-cpp::yaml-cpp)
```

При настройке целей можно как использовать заранее подготовленные списки библиотек, так и указывать библиотеки непосредственно.


### Добавление целей

Настраиваем цель (исполняемый файл) проекта:

**CMakeLists.txt:0**
```cmake
```

При необходимости дополнительные цели можно добавлять аналогично.


## Настройка запуска MSVC

Для открытия проекта в MSVC нужно настроить название "солюшена":

**set_sln.bat:2**
```
@set SLN=umba-hex
```

Это название должно совпадать с тем, что указано в директиве `project` файла `CMakeLists.txt`.


# Сборка и отладка проекта


## Извлечение библиотек

Для сборки проекта требуются внешние библиотеки. Внешние библиотеки не подгружаются автоматически.
Для их подгрузки требуется выполнить скрипт `_libs/_clone_libs.bat`.


## Генерация сборочных скриптов и сборка

Для генерации сборочных скриптов и их сборки при помощи `MSBuild` предназначены файлы:

 - `.bat/cmake_generate_all_msvc2017.bat`
 - `.bat/cmake_generate_all_msvc2019.bat`
 - `.bat/cmake_generate_all_msvc2022.bat`
 - `.bat/cmake_build_all_msvc2017.bat`
 - `.bat/cmake_build_all_msvc2019.bat`
 - `.bat/cmake_build_all_msvc2022.bat`


## Открытие проекта в IDE

### Запуск VSCode

Для запуска VSCode предназначен файл `_start_code.bat`. 
VSCode с плагином "CMake Tools" сам обнаружит `CMakeLists.txt`. Следует выбрать подходящий тулчейн
и можно начинать работу.


### Запуск MSVC

Для запуска MSVC предназначен файл `_start_msvc.bat`.
По умолчанию будет сгенерированна и открыта в 2019ой студии конфигурация `x86`.
Для того, чтобы открывалась другая студия с другой конфигурацией, следует
переименовать файл `setup_msvc.bat.example` в `setup_msvc.bat` и настроить нужную конфигурацию там.





