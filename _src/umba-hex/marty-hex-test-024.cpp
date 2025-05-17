/*! \file
    \brief Тестим marty::BigInt
 */


#define MARTY_BIGINT_FORCE_NUMBER_UNDERLYING_TYPE  std::uint8_t

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

#include "marty_bigint/marty_bigint.h"

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

inline
std::string mkMarker(bool bGood)
{
    return std::string(bGood ? "+" : "-") + "   ";
}

inline
bool testBigIntPlus(int &nPassed, std::int64_t i1, std::int64_t i2)
{
    std::int64_t  iRes = std::int64_t(i1+i2);
    marty::BigInt bRes = marty::BigInt(i1) + marty::BigInt(i2);

    using std::to_string;

    auto iStr  = to_string(iRes);
    auto bStr  = to_string(bRes);

    bool bGood = iStr==bStr;

    std::cout << mkMarker(bGood);
    std::cout << i1 << " + " << i2 << " = " << iStr;

    if (bGood)
    {
        std::cout << " - passed\n";
    }
    else
    {
        std::cout << " - failed, result: " << bStr << "\n";
    }

    if (bGood)
       ++nPassed;

    return bGood;
}

inline
bool testBigIntMinus(int &nPassed, std::int64_t i1, std::int64_t i2)
{
    std::int64_t  iRes = std::int64_t(i1+i2);
    marty::BigInt bRes = marty::BigInt(i1) + marty::BigInt(i2);

    using std::to_string;

    auto iStr  = to_string(iRes);
    auto bStr  = to_string(bRes);

    bool bGood = iStr==bStr;

    std::cout << mkMarker(bGood);
    std::cout << i1 << " - " << i2 << " = " << iStr;

    if (bGood)
    {
        std::cout << " - passed\n";
    }
    else
    {
        std::cout << " - failed, result: " << bStr << "\n";
    }

    if (bGood)
       ++nPassed;

    return bGood;
}

inline
void doTestImpl(int &nTest, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    ++nTest;
    testBigIntPlus (nPassed, i1, i2);

    ++nTest;
    testBigIntMinus(nPassed, i1, i2);
}

inline
void doTest(int &nTest, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    doTestImpl(nTest, nPassed,  i1,  i2);
    doTestImpl(nTest, nPassed,  i1, -i2);
    doTestImpl(nTest, nPassed, -i1,  i2);
    doTestImpl(nTest, nPassed, -i1, -i2);
}

// std::string testFormatString(int nTest, const std::string& fmt, const marty::format::Args &args)

int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    int nTest   = 0;
    int nPassed = 0;

    doTest(nTest, nPassed,     1,     2);
    doTest(nTest, nPassed,     2,     1);
    doTest(nTest, nPassed,     1,     3);
    doTest(nTest, nPassed,     3,     1);
    doTest(nTest, nPassed,     2,     3);
    doTest(nTest, nPassed,     3,     2);
    doTest(nTest, nPassed,     5,     3);
    doTest(nTest, nPassed,     3,     5);

    doTest(nTest, nPassed,   125,   253);
    doTest(nTest, nPassed,   253,   125);

    doTest(nTest, nPassed, 41125, 62253);
    doTest(nTest, nPassed, 62253, 41125);

    int nFailed = nTest - nPassed;

    std::cout << "\n\nTotal tests: " << nTest << ", passed: " << nPassed << ", failed: " << nFailed << "\n\n";

    return 0;
}


