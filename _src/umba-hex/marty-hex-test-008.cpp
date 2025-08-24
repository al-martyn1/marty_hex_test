/*! \file
    \brief Тестим чтение HEX, первичную проверку, получение диапазонов блоков занятой памяти
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

//
#include "umba/tokenizer.h"
//
#include "umba/tokenizer/tokenizer_log.h"
#include "umba/tokenizer/tokenizer_log_console.h"
#include "umba/tokenizer/token_collection.h"
// #include "umba/tokenizer/parser_base.h"
// #include "umba/tokenizer/lang/mermaid_packet_diagram.h"
//
#include "umba/tokenizer/parsers/mermaid/packet_diagram_parser.h"
#include "umba/tokenizer/parsers/mermaid/packet_diagram_cpp.h"
//
#include "umba/filename_set.h"
#include "umba/escape_string.h"

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
#include "marty_hex/marty_hex.h"
#include "marty_hex/intel_hex_parser.h"
#include "marty_hex/memory_fill_map.h"
#include "marty_hex/file_pos_info.h"
#include "marty_hex/hex_entry.h"


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

#include "umba/tokenizer/parsers/mermaid/packet_diagram_parser.h"



//#define MERMAID_TYPE_UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST

//struct




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

        // inputFilename = rootPath + "tests/hex/01.hex";
        // inputFilename = rootPath + "tests/hex/111.hex";
        inputFilename = rootPath + "tests/hex/pki4CPU.hex";

    } // if (umba::isDebuggerPresent())
    else
    {
        if (argc>1)
            inputFilename = argv[1];
    }

    if (inputFilename.empty())
    {
        LOG_MSG << "---------------------\n";
        LOG_ERR << "No input file taken\n";
        return 2;
    }

    std::string inputText;
    if (!umba::filesys::readFile(inputFilename, inputText))
    {
        LOG_MSG << "---------------------\n";
        LOG_MSG << "failed to read file: '" << inputFilename << "'\n";
        return 3;
    }

    marty::hex::utils::prepareTextChunkForParsing(inputText);


    marty::hex::IntelHexParser hexParser;
    std::vector<marty::hex::HexEntry> hexVec;

    // std::size_t errorOffset = 0;

    // allowMultiHex пока не работает, а вот allowSpaces нужен для файлов, где сделано дополнение до стандартной ширины строки пробелами
    auto res = hexParser.parseTextChunk(hexVec, inputText, 0, marty::hex::ParsingOptions::allowMultiHex | marty::hex::ParsingOptions::allowSpaces); // 
    if (res!=marty::hex::ParsingResult::ok)
    {
        LOG_MSG << "---------------------\n";
        LOG_MSG << "Input file: '" << inputFilename << "'\n";
        LOG_ERR << "parsing hex failed: " << enum_serialize(res) << ", line: " << hexParser.filePosInfo.line+1 << "\n";
        if (res==marty::hex::ParsingResult::checksumMismatch)
        {
            LOG_MSG << "CRC received: " << marty::hex::utils::byteToString(hexParser.getCurEntry().csumReaded) << ", "
                    << "CRC calculated: " << marty::hex::utils::byteToString(hexParser.getCurEntry().csumCalculated)
                    << "\n";
            
        }
        else if (res==marty::hex::ParsingResult::invalidRecord)
        {
        }
        
        return 1;
    }

    marty::hex::updateHexEntriesAddressAndMode(hexVec);

    // std::size_t errLineNo=0;
    // std::size_t errFileId=0;
    marty::hex::MemoryFillMap memFillMap;
    marty::hex::HexRecordsCheckReport report;
    if (marty::hex::checkHexRecords(hexVec, &memFillMap, &report)!=0)
    {
        //LOG_ERR << "data bytes overlaps, line: " << errLineNo+1 << "\n";
        using HexRecordsCheckCode = marty::hex::HexRecordsCheckCode;
        LOG_MSG << "---------------------\n";
        LOG_MSG << "Input file: '" << inputFilename << "'\n";
        LOG_ERR << "something wrong in HEX file\n";
        for(auto &&rep : report)
        {
            switch(rep.code)
            {
                case HexRecordsCheckCode::none:
                     break;

                case HexRecordsCheckCode::memoryOverlaps:
                     LOG_ERR << "data bytes overlaps, line: " << rep.filePosInfo.line+1 << "\n";
                     break;

                case HexRecordsCheckCode::mismatchAddressMode:
                     LOG_ERR << "previous address mode is not the same: " << rep.filePosInfo.line+1 << "\n";
                     break;

                case HexRecordsCheckCode::mismatchStartAddressMode:
                     LOG_ERR << "starting address mode is not match to address mode: " << rep.filePosInfo.line+1 << "\n";
                     break;
            }
        }

        return 1;
    }

    marty::hex::normalizeAddressOrder(hexVec);

    // Выводим hex

    std::cout << "Address Mode : " << marty::hex::utils::addressModeToString(hexParser.hexInfo.addressMode, true) << " (" << marty::hex::utils::addressModeToString(hexParser.hexInfo.addressMode, false) << ")\n";
    std::cout << "Base Address : " << marty::hex::utils::address32ToString(hexParser.hexInfo.baseAddress , hexParser.hexInfo.addressMode) << "\n";
    std::cout << "Start Address: " << marty::hex::utils::address32ToString(hexParser.hexInfo.startAddress, hexParser.hexInfo.addressMode) << "\n";

    std::size_t addrRecordsCount = 0;
    std::cout << "Address records:\n";
    for(auto &&r : hexVec)
    {
        if (!r.isBaseAddressEntry() && !r.isStartupAddressEntry())
            continue;

        ++addrRecordsCount;

        if (r.isBaseAddressEntry())
            std::cout << "    Base  : " << r.getFullAddressString() << "\n";

        else if (r.isStartupAddressEntry())
            std::cout << "    Entry : " << r.getFullAddressString() << "\n";
    }

    if (!addrRecordsCount)
        std::cout << "    <No address records found>\n";

    std::cout << "Data blocks:\n";
    auto hexRanges = memFillMap.makeRanges();
    memFillMap.printRanges(std::cout, hexRanges, std::string(4, ' '));



    LOG_MSG << "---------------------\n";
    LOG_MSG << "Input file: '"   << inputFilename << "'\n";
    LOG_MSG << "Address Mode : " << marty::hex::utils::addressModeToString(hexParser.hexInfo.addressMode, true) << " (" << marty::hex::utils::addressModeToString(hexParser.hexInfo.addressMode, false) << ")\n";
    LOG_MSG << "Base Address : " << marty::hex::utils::address32ToString(hexParser.hexInfo.baseAddress , hexParser.hexInfo.addressMode) << "\n";
    LOG_MSG << "Start Address: " << marty::hex::utils::address32ToString(hexParser.hexInfo.startAddress, hexParser.hexInfo.addressMode) << "\n";

    addrRecordsCount = 0;
    std::cerr << "Address records:\n";
    for(auto &&r : hexVec)
    {
        if (!r.isBaseAddressEntry() && !r.isStartupAddressEntry())
            continue;

        ++addrRecordsCount;

        if (r.isBaseAddressEntry())
            std::cerr << "    Base  : " << r.getFullAddressString() << "\n";

        else if (r.isStartupAddressEntry())
            std::cerr << "    Entry : " << r.getFullAddressString() << "\n";
    }

    if (!addrRecordsCount)
        std::cerr << "    <No address records found>\n";

    LOG_MSG << "Data blocks:\n";
    //auto hexRanges = memFillMap.makeRanges();
    memFillMap.printRanges(std::cerr, hexRanges, std::string(4, ' '));


    // std::cout << "Fill map\n";
    // std::cout << memFillMap << "\n\n\n";
    // std::cout << "--------------\n";


    for(auto &&r : hexVec)
    {
        // std::string rstr = r.serialize(); // enum_serialize(r.recordType);
        // std::size_t appendWidth = 0;
        // if (rstr.size()<48)
        //     appendWidth = 46-rstr.size();

        std::cout // << rstr << std::string(appendWidth, ' ') 
                  // << " # " 
                  << r.toString() << "\n";
    }


    return 0;
}




