/*! \file
    \brief Тестим byteSwap и генерацию маски по количеству бит
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
#include "umba/tokenizer/parsers/mermaid/packet_diagram_parser_types.h"

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


template<typename IntType>
void testSwapBytes(IntType i)
{
    using namespace marty::mem;
    using namespace marty::mem::utils;
    using namespace marty::mem::bits;

    std::string typeNameStr = getFixedSizeTypeName<IntType>();

    std::cout << stringExtend(typeNameStr, 8) << " : " << makeHexString(i) << " : " << makeHexString(swapBytes(i)) << "\n";
}



int unsafeMain(int argc, char* argv[])
{

    using namespace marty::mem;
    using namespace marty::mem::utils;
    using namespace marty::mem::bits;

    UMBA_USED(argc);
    UMBA_USED(argv);

    std::uint64_t v = 0x8877665544332211ull;

    using namespace marty::mem;

    testSwapBytes(uint8_t (v));
    testSwapBytes( int8_t (v));

    testSwapBytes(uint16_t(v));
    testSwapBytes( int16_t(v));

    testSwapBytes(uint32_t(v));
    testSwapBytes( int32_t(v));

    testSwapBytes(uint64_t(v));
    testSwapBytes( int64_t(v));

    std::cout << "\n\n";

    for(auto i=0; i<=64; ++i)
    {
        std::cout << "Mask for " << std::setw(2) << i << std::setw(0) << " bits: " << makeHexString(makeMask(i)) << "\n";
    }

    return 0;
}


