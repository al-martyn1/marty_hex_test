/*! \file
    \brief Чтение диаграм и подготовка к рисованию
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

// 
#include "mermaid_svg.h"
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

    using TokenizerBuilderType          = decltype(umba::tokenizer::mermaid::packet_diagram::makeTokenizerBuilder<char>());
    using TokenizerType                 = decltype(TokenizerBuilderType().makeTokenizer());
    using TokenizerPayloadType          = umba::tokenizer::payload_type;
    using TokenizerIteratorType         = typename TokenizerType::iterator_type;
    using TokenizerTokenParsedDataType  = typename TokenizerType::token_parsed_data_type;
    using TokenCollectionType           = umba::tokenizer::TokenCollection<TokenizerType>;
    using ParserType                    = umba::tokenizer::mermaid::PacketDiagramParser<TokenizerType>;


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

        // valuesFilename = rootPath + "tests/layouts/22_values.txt";

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

    TokenizerBuilderType tokenizerBuilder = umba::tokenizer::mermaid::packet_diagram::makeTokenizerBuilder<char>();
    auto pTokenCollection = std::make_shared<TokenCollectionType>( tokenizerBuilder.makeTokenizer()
                                                                 , umba::tokenizer::mermaid::packet_diagram::TokenizerConfigurator()
                                                                 , pParserLog
                                                                 , inputText
                                                                 , pFilenameSet->addFile(inputFilename)
                                                                 );
    ParserType parser = ParserType(pTokenCollection, pParserLog);
    parser.setDiagramTitle(tags["title"]);

    if (!parser.parse())
    {
        LOG_ERR << "there is some errors\n";
        return 4;
    }

    LOG_MSG << "File processed: '" << inputFilename << "'\n\n";

    //LOG_MSG << "C/C++ structs:\n\n";


    auto diagram = parser.getDiagram();
    //umba::tokenizer::mermaid::cpp::printCppPacketDiagram( std::cout, diagram );

    using DiagramType           = decltype(diagram);
    using PacketDiagramItemType = typename DiagramType::PacketDiagramItemType;


    LOG_MSG << "\n\n";


    marty::mem::MemoryTraits memTraits;
    // Endianness           endianness         = marty::mem::Endianness::littleEndian; // bigEndian
    // MemoryOptionFlags    memoryOptionFlags  = MemoryOptionFlags::defaultFf;

    if (diagram.endianness==umba::tokenizer::mermaid::Endianness::littleEndian)
       memTraits.endianness = marty::mem::Endianness::littleEndian;
    else
       memTraits.endianness = marty::mem::Endianness::bigEndian;

    auto mem = marty::mem::Memory(memTraits);

    std::uint64_t numberedBytes8 = 0x0706050403020100ull;

    for(const auto &item: diagram.data)
    {
        if (!item.isDataEntry())
             continue;

        auto entryEndianness = diagram.getItemEndianness(item);
        std::uint64_t entryTypeSize = item.getTypeSize();

        std::uint64_t valueMask = marty::mem::bits::makeByteSizeMask(int(entryTypeSize));
        std::uint64_t value     = std::uint64_t(numberedBytes8&valueMask);

        auto entryBv = umba::tokenizer::mermaid::utils::makeByteVector(value, entryTypeSize, entryEndianness);

        // mem.write(IntType val, uint64_t addr, MemoryAccessRights requestedMode=MemoryAccessRights::write)
        // memorySetVariable

        if (item.isArray())
        {
            auto memIt = diagram.createMemoryIterator(item, &mem, true /* !errorOnWrappedAccess */ );
            std::uint64_t arraySz = item.getArraySize();
            for(std::uint64_t i=0ull; i!=arraySz; ++i)
            {
                for(auto byte : entryBv)
                {
                    *memIt = byte;
                    ++memIt;
                }
            }

            // auto addr  = std::uint64_t(memIt);
            // std::uint64_t sz = item.getArraySize();
            // for(std::uint64_t i=0ull; i!=sz; ++i)
            // {
            //     auto addrIdx = addr+entryTypeSize*i;
            //     mem.write(value, addrIdx);
            // }
        }
        else
        {
            auto memIt = diagram.createMemoryIterator(item, &mem, true /* !errorOnWrappedAccess */ );
            for(auto byte : entryBv)
            {
                *memIt = byte;
                ++memIt;
            }


            // auto addr  = std::uint64_t(memIt);
            // mem.write(value, addr);
        }
    }


    #if 0
    auto valueLines = marty_cpp::simple_string_split(valuesText, '\n');
    for(auto valueLine : valueLines)
    {
        valueLine = umba::string::trim_copy(valueLine);

        if (valueLine.empty() || valueLine.front()==';' || valueLine.front()=='#')
            continue;

        std::string varFullName, value;
        if (!umba::string::split_to_pair(valueLine, varFullName, value, ':'))
        {
            LOG_ERR << "wrong setting value line\n";
            return 5;
        }

        try
        {
            LOG_MSG << "---\n";
            umba::tokenizer::mermaid::utils::memorySetVariable(diagram, mem, varFullName, value
                                                              , [&](const std::string &str)
                                                                {
                                                                    LOG_MSG << str << "\n";
                                                                }
                                                              );
        }
        catch(const std::exception &e)
        {
            LOG_ERR << e.what() << "\n";
            return 6;
        }

    } // end of commands 
    #endif

    #if 0
    LOG_MSG << "\n\n-----\nSections (Full):\n";
    auto sectionPairs = diagram.findAllSections(true /* fullSections */ );
    for(auto p : sectionPairs)
    {
        LOG_MSG << p.first << " - " << p.second << "\n";
    }

    LOG_MSG << "\n\n-----\nSections (No fill):\n";
    sectionPairs = diagram.findAllSections(false /* !fullSections */ );
    for(auto p : sectionPairs)
    {
        LOG_MSG << p.first << " - " << p.second << "\n";
    }
    #endif

    mermaid::svg::ByteDiagramViewData diagramViewData;
    diagramViewData.title = diagram.title;


    auto sectionPairs = diagram.findAllSections(true /* fullSections */ );
    for(auto p : sectionPairs)
    {
        mermaid::svg::ByteDiagramViewSectionData sectionData;

        std::size_t idx    = p.first;
        std::size_t idxEnd = p.second; // +1;
        for(; idx!=idxEnd; ++idx)
        {
            const auto &item = diagram.data[idx];
            if (item.isOrgEntry())
            {
                 sectionData.name = item.text;
                 
            }
            else if (item.isDataEntry())
            {
                std::uint64_t entryTypeSize = item.getTypeSize();

                mermaid::svg::ByteDiagramViewEntry viewEntry;
                viewEntry.name = item.text;

                if (item.isArray())
                {
                    viewEntry.bArray = true;
                    auto memIt = diagram.createMemoryIterator(item, &mem, true /* !errorOnWrappedAccess */ );
                    std::uint64_t arraySz = item.getArraySize();
                    for(std::uint64_t i=0ull; i!=arraySz; ++i)
                    {
                        //byte_vector_t bv;
                        mermaid::svg::ByteDiagramViewEntryData entryData;
                        std::uint64_t byteIdx = 0;
                        for(; byteIdx!=entryTypeSize; ++byteIdx)
                        {
                            if (sectionData.addressStr.empty())
                                sectionData.addressStr = std::string(memIt);
                            if (viewEntry.addressStr.empty())
                                viewEntry.addressStr   = std::string(memIt);
                            if (entryData.addressStr.empty())
                                entryData.addressStr   = std::string(memIt);
                            
                            entryData.bytes.push_back(*memIt);
                            ++memIt;
                        }

                        viewEntry.data.emplace_back(entryData);
                    }
                }
                else
                {
                    auto memIt = diagram.createMemoryIterator(item, &mem, true /* !errorOnWrappedAccess */ );
                    mermaid::svg::ByteDiagramViewEntryData entryData;
                    std::uint64_t byteIdx = 0;
                    for(; byteIdx!=entryTypeSize; ++byteIdx)
                    {
                        if (sectionData.addressStr.empty())
                            sectionData.addressStr = std::string(memIt);
                        if (viewEntry.addressStr.empty())
                            viewEntry.addressStr   = std::string(memIt);
                        if (entryData.addressStr.empty())
                            entryData.addressStr   = std::string(memIt);

                        entryData.bytes.push_back(*memIt);
                        ++memIt;
                    }

                    viewEntry.data.emplace_back(entryData);
                }

                sectionData.entries.emplace_back(viewEntry);
            }
        }

        diagramViewData.sections.emplace_back(sectionData);
    }


    std::stringstream oss;

    int posX = 0;
    int posY = 16; // Делаем отступ по вертикали, чтобы title влезал
    int maxPosX = 0;
    int maxPosY = 0;
    const int displayWidth   = diagram.getDisplayWidth();
    // const int labelMaxWidth  = 96;
    const int byteStartPosX  = 192; // 160; // 128;
    const int byteWidth      = 1024 / displayWidth;
    const int lineHeight     = 24;
    const int lineGap        = 8;
    const int r              = 6;
    const int indent         = 4;
    const int left           = 8;

    auto checkMaxXY = [&](std::size_t nBytes)
    {
        maxPosX  = std::max(maxPosX, byteStartPosX+int(nBytes*byteWidth)+1);
        maxPosY  = std::max(maxPosY, posY+lineHeight+1);
        maxPosX += (maxPosX&1) ? 1 : 0;
        maxPosY += (maxPosY&1) ? 1 : 0;
    };

    checkMaxXY(0);

    if (!diagramViewData.title.empty())
    {
        mermaid::svg::drawText( oss, left+posX, posY, diagramViewData.title, "mdppPacketDiaTitle");
        posY += 24;
        checkMaxXY(0);
    }
    
    for(const auto &sectionInfo : diagramViewData.sections)
    {
        if (diagramViewData.sections.size()>1)
        {
            posY += 16;
            if (!sectionInfo.name.empty())
            {
                mermaid::svg::drawText( oss, left+posX, posY, sectionInfo.name, "mdppPacketDiaOrgLabel", "middle");
                posY += 18;
            }
            checkMaxXY(0);
        }

        for(const auto &entry: sectionInfo.entries)
        {
            auto entryName = entry.name;
            if (entry.bArray)
                entryName += "[" + std::to_string(entry.data.size()) + "]";

            mermaid::svg::drawText( oss, left+posX, posY+lineHeight/2, entry.name, "mdppPacketDiaLabel", "middle");

            if (!entry.bArray)
            {
                UMBA_ASSERT(entry.data.size()==1);
                posX = byteStartPosX;
                mermaid::svg::drawWord( oss, entry.data[0], entry.data[0].idxFirst, posX, posY
                                      , "mdppPacketDiaBlock"
                                      , "mdppPacketDiaBlock"
                                      , "mdppArrayBoundsLine"
                                      , "mdppPacketDiaByteNumberLabel"
                                      , "mdppPacketDiaIndexLabel"
                                      , byteWidth, lineHeight // byte width, line height
                                      , r, indent   // r, indent
                                      , false
                                      , false
                                      , 0, entry.data[0].bytes.size()-1   // range
                                      , diagram.displayOptionFlags
                                      );
                posX += int(entry.data[0].bytes.size()*byteWidth);
                checkMaxXY(entry.data[0].bytes.size());
            }
            else // array
            {
                std::size_t nBytes = 0u;
                posX = byteStartPosX;
                for(const auto &data: entry.data)
                {
                    mermaid::svg::drawWord( oss, data, data.idxFirst, posX, posY
                                          , "mdppPacketDiaBlock"
                                          , "mdppPacketDiaBlock"
                                          , "mdppArrayBoundsLine"
                                          , "mdppPacketDiaByteNumberLabel"
                                          , "mdppPacketDiaIndexLabel"
                                          , byteWidth, lineHeight // byte width, line height
                                          , r, indent   // r, indent
                                          , false
                                          , false
                                          , 0, data.bytes.size()-1   // range
                                          , diagram.displayOptionFlags
                                          );
                    posX += int(data.bytes.size()*byteWidth);
                    nBytes += data.bytes.size();
                }
                checkMaxXY(nBytes);
            }

            posX = 0;
            posY += lineHeight+lineGap;
            checkMaxXY(0);
        }
    }


