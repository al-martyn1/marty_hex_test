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
    // std::string res; res.reserve(str.size());
    //  
    // for(std::size_t i=0; i!=str.size(); ++i)
    // {
    //     if (LocaleInfo::testAppendGroupSeparator(i, grpInfo))
    //         res.append(1, ',');
    //         //res.append(1, '\'');
    //     res.append(1u, str[str.size()-i-1u]);
    // }
    //  
    // std::reverse(res.begin(), res.end());
    // return res;

    return LocaleInfo::insertGroupSeparators(str, std::string(1, ','), grpInfo, false /* bFractionalPart */ );
}

inline
void testMakeNumberGroupped(const std::string &str, LocaleInfo::group_info_t &grpInfo, const std::string &expectedGrp, const std::string &expectedRes)
{
    auto composedGroupingString = LocaleInfo::composeGroupingString(grpInfo);
    auto grouppedNumber         = makeNumberGroupped(str, grpInfo);
    std::cout << str << " + " << composedGroupingString << " -> " << grouppedNumber << ", expected: " << expectedGrp << " -> " << expectedRes << "\n";
}

inline
void printLocaleInfoGroupping(const LocaleInfo *pLocaleInfo, NumeralSystem ns)
{
    auto grpInt  = LocaleInfo::composeGroupingString(pLocaleInfo->getGroupInfo(ns, false));
    auto grpFrac = LocaleInfo::composeGroupingString(pLocaleInfo->getGroupInfo(ns, true ));
    std::cout << enum_serialize(ns) << ", int part grouping: " << grpInt << ", fractional part grouping: " << grpFrac << "\n";
}

#define EXPAND_TO_NUM_DIGITS

inline
void testPrintExpandWithGroupSeparator(std::string numStrBase)
{
    std::cout << "-------\n";
    std::cout << "Base number string: " << numStrBase << "\n";

    LocaleInfo::group_info_t grpInfo = LocaleInfo::parseGroupingString("2;0");
    std::string sep = "'";

    #ifndef EXPAND_TO_NUM_DIGITS
    std::size_t sepLen       = sep.size();
    #endif
    std::size_t maxLen       = 13;

    // std::string numStr       = numStrBase;

    // auto firstTimeExpandedStr = LocaleInfo::insertGroupSeparators(numStrBase, sep, grpInfo, false /* bFractionalPart */ );
    // std::cout << "First Time Expanded Str: " << firstTimeExpandedStr << "\n";

    for(std::size_t i=3; i!=maxLen+2; ++i)
    {
        auto iStr = utils::expandBeforeUpTo(std::to_string(i), std::to_string(i).size(), 3, ' ');
        std::cout << "  expand to " << iStr /* i */  << "  : ";

        std::string numStr = LocaleInfo::insertGroupSeparators(numStrBase, sep, grpInfo, false /* bFractionalPart */ );

        std::size_t digitsCount = numStrBase.size();

        #ifndef EXPAND_TO_NUM_DIGITS
        std::size_t strLen      = numStrBase.size();
        numStr = LocaleInfo::expandWithGroupSeparator( numStr, sep, grpInfo
                                                     , false  // !bFractionalPart
                                                     , sepLen // sepCalculatedLen
                                                     , strLen
                                                     , digitsCount
                                                     , i // maxLen
                                                     );
        #else
        numStr = LocaleInfo::expandWithGroupSeparatorToNumDigits( numStr, sep, grpInfo
                                                     , false  // !bFractionalPart
                                                     , digitsCount
                                                     , i // maxLen
                                                     );
        #endif

        auto numStrPrint = utils::expandBeforeUpTo(numStr, numStr.size(), 20, ' ');
        std::cout << numStrPrint;

        std::cout << "\n";
    }



    // static std::string expandWithGroupSeparator( std::string numStr, std::string sep, const group_info_t &grpInfo
    //                                            , bool bFractionalPart
    //                                            , std::size_t sepCalculatedLen  // Посчитанная снаружи длина разделителя
    //                                            , std::size_t &numStrLen // Посчитанная снаружи полная длина строки, которую дополняем, включая сепараторы
    //                                            , std::size_t &digitsCount
    //                                            , std::size_t maxLen
    //                                            );

 
}


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


    auto pLocaleInfoInvariant = getLocaleInfo(LocaleInfoType::invariant);
    // auto pLocaleInfoUser      = getLocaleInfo(LocaleInfoType::user);

    std::cout << "-----------------------\n";
    std::cout << "Grouping\n";

    printLocaleInfoGroupping(pLocaleInfoInvariant, NumeralSystem::currency);
    printLocaleInfoGroupping(pLocaleInfoInvariant, NumeralSystem::bin     );
    printLocaleInfoGroupping(pLocaleInfoInvariant, NumeralSystem::hex     );
    printLocaleInfoGroupping(pLocaleInfoInvariant, NumeralSystem::oct     );
    printLocaleInfoGroupping(pLocaleInfoInvariant, NumeralSystem::dec     );

    std::cout << "-----------------------\n";
    std::cout << "Expanding With Group Separator\n";
    testPrintExpandWithGroupSeparator("3");

    return 0;
}


