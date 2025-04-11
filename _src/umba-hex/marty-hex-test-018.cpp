/*! \file
    \brief Парсим форматную строку (старая версия, поломана)
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
    UMBA_USED(ignoreErrors);

    enum class State
    {
        normalChars,
        formatCharFound,
        closingEscapeFound,
        readingArgId,
        startReadingOptions,
        readingSign,
        readingAlter,
        readingZero,
        readingWidth,
        waitPrecision,
        readingPrecision,
        //readingPrecisionStart,
        //readingLocaleMark,
        readingTypeChar,
        readingIndirectWidth,
        readingWidthDirect,
        readClosingBrace
    };

    std::string strRes;

    using namespace marty::format;
    FormattingOptions formattingOptions;

    auto outIt = marty::utf::UtfOutputIterator<char>(strRes);
    auto b = marty::utf::UtfInputIterator<char>(str);
    auto e = marty::utf::UtfInputIterator<char>();

    State st = State::normalChars;

    using utf32_char_t = marty::utf::utf32_char_t;
    using utfch_t = marty::utf::utf32_char_t;

    auto doFormat = [&]()
    {
        out << "\n" << formattingOptions << "\n";
    
    };

    auto finalizeReadAlignMarker_goToReadingSign = [&](utfch_t ch)
    {
        // Символа заполнения нет, сразу задан символ выравнивания
        //formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::alignTaken;
        formattingOptions.alignment     = char(ch);
        st = State::readingSign;
    };

    auto finalizeReadSignMarker_goToReadingAlter = [&](utfch_t ch)
    {
        formattingOptions.optionsFlags |= (ch==utfch_t('-')) ? marty::format::FormattingOptionsFlags::signMinus : marty::format::FormattingOptionsFlags::signPlus;
        st = State::readingAlter;
    };

    auto finalizeReadAlter_goToReadingZero = [&]()
    {
        formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::signAlterForm;
        st = State::readingZero;
    };

    auto finalizeReadZero_goToReadingWidth = [&]()
    {
        formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::signZero;
        st = State::readingWidth;
    };

    auto finalizeFirstWidthDigit_goToReadingWidthDirect = [&](utfch_t ch)
    {
        formattingOptions.fieldWidth = marty::format::utils::toDigit(ch);
        formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::fieldWidthTaken;
        st = State::readingWidthDirect;
    };

    auto foundIndirectWidthOpenBracket_goToReadingIndirectWidth = [&]()
    {
        formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::fieldWidthIndirect;
        formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::fieldWidthTaken;
        st = State::readingIndirectWidth;
    };

    auto foundPeriodChar_goToReadingPrecision = [&]()
    {
        formattingOptions.precision = 0; // Сбрасываем дефолтную точность в ноль
        formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::precisionTaken;
        st = State::readingPrecision;
    };

    auto foundLocaleChar_goToReadingTypeChar = [&]()
    {
        formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::localeFormatting;
        st = State::readingTypeChar;
    };

    auto foundTypeChar_goToReadClosingBrace = [&](utfch_t ch)
    {
        formattingOptions.typeChar = char(ch);
        st = State::readClosingBrace;
    };

    // else if (utils::isFormatCloseBrace(ch))

    auto foundCloseBrace_doFormat_doReset = [&]()
    {
        doFormat();
        st = State::normalChars;
    };

    // Порядок
    // Fill and align
    // Sign, #, and 0
    // Width and precision
    // Type char: s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G

    // https://perveevm.ru/docs/cpp/reference/en/cpp/utility/format/formatter.html#Standard_format_specification
    // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		

    // https://www.modernescpp.com/index.php/the-formatting-library-in-c20-the-format-string/


    for(; b!=e; ++b)
    {
        utf32_char_t ch = *b;

        switch(st)
        {
            //------------------------------
            case State::normalChars:
            {
                if (ch==utfch_t('{'))
                    st = State::formatCharFound;
                else if (ch==utfch_t('}'))
                    st = State::closingEscapeFound;
                else
                    *outIt++ = ch;
            }
            break;

            //------------------------------
            case State::closingEscapeFound:
            {
                if (ch!=utfch_t('}'))
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("invalid '{' escape sequence");
                    // Игнорим ошибку
                }
                *outIt++ = ch;
                st = State::normalChars;
            }
            break;

            //------------------------------
            case State::formatCharFound:
            {
                if (ch!=utfch_t('{'))
                {
                   formattingOptions = marty::format::FormattingOptions{};
                   st = State::readingArgId;
                   break;
                }
                // Просто экранированная скобка
                *outIt++ = ch;
                st = State::normalChars;
            }
            break;

            //------------------------------
            case State::readingArgId:
            {
                int d = marty::format::utils::toDigit(ch);
                if (d<0)
                {
                    if (ch==utfch_t('}'))
                    {
                        // if (formattingOptions.argId<1 && !ignoreErrors)
                        //     throw std::runtime_error("invalid argId");
                        doFormat();
                        st = State::normalChars;
                    }
                    else if (ch!=utfch_t(':'))
                    {
                        if (!ignoreErrors)
                            throw std::runtime_error("invalid argId, waiting for options start, but found unexpected char");
                        goto startReadingOptionsLabel;
                    }

                    st = State::startReadingOptions;
                }
                else
                {
                    // formattingOptions.argId *= 10;
                    // formattingOptions.argId += d;
                    // Помечаем, что argId был задан.
                    // Если при этом argId равен или меньше нуля - то это неверно заданный argId
                    formattingOptions.optionsFlags = marty::format::FormattingOptionsFlags::argIdTaken;
                    // formattingOptions fieldWidthTaken
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            startReadingOptionsLabel:
            case State::startReadingOptions:
            {
                if (utils::isFormatAlignMarker(ch)) // '<', '>', '^'
                    finalizeReadAlignMarker_goToReadingSign(ch);

                else if (utils::isFormatSignMarker(ch)) // '+'-'-'
                    finalizeReadSignMarker_goToReadingAlter(ch);

                else if (utils::isFormatAlterChar(ch)) // '#'
                    finalizeReadAlter_goToReadingZero();

                else if (utils::isFormatDigit(ch,0,0)) // '0'
                    finalizeReadZero_goToReadingWidth();

                else if (utils::isFormatDigit(ch,1,9)) // '1'-'9'
                    finalizeFirstWidthDigit_goToReadingWidthDirect(ch);

                else if (utils::isFormatOpenBrace(ch)) // Indirect width starts ('{')
                    foundIndirectWidthOpenBracket_goToReadingIndirectWidth();

                else if (utils::isFormatPeriodChar(ch)) // '.'
                    foundPeriodChar_goToReadingPrecision();

                else if (utils::isFormatLocaleChar(ch)) // 'L'
                    foundLocaleChar_goToReadingTypeChar();

                else if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                    foundCloseBrace_doFormat_doReset();

                else
                {
                    if ((formattingOptions.optionsFlags&marty::format::FormattingOptionsFlags::fillingTaken)!=0)
                    {
                        if (!ignoreErrors)
                            throw std::runtime_error("too many fill chars taken");
                    }

                    formattingOptions.fillChar = ch;
                    formattingOptions.optionsFlags |= marty::format::FormattingOptionsFlags::fillingTaken;
                    // Состояние не меняем
                }

            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::readingSign:
            {
                if (utils::isFormatSignMarker(ch)) // '+'-'-'
                    finalizeReadSignMarker_goToReadingAlter(ch);

                else if (utils::isFormatAlterChar(ch)) // '#'
                    finalizeReadAlter_goToReadingZero();

                else if (utils::isFormatDigit(ch,0,0)) // '0'
                    finalizeReadZero_goToReadingWidth();

                else if (utils::isFormatDigit(ch,1,9)) // '1'-'9'
                    finalizeFirstWidthDigit_goToReadingWidthDirect(ch);

                else if (utils::isFormatOpenBrace(ch)) // Indirect width starts ('{')
                    foundIndirectWidthOpenBracket_goToReadingIndirectWidth();

                else if (utils::isFormatPeriodChar(ch)) // '.'
                    foundPeriodChar_goToReadingPrecision();

                else if (utils::isFormatLocaleChar(ch)) // 'L'
                    foundLocaleChar_goToReadingTypeChar();

                else if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                    foundCloseBrace_doFormat_doReset();

                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while waiting for sign");
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::readingAlter:
            {
                if (utils::isFormatAlterChar(ch)) // '#'
                    finalizeReadAlter_goToReadingZero();

                else if (utils::isFormatDigit(ch,0,0)) // '0'
                    finalizeReadZero_goToReadingWidth();

                else if (utils::isFormatDigit(ch,1,9)) // '1'-'9'
                    finalizeFirstWidthDigit_goToReadingWidthDirect(ch);

                else if (utils::isFormatOpenBrace(ch)) // Indirect width starts ('{')
                    foundIndirectWidthOpenBracket_goToReadingIndirectWidth();

                else if (utils::isFormatPeriodChar(ch)) // '.'
                    foundPeriodChar_goToReadingPrecision();

                else if (utils::isFormatLocaleChar(ch)) // 'L'
                    foundLocaleChar_goToReadingTypeChar();

                else if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                    foundCloseBrace_doFormat_doReset();

                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while waiting for alter ('#') sign");
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::readingZero:
            {
                if (utils::isFormatDigit(ch,0,0)) // '0'
                    finalizeReadZero_goToReadingWidth();

                else if (utils::isFormatDigit(ch,1,9)) // '1'-'9'
                    finalizeFirstWidthDigit_goToReadingWidthDirect(ch);

                else if (utils::isFormatOpenBrace(ch)) // Indirect width starts ('{')
                    foundIndirectWidthOpenBracket_goToReadingIndirectWidth();

                else if (utils::isFormatPeriodChar(ch)) // '.'
                    foundPeriodChar_goToReadingPrecision();

                else if (utils::isFormatLocaleChar(ch)) // 'L'
                    foundLocaleChar_goToReadingTypeChar();

                else if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                    foundCloseBrace_doFormat_doReset();

                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while waiting for alter ('#') sign");
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::readingWidth:
            {
                if (utils::isFormatDigit(ch,1,9)) // '1'-'9'
                    finalizeFirstWidthDigit_goToReadingWidthDirect(ch);

                else if (utils::isFormatOpenBrace(ch)) // Indirect width starts ('{')
                    foundIndirectWidthOpenBracket_goToReadingIndirectWidth();

                else if (utils::isFormatPeriodChar(ch)) // '.'
                    foundPeriodChar_goToReadingPrecision();

                else if (utils::isFormatLocaleChar(ch)) // 'L'
                    foundLocaleChar_goToReadingTypeChar();

                else if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                {
                    doFormat();
                    st = State::normalChars;
                }
                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while reading field width");
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::readingWidthDirect:
            {
                if (utils::isFormatDigit(ch))
                {
                    formattingOptions.fieldWidth   *= 10;
                    formattingOptions.fieldWidth   += marty::format::utils::toDigit(ch);
                }
                else if (utils::isFormatPeriodChar(ch)) // '.'
                    foundPeriodChar_goToReadingPrecision();

                else if (utils::isFormatLocaleChar(ch)) // 'L'
                    foundLocaleChar_goToReadingTypeChar();

                else if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                {
                    doFormat();
                    st = State::normalChars;
                }
                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while reading field width");
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::readingIndirectWidth:
            {
                if (utils::isFormatDigit(ch)) // '1'-'9'
                {
                    formattingOptions.fieldWidth   *= 10;
                    formattingOptions.fieldWidth   += marty::format::utils::toDigit(ch);
                }
                else if (utils::isFormatCloseBrace(ch))
                {
                    // Заканчиваем чтение индекса ширины
                    st = State::waitPrecision;
                }
                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while reading indirect field width");
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::waitPrecision:
            {
                if (utils::isFormatPeriodChar(ch)) // '.'
                    foundPeriodChar_goToReadingPrecision();

                else if (utils::isFormatLocaleChar(ch)) // 'L'
                    foundLocaleChar_goToReadingTypeChar();

                else if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                {
                    doFormat();
                    st = State::normalChars;
                }
                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while reading field width");
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::readingPrecision:
            {
                if (utils::isFormatDigit(ch))
                {
                    formattingOptions.precision   *= 10;
                    formattingOptions.precision   += marty::format::utils::toDigit(ch);
                }
                else if (utils::isFormatLocaleChar(ch)) // 'L'
                    foundLocaleChar_goToReadingTypeChar();

                else if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                {
                    doFormat();
                    st = State::normalChars;
                }
                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while reading field precision");
                }
            }
            break;

            //------------------------------
            // fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)		
            case State::readingTypeChar:
            {
                if (utils::isFormatTypeChar(ch)) // s, ?, b, B, c, d, o, x, X, a, A, e, E, f, F, g, G
                    foundTypeChar_goToReadClosingBrace(ch);

                else if (utils::isFormatCloseBrace(ch))
                {
                    doFormat();
                    st = State::normalChars;
                }
                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while reading type option");
                }
            }
            break;

            //------------------------------
            case State::readClosingBrace:
            {
                if (utils::isFormatCloseBrace(ch))
                {
                    doFormat();
                    st = State::normalChars;
                }
                else
                {
                    if (!ignoreErrors)
                        throw std::runtime_error("unexpected symbol while reading format closing bracket");
                }
            }
            break;

        }
    }

    if (st!=State::normalChars)
    {
        if (!ignoreErrors)
            throw std::runtime_error("incomplete format string");

        if (st==State::formatCharFound || st==State::closingEscapeFound)
        {}
        else
        {
            doFormat();
            st = State::normalChars;
        }
    }
    // enum class State
    // {
    //     normalChars,
    //     formatCharFound,
    //     closingEscapeFound,


    return strRes;
}


inline
std::string testParseFormatString(int nTest, const std::string& str, bool ignoreErrors = true)
{
    out << "\n";
    out << "Test #" << nTest << "\n";
    out << "InputStr : \"" << str << "\"\n";
    auto resStr = parseFormatString(str, ignoreErrors);
    out << "OutputStr: \"" << resStr << "\"\n";
    out << "-----\n\n";

    return resStr;
}


int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    {
        std::string strTestUtfInputIter = "Тут у нас что-то по-русски";
        auto b = marty::utf::UtfInputIterator<char>(strTestUtfInputIter);
        auto e = marty::utf::UtfInputIterator<char>();

        std::string strUtfTextCopy;
        auto outIt = marty::utf::UtfOutputIterator<char>(strUtfTextCopy);

        using namespace umba::omanip;

        for(; b!=e; ++b)
        {
            auto ch = *b;
            *outIt++ = ch;
            out << hex << width(4) << ch << "\n";
        }

        out << "InputStr: " << strTestUtfInputIter << "\n";
        out << "CopyStr : " << strUtfTextCopy << "\n";

    }


    //marty::format::FormattingOptions formattingOptions;

    /*
         Предполагается, что у нас UTF-8
         {:+06d}
         {:#06x}
         {:.<5.5s}

     */

    bool ignoreErrors = false; // true;
    int nTest = 0;
    // );
    // , ignoreErrors);
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
    testParseFormatString(++nTest, "{:10.5f}", ignoreErrors);
    testParseFormatString(++nTest, "{:{}.{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:.{}f}", ignoreErrors);
    testParseFormatString(++nTest, "{:.^5s}", ignoreErrors);
    testParseFormatString(++nTest, "{:.5s}", ignoreErrors);
    testParseFormatString(++nTest, "{:.<5.5s}", ignoreErrors);
    testParseFormatString(++nTest, "{:?}", ignoreErrors);


    return 0;
}


