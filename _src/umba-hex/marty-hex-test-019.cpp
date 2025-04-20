/*! \file
    \brief Парсим форматную строку - без автомата
 */


// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "marty_format/marty_format.h"

// 
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>
#include <utility>

#include "encoding/encoding.h"


//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

umba::SimpleFormatter &out = umbaLogStreamMsg;

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

bool bOverwrite         = false;

//
#include "log.h"
//
#include "AppConfig.h"

AppConfig appConfig;

#include "ArgParser.h"



int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        LOG_ERR << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        LOG_ERR << "unknown error\n";
        return 2;
    }

}




// namespace marty_utf {
// typedef std::uint32_t    unicode_char_t;
// typedef unicode_char_t   utf32_char_t  ;
// typedef std::uint16_t    utf16_char_t  ;
// typedef std::uint8_t     utf8_char_t   ;

inline
std::string parseFormatString(const std::string &str, bool ignoreErrors=true)
{
    // UMBA_USED(ignoreErrors);

    // https://en.cppreference.com/w/cpp/utility/format/spec
    // Based on the format specification in Python - https://docs.python.org/3/library/string.html#formatspec

    // format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]

    // '+' - indicates that a sign should be used for both positive as well as negative numbers.
    // '-' - indicates that a sign should be used only for negative numbers (this is the default behavior).
    // space - indicates that a leading space should be used on positive numbers, and a minus sign on negative numbers.

    // The '#' option causes the “alternate form” to be used for the conversion. 
    // The ',' option signals the use of a comma for a thousands separator for floating-point presentation types and for integer presentation type 'd'
    // he '_' option signals the use of an underscore for a thousands separator for floating-point presentation types and for integer presentation type 'd'. For integer presentation types 'b', 'o', 'x', and 'X', underscores will be inserted every 4 digits. For other presentation types, specifying this option is an error.

    std::string strRes;

    using namespace marty::format;
    FormattingOptions formattingOptions;

    auto outIt = marty::utf::UtfOutputIterator<char>(strRes);
    auto b = marty::utf::UtfInputIterator<char>(str);
    auto e = marty::utf::UtfInputIterator<char>();

    //State st = State::normalChars;

    using utf32_char_t = marty::utf::utf32_char_t;
    using utfch_t = marty::utf::utf32_char_t;

    bool parsingFormat = false; // else in processing text mode

    auto doFormat = [&]()
    {
        out << "\n" << formattingOptions << "\n" << umba::omanip::flush;

        parsingFormat = false;
    };

    auto finalizeParsing = [&](const std::string &msg) -> std::string
    {
        if (parsingFormat && !ignoreErrors)
            throw std::runtime_error(msg);
        if (parsingFormat)
            doFormat(); // Незакрытую форматную последовательность обрабатываем, как будто она закрыта
        return strRes;
    };

    auto optionalError = [&](const std::string &msg)
    {
        if (ignoreErrors)
            throw std::runtime_error(msg);
    };


    utfch_t ch = 0;
    if (b!=e)
        ch = *b;

    auto incB = [&]()
    {
        ++b;
        if (b!=e)
            ch = *b;
    };

    // b++;


    while(b!=e)
    {

        // Обычный режим - копируем символы со входа на выход, и ждем управляющие
        parsingFormat = false;
        formattingOptions = FormattingOptions{}; // сбрасываем опции форматирования на дефолтные

        while(b!=e)
        {
            if (ch==utfch_t('{') || ch==utfch_t('}'))
                break; // Встретили управляющий символ

            *outIt++ = ch;
            incB();
        }


        {
            bool bPevOpen = ch==utfch_t('{');
            incB();
            if (b==e)
            {
                if (bPevOpen)
                {
                    parsingFormat = true;
                    return finalizeParsing("unexpected end reached after format open bracket (incomplete format specification or unfinished escape sequence)");
                }
                else
                {
                    return finalizeParsing("unexpected end reached after format close bracket (unfinished escape sequence?)");
                }
            }

            //bool bOpen = *b==utfch_t('{');
            if (!bPevOpen) // предыдущая была закрывающей
            {
                if (ch==utfch_t('}')) // текущая - тоже закрывающая
                {
                    *outIt++ = ch; // Двойную закрывающую преобразуем в одинарную закрывающую
                    incB();
                    continue; // Продолжаем обычный разбор
                }

                // if (!ignoreErrors) // Или не реагируем на такие ошибки?
                //     throw std::runtime_error("invalid escape sequence");

                // Одинокую закрывающую скобку копируем как есть
                *outIt++ = utfch_t('}');
                *outIt++ = ch;
                incB();
                continue;
            }
            else // предыдущая была открывающей
            {
                if (ch==utfch_t('{')) // текущая - тоже открывающая
                {
                    *outIt++ = ch; // Двойную открывающую преобразуем в одинарную открывающую
                    incB();
                    continue; // Продолжаем обычный разбор
                }

                if (ch==utfch_t('}')) // текущая - закрывающая - это строка форматирования со ссылкой на аргумент, индекс которого вычисляется автоматически
                {
                    incB(); doFormat(); continue;
                }
            }

            // предыдущая скобка была открывающей, но второй открывающей скобки не последовало, как и закрывающей, значит, началась форматная строка
            // *b - уже символ форматной строки
            parsingFormat = true;
        }

        // Парсим форматную строку

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        // В качестве argId позволяем вообще всё
        // Это позволит использовать именованные параметры
        // Если argId конвертируется в число - то это числовой позиционный индекс
        // Если в качестве списка элементов - плоский вектор без имён, то тогда
        // именованные argId недопустимы
        {
            std::string tmp;
            auto argIdOutIt = marty::utf::UtfOutputIterator<char>(tmp /*formattingOptions.argId */ );
            while(b!=e)
            {
                if (ch==utfch_t(':') || ch==utfch_t('}'))
                    break;
                *argIdOutIt++ = ch;
                incB();
            }

            //if (!formattingOptions.argId.empty())
            //    formattingOptions.optionsFlags |= FormattingOptionsFlags::argIdTaken;
        }

        if (b==e) // Дошли до конца
            return finalizeParsing("unexpected end reached while reading ArgId");
        if (ch==utfch_t('}'))
        {
            incB(); doFormat(); continue;
        }

    continueFormatParsing:

        incB(); // Пропустили двоеточие

        if (b==e) // Дошли до конца
            return finalizeParsing("unexpected end reached while waiting for format spec");
        if (ch==utfch_t('}'))
        {
            incB(); doFormat(); continue;
        }


        if (!utils::isFormatAnySpecialChar(ch))
        {
            formattingOptions.fillChar = ch;
            formattingOptions.optionsFlags |= FormattingOptionsFlags::fillingTaken;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatAlignMarker(ch))
        {
            formattingOptions.alignment = char(ch); // флаги не нужны - выравнивание задано по умолчанию

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }


        if (utils::isFormatSignMarker(ch))
        {
            formattingOptions.optionsFlags |= (ch==utfch_t('+')) ? FormattingOptionsFlags::signPlus : FormattingOptionsFlags::signMinus;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }


        if (utils::isFormatAlterChar(ch))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::signAlterForm;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatDigit(ch, 0, 0))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::signZero;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }


        if (utils::isFormatDigit(ch, 1, 9))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken;
            formattingOptions.width   = marty::format::width_t(utils::toDigit(ch));

            incB();
            while(b!=e && utils::isFormatDigit(ch, 0, 9))
            {
                formattingOptions.width *= 10;
                formattingOptions.width += marty::format::width_t(utils::toDigit(ch));
                incB();
            }

            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }
        else if (ch==utfch_t('{'))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken | FormattingOptionsFlags::fieldWidthIndirect;
            std::string tmp;
            auto widthIdOutIt = marty::utf::UtfOutputIterator<char>(tmp /*formattingOptions.fieldWidthRef*/);

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec (WidthRef)");
            while(b!=e)
            {
                if (ch==utfch_t('}'))
                    break;
                *widthIdOutIt++ = ch;
                incB();
            }

            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec (WidthRef)");

            // Тут у нас закрывающая WidthRef скобка
            incB(); // Пропускаем закрывающую скобку
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatFormatThousandSep(ch))
        {
            formattingOptions.grouppingChar = ch;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatPeriodChar(ch))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionTaken;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('{')) // ref
            {
                formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionIndirect;
                std::string tmp;
                auto precisionIdOutIt = marty::utf::UtfOutputIterator<char>(tmp /*formattingOptions.precisionRef*/);

                incB();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec (PrecisionRef)");

                while(b!=e)
                {
                    if (ch==utfch_t('}'))
                        break;
                    *precisionIdOutIt++ = ch;
                    incB();
                }

                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec (PrecisionRef)");

                incB();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec");
                if (ch==utfch_t('}'))
                {
                    incB(); doFormat(); continue;
                }
            }
            else if (utils::isFormatDigit(ch, 0, 9))
            {
                formattingOptions.precision = marty::format::width_t(utils::toDigit(ch));

                incB();
                while(b!=e && utils::isFormatDigit(ch, 0, 9))
                {
                    formattingOptions.precision *= 10;
                    formattingOptions.precision += marty::format::width_t(utils::toDigit(ch));
                    incB();
                }

                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec");
                if (ch==utfch_t('}'))
                {
                    incB(); doFormat(); continue;
                }
            }

        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatLocaleChar(ch))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::localeFormatting;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        if (utils::isFormatTypeChar(ch))
        {
            formattingOptions.typeChar = char(ch);

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        if (ch==utfch_t('}'))
        {
            incB(); doFormat(); continue;
        }

        optionalError("unexpected symbol reached while parsing format spec");

        // incB();
        // if (b==e) // Дошли до конца
        //     return finalizeParsing("unexpected end reached while reading format spec");

        goto continueFormatParsing;

    }

    return strRes;

}


