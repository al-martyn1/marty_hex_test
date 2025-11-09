Вот несколько способов обработки такого файла в bat-файле:

## Способ 1: Простой перебор строк

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=DEF_"  :: Предопределённый префикс
set "INPUT_FILE=data.txt"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    if "%%B"=="" (
        echo Одно значение: !PREFIX!%%A
        :: Ваше действие для одного значения
        call :process_single "%%A"
    ) else (
        echo Два значения: %%A и %%B
        :: Ваше действие для двух значений
        call :process_double "%%A" "%%B"
    )
)
goto :eof

:process_single
echo Обрабатываю одно значение: %1 с префиксом %PREFIX%
:: Ваша логика здесь
goto :eof

:process_double
echo Обрабатываю два значения: %1 и %2
:: Ваша логика здесь
goto :eof
```

## Способ 2: Более гибкая обработка

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=PRE_"
set "INPUT_FILE=data.txt"

for /f "usebackq delims=" %%L in ("%INPUT_FILE%") do (
    set "line=%%L"
    :: Убираем лишние пробелы
    for /f "tokens=1,2" %%A in ("!line!") do (
        if "%%B"=="" (
            echo Строка с одним значением: %%A
            call :your_action "!PREFIX!%%A"
        ) else (
            echo Строка с двумя значениями: %%A %%B
            call :your_action "%%A" "%%B"
        )
    )
)
goto :eof

:your_action
if "%~2"=="" (
    echo Выполняю действие для одного параметра: %1
) else (
    echo Выполняю действие для двух параметров: %1 и %2
)
goto :eof
```

## Способ 3: С обработкой пустых строк

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=MY_PREFIX_"
set "INPUT_FILE=data.txt"
set "LINE_NUMBER=0"

for /f "tokens=1,2 delims= " %%A in ('type "%INPUT_FILE%" ^| findstr /n /r "."') do (
    set /a LINE_NUMBER+=1
    set "param1=%%A"
    set "param2=%%B"
    
    :: Убираем номер строки из первого параметра
    for /f "tokens=1,* delims=:" %%I in ("!param1!") do (
        set "param1=%%J"
    )
    
    if "!param2!"=="" (
        if not "!param1!"=="" (
            echo Строка !LINE_NUMBER!: Одно значение '!param1!'
            call :custom_processing "!PREFIX!!param1!"
        )
    ) else (
        echo Строка !LINE_NUMBER!: Два значения '!param1!' и '!param2!'
        call :custom_processing "!param1!" "!param2!"
    )
)
goto :eof

:custom_processing
echo ---
echo Получены параметры: %*
echo ---
:: Ваша основная логика обработки
goto :eof
```

## Пример файла данных (data.txt):

```
значение1
парам1 парам2
одиночное
два значения
test
alpha beta
```

## Ключевые моменты:

1. **`for /f "tokens=1,2"`** - разбивает строку на 1 или 2 токена
2. **`if "%%B"==""`** - проверяет, есть ли второе значение
3. **`setlocal enabledelayedexpansion`** - для работы с переменными внутри цикла
4. **`call :function`** - вызов подпрограммы для обработки

## Для разных разделителей:

Если значения разделены не пробелом, а например, табуляцией или запятой:
```batch
for /f "tokens=1,2 delims=," %%A in (%INPUT_FILE%) do (
    :: обработка для разделителя-запятой
)
```

Выберите подходящий способ в зависимости от сложности вашей задачи и формата входного файла.


# Вопрос

> мне нужно также проверить, есть ли у второй строки префикс вида https, git или http. Как это сделать?

Вот расширенный код с проверкой префиксов для второго значения:

## Способ 1: Простая проверка префиксов

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=DEF_"
set "INPUT_FILE=data.txt"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    if "%%B"=="" (
        echo Одно значение: !PREFIX!%%A
        call :process_single "%%A"
    ) else (
        call :check_prefix "%%B"
        if !HAS_PREFIX!==1 (
            echo Два значения: %%A и %%B (второе с префиксом)
        ) else (
            echo Два значения: %%A и %%B
        )
        call :process_double "%%A" "%%B"
    )
)
goto :eof

:check_prefix
set "HAS_PREFIX=0"
set "value=%~1"
:: Проверяем префиксы
if /i "!value:~0,5!"=="https" set "HAS_PREFIX=1"
if /i "!value:~0,4!"=="http" set "HAS_PREFIX=1"
if /i "!value:~0,3!"=="git" set "HAS_PREFIX=1"
goto :eof

:process_single
echo Обрабатываю одно значение: %1
goto :eof

:process_double
echo Обрабатываю два значения: %1 и %2
goto :eof
```

