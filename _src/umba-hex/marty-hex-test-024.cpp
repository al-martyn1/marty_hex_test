/*! \file
    \brief Тестим marty::BigInt
 */

#ifndef MARTY_BIGINT_FORCE_NUMBER_UNDERLYING_TYPE
    #define MARTY_BIGINT_FORCE_NUMBER_UNDERLYING_TYPE  std::uint8_t
#endif

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
#include <type_traits>

//
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
bool isIgnoreResultOpSign(const std::string &s)
{
    return s.size()>=3;
}

inline
std::string removeIgnoreResultFromOpSign(std::string s)
{
    if (!isIgnoreResultOpSign(s))
        return s;
    s.erase(0,3);
    return s;
}


inline
std::string mkMarker(bool bGood, bool bIgnore)
{
    return bIgnore
         ? std::string("!") + "   "
         : std::string(bGood ? "+" : "-") + "   ";
         ;
}

template<typename Op>
bool testBigIntImpl(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2, Op op)
{
    std::int64_t  iRes = 0; 
    marty::BigInt bRes = 0; 

    std::string opStr = op(iRes, bRes, i1, i2);

    bool bIgnoreRes = isIgnoreResultOpSign(opStr);
    opStr = removeIgnoreResultFromOpSign(opStr);

    if (opStr.empty())
        return true;


    // std::int64_t(i1+i2);
    // marty::BigInt(i1) + marty::BigInt(i2);

    using std::to_string;

    auto iStr  = to_string(iRes);
    auto bStr  = to_string(bRes);

    bool bGood = iStr==bStr;

    std::cout << mkMarker(bGood, bIgnoreRes);
    std::cout << i1 << " " << opStr << " " << i2 << " = ";

    if (bIgnoreRes)
    {
        std::cout << bStr;
        std::cout << " - ignored\n";
        bGood = true;
    }
    else
    {
        std::cout << iStr;

        if (bGood)
        {
            std::cout << " - passed\n";
        }
        else
        {
            std::cout << " - failed, result: " << bStr << "\n";
        }
    }

    ++nTotal;

    if (bGood)
       ++nPassed;

    return bGood;
}



inline
bool testBigIntPlus(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        iRes = i1 + i2;
                        bRes = marty::BigInt(i1) + marty::BigInt(i2);
                        return "+";
                    }
                  );
}

inline
bool testBigIntMinus(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        iRes = i1 - i2;
                        bRes = marty::BigInt(i1) - marty::BigInt(i2);
                        return "-";
                    }
                  );
}

inline
bool testBigIntMul(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        iRes = i1 * i2;
                        bRes = marty::BigInt(i1) * marty::BigInt(i2);
                        return "*";
                    }
                  );
}

inline
bool testBigIntShiftLeft(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        i2 = 23;
                        iRes = i1 << 23;
                        bRes = marty::BigInt(i1) << 23;
                        return "<<";
                    }
                  );
}

inline
bool testBigIntShiftRight(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        if (i1<0)
                            return std::string();
                        i2 = 23;
                        iRes = i1 >> 23;
                        bRes = marty::BigInt(i1) >> 23;
                        return ">>";
                    }
                  );
}

inline
bool testBigIntAnd(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        if (i1<0) i1 = -i1;
                        if (i2<0) i2 = -i2;
                        iRes = i1 & i2;
                        bRes = marty::BigInt(i1) & marty::BigInt(i2);
                        return "&";
                    }
                  );
}

inline
bool testBigIntOr(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        if (i1<0) i1 = -i1;
                        if (i2<0) i2 = -i2;
                        iRes = i1 | i2;
                        bRes = marty::BigInt(i1) | marty::BigInt(i2);
                        return "|";
                    }
                  );
}

inline
bool testBigIntXor(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        if (i1<0) i1 = -i1;
                        if (i2<0) i2 = -i2;
                        iRes = i1 ^ i2;
                        bRes = marty::BigInt(i1) ^ marty::BigInt(i2);
                        return "^";
                    }
                  );
}

inline
bool testBigIntInvert(int &nTotal, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    return
    testBigIntImpl( nTotal, nPassed, i1, i2
                  , [](std::int64_t &iRes, marty::BigInt &bRes, std::int64_t &i1, std::int64_t &i2) -> std::string
                    {
                        i2 = 0;
                        iRes = ~i1;
                        bRes = ~marty::BigInt(i1);
                        return "---~";
                    }
                  );
}


inline
void doTestImpl(int &nTest, int &nPassed, std::int64_t i1, std::int64_t i2)
{
    testBigIntPlus (nTest, nPassed, i1, i2);
    testBigIntMinus(nTest, nPassed, i1, i2);
    testBigIntMul  (nTest, nPassed, i1, i2);
    
    testBigIntShiftLeft(nTest, nPassed, i1, i2);
    testBigIntShiftRight(nTest, nPassed, i1, i2);

    testBigIntAnd   (nTest, nPassed, i1, i2);
    testBigIntOr    (nTest, nPassed, i1, i2);
    testBigIntXor   (nTest, nPassed, i1, i2);
    testBigIntInvert(nTest, nPassed, i1, i2);

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

// Маска старшего бита
template <typename T>
constexpr auto highest_bit_mask() {
    using UT = typename std::make_unsigned<T>::type;
    return UT{1} << (sizeof(T) * 8 - 1);
}

// Маска N старших битов
template <size_t N, typename T>
constexpr auto high_bits_mask() {
    using UT = typename std::make_unsigned<T>::type;
    constexpr size_t type_bits = sizeof(T) * 8;
    
    if constexpr (type_bits <= N) {
        return ~UT{0};
    } else {
        return ( (UT{1} << N) - UT{1} ) << (type_bits - N);
    }
}

int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    // marty::BigInt bi = -1;
    // bi <<= 23;

    marty::BigInt bi = 1;
    bi *= 2;


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

    #if 0
    std::cout << "h1: " << highest_bit_mask<std::uint8_t>()  << "\n"; // 128
    std::cout << "h4: " << high_bits_mask<4, std::uint8_t>() << "\n"; // 240
    std::cout << "h8: " << high_bits_mask<8, std::uint8_t>() << "\n"; // 255

    std::cout << "\n\n";

    std::cout << "h1: " << marty::bigint_utils::makeHighBitsMask<1, std::uint16_t>() << "\n"; // 128
    std::cout << "h4: " << marty::bigint_utils::makeHighBitsMask<4, std::uint16_t>() << "\n"; // 240
    std::cout << "h8: " << marty::bigint_utils::makeHighBitsMask<8, std::uint16_t>() << "\n"; // 255
    #endif


    return 0;
}


