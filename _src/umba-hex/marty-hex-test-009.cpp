/*! \file
    \brief Тестим чтение диаграм, и заполнение их данными
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
#include "umba/tokenizer/parsers/mermaid_packet_diagram_parser.h"
#include "umba/tokenizer/parsers/mermaid_packet_diagram_cpp.h"
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

#include "umba/tokenizer/parsers/mermaid_packet_diagram_parser.h"



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

    using FilenameSetType               = umba:: /* Simple */ FilenameSet<std::size_t /* , std::string */ >;
    using SharedFilenameSetType         = std::shared_ptr<FilenameSetType>;
    using ParserConsoleErrorLog         = umba::tokenizer::log::ParserConsoleErrorLog<FilenameSetType>;

    using TokenizerBuilderType          = decltype(umba::tokenizer::mermaid::makeTokenizerBuilderPacketDiagram<char>());
    using TokenizerType                 = decltype(TokenizerBuilderType().makeTokenizer());
    using TokenizerPayloadType          = umba::tokenizer::payload_type;
    using TokenizerIteratorType         = typename TokenizerType::iterator_type;
    using TokenizerTokenParsedDataType  = typename TokenizerType::token_parsed_data_type;
    using TokenCollectionType           = umba::tokenizer::TokenCollection<TokenizerType>;
    using ParserType                    = umba::tokenizer::mermaid::PacketDiagramParser<TokenizerType>;


    auto pFilenameSet = std::make_shared<FilenameSetType>();
    auto pParserLog   = std::make_shared<ParserConsoleErrorLog>(pFilenameSet);


    std::string inputFilename;
    std::string valuesFilename;

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
        valuesFilename = rootPath + "tests/layouts/22_values.txt";

    } // if (umba::isDebuggerPresent())
    else
    {
        if (argc>1)
            inputFilename = argv[1];
        if (argc>2)
            valuesFilename = argv[2];
    }

    if (inputFilename.empty())
    {
        LOG_ERR << "No input file taken\n";
        return 2;
    }

    if (valuesFilename.empty())
    {
        LOG_ERR << "No input file taken\n";
        return 2;
    }
    

    std::string valuesText;
    if (!umba::filesys::readFile(valuesFilename, valuesText))
    {
        LOG_ERR << "failed to read file: '" << valuesFilename << "'\n";
        return 3;
    }

    std::string inputText;
    if (!umba::filesys::readFile(inputFilename, inputText))
    {
        LOG_ERR << "failed to read file: '" << inputFilename << "'\n";
        return 3;
    }

    inputText  = marty_cpp::normalizeCrLfToLf(inputText);
    valuesText = marty_cpp::normalizeCrLfToLf(valuesText);


    auto viewsVec = umba::tokenizer::mermaid::utils::makeTextStringViewsHelper(inputText);
    auto viewsVecNoLf = umba::tokenizer::mermaid::utils::stripLinefeedsFromStringViewsVector(viewsVec);
    auto viewsVecTrimmed = umba::tokenizer::mermaid::utils::rtrim_copy(viewsVecNoLf);
    std::vector<std::string> frontMatter;
    umba::tokenizer::mermaid::utils::extractYamlFrontMatter(viewsVecTrimmed, &frontMatter);
    std::vector<std::string> style;
    umba::tokenizer::mermaid::utils::extractMarkeredPart(viewsVecTrimmed, "<Style>", "</style>", true, &style);


    TokenizerBuilderType tokenizerBuilder = umba::tokenizer::mermaid::makeTokenizerBuilderPacketDiagram<char>();
    auto pTokenCollection = std::make_shared<TokenCollectionType>( tokenizerBuilder.makeTokenizer()
                                                                 , umba::tokenizer::mermaid::PacketDiagramTokenizerConfigurator()
                                                                 , pParserLog
                                                                 , inputText
                                                                 , pFilenameSet->addFile(inputFilename)
                                                                 );
    ParserType parser = ParserType(pTokenCollection, pParserLog);

    if (!parser.parse())
    {
        LOG_ERR << "there is some errors\n";
        return 4;
    }

    LOG_MSG << "File processed: '" << inputFilename << "'\n\n";

    LOG_MSG << "C/C++ structs:\n\n";


    //umba::tokenizer::mermaid::cpp::simplePrintCppPacketDiagram( std::cout, parser.getDiagram() );
    auto diagram = parser.getDiagram();
    umba::tokenizer::mermaid::cpp::printCppPacketDiagram( std::cout, diagram );

    using DiagramType = decltype(diagram);


    LOG_MSG << "\n\n";


    marty::mem::MemoryTraits memTraits;

    if (diagram.endianness==umba::tokenizer::mermaid::Endianness::littleEndian)
       memTraits.endianness = marty::mem::Endianness::littleEndian;
    else
       memTraits.endianness = marty::mem::Endianness::bigEndian;


    auto mem = marty::mem::Memory(memTraits);