## Способ 2: Более точная проверка с использованием findstr

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=DEF_"
set "INPUT_FILE=data.txt"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    if "%%B"=="" (
        echo Одно значение: !PREFIX!%%A
        call :process_single "%%A"
    ) else (
        call :check_prefix_advanced "%%B"
        if !HAS_PREFIX!==1 (
            echo [С ПРЕФИКСОМ] %%A -> %%B
        ) else (
            echo [БЕЗ ПРЕФИКСА] %%A -> %%B
        )
        call :process_double "%%A" "%%B"
    )
)
goto :eof

:check_prefix_advanced
set "HAS_PREFIX=0"
set "value=%~1"
:: Используем findstr для более надежной проверки
echo !value! | findstr /i /r "^https:// ^http:// ^git@" >nul
if not errorlevel 1 set "HAS_PREFIX=1"
goto :eof
```

## Способ 3: Детальная проверка с определением типа префикса

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=DEF_"
set "INPUT_FILE=data.txt"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    if "%%B"=="" (
        echo Одно значение: !PREFIX!%%A
        call :process_single "%%A"
    ) else (
        call :analyze_second_value "%%B"
        echo Два значения: %%A и %%B [!PREFIX_TYPE!]
        call :process_double "%%A" "%%B" "!PREFIX_TYPE!"
    )
)
goto :eof

:analyze_second_value
set "PREFIX_TYPE=без префикса"
set "value=%~1"

if /i "!value:~0,8!"=="https://" (
    set "PREFIX_TYPE=HTTPS"
) else if /i "!value:~0,7!"=="http://" (
    set "PREFIX_TYPE=HTTP"
) else if /i "!value:~0,4!"=="git@" (
    set "PREFIX_TYPE=GIT"
) else if /i "!value:~0,6!"=="ssh://" (
    set "PREFIX_TYPE=SSH"
) else if /i "!value:~0,4!"=="ftp:" (
    set "PREFIX_TYPE=FTP"
)
goto :eof

:process_single
echo Обрабатываю одно значение: %1
goto :eof

:process_double
echo Обрабатываю два значения: %1 и %2 (тип: %3)
goto :eof
```

## Способ 4: Универсальная проверка с поддержкой разных протоколов

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=DEF_"
set "INPUT_FILE=data.txt"

:: Список префиксов для проверки
set "URL_PREFIXES=https:// http:// git@ ftp:// ssh:// sftp://"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    if "%%B"=="" (
        echo Одно значение: !PREFIX!%%A
        call :process_single "%%A"
    ) else (
        set "HAS_URL_PREFIX=0"
        set "FOUND_PREFIX="
        
        :: Проверяем все префиксы из списка
        for %%P in (%URL_PREFIXES%) do (
            if "!FOUND_PREFIX!"=="" (
                if /i "%%B"=="%%P*" (
                    set "HAS_URL_PREFIX=1"
                    set "FOUND_PREFIX=%%P"
                )
            )
        )
        
        if !HAS_URL_PREFIX!==1 (
            echo [URL] %%A -> %%B (префикс: !FOUND_PREFIX!)
        ) else (
            echo [Обычное] %%A -> %%B
        )
        call :process_double "%%A" "%%B" "!HAS_URL_PREFIX!" "!FOUND_PREFIX!"
    )
)
goto :eof

:process_single
echo Обрабатываю одно значение: %1
goto :eof

