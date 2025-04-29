/*! \file
    \brief Тестим LocaleInfo
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
#include "marty_format/locale_info.h"

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

#if 1
//
#include "log.h"
//
#include "AppConfig.h"

AppConfig appConfig;

#include "ArgParser.h"
#endif


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

using namespace marty::format;

LocaleInfo::group_info_t grp_3_0     = LocaleInfo::parseGroupingString("3;0");   // 3;0     3,000,000,000,000 
LocaleInfo::group_info_t grp_3_2_0   = LocaleInfo::parseGroupingString("3;2;0"); // 3;2;0   30,00,00,00,00,000
LocaleInfo::group_info_t grp_3       = LocaleInfo::parseGroupingString("3");     // 3       3000000000,000    
LocaleInfo::group_info_t grp_3_2     = LocaleInfo::parseGroupingString("3;2");   // 3;2     30000000,00,000   
LocaleInfo::group_info_t grp_3_0_0   = LocaleInfo::parseGroupingString("3;0;0"); // Требует фикса - ноль не должен быть в середине -> 3;3;0

inline
std::string makeNumberGroupped(const std::string &str, const LocaleInfo::group_info_t &grpInfo)
{
    std::string res; res.reserve(str.size());

    for(std::size_t i=0; i!=str.size(); ++i)
    {
        if (LocaleInfo::testAppendGroupSeparator(i, grpInfo))
            res.append(1, ',');
            //res.append(1, '\'');
        res.append(1u, str[str.size()-i-1u]);
    }

    std::reverse(res.begin(), res.end());
    return res;
}

inline
void testMakeNumberGroupped(const std::string &str, LocaleInfo::group_info_t &grpInfo, const std::string &expectedGrp, const std::string &expectedRes)
{
    auto composedGroupingString = LocaleInfo::composeGroupingString(grpInfo);
    auto grouppedNumber         = makeNumberGroupped(str, grpInfo);
    std::cout << str << " + " << composedGroupingString << " -> " << grouppedNumber << ", expected: " << expectedGrp << " -> " << expectedRes << "\n";
}

// std::string composeGroupingString(const group_info_t &grpInfo)
// marty::format::


int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    std::string str = "3000000000000";

    testMakeNumberGroupped(str, grp_3_0  , "3;0"  , "3,000,000,000,000");
    testMakeNumberGroupped(str, grp_3_2_0, "3;2;0", "30,00,00,00,00,000");
    testMakeNumberGroupped(str, grp_3    , "3"    , "3000000000,000");
    testMakeNumberGroupped(str, grp_3_2  , "3;2"  , "30000000,00,000   ");
    testMakeNumberGroupped(str, grp_3_0_0, "3;0;0", "3,000,000,000,000");
    //testMakeNumberGroupped(str, , , "");

    return 0;
}


