/*! \file
    \brief Парсим форматную строку - без автомата (функцию вынес в библиотеку)
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






inline
std::string testParseFormatString(int nTest, const std::string& str, bool ignoreErrors = true)
{
    out << "\n";
    out << "Test #" << nTest << "\n" << umba::omanip::flush;
    out << "InputStr : \"" << str << "\"\n" << umba::omanip::flush;

    auto resStr = marty::format::processFormatStringImpl<std::string>( str.begin(), str.end()
                                                        , [&](const marty::format::FormattingOptions &formattingOptions) -> std::string
                                                          {
                                                              out << "\n" << formattingOptions << "\n" << umba::omanip::flush;
                                                              return "777";
                                                          }
                                                        , [&](const char* pb, const char* pe, marty::format::FormatIndexType indexType)
                                                          {
                                                              MARTY_ARG_USED(pb);
                                                              MARTY_ARG_USED(pe);
                                                              MARTY_ARG_USED(indexType);
                                                              return std::size_t(0);
                                                          }
                                                        , ignoreErrors
                                                        );

    out << "OutputStr: \"" << resStr << "\"\n";
    out << "-----\n\n";

    return resStr;
}



template<typename T>
std::string getValueTypeName()
{
    //const std::type_info& ti = typeid(typename marty::format::ContainerValueTypeDeducer<ContainerType>::value_type);
    const std::type_info& ti = typeid(T);
    return ti.name();
}

template<typename T>
void printHasFeatures()
{
    const std::type_info& ContainerTi = typeid(T);
    out << "Container: " << ContainerTi.name() << "\n";
    out << "Container::value_type: " << getValueTypeName<typename T::value_type>() << "\n";
    out << "Deduced value_type: " << getValueTypeName< typename marty::format::ContainerValueTypeDeducer<T>::value_type >() << "\n";

    out << "\n";

    if (marty::format::utils::has_first_type<typename T::value_type>::value)
        out << "value_type has first_type\n";
    else
        out << "value_type has NO first_type\n";

    if (marty::format::utils::has_second_type<typename T::value_type>::value)
        out << "value_type has second_type\n";
    else
        out << "value_type has NO second_type\n";

    if (marty::format::utils::has_string_find<T>::value)
        out << "Container has string find\n";
    else
        out << "Container has NO string find\n";

    if (marty::format::utils::has_size_t_find<T>::value)
        out << "Container has size_t find\n";
    else
        out << "Container has NO size_t find\n";

    if (marty::format::utils::has_find_by_pos<T>::value)
        out << "Container has has_find_by_pos\n";
    else
        out << "Container has NO has_find_by_pos\n";

    if (marty::format::utils::is_range<T>::value)
        out << "Container is range\n";
    else
        out << "Container is NOT range\n";

    out << "\n\n\n";
}

template<typename ContainerType>
void testMartyFormatValueGetter(const ContainerType &c, const std::string &argId)
{
    out << "c[\"" << argId << "\"] = " << umba::omanip::flush;
    std::size_t idxDefault = 0;
    auto val = marty::format::MartyFormatValueGetter<ContainerType>()(c, argId, idxDefault);
    out << "\"" << val << "\"" << "\n";
}


int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    using Args = marty::format::Args;

    #if 0
    if (marty::format::has_string_find<Args>::value)
    {
        out << "Args has string find\n";
    }
    else
    {
        out << "Args has NO string find\n";
    }

    if (marty::format::has_size_t_find<Args>::value)
    {
        out << "Args has size_t find\n";
    }
    else
    {
        out << "Args has NO size_t find\n";
    }

    if (marty::format::has_second_type<Args::value_type>::value)
    {
        out << "Args::value_type has second_type\n";
    }
    else
    {
        out << "Args::value_type has NO second_type\n";
    }

    out << "\n\n";
    #endif

// , typename std::enable_if< has_string_find<ContainerType>::value 
//                      && has_size_t_find<ContainerType>::value
//                      && has_second_type<typename ContainerType::value_type>::value


    using StringToIntKeyValue       = std::pair<std::string, int>;
    using StringToIntKeyValueVector = std::vector<StringToIntKeyValue>;
    using StringVector              = std::vector<std::string>;

    printHasFeatures<Args>();
    printHasFeatures<StringToIntKeyValueVector>();
    printHasFeatures<StringVector>();
    printHasFeatures< std::map<std::string, std::string> >();
    printHasFeatures< std::vector< std::pair<std::string, std::string> > >();
    printHasFeatures< std::vector< std::string > >();
    // printHasFeatures<  >();
    // printHasFeatures<  >();

    // out << "Value type name for StringToIntKeyValueVector: " << getValueTypeName<StringToIntKeyValueVector>() << ", container::value_type";
    // printHasFirstSecondTypes<typename StringToIntKeyValueVector::value_type>();
    // out << "\n";
    //  
    // out << "Value type name for StringVector             : " << getValueTypeName<StringVector>() << ", container::value_type";
    // printHasFirstSecondTypes<typename StringVector::value_type>();
    // out << "\n";
    //  
    // out << "\n";
    // out << "\n";

    std::map<std::string, std::string> m = 
        { {"m1", "MapArg1"}
        , {"m2", "MapArg2"}
        , {"m3", "MapArg3"}
        , {"m4", "MapArg4"}
        , {"m5", "MapArg5"}
        , {"m6", "MapArg6"}
        , {"m7", "MapArg7"}
        , {"m8", "MapArg8"}
        };

    std::vector< std::pair<std::string, std::string> > vp = 
        { {"v1", "VpArg1"}
        , {"v2", "VpArg2"}
        , {"v3", "VpArg3"}
        , {"v4", "VpArg4"}
        , {"v5", "VpArg5"}
        , {"v6", "VpArg6"}
        , {"v7", "VpArg7"}
        , {"v8", "VpArg8"}
        };

    std::vector< std::string > vs = 
        { "VsArg1"
        , "VsArg2"
        , "VsArg3"
        , "VsArg4"
        , "VsArg5"
        , "VsArg6"
        , "VsArg7"
        , "VsArg8"
        };

    
    auto stdArgs = Args().arg("a1", "SaArg1")
                         .arg("a2", "SaArg2")
                         .arg("a3", "SaArg3")
                         .arg("a4", "SaArg4")
                         .arg("a5", "SaArg5")
                         .arg("a6", "SaArg6")
                         .arg("a7", "SaArg7")
                         .arg("a8", "SaArg8")
                         ;

    UMBA_USED(m);
    UMBA_USED(vp);
    UMBA_USED(vs);
    UMBA_USED(stdArgs);
    //UMBA_USED();

    // Initializer list
    // testMartyFormatValueGetter({std::string("i1"), std::string("i2"), std::string("i3")}, "1");

    testMartyFormatValueGetter(m, "3");
    testMartyFormatValueGetter(m, "m6");
    testMartyFormatValueGetter(vp, "3");
    testMartyFormatValueGetter(vp, "v6");
    testMartyFormatValueGetter(vs, "3");
    testMartyFormatValueGetter(vs, "6");
    // testMartyFormatValueGetter(stdArgs, "a3");
    // testMartyFormatValueGetter(stdArgs, "6");
    // testMartyFormatValueGetter(vs, "m6");

#if 1

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
#endif

    return 0;
}