// struct MemoryTraits
// {
//     Endianness           endianness         = marty::mem::Endianness::littleEndian; // bigEndian
//     MemoryOptionFlags    memoryOptionFlags  = MemoryOptionFlags::defaultFf;
//  
// }; // struct MemoryTraits

 

    auto valueLines = marty_cpp::simple_string_split(valuesText, '\n');
    for(auto valueLine : valueLines)
    {
        valueLine = umba::string::trim_copy(valueLine);

        if (valueLine.empty() || valueLine.front()==';' || valueLine.front()=='#')
            continue;

        //  
        // std::string cmd, varValue;
        // if (!umba::string::split_to_pair(cmdValueLine, cmd, varValue, '='))
        // {
        //     LOG_ERR << "wrong setting value line\n";
        //     return 5;
        // }

        std::string varFullName, value;
        if (!umba::string::split_to_pair(valueLine, varFullName, value, ':'))
        {
            LOG_ERR << "wrong setting value line\n";
            return 5;
        }

        varFullName = umba::string::trim_copy(varFullName);
        value = umba::string::trim_copy(value);

        std::string varName;
        std::uint64_t varArrayIndex = 0;
        bool doSetArrayItem = false;
        UMBA_USED(doSetArrayItem);

        // if (umba::string::is_quoted(varFullName, '\'', '\''))
        //     umba::string::unquote(varFullName, '\'', '\'');
        // else if (umba::string::is_quoted(varFullName, '\"', '\"'))
        //     umba::string::unquote(varFullName, '\"', '\"');
        umba::string::unquote(varFullName);


        //! Отделяет индекс от имени поля. Простая реализация, без использования парсера. 0 - ошибка, 1 - есть индекс, -1 - индекса нет
        int parseVarNameRes = umba::tokenizer::mermaid::utils::simpleSplitNameAndIndex(varFullName , &varName, &varArrayIndex);
        if (parseVarNameRes==0)
        {
            LOG_ERR << "error in entry name ('" << varFullName << "')\n";
            return 6;
        }

        if (parseVarNameRes>0)
            doSetArrayItem = true;

        std::size_t entryIdx = diagram.findEntryByName(varName);
        if (entryIdx==std::size_t(-1))
        {
            LOG_ERR << "entry not foud: '" << varFullName << "'\n";
            return 7;
        }

        const auto &entryItem = diagram.data[entryIdx];
        if (!entryItem.isDataEntry())
        {
            LOG_ERR << "entry not a data entry: '" << varFullName << "'\n";
            return 8;
        }

        // Endianness
        auto entryEndianness = diagram.getItemEndianness(entryItem);
        std::uint64_t entryTypeSize = entryItem.getTypeSize();

        auto valueQuotType = umba::string::unquote(value);
        // SimpleQuotesType unquote(StringType &s)

        try
        {
            using namespace umba::tokenizer::mermaid::utils;

            byte_vector_t bv;
            typename DiagramType::MemoryIteratorType fieldMemoryIt;

            if (doSetArrayItem)
            {
                if (!entryItem.isArray())
                {
                    LOG_ERR << "Try to set indexed variable, but data entry is not an array: '" << varFullName << "'\n";
                    return 9;
                }
    
                if (varArrayIndex>=entryItem.getArraySize())
                {
                    LOG_ERR << "Taken index is out of range: '" << varFullName << "'\n";
                    return 10;
                }
    
                if (valueQuotType==umba::string::SimpleQuotesType::notQuoted)
                {
                    bv = makeByteVectorFromIntStr(value, entryTypeSize, entryEndianness, entryItem.isSigned() /* , std::uint64_t *pParsedVal=0 */ );
                }
                else if (valueQuotType==umba::string::SimpleQuotesType::aposQuoted)
                {
                    bv = makeByteVectorFromCharLiteral(value, entryTypeSize, entryEndianness /* , std::uint64_t *pParsedVal=0 */ );
                }
                else // valueQuotType==umba::string::SimpleQuotesType::dblQuoted
                {
                    LOG_ERR << "Can't assign string to an array item: '" << varFullName << "'\n";
                    return 11;
                }

                // TODO: !!! Получить итератор на элемент массива 

                // MemoryIteratorType createMemoryIterator(const PacketDiagramItemType &item, marty::mem::Memory *pMem = 0) const
                fieldMemoryIt  = diagram.createMemoryIterator(entryItem, &mem, true /* errorOnWrappedAccess */ );
                fieldMemoryIt += std::int64_t(entryTypeSize*varArrayIndex);
                
            }
            else // задаём поле, индекс массива не указан. Если размер типа - 1, и у нас задана строка или дамп - то нормас
            {
                if (entryItem.isArray()) // поле объявлено как массив
                {
                    if (entryTypeSize!=1)
                    {
                        LOG_ERR << "Can't assign arrays of elements with size greater than 1: '" << varFullName << "'\n";
                        return 12;
                    }
                   
                    if (valueQuotType==umba::string::SimpleQuotesType::dblQuoted)
                    {
                        bv = makeByteVectorStringFromStringLiteral(value, entryItem.getArraySize(), entryItem.isAsciiZet());
                    }
                    else if (valueQuotType==umba::string::SimpleQuotesType::notQuoted)
                    {
                        bv = makeByteVectorStringFromDumpString(value, entryItem.getArraySize(), entryItem.isAsciiZet());
                    }
                    else
                    {
                        LOG_ERR << "Can't assign char literal to an string/array: '" << varFullName << "'\n";
                        return 13;
                    }

                }
                else // обычное поле
                {
                    if (valueQuotType==umba::string::SimpleQuotesType::notQuoted)
                    {
                        bv = makeByteVectorFromIntStr(value, entryTypeSize, entryEndianness, entryItem.isSigned() /* , std::uint64_t *pParsedVal=0 */ );
                    }
                    else if (valueQuotType==umba::string::SimpleQuotesType::aposQuoted)
                    {
                        bv = makeByteVectorFromCharLiteral(value, entryTypeSize, entryEndianness /* , std::uint64_t *pParsedVal=0 */ );
                    }
                    else // valueQuotType==umba::string::SimpleQuotesType::dblQuoted
                    {
                        LOG_ERR << "Can't assign string to simple field: '" << varFullName << "'\n";
                        return 14;
                    }
                }

                fieldMemoryIt  = diagram.createMemoryIterator(entryItem, &mem, true /* errorOnWrappedAccess */ );
            }

            // assign bv to memory here

            LOG_MSG << "---\n";
            LOG_MSG << "Set '" << varFullName << "' to " << value << "\n";

            for(auto b : bv)
            {
                auto byte = b; // std::uint8_t(*fieldMemoryIt); // b; // std::uint8_t(*it);
                auto byteStr = marty::mem::utils::makeHexString(byte, 1);

                LOG_MSG << std::string(fieldMemoryIt) << ": " << byteStr << "\n";

                *fieldMemoryIt++ = b;
            }

        }
        catch(const std::exception &e)
        {
            LOG_ERR << "Something goes wrong while setting field '" << varFullName << "'" << e.what() << "\n";
            return 10;
        }


        // isArray()
        //isAsciiZet()
        //getArraySize()



    } // end of commands 


    // std::cout << 
    LOG_MSG << "\n\n-----\nResult memory dump:\n";

    // for(auto b : mem)
    for(auto it=mem.begin(); it!=mem.end(); ++it)
    {
        //std::cout << makeHexString(std::uint64_t(it), 8) << ": ";
        LOG_MSG << std::string(it) << ": ";
        try
        {
            auto byte = std::uint8_t(*it); // b; // std::uint8_t(*it);
            auto byteStr = marty::mem::utils::makeHexString(byte, 1);
	        LOG_MSG << byteStr << "\n";
        }
        catch(const marty::mem::unassigned_memory_access &)
        {
            LOG_MSG << "--\n"; // "XX\n";
        }
    }