:process_double
if "%3"=="1" (
    echo Обрабатываю URL значение: %1 -> %2 (префикс: %4)
) else (
    echo Обрабатываю обычное значение: %1 -> %2
)
goto :eof
```

## Пример файла данных для тестирования:

```
project1
repo1 https://github.com/user/repo.git
library1 git@gitlab.com:user/library.git
app1 http://example.com/source
tool1 ftp://fileserver.com/tool.zip
config1 no_prefix_value
database1 ssh://server/db
package1 sftp://host/package
local1 just_value
```

## Ключевые особенности:

1. **`/i`** - регистронезависимое сравнение
2. **`!value:~0,5!`** - берет первые 5 символов значения
3. **`findstr /r "^https://"`** - регулярное выражение для точной проверки
4. **Проверка конкретных протоколов** - https, http, git, ssh, ftp и др.

Выберите подходящий способ в зависимости от того, насколько строгой должна быть проверка префиксов.


# Вопрос

> Как убрать кавычки вокруг аргумента %1, %2 и тп?

Вот способы убрать кавычки вокруг аргументов:

## Способ 1: Использование `%~1`, `%~2` и т.д.

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=DEF_"
set "INPUT_FILE=data.txt"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    if "%%B"=="" (
        echo Одно значение: !PREFIX!%%A
        call :process_single %%~A
    ) else (
        call :check_prefix %%~B
        if !HAS_PREFIX!==1 (
            echo Два значения: %%~A и %%~B (второе с префиксом)
        ) else (
            echo Два значения: %%~A и %%~B
        )
        call :process_double %%~A %%~B
    )
)
goto :eof

:check_prefix
set "HAS_PREFIX=0"
set "value=%~1"
:: Проверяем префиксы
if /i "!value:~0,5!"=="https" set "HAS_PREFIX=1"
if /i "!value:~0,4!"=="http" set "HAS_PREFIX=1"
if /i "!value:~0,3!"=="git" set "HAS_PREFIX=1"
goto :eof

:process_single
echo Обрабатываю одно значение: %1  (без кавычек)
goto :eof

:process_double
echo Обрабатываю два значения: %1 и %2 (без кавычек)
goto :eof
```

## Способ 2: Убираем кавычки при сохранении в переменные

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=DEF_"
set "INPUT_FILE=data.txt"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    set "param1=%%~A"
    set "param2=%%~B"
    
    if "!param2!"=="" (
        echo Одно значение: !PREFIX!!param1!
        call :process_single "!param1!"
    ) else (
        call :check_prefix "!param2!"
        if !HAS_PREFIX!==1 (
            echo Два значения: !param1! и !param2! (второе с префиксом)
        ) else (
            echo Два значения: !param1! и !param2!
        )
        call :process_double "!param1!" "!param2!"
    )
)
goto :eof
```

## Синтаксис для удаления кавычек:

- **`%~1`** - удаляет окружающие кавычки
- **`%~f1`** - полный путь
- **`%~d1`** - только диск
- **`%~p1`** - только путь
- **`%~n1`** - только имя файла
- **`%~x1`** - только расширение
- **`%~s1`** - короткий путь
- **`%~a1`** - атрибуты файла
- **`%~t1`** - дата/время файла
- **`%~z1`** - размер файла

## Способ 3: Для работы с путями, содержащими пробелы

```batch
@echo off
setlocal enabledelayedexpansion

set "PREFIX=DEF_"
set "INPUT_FILE=data.txt"

for /f "tokens=1,2" %%A in (%INPUT_FILE%) do (
    if "%%B"=="" (
        echo Одно значение: !PREFIX!%%~A
        :: Если значение может содержать пробелы, передаем в кавычках
        call :process_single "%%~A"
    ) else (
        :: Убираем кавычки для проверки
        call :check_prefix %%~B
        if !HAS_PREFIX!==1 (
            echo Два значения: %%~A и %%~B (второе с префиксом)
        ) else (
            echo Два значения: %%~A и %%~B
        )
        :: Передаем без кавычек, если уверены, что нет пробелов
        call :process_double %%~A %%~B
    )
)
goto :eof

:process_single
:: Убираем кавычки внутри функции
set "clean_param=%~1"
echo Обрабатываю одно значение: %clean_param% (без кавычек)
goto :eof

:process_double
set "clean1=%~1"
set "clean2=%~2"
echo Обрабатываю два значения: %clean1% и %clean2% (без кавычек)
goto :eof
```

## Важные замечания:

1. **Если значения содержат пробелы**, лучше передавать в кавычках, а убирать их внутри функции
2. **`%%~A`** убирает кавычки сразу при обработке строки
3. **`%~1`** убирает кавычки из параметров функции
4. **Будьте осторожны** с путями, содержащими пробелы - без кавычек они могут разбиться на несколько аргументов

## Пример для безопасной работы:

```batch
:safe_function
set "arg1=%~1"
set "arg2=%~2"
if defined arg1 (
    echo Аргумент 1: %arg1%
) else (
    echo Аргумент 1 отсутствует
)
if defined arg2 (
    echo Аргумент 2: %arg2%
)
goto :eof
```

Таким образом, используйте `%~1`, `%~2` и т.д. для удаления кавычек из параметров функций.