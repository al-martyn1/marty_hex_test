/*! \file
    \brief Выводим мин-максы для разных интегральных типов - https://en.cppreference.com/w/cpp/types/climits. Также тестим различные утилитные функции.
 */


#include "marty_hex/marty_hex.h"
#include "marty_hex/memory_fill_map.h"


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

#include "umba/debug_helpers.h"
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
#include <initializer_list>
//

#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"

#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//
#include "umba/tokenizer/parsers/marmaid_packet_diagram_parser_types.h"

//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

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
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
        return 2;
    }

}

inline 
void testSingleSize(std::uint64_t size)
{
    using namespace umba::tokenizer::marmaid::utils;

    std::cout << "Size: " << size << "";
    auto immp = getMinMaxValuesSigned(size);
    auto ummp = getMinMaxValuesUnsigned(size);

    std::cout << "                  Signed                                     Unsigned\n";
    std::cout << "              min                    max                   min                   max\n";

    std::cout << "        ";
    std::cout << "0x" << makeHexString(std::uint64_t(immp.first ), 8) << "    ";
    std::cout << "0x" << makeHexString(std::uint64_t(immp.second), 8) << "    ";
    std::cout << "0x" << makeHexString(std::uint64_t(ummp.first ), 8) << "    ";
    std::cout << "0x" << makeHexString(std::uint64_t(ummp.second), 8) << "    ";
    std::cout << "\n\n";
}

inline
void testDumpString(const std::string &strDump)
{
    using namespace umba::tokenizer::marmaid::utils;

    byte_vector_t bytes;
    int res = isStringDumpString(strDump, &bytes);
    std::cout << ((res==0) ? "+ " : "- ") << strDump << "\n";
    if (res==0)
        std::cout << "  " << makeByteVectorDump(bytes) << "\n";
    else if (res<0)
        std::cout << "  " << "Invalid char found" << "\n";
    else 
        std::cout << "  " << "Wrong number of digits" << "\n";

    std::cout << "\n";
}

inline
void testSimpleSplitNameAndIndex(const std::string &fullName)
{
    //! Отделяет индекс от имени поля. Простая реализация, без использования парсера. 0 - ошибка, 1 - есть индекс, -1 - индекса нет
    using namespace umba::tokenizer::marmaid::utils;

    std::string name;
    std::uint64_t idx;

    int res = simpleSplitNameAndIndex(fullName, &name, &idx);

    std::cout << ((res!=0) ? "+ " : "- ") << "\"" << fullName << "\"\n";
    if (res==0)
        std::cout << "  " << "Error\n";
    else if (res<0)
        std::cout << "  " << "Name: \"" << name << "\" without index\n";
    else 
        std::cout << "  " << "Name: \"" << name << "\" with index " << idx << "\n";
}

inline 
void testUnorderedMemoryIterator(const std::string &strDump, std::uint64_t baseAddr, std::uint64_t printLen)
{
    using namespace umba::tokenizer::marmaid::utils;

    byte_vector_t bytes;
    int res = isStringDumpString(strDump, &bytes);
    if (res!=0)
    {
       std::cout << "testUnorderedMemoryIterator: invalid input dump";
       return;
    }

    unordered_memory_t mem;

    UnorderedMemoryIterator b = UnorderedMemoryIterator(&mem, baseAddr);
    UnorderedMemoryIterator e = UnorderedMemoryIterator(&mem, baseAddr+printLen);

    UnorderedMemoryIterator it = b;
    for(auto byte: bytes)
    {
        *it++ = byte;
    }

    for(it=b; it!=e; ++it)
    {
        std::cout << makeHexString(std::uint64_t(it), 8) << ": ";
        try
        {
            auto byte    = std::uint8_t(*it);
            auto byteStr = makeHexString(byte, 1);
	            std::cout << byteStr << "\n";
        }
        catch(...)
        {
            std::cout << "XX\n";
        }
    }

    std::cout << "\n";
}


int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    // https://en.cppreference.com/w/cpp/types/climits

    using namespace umba::tokenizer::marmaid::utils;

    constexpr int w = 14;
    std::cout << std::left;
