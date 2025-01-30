/*! \file
    \brief 
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

    std::string inputFilename;

    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        inputFilename = rootPath + "tests/hex/01.hex";

    } // if (umba::isDebuggerPresent())
    else
    {
        if (argc>1)
            inputFilename = argv[1];
    }

    if (inputFilename.empty())
    {
        LOG_ERR << "No input file taken\n";
        return 1;
    }

    std::string inputText;
    if (!umba::filesys::readFile(inputFilename, inputText))
    {
        LOG_ERR << "failed to read file: '" << inputFilename << "\n";
    }

    marty::hex::HexParser hexParser;
    std::vector<marty::hex::HexEntry> hexVec;

    // std::size_t errorOffset = 0;

    auto res = hexParser.parseTextChunk(hexVec, inputText, 0, marty::hex::ParsingOptions::allowMultiHex);
    if (res!=marty::hex::ParsingResult::ok)
    {
        LOG_ERR << "parsing hex failed: " << enum_serialize(res) << "\n";
    }

    marty::hex::updateHexEntriesEffectiveAddress(hexVec);

    for(auto &&r : hexVec)
    {
        std::string rstr = r.serialize(); // enum_serialize(r.recordType);
        std::size_t appendWidth = 0;
        if (rstr.size()<48)
            appendWidth = 46-rstr.size();

        std::cout << rstr << std::string(appendWidth, ' ') << " # " << r.toString() << "\n";
    }

    marty::hex::MemoryFillMap fillMap;

    std::vector<std::uint32_t> addrs1{ 0,3,9,15,34,35,36,36,42,50,63,67,98,122,145,173,210,239};
    std::vector<std::uint32_t> addrs2{ 0,3,9,15,34,35,36};
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