// // Для одного элемента
// struct ByteDiagramViewEntryData
// {
//     std::string     addressStr;
//     byte_vector_t   bytes;
// };
//  
// // Поле, одиночное или массив/диапазон
// struct ByteDiagramViewEntry
// {
//     std::string                              addressStr; // Совпадает с тем, что в первом элементе в data
//     std::string                              name      ;
//     std::vector<ByteDiagramViewEntryData>    data      ;
//     bool                                     bArray = false; // or range
// };
//  
// // Секция (org)
// struct ByteDiagramViewSectionData
// {
//     std::string                              addressStr;
//     std::string                              name      ;
//     std::vector<ByteDiagramViewEntry>        entries   ;
// };
//  
// // Вся диаграмма
// struct ByteDiagramViewData
// {
//     std::string                              title   ;
//     std::vector<ByteDiagramViewSectionData>  sections;
// };

// .mdppPacketDiaLabel{fill:black;font-size:12px;}\n

// mermaid::svg::drawWord( oss, bv4, 50, 250
//         , "mdppPacketDiaBlock"
//         , "mdppPacketDiaBlock"
//         , "mdppPacketDiaByteNumberLabel"
//         , 32, 26 // byte width, line height
//         , 8, 4   // r, indent
//         , 0, 3   // range
//         );


// template<typename StreamType>
// void mermaid::svg::drawText( StreamType &oss
//              , int  posX, int posY
//              , const std::string &text
//              , const std::string &textClass
//              , const std::string &baseLine  = "auto" // auto|middle|hanging - https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/dominant-baseline
//              , const std::string &hAlign    = "start" // start|middle|end    - https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/text-anchor
//              )


    mermaid::svg::writeSvg(std::cout, maxPosX, maxPosY, style, oss.str());



    #if 0
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


    LOG_MSG << "\n\n-----\nResult memory dump (split sections and using virtual iterators):\n";
    for(auto p : sectionPairs)
    {
        LOG_MSG << "---\n";

        LOG_MSG << "Section: " << p.first << " - " << p.second << "\n";

        auto it=diagram.createConstMemoryIterator(p.first , &mem, true);
        auto e =diagram.createConstMemoryIterator(p.second, &mem, true);

        for( ; it!=e; ++it)
        {
            auto addrStr = std::string(it);
            
            LOG_MSG << addrStr << ": ";

            // if (addrStr=="1000:0000" || addrStr=="1000:FFFF")
            // {
            //     addrStr.clear();
            // }

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
    }
    #endif

    return 0;
}




