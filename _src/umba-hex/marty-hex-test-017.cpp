/*! \file
    \brief Выносим подготовку к рисованию диаграммы в либу
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
#include "umba/filename_set.h"
#include "umba/escape_string.h"

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/debug_helpers.h"

//

// 
#include "umba/tokenizer/parsers/usketch_parser.h"
#include "marty_svg/marty_svg.h"

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

#include "umba/tokenizer/parsers/mermaid_packet_diagram_parser.h"



//#define MERMAID_TYPE_UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST

//struct

#include "svg-style.h"



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



int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    using FilenameSetType               = umba:: /* Simple */ FilenameSet<std::size_t /* , std::string */ >;
    using SharedFilenameSetType         = std::shared_ptr<FilenameSetType>;
    using ParserConsoleErrorLog         = umba::tokenizer::log::ParserConsoleErrorLog<FilenameSetType>;

    using TokenizerBuilderType          = decltype(umba::tokenizer::usketch::makeTokenizerBuilderUSketch<char>());
    using TokenizerType                 = decltype(TokenizerBuilderType().makeTokenizer());
    using TokenizerPayloadType          = umba::tokenizer::payload_type;
    using TokenizerIteratorType         = typename TokenizerType::iterator_type;
    using TokenizerTokenParsedDataType  = typename TokenizerType::token_parsed_data_type;
    using TokenCollectionType           = umba::tokenizer::TokenCollection<TokenizerType>;
    using ParserType                    = umba::tokenizer::usketch::USketchParser<TokenizerType>;


    auto pFilenameSet = std::make_shared<FilenameSetType>();
    auto pParserLog   = std::make_shared<ParserConsoleErrorLog>(pFilenameSet);


    std::string inputFilename;
    //std::string valuesFilename;

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


        inputFilename = rootPath + "tests/usketch/01.txt";
        // inputFilename = rootPath + "tests/usketch/02.txt";
        // inputFilename = rootPath + "tests/usketch/03.txt";
        // inputFilename = rootPath + "tests/usketch/04.txt";
        // inputFilename = rootPath + "tests/usketch/05.txt";
        // inputFilename = rootPath + "tests/usketch/06.txt";
        // inputFilename = rootPath + "tests/usketch/07.txt";
        // inputFilename = rootPath + "tests/usketch/08.txt";
        // inputFilename = rootPath + "tests/usketch/09.txt";
        // inputFilename = rootPath + "tests/usketch/10.txt";
        // inputFilename = rootPath + "tests/usketch/11.txt";
        // inputFilename = rootPath + "tests/usketch/12.txt";
        // inputFilename = rootPath + "tests/usketch/13.txt";
        // inputFilename = rootPath + "tests/usketch/14.txt";
        // inputFilename = rootPath + "tests/usketch/15.txt"; // !!!
        // inputFilename = rootPath + "tests/usketch/16.txt";
        // inputFilename = rootPath + "tests/usketch/17.txt";
        // inputFilename = rootPath + "tests/usketch/18.txt";
        // inputFilename = rootPath + "tests/usketch/19.txt";
        // inputFilename = rootPath + "tests/usketch/20.txt";
        // inputFilename = rootPath + "tests/usketch/21.txt";
        // inputFilename = rootPath + "tests/usketch/22.txt";
        // inputFilename = rootPath + "tests/usketch/23.txt";
        // inputFilename = rootPath + "tests/usketch/24.txt";
        // inputFilename = rootPath + "tests/usketch/25.txt";
        // inputFilename = rootPath + "tests/usketch/26.txt";
        // inputFilename = rootPath + "tests/usketch/27.txt";
        // inputFilename = rootPath + "tests/usketch/28.txt";
        // inputFilename = rootPath + "tests/usketch/29.txt";

        // valuesFilename = rootPath + "tests/usketch/22_values.txt";

    } // if (umba::isDebuggerPresent())
    else
    {
        if (argc>1)
            inputFilename = argv[1];
        // if (argc>2)
        //     valuesFilename = argv[2];
    }

    if (inputFilename.empty())
    {
        LOG_ERR << "No input file taken\n";
        return 2;
    }

    // if (valuesFilename.empty())
    // {
    //     LOG_ERR << "No input file taken\n";
    //     return 2;
    // }
    

    // std::string valuesText;
    // if (!umba::filesys::readFile(valuesFilename, valuesText))
    // {
    //     LOG_ERR << "failed to read file: '" << valuesFilename << "'\n";
    //     return 3;
    // }

    std::string inputText;
    if (!umba::filesys::readFile(inputFilename, inputText))
    {
        LOG_ERR << "failed to read file: '" << inputFilename << "'\n";
        return 3;
    }

    std::unordered_map<std::string, std::string> tags;
    std::vector<std::string> styleStr;

    inputText  = marty_cpp::normalizeCrLfToLf(inputText);
    umba::tokenizer::utils::prepareTextForParsing(inputText, &styleStr, &tags);
    // valuesText = marty_cpp::normalizeCrLfToLf(valuesText);

    TokenizerBuilderType tokenizerBuilder = umba::tokenizer::usketch::makeTokenizerBuilderUSketch<char>();
    auto pTokenCollection = std::make_shared<TokenCollectionType>( tokenizerBuilder.makeTokenizer()
                                                                 , umba::tokenizer::usketch::USketchTokenizerConfigurator()
                                                                 , pParserLog
                                                                 , inputText
                                                                 , pFilenameSet->addFile(inputFilename)
                                                                 );
    ParserType parser = ParserType(pTokenCollection, pParserLog);
    //parser.setDiagramTitle(tags["title"]);

    if (!parser.parse())
    {
        LOG_ERR << "there is some errors\n";
        return 4;
    }

    LOG_MSG << "File processed: '" << inputFilename << "'\n\n";

    //LOG_MSG << "C/C++ structs:\n\n";


    //auto diagram = parser.getDiagram();

    return 0;
}


