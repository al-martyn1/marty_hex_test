/*! \file
    \brief Рисуем диаграмму и добавляем опции
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

    TokenizerBuilderType tokenizerBuilder = umba::tokenizer::mermaid::makeTokenizerBuilderPacketDiagram<char>();
    auto pTokenCollection = std::make_shared<TokenCollectionType>( tokenizerBuilder.makeTokenizer()
                                                                 , umba::tokenizer::mermaid::PacketDiagramTokenizerConfigurator()
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


    // Заполняем mem маркерными байтами

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
                std::size_t byteIdx = std::size_t(-1);
                for(auto byte : entryBv)
                {
                    ++byteIdx;

                    try
                    {
                        *memIt = byte;
                    }
                    catch(const marty::mem::address_wrap &e)
                    {
                        std::string msg = e.what();
                        msg += ": field name: '" + item.text + "', index: " + std::to_string(i) + " (byte index: " + std::to_string(byteIdx) + ")";
                        throw marty::mem::address_wrap(msg);
                    }

                    ++memIt;
                }
            }
        }

        else // (!item.isArray())
        {
            auto memIt = diagram.createMemoryIterator(item, &mem, true /* !errorOnWrappedAccess */ );
            std::size_t byteIdx = std::size_t(-1);
            for(auto byte : entryBv)
            {
                ++byteIdx;

                try
                {
                    *memIt = byte;
                }
                catch(const marty::mem::address_wrap &e)
                {
                    std::string msg = e.what();
                    msg += ": field name: '" + item.text + "' (byte index: " + std::to_string(byteIdx) + ")";
                    throw marty::mem::address_wrap(msg);
                }

                ++memIt;
            }

        }
    }


    // bool optShowSigleByteNumbers = false;

    /* Мы хотим что? 

       Мы хотим, чтобы некоторые типы данных (только однобайтовые)
       отображались не в виде отдельных байт, а в виде единого блока.

       Заводим set<unsigned> для проверки.

       Также мы хотим, отображать это не по типам, а по именам полей.
       Также заводим set<std::string>, но туда добавляем после превращения в ID.

       Это для задания типа для ренджей
       range-as-chars
       range-as-bytes

       Это для задания оптом блоков для ренджей/массивов
       uint-bytes-as-block
       int-bytes-as-block
       char-bytes-as-block

       Для задания индивидуально есть опция block

     */

    // static std::string makeFieldId(const std::string &t)

    auto updateEntryAddress = [&]( mermaid::svg::ByteDiagramViewSectionData &sectionData
                                 , mermaid::svg::ByteDiagramViewEntry       &viewEntry
                                 , mermaid::svg::ByteDiagramViewEntryData   &entryData
                                 , auto                                                         memIt
                                 )
    {
        if (sectionData.addressStr.empty())
        {
            sectionData.linearAddress = std::uint64_t(memIt);
            sectionData.addressStr    = std::string(memIt);
        }

        if (viewEntry.addressStr.empty())
        {
            viewEntry.linearAddress   = std::uint64_t(memIt);
            viewEntry.addressStr      = std::string(memIt);
        }

        if (entryData.addressStr.empty())
        {
            entryData.linearAddress   = std::uint64_t(memIt);
            entryData.addressStr      = std::string(memIt);
        }
    };

    // Тут мы готовим данные для рисования

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

                if (item.isBlock())
                {
                    auto memIt = diagram.createMemoryIterator(item, &mem, true /* !errorOnWrappedAccess */ );
                    auto sz    = item.getTypeFieldSize();
                    mermaid::svg::ByteDiagramViewEntryData entryData;
                    entryData.bytes = umba::tokenizer::mermaid::utils::makeByteVector(sz);
                    entryData.bFlatByteArray = true;
                    viewEntry.data.emplace_back(entryData);
                    updateEntryAddress(sectionData, viewEntry, entryData, memIt);
                }

                else if (item.isArray())
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
                            updateEntryAddress(sectionData, viewEntry, entryData, memIt);
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
                        updateEntryAddress(sectionData, viewEntry, entryData, memIt);
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

    // int posX = 0;
    int posY = 0; // 16; // Делаем отступ по вертикали, чтобы title влезал - но если title сверху, и 0 - если снизу
    int maxPosX = 1024;
    int maxPosY = 0;
    int curLineBytePos = 0; // displayWidth

    const int displayWidth        = diagram.getDisplayWidth();
    //const int labelMaxWidth       = 96;
    const int byteStartPosX       = 0; // 128;
    const int byteLineHeight      = 24;
    const int byteHeight          = 20;
    const int byteWidth           = 1024 / displayWidth;
    const int textLineHeight      = 18;
    const int numLabelsLineHeight = 18;
    const int lineGap             = 8;
    const int secGap              = 16;
    const int r                   = 6;
    const int indent              = 4;
    const int left                = 0; // 8;

    auto checkMaxXY = [&]( /* std::size_t nBytes */ )
    {
        maxPosY  = std::max(maxPosY, posY + byteLineHeight      +
                                            textLineHeight      + 
                                            numLabelsLineHeight + 1
                           );

        maxPosY += (maxPosY&1) ? 1 : 0;
    };

    auto incPosY = [&]()
    {
        curLineBytePos = 0;
        posY += byteLineHeight     ;
        // posY += textLineHeight     ;
        // posY += numLabelsLineHeight;
        posY += lineGap;
        checkMaxXY();
    };

    auto drawByteEntryFirstLast = [&](const mermaid::svg::ByteDiagramViewEntryData &entryData, std::size_t first, std::size_t last)// -> bool
    {
        mermaid::svg::drawWord( oss, entryData, entryData.idxFirst, left+byteStartPosX+curLineBytePos*byteWidth, posY
                              , "mdppPacketDiaBlock"
                              , "mdppPacketDiaBlock"
                              , "mdppArrayBoundsLine"
                              , "mdppPacketDiaByteNumberLabel"
                              , "mdppPacketDiaIndexLabel"
                              , byteWidth, byteHeight // byte width, line height
                              , r, indent   // r, indent (in byte for its label)
                              , entryData.bFlatByteArray
                              , false // bSmallFonts
                              , first, last // range
                              , diagram.displayOptionFlags
                              );
    };

    auto drawByteEntry = [&](const mermaid::svg::ByteDiagramViewEntryData &entryData)// -> bool
    {
        if (curLineBytePos>=displayWidth)
            incPosY();

        std::size_t begin = 0;
        std::size_t end   = entryData.bytes.size();

        while(begin!=end)
        {
            auto numBytesNeedToDraw = end - begin;
            auto numBytesCanDraw    = std::size_t(displayWidth - curLineBytePos);
            auto numBytesToDraw     = std::min(numBytesNeedToDraw, numBytesCanDraw);
            if (numBytesToDraw==0)
            {
                incPosY();
                continue;
            }

            std::size_t drawEnd = begin+numBytesToDraw;
            drawByteEntryFirstLast(entryData, begin, drawEnd-1);
            begin = drawEnd;
            curLineBytePos += int(numBytesToDraw);
        }

        if (curLineBytePos>=displayWidth)
            incPosY();

        checkMaxXY();
    };

    checkMaxXY();

    if (!diagramViewData.title.empty())
    {
        // mermaid::svg::drawText( oss, left+posX, posY, diagramViewData.title, "mdppPacketDiaTitle");
        // posY += 24;
        checkMaxXY();
    }

    /* При выбранном размере шрифта на один байт по ширине приходится примерно 4 символа.

     */

    std::size_t secCnt = std::size_t(-1);
    for(const auto &sectionInfo : diagramViewData.sections)
    {
        ++secCnt;

        curLineBytePos = int(sectionInfo.linearAddress%std::uint64_t(displayWidth));

        if (secCnt)
        {
            posY += byteLineHeight     ;
            posY += lineGap;
            posY += secGap;
            
            checkMaxXY();
        }

        //if (diagramViewData.sections.size()>1)
        {
            auto secStr = sectionInfo.addressStr;
            if (!sectionInfo.name.empty())
            {
                if (!secStr.empty())
                    secStr += " - " + sectionInfo.name;
                else 
                    secStr = sectionInfo.name;
            }

            if (!secStr.empty())
            {
                mermaid::svg::drawText( oss, left /* +posX */ , posY, secStr, "mdppPacketDiaOrgLabel", "hanging" /* "middle" */, "start" /* hAlign */);
                posY += textLineHeight;
                posY += lineGap;
                checkMaxXY();
            }
        }

        for(const auto &entry: sectionInfo.entries)
        {
            auto entryName = entry.name;
            if (entry.bArray)
                entryName += "[" + std::to_string(entry.data.size()) + "]";

            if (!entry.bArray)
            {
                UMBA_ASSERT(entry.data.size()==1);
                drawByteEntry(entry.data[0]);
            }
            else // array
            {
                for(const auto &data: entry.data)
                {
                    drawByteEntry(data);
                }
            }

            checkMaxXY();
        }
    }


    mermaid::svg::writeSvg(std::cout, maxPosX, maxPosY, style, oss.str());

    return 0;
}