inline
std::string testParseFormatString(int nTest, const std::string& str, bool ignoreErrors = true)
{
    out << "\n";
    out << "Test #" << nTest << "\n" << umba::omanip::flush;
    out << "InputStr : \"" << str << "\"\n" << umba::omanip::flush;
    auto resStr = parseFormatString(str, ignoreErrors);
    out << "OutputStr: \"" << resStr << "\"\n";
    out << "-----\n\n";

    return resStr;
}


int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);


    bool ignoreErrors = false; // true;
    int nTest = 0;
    // );
    // , ignoreErrors);
    testParseFormatString(++nTest, "{:{}.{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{0:}", ignoreErrors);
    testParseFormatString(++nTest, "{:10.5f}", ignoreErrors);
    testParseFormatString(++nTest, "Т{:6}", ignoreErrors);
    testParseFormatString(++nTest, "{:6}", ignoreErrors);
    testParseFormatString(++nTest, "{:*<{}}", ignoreErrors);
    testParseFormatString(++nTest, "{:*>6}", ignoreErrors);
    testParseFormatString(++nTest, "{:*^6}", ignoreErrors);
    testParseFormatString(++nTest, "{:6d}", ignoreErrors);
    testParseFormatString(++nTest, "{:6}", ignoreErrors);
    testParseFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", ignoreErrors);
    testParseFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", ignoreErrors);
    testParseFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", ignoreErrors);
    testParseFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", ignoreErrors);
    testParseFormatString(++nTest, "{:+06d}", ignoreErrors);
    testParseFormatString(++nTest, "{:#06x}", ignoreErrors);
    testParseFormatString(++nTest, "{:<06}", ignoreErrors);
    testParseFormatString(++nTest, "{:10f}", ignoreErrors);
    testParseFormatString(++nTest, "{:{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:.5f}", ignoreErrors);
    testParseFormatString(++nTest, "{:.{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:.{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:.^5s}", ignoreErrors);
    testParseFormatString(++nTest, "{:.5s}", ignoreErrors);
    testParseFormatString(++nTest, "{:.<5.5s}", ignoreErrors);
    testParseFormatString(++nTest, "{:?}", ignoreErrors);
    testParseFormatString(++nTest, "Coordinates: {latitude}, {longitude}", ignoreErrors);
    testParseFormatString(++nTest, "{:<30}", ignoreErrors);
    testParseFormatString(++nTest, "{:>30}", ignoreErrors);
    testParseFormatString(++nTest, "{:^30}", ignoreErrors);
    testParseFormatString(++nTest, "{:*^30}", ignoreErrors);
    testParseFormatString(++nTest, "{:+f}; {:+f}", ignoreErrors);
    testParseFormatString(++nTest, "{: f}; {: f}", ignoreErrors);
    testParseFormatString(++nTest, "{:-f}; {:-f}", ignoreErrors);
    testParseFormatString(++nTest, "int: {0:d};  hex: {0:x};  oct: {0:o};  bin: {0:b}", ignoreErrors);
    testParseFormatString(++nTest, "int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", ignoreErrors);
    testParseFormatString(++nTest, "{:,}", ignoreErrors);
    testParseFormatString(++nTest, "{:_}", ignoreErrors);
    testParseFormatString(++nTest, "{:'}", ignoreErrors);
    testParseFormatString(++nTest, "Correct answers: {:.2%}", ignoreErrors);
    testParseFormatString(++nTest, "{:02X}{:02X}{:02X}{:02X}", ignoreErrors);

    // testParseFormatString(++nTest, "", ignoreErrors);
    // testParseFormatString(++nTest, "", ignoreErrors);
    // testParseFormatString(++nTest, "", ignoreErrors);


    return 0;
}


