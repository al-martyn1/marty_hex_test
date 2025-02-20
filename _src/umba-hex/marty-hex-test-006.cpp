/*! \file
    \brief Тестируем marty::mem::Memory
 */


#include "marty_hex/marty_hex.h"
#include "marty_hex/memory_fill_map.h"
//
#include "marty_mem/marty_mem.h"


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

//
#include "marty_mem/utils.h"
#include "marty_mem/bits.h"




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
void testMemoryIterator(const std::string &strDump, std::uint64_t baseAddr, std::uint64_t printLen)
{
    using namespace umba::tokenizer::marmaid::utils;
    using namespace marty::mem;

    byte_vector_t bytes;
    int res = isStringDumpString(strDump, &bytes);
    if (res!=0)
    {
       std::cout << "testMemoryIterator: invalid input dump";
       return;
    }

    Memory mem;

    auto addIt = mem.iterator<byte_t>(baseAddr);
    //auto e = UnorderedMemoryIterator(&mem, baseAddr+printLen);

    //UnorderedMemoryIterator it = b;
    for(auto byte: bytes)
    {
        *addIt++ = byte;
    }

    //for(auto it=mem.begin<byte_t>(); it!=mem.end<byte_t>(); ++it)
    auto i=0u;
    for(auto it=mem.begin<byte_t>(); i!=printLen; ++it, ++i)
    {
        std::cout << makeHexString(std::uint64_t(it), 8) << ": ";
        try
        {
            auto byte    = std::uint8_t(*it);
            auto byteStr = makeHexString(byte, 1);
	            std::cout << byteStr << "\n";
        }
        //catch(...)
        catch(const unassigned_memory_access &)
        {
            std::cout << "XX\n";
        }
    }

    std::cout << "\n";
}



int unsafeMain(int argc, char* argv[])
{
    using namespace marty::mem;
    using namespace marty::mem::utils;
    using namespace marty::mem::bits;
    using namespace umba::tokenizer::marmaid;
    using namespace umba::tokenizer::marmaid::utils;

    UMBA_USED(argc);
    UMBA_USED(argv);

    // 1030000090180020D9350008E1350008E3350008A4
    testMemoryIterator("13D93598A4", 0x20, 5);
    testMemoryIterator("13D93598A4", 0x20, 3);
    testMemoryIterator("13D93598A4", 0x20, 7);

    Memory mem;

    mem.write(makeByteVectorFromDumpString("13D93350"  ), 0x020);
    mem.write(makeByteVectorFromDumpString("020D9350"  ), 0x028);
    mem.write(makeByteVectorFromDumpString("08E33508A4"), 0x040);

    for(auto b : mem)
    {
        //std::cout << makeHexString(std::uint64_t(it), 8) << ": ";
        try
        {
            auto byte = b; // std::uint8_t(*it);
            auto byteStr = makeHexString(byte, 1);
	            std::cout << byteStr << "\n";
        }
        catch(const unassigned_memory_access &)
        {
            std::cout << "--\n"; // "XX\n";
        }
    }

    std::cout << "\n";
    
    return 0;
}


