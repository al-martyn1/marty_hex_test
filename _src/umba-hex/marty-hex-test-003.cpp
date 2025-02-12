/*! \file
    \brief Тестим сериализацию чисел в разные форматы и разные размеры
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
void testSingle(std::uint64_t val, std::uint64_t size, umba::tokenizer::marmaid::Endianness endianness, bool forceFit=true)
{
    using namespace umba::tokenizer::marmaid::utils;

    if (size>8)
        size = 8;

    if (forceFit)
        val &= makeByteSizeMask(size);

    auto bv    = makeByteVector(val, size, endianness);
    auto bvStr = makeByteVectorDump(bv);
    

    std::cout << "Value     : " << makeHexString(val, size) << "\n";
    std::cout << "Size      : " << size << "\n";
    std::cout << "Endianness: " << endiannessToString(endianness) << "\n";
    std::cout << "Result    : " << bvStr << "\n";
    std::cout << "\n";

}

inline
void testEndiannessList(std::uint64_t val, std::uint64_t size, std::initializer_list<umba::tokenizer::marmaid::Endianness> endiannessList , bool forceFit=true)
{
    using namespace umba::tokenizer::marmaid;
    using namespace umba::tokenizer::marmaid::utils;
    for(auto e : endiannessList)
    {
        if ((e==Endianness::leMiddleEndian || e==Endianness::beMiddleEndian) && size!=4 && size!=8)
            continue;
        testSingle(val, size, e, forceFit);
    }
}

inline
void testEndiannessListSizeList( std::uint64_t val, std::initializer_list<std::uint64_t> sizeList, std::initializer_list<umba::tokenizer::marmaid::Endianness> endiannessList , bool forceFit=true)
{
    for(auto s : sizeList)
    {
        testEndiannessList(val, s, endiannessList, forceFit);
    }
}




int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    using namespace umba::tokenizer::marmaid;
    using namespace umba::tokenizer::marmaid::utils;

    // Нумерация от старшего к младшему
    // std::uint64_t val = 0x1122334455667788ull;

    // Нумерация от младшего к старшему
    std::uint64_t val = 0x8877665544332211ull;

    testEndiannessListSizeList( val, {2,3,4,5,6,7,8}, {Endianness::littleEndian, Endianness::bigEndian, Endianness::leMiddleEndian, Endianness::beMiddleEndian}, true /* forceFit */ );

    std::int64_t val = 0xFF77665544332211ll;



    return 0;
}