#   define COUT(x) std::cout << std::setw(w) << #x << " = " << std::setw(0) << x << " (0x" << makeHexString(std::uint64_t(x), 8) << ")" << "\n"

    std::cout << "Limits:\n";

    COUT( CHAR_BIT       );
    COUT( MB_LEN_MAX     );
    COUT( CHAR_MIN       );
    COUT( CHAR_MAX       );
    COUT( SCHAR_MIN      );
    COUT( SHRT_MIN       );
    COUT( INT_MIN        );
    COUT( LONG_MIN       );
    COUT( LLONG_MIN      );
    COUT( SCHAR_MAX      );
    COUT( SHRT_MAX       );
    COUT( INT_MAX        );
    COUT( LONG_MAX       );
    COUT( LLONG_MAX      );
    COUT( UCHAR_MAX      );
    COUT( USHRT_MAX      );
    COUT( UINT_MAX       );
    COUT( ULONG_MAX      );
    COUT( ULLONG_MAX     );
    COUT( PTRDIFF_MIN    );
    COUT( PTRDIFF_MAX    );
    COUT( SIZE_MAX       );
    COUT( SIG_ATOMIC_MIN );
    COUT( SIG_ATOMIC_MAX );
    COUT( WCHAR_MIN      );
    COUT( WCHAR_MAX      );
    COUT( WINT_MIN       );
    COUT( WINT_MAX       );


    std::cout << "\n\n";
    std::cout << "Test value fits to size:\n\n";

    for(auto i=0u; i!=8; ++i)
        testSingleSize(i+1);


    std::cout << "\n\n";
    std::cout << "Test dump parsing:\n\n";

    testDumpString("1030000090180020D9350008E1350008E3350008A4");
    testDumpString("1030000090180020D9350008E1350008E3350008A");
    testDumpString("1030000090180020Q9350008E1350008E3350008A4");
    testDumpString("10 3 0 00 00 90  18 00 20 D9 350008E1350008E3350008A4");


    std::cout << "\n\n";
    std::cout << "Test split name and index:\n\n";

    testSimpleSplitNameAndIndex(" Device Name [ 12 ] ");
    testSimpleSplitNameAndIndex(" Device Name 12 ] ");
    testSimpleSplitNameAndIndex(" Device Name [ 12");
    testSimpleSplitNameAndIndex(" Device Name");
    testSimpleSplitNameAndIndex(" Device Name [ -12 ] ");


    std::cout << "\n\n";
    std::cout << "Test UnorderedMemoryIterator:\n\n";

    // 1030000090180020D9350008E1350008E3350008A4
    testUnorderedMemoryIterator("13D93598A4", 0x20, 5);
    testUnorderedMemoryIterator("13D93598A4", 0x20, 3);
    testUnorderedMemoryIterator("13D93598A4", 0x20, 7);



/*
CHAR_BIT       = 8 (0x0000000000000008)
MB_LEN_MAX     = 5 (0x0000000000000005)
CHAR_MIN       = -128 (0xFFFFFFFFFFFFFF80)
CHAR_MAX       = 127 (0x000000000000007F)
SCHAR_MIN      = -128 (0xFFFFFFFFFFFFFF80)
SHRT_MIN       = -32768 (0xFFFFFFFFFFFF8000)
INT_MIN        = -2147483648 (0xFFFFFFFF80000000)
LONG_MIN       = -2147483648 (0xFFFFFFFF80000000)
LLONG_MIN      = -9223372036854775808 (0x8000000000000000)
SCHAR_MAX      = 127 (0x000000000000007F)
SHRT_MAX       = 32767 (0x0000000000007FFF)
INT_MAX        = 2147483647 (0x000000007FFFFFFF)
LONG_MAX       = 2147483647 (0x000000007FFFFFFF)
LLONG_MAX      = 9223372036854775807 (0x7FFFFFFFFFFFFFFF)
UCHAR_MAX      = 255 (0x00000000000000FF)
USHRT_MAX      = 65535 (0x000000000000FFFF)
UINT_MAX       = 4294967295 (0x00000000FFFFFFFF)
ULONG_MAX      = 4294967295 (0x00000000FFFFFFFF)
ULLONG_MAX     = 18446744073709551615 (0xFFFFFFFFFFFFFFFF)
PTRDIFF_MIN    = -9223372036854775808 (0x8000000000000000)
PTRDIFF_MAX    = 9223372036854775807 (0x7FFFFFFFFFFFFFFF)
SIZE_MAX       = 18446744073709551615 (0xFFFFFFFFFFFFFFFF)
SIG_ATOMIC_MIN = -2147483648 (0xFFFFFFFF80000000)
SIG_ATOMIC_MAX = 2147483647 (0x000000007FFFFFFF)
WCHAR_MIN      = 0 (0x0000000000000000)
WCHAR_MAX      = 65535 (0x000000000000FFFF)
WINT_MIN       = 0 (0x0000000000000000)
WINT_MAX       = 65535 (0x000000000000FFFF)
*/    

    return 0;
}


