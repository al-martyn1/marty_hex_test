/*! \file
    \brief Тестим функцию форматирования с аргументами в виде marty::format::Args
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




// , typename ArgumentVariantType=FormatArgumentVariant
// , typename VectorType=std::vector<ArgumentVariantType>
// , typename MapType=std::unordered_map<StringType, std::size_t>
// , typename ArgsType = Args<ArgumentVariantType, VectorType, MapType>
//  
//  
// using Args
// marty::format::Args<ArgumentVariantType, VectorType, MapType>

inline
std::string testFormatString(int nTest, const std::string& fmt, const marty::format::Args &args)
{
    out << "\n";
    out << "Test #" << nTest << "\n" << umba::omanip::flush;
    out << "InputStr : \"" << fmt << "\"\n" << umba::omanip::flush;
    out << "Args: ";
    for(auto && valToFormat : args)
    {
        auto val = std::visit( [](auto && a) -> std::string
                               {
                                   return marty::format::martyFormatSimpleConvertToString<std::string>(a);
                               }
                             , valToFormat
                             );
        out << val << ", ";
    }
    out << "\n";

    auto resStr = marty::format::formatMessage( fmt, args );

    out << "OutputStr: \"" << resStr << "\"\n";
    out << "-----\n\n";

    return resStr;
}




int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    out << "Test initializer_list\n";
    out << marty::format::formatMessage( std::string("{} {0} {} {}"), {0, 1, 2, 3} );
    out << "\n";
    out << "\n";


    using Args = marty::format::Args;

    int nTest = 0;
    // );
    // , ignoreErrors);
    testFormatString(++nTest, "{:{}.{}f}", Args().arg(1.5f).arg(2).arg(3) );
    testFormatString(++nTest, "{0:}", Args().arg(10));
#if 1
    testFormatString(++nTest, "{:10.5f}", Args().arg(31.4159));
    testFormatString(++nTest, "Текст и {:6}", Args().arg("строка"));
    testFormatString(++nTest, "{:6}", Args().arg(L"L\"Широкая строка\""));
    testFormatString(++nTest, "{:*<{}}", Args().arg(99));
    testFormatString(++nTest, "{:*>6}", Args().arg(99));
    testFormatString(++nTest, "{:*^6}", Args().arg(99));
    testFormatString(++nTest, "{:6d}", Args().arg(99));
    testFormatString(++nTest, "{:6}", Args().arg(99));
    testFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", Args().arg(99));
    testFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", Args().arg(99));
    testFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", Args().arg(99));
    testFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", Args().arg(99));
    testFormatString(++nTest, "{:+06d}", Args().arg(99));
    testFormatString(++nTest, "{:#06x}", Args().arg(99));
    testFormatString(++nTest, "{:<06}", Args().arg(99));
    testFormatString(++nTest, "{:10f}", Args().arg(99.9));
    testFormatString(++nTest, "{:{}f}", Args().arg(3).arg(99.9));
    testFormatString(++nTest, "{:.5f}", Args().arg(99.9));
    testFormatString(++nTest, "{:.{}f}", Args().arg(3).arg(99.9));
    testFormatString(++nTest, "{:{}f}", Args().arg(3).arg(99.9));
    testFormatString(++nTest, "{:.^5s}", Args().arg("123"));
    testFormatString(++nTest, "{:.5s}", Args().arg("123"));
    testFormatString(++nTest, "{:.<5.5s}", Args().arg("1234567"));
    testFormatString(++nTest, "{:?}", Args().arg("123456")); // !!! Тут надо потом вставить непечатные символы, чтобы проверить escape
    testFormatString(++nTest, "Coordinates: {latitude}, {longitude}", Args().arg("latitude", "59.958516").arg("longitude", "30.312167"));   // 59.958516, 30.312167
    testFormatString(++nTest, "{:<30}", Args().arg(123)); // Ширина 30?
    testFormatString(++nTest, "{:>30}", Args().arg("1234567"));
    testFormatString(++nTest, "{:^30}", Args().arg(99.9));
    testFormatString(++nTest, "{:*^30}", Args().arg(999));
    testFormatString(++nTest, "{:+f}; {:+f}", Args().arg(1.1).arg(2.2));
    testFormatString(++nTest, "{: f}; {: f}", Args().arg(1.1).arg(2.2));
    testFormatString(++nTest, "{:-f}; {:-f}", Args().arg(1.1).arg(2.2));
    testFormatString(++nTest, "int: {0:d};  hex: {0:x};  oct: {0:o};  bin: {0:b}", Args().arg(12345));
    testFormatString(++nTest, "int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", Args().arg(12345));
    testFormatString(++nTest, "{:,}", Args().arg(1234567));
    testFormatString(++nTest, "{:_}", Args().arg(1234567));
    testFormatString(++nTest, "{:'}", Args().arg(1234567));
    testFormatString(++nTest, "Correct answers: {:.2%}", Args().arg(0.75));
    testFormatString(++nTest, "{:02X}{:02X}{:02X}{:02X}", Args().arg(123).arg(456).arg(789).arg(912));

    // testFormatString(++nTest, "", Args().arg());
    // testFormatString(++nTest, "", Args().arg());
    // testFormatString(++nTest, "", Args().arg());
#endif

    return 0;
}


