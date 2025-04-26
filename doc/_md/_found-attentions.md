---
Generator: Umba Brief Scanner
---

# _libs/encoding

- `[_libs/encoding/encoding.h:315]`
  Надо бы тут разобраться с файлами, которые UTF-16



# _libs/marty_cpp

- `[_libs/marty_cpp/marty_cpp.h:3226]`
  название? UPD: чего я тут хотел, уже не помню, надо бы коменты более развёрнуто
  делать

- `[_libs/marty_cpp/marty_cpp.h:4804]`
  std::make_tuple - что тут сказать хотел, непонятно, забыл уже

- `[_libs/marty_cpp/marty_cpp.h:4985]`
  заменить std::stoll, чтоб умело парсить двоичные константы и разделители try



# _libs/marty_decimal

- `[_libs/marty_decimal/marty_bcd.h:377]`
  Оно не нужно - что не нужно? Надо будет глянуть сюда повнимательнее



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

- `[_libs/umba/cmd_line.h:3002]`
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

- `[_src/umba-hex/marty-hex-test-021.cpp:269]`
  Тут надо потом вставить непечатные символы, чтобы проверить escape

