/*! \file
    \brief Тестим marty::hex::MemoryFillMap
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



int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    marty::hex::MemoryFillMap fillMap;

    std::vector<std::uint32_t> addrs1{ 0,1,2,3,9,15,34,35,36,36,42,50,63,67,98,122,145,173,210,239};
    std::vector<std::uint32_t> addrs2{ 0,1,2,3,9,15,34,35,36};
    std::vector<std::uint32_t> addrs3{ 65531 };
    std::vector<std::uint32_t> addrs64;
    for(auto i=0u; i!=64; ++i)
        addrs64.emplace_back(i);

    for(auto a: addrs1)
        fillMap.setFilled(0x00030000+a);
    for(auto a: addrs64)
        fillMap.setFilled(0x00030040+a);
    // for(auto a: addrs2)
    //     fillMap.setFilled(0x00030140+a);
    // for(auto a: addrs64)
    //     fillMap.setFilled(0x00030180+a);
    // for(auto a: addrs1)
    //     fillMap.setFilled(0x000301C0+a);

    for(auto a: addrs2)
        fillMap.setFilled(0x00040000+a);

    for(auto a: addrs2)
        fillMap.setFilled(0x00040080+a);

    for(auto a: addrs2)
        fillMap.setFilled(0x00060000+a);


    std::cout << "--------------\n";
    std::cout << "Fill map\n";
    std::cout << "--------------\n";
    std::cout << fillMap << "\n";

    std::cout << "--------------\n";
    std::cout << "Filled ranges\n";
    std::cout << "--------------\n";
    auto ranges = fillMap.makeRanges();
    fillMap.printRanges(std::cout, ranges);

    std::cout << "\n";


    return 0;
}