// umba::tokenizer::mermaid::utils::
    // byte_vector_t makeByteVectorFromIntStr(const std::string &valStr, std::uint64_t size, Endianness endianness, bool bSigned, std::uint64_t *pParsedVal=0)
    // byte_vector_t makeByteVectorFromCharLiteral(const std::string &valStr, std::uint64_t size, Endianness endianness, std::uint64_t *pParsedVal=0)
    // byte_vector_t makeByteVectorFromStringLiteral(const std::string &valStr, std::uint64_t size, bool asciiZ=false)
    // byte_vector_t makeByteVectorFromDumpString(const std::string &strDump)


    #if 0
    LOG_MSG << "Diagram:\n\n";
    
    // Пока поэлементно выводим, а массивы на одной строке
    for(auto item : diagram.data)
    {
        if (!item.isDataEntry())
           continue;

        if (item.isArray())
        {
            for(auto i=0u; i!=item.getArraySize(); ++i)
            {
                auto str = umba::tokenizer::mermaid::cpp::makeIntegralTypeTextDiagramRepresentation(item.getTypeSize(), true);
                std::cout << str;
            }
            std::cout << "\n";
        }
        else
        {
            auto str = umba::tokenizer::mermaid::cpp::makeIntegralTypeTextDiagramRepresentation(item.getTypeSize(), true);
            std::cout << str << "\n";
        }
        
    }
    #endif




    return 0;
}




