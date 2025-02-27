/*! \file
    \brief Тестим чтение диаграм, и заполнение их данными - код вынесен в отдельную функцию
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
// #include "umba/tokenizer/lang/marmaid_packet_diagram.h"
//
#include "umba/tokenizer/parsers/marmaid_packet_diagram_parser.h"
#include "umba/tokenizer/parsers/marmaid_packet_diagram_cpp.h"
//
#include "umba/filename_set.h"
#include "umba/escape_string.h"

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/debug_helpers.h"

//
#include "marty_mem/marty_mem.h"
#include "marty_mem/virtual_address_memory_iterator.h"
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

#include "umba/tokenizer/parsers/marmaid_packet_diagram_parser.h"



//#define MARMAID_TYPE_UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST

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

        // inputFilename = rootPath + "tests/layouts/01.txt";
        // inputFilename = rootPath + "tests/layouts/02.txt";
        // inputFilename = rootPath + "tests/layouts/03.txt";
        // inputFilename = rootPath + "tests/layouts/04.txt";
        // inputFilename = rootPath + "tests/layouts/05.txt";
        // inputFilename = rootPath + "tests/layouts/06.txt";
        // inputFilename = rootPath + "tests/layouts/07.txt";
        // inputFilename = rootPath + "tests/layouts/08.txt";
        // inputFilename = rootPath + "tests/layouts/09.txt";
        // inputFilename = rootPath + "tests/layouts/10.txt";
        // inputFilename = rootPath + "tests/layouts/11.txt";
        // inputFilename = rootPath + "tests/layouts/12.txt";
        // inputFilename = rootPath + "tests/layouts/13.txt";
        // inputFilename = rootPath + "tests/layouts/14.txt";
        // inputFilename = rootPath + "tests/layouts/15.txt"; // !!!
        // inputFilename = rootPath + "tests/layouts/16.txt";
        // inputFilename = rootPath + "tests/layouts/17.txt";
        // inputFilename = rootPath + "tests/layouts/18.txt";
        // inputFilename = rootPath + "tests/layouts/19.txt";
        // inputFilename = rootPath + "tests/layouts/20.txt";
        // inputFilename = rootPath + "tests/layouts/21.txt";
        inputFilename = rootPath + "tests/layouts/22.txt";
        // inputFilename = rootPath + "tests/layouts/23.txt";
        // inputFilename = rootPath + "tests/layouts/24.txt";
        // inputFilename = rootPath + "tests/layouts/25.txt";
        // inputFilename = rootPath + "tests/layouts/26.txt";
        // inputFilename = rootPath + "tests/layouts/27.txt";
        // inputFilename = rootPath + "tests/layouts/28.txt";
        // inputFilename = rootPath + "tests/layouts/29.txt";

    } // if (umba::isDebuggerPresent())
    else
    {
        if (argc>1)
            inputFilename = argv[1];
    }

    if (inputFilename.empty())
    {
        LOG_ERR << "No input file taken\n";
        return 2;
    }


    std::string inputText;
    if (!umba::filesys::readFile(inputFilename, inputText))
    {
        LOG_ERR << "failed to read file: '" << inputFilename << "'\n";
        return 3;
    }

    inputText  = marty_cpp::normalizeCrLfToLf(inputText);

    auto viewsVec = umba::tokenizer::marmaid::utils::makeTextStringViewsHelper(inputText);
    std::cout << "---------------------------\n";
    std::cout << "Strings with linefeeds:\n";
    for(auto sv: viewsVec)
    {
        std::cout << sv;
    }

    std::cout << "---------------------------\n";
    std::cout << "Strings without linefeeds:\n";
    auto viewsVecNoLf = umba::tokenizer::marmaid::utils::stripLinefeedsFromStringViewsVector(viewsVec);
    for(auto sv: viewsVecNoLf)
    {
        std::cout << "[" << sv << "]\n";
    }

    std::cout << "---------------------------\n";
    std::cout << "Strings rtrimmed:\n";
    auto viewsVecTrimmed = umba::tokenizer::marmaid::utils::rtrim_copy(viewsVecNoLf);
    for(auto sv: viewsVecTrimmed)
    {
        std::cout << "[" << sv << "]\n";
    }

    std::cout << "---------------------------\n";
    std::cout << "Front matter stripped:\n";
    std::vector<std::string> frontMatter;
    if (umba::tokenizer::marmaid::utils::extractYamlFrontMatter(viewsVecTrimmed, &frontMatter))
    {
        std::cout << "Front matter:\n";
        for(auto sv: frontMatter)
        {
            std::cout << "[" << sv << "]\n";
        }
        std::cout << "---\n";

        std::unordered_map<std::string, std::string> tags;
        if (umba::tokenizer::marmaid::utils::simpleParseYamlFrontMatter(frontMatter, tags))
        {
            std::cout << "Tags:\n";
            for(auto &&kv : tags)
                std::cout << kv.first << ": " << kv.second << "\n";
            std::cout << "---\n";
        }
        std::cout << "Text:\n";
    }
    for(auto sv: viewsVecTrimmed)
    {
        std::cout << "[" << sv << "]\n";
    }

    std::cout << "---------------------------\n";
    std::cout << "Style stripped:\n";
    std::vector<std::string> style;
    if (umba::tokenizer::marmaid::utils::extractMarkeredPart(viewsVecTrimmed, "<Style>", "</style>", true, &style))
    {
        std::cout << "Style:\n";
        for(auto sv: style)
        {
            std::cout << "[" << sv << "]\n";
        }
        std::cout << "---\n";
        std::cout << "Text:\n";
    }
    for(auto sv: viewsVecTrimmed)
    {
        std::cout << "[" << sv << "]\n";
    }

    std::cout << "---------------------------\n";


    return 0;
}




