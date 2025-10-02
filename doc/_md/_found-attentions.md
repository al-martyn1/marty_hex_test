---
Generator: Umba Brief Scanner
---

# _libs/encoding

- `[_libs/encoding/encoding.h:318]`
  Надо бы тут разобраться с файлами, которые UTF-16



# _libs/marty_cpp

- `[_libs/marty_cpp/marty_cpp.h:3276]`
  название? UPD: чего я тут хотел, уже не помню, надо бы коменты более развёрнуто
  делать

- `[_libs/marty_cpp/marty_cpp.h:4232]`
  Тут получить локализацию

- `[_libs/marty_cpp/marty_cpp.h:4407]`
  Тут получить локализацию

- `[_libs/marty_cpp/marty_cpp.h:5364]`
  std::make_tuple - что тут сказать хотел, непонятно, забыл уже

- `[_libs/marty_cpp/marty_cpp.h:5549]`
  заменить std::stoll, чтоб умело парсить двоичные константы и разделители try



# _libs/marty_decimal

- `[_libs/marty_decimal/marty_bcd.h:377]`
  Оно не нужно - что не нужно? Надо будет глянуть сюда повнимательнее



# _libs/marty_expressions

- `[_libs/marty_expressions/base_types.h:106]`
  сделать вектор

- `[_libs/marty_expressions/base_types.h:134]`
  Добавить тип скобки

- `[_libs/marty_expressions/base_types.h:145]`
  Надо как-то пометить окончание - пустое имя?

- `[_libs/marty_expressions/base_types.h:156]`
  Надо как-то пометить окончание - пустое имя?

- `[_libs/marty_expressions/tokenizer_helpers.h:611]`
  Надо подумать, как получить текст, если у нас нет полезной нагрузки, и нет
  итераторов else operatorText = umba::tokenizer::utils::makeTokenText(tokenType,
  b, e);



# _libs/marty_format/std_format_tests

- `[_libs/marty_format/std_format_tests/ds01.cpp:36]`
  Error explicit specialization in non-namespace scope 'struct
  marty::format::variant_filter_traits<std::variant<_Types ...> >' template
  parameters not deducible in partial specialization: struct find_first_filter<>
  {



# _libs/umba

- `[_libs/umba/assert.h:153]`
  Сделать как надо UMBA_ASSERT для GCC/Clang



- `[_libs/umba/cmd_line.h:1647]`
  Тут закоменчено что-то старое, хз зачем я тут внимание акцентировал.
    std::map<std::string, int>::const_iterator eit = optInfo.enumVals.find(optArgCopy);
    if (eit == optInfo.enumVals.end())
    {
    errMsg = std::string("Invalid option value taken. Option value can be one of: ") + optInfo.getAllEnumNames(", ", " or ") + std::string(" (") + optInfo.getAllOptionNames("/") + std::string(")");
    return false;
    }

- `[_libs/umba/cmd_line.h:3026]`
  Надо ProgramLocation проверить на юникод

- `[_libs/umba/macros.h:417]`
  Чего-то с прокси не срослось - компилятор помирает от вложенности шаблонов
  Порешал, сделав getter нешаблонным параметром с виртуальным оператором ()

- `[_libs/umba/macros.h:421]`
  Пока не будем ничего делать, потом разберёмся

- `[_libs/umba/shellapi.h:336]`
  тут нужен замут через dl*, но пока лень и не особо нужно

- `[_libs/umba/time_service.cpp:184]`
  Сделать как надо umba::time_service::init()

- `[_libs/umba/time_service.cpp:196]`
  Сделать как надо umba::time_service::start()

- `[_libs/umba/time_service.cpp:205]`
  Сделать как надо umba::time_service::stop()

- `[_libs/umba/time_service.cpp:232]`
  Сделать как надо

- `[_libs/umba/time_service.cpp:258]`
  Сделать как надо umba::time_service::getCurTimeHires()

- `[_libs/umba/zz_detect_environment.h:683]`
  ??? Чо сказать хотел, не понятно

- `[_libs/umba/zz_mcu_low_level.h:30]`
  Сделать как надо UMBA_INSTRUCTION_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:54]`
  Сделать как надо UMBA_DATA_MEMORY_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:72]`
  Сделать как надо UMBA_DATA_SYNCHRONIZATION_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:123]`
  Сделать как надо UMBA_INTERRUPTS_DISABLED

- `[_libs/umba/zz_mcu_low_level.h:139]`
  Сделать как надо UMBA_DISABLE_IRQ

- `[_libs/umba/zz_mcu_low_level.h:148]`
  Сделать как надо UMBA_ENABLE_IRQ



# _src/umba-hex

- `[_src/umba-hex/marty-hex-test-021.cpp:302]`
  Тут надо потом вставить непечатные символы, чтобы проверить escape

