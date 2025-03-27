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
    umba::tokenizer::mermaid::utils::prepareTextForDiagramParsing(inputText, &styleStr, &tags);
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


    auto mem = marty::mem::Memory(diagram.createMemTraits());
    diagram.fillMemWithByteOrderMarkers(mem);


    auto updateEntryAddress = [&]( mermaid::svg::ByteDiagramViewSectionData  &sectionData
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
        std::uint64_t   idxCur = 0; // От начала текущего ORG

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
                    entryData.idxFirst       = idxCur;
                    entryData.arrayFirst = true;
                    entryData.arrayLast  = true;
                    viewEntry.data.emplace_back(entryData);
                    updateEntryAddress(sectionData, viewEntry, entryData, memIt);

                    idxCur += std::size_t(item.getFieldSize());
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

                        entryData.idxFirst   = idxCur;
                        entryData.arrayFirst = (i==0 ? true : false);
                        entryData.arrayLast  = (i==(arraySz-1) ? true : false);
                        viewEntry.data.emplace_back(entryData);
                        idxCur += std::size_t(item.getTypeSize());
                    }
                }

                else // не блок из байт и не массив
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

                    entryData.idxFirst       = idxCur;
                    viewEntry.data.emplace_back(entryData);
                    idxCur += std::size_t(item.getFieldSize());
                }

                sectionData.entries.emplace_back(viewEntry);
            }
        }

        diagramViewData.sections.emplace_back(sectionData);
    }


    // Константы/параметры отрисовки

    const bool showTitle  = diagram.testDisplayOption(umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::showTitle);
    const bool titleOnTop = diagram.testDisplayOption(umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::titleOnTop);

    const int titleLineHeight  = 28;
    int posY = 0; // (!diagram.title.empty() && showTitle && titleOnTop) ? titleLineHeight : 0; // 16; // Делаем отступ по вертикали, чтобы title влезал - но если title сверху, и 0 - если снизу
            
    int sizeX   = 1024;
    int maxPosX = sizeX-1;
    int maxPosY = 0;
    int curLineBytePos = 0; // displayWidth

    const int displayWidth          = diagram.getDisplayWidth();
    const int labelMaxWidth         = 96;
    const int byteStartPosX         = 0; // 128;
    const int byteLineHeight        = 24;
    const int byteHeight            = 20;
    const int byteWidth             = sizeX / displayWidth;
    const int fieldLabelAvgSymbolWidth = 6;
    const int footnoteLeft          = 15;
    const int footnoteTextLeft      = footnoteLeft + 4*fieldLabelAvgSymbolWidth + fieldLabelAvgSymbolWidth/2;
    //const int textLineHeight      = 18;
    const int fieldLabelLineHeight  = 16;
    const int indexLabelLineHeight  = 12;
    //const int numLabelsLineHeight   = 18;
    const int lineGap               = 8;
    const int secGap                = 16;
    const int r                     = 6;
    const int indent                = 4;
    const int left                  = 0; // 8;
    const bool bSmallFonts          = displayWidth>32;
    const bool showFieldIndex       = (diagram.displayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::showFieldIndex)!=0;
    const bool showFieldLabels      = (diagram.displayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::showFieldLabels)!=0;


    auto checkMaxXY = [&]( /* std::size_t nBytes */ )
    {
        auto forTestPosY = posY + byteLineHeight + 4;
        if (showFieldIndex)
        {
            forTestPosY += indexLabelLineHeight;
        }
        if (showFieldLabels)
        {
            forTestPosY += fieldLabelLineHeight;
        }

        maxPosY  = std::max(maxPosY, forTestPosY);

        maxPosY += (maxPosY&1) ? 1 : 0;
    };

    auto plainIncPosY =[&]()
    {
        posY += byteLineHeight     ;
        if (showFieldIndex)
           posY += indexLabelLineHeight;
        if (showFieldLabels)
           posY += fieldLabelLineHeight;
    };

    auto incPosY = [&]()
    {
        curLineBytePos = 0;
        plainIncPosY();
        // posY += textLineHeight     ;
        // posY += numLabelsLineHeight;
        posY += lineGap;
        checkMaxXY();
    };

    auto drawByteEntryFirstLast = [&]( auto &oss
                                     , const mermaid::svg::ByteDiagramViewEntryData &entryData
                                     , std::size_t first, std::size_t last
                                     , int *pCalculatedPosLeft  = 0
                                     , int *pCalculatedPosRight = 0
                                     )// -> bool
    {
        mermaid::svg::drawWord( oss, entryData, entryData.idxFirst, left+byteStartPosX+curLineBytePos*byteWidth, posY
                              , "mdppPacketDiaBlock"
                              , "mdppPacketDiaBlock"
                              , "mdppArrayBoundsLine"
                              , bSmallFonts ? "mdppPacketDiaByteNumberLabelSmall" : "mdppPacketDiaByteNumberLabel"
                              , bSmallFonts ? "mdppPacketDiaIndexLabelSmall"      : "mdppPacketDiaIndexLabel"
                              //, bSmallFonts ? "mdppPacketDiaLabelSmall"           : "mdppPacketDiaLabel"
                              //, label
                              , byteWidth, byteHeight // byte width, line height
                              //, indexLabelLineHeight
                              , r, indent   // r, indent (in byte for its label)
                              , entryData.bFlatByteArray
                              , bSmallFonts
                              , first, last // range
                              , diagram.displayOptionFlags
                              , pCalculatedPosLeft, pCalculatedPosRight
                              );
    };


    std::vector<mermaid::svg::ByteDiagramFieldLabelDrawInfo> fieldLabelsDrawInfo;

    auto drawByteEntry = [&](const mermaid::svg::ByteDiagramViewEntryData &entryData, const std::string &label, std::size_t idx, std::size_t arraySize)// -> bool
    {
        UMBA_USED(label);
        UMBA_USED(idx);
        UMBA_USED(arraySize);

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
                checkMaxXY();
                continue;
            }

            int calculatedPosLeft  = 0;
            int calculatedPosRight = 0;
            std::size_t drawEnd = begin+numBytesToDraw;
            std::stringstream ss;
            drawByteEntryFirstLast(ss, entryData, begin, drawEnd-1, &calculatedPosLeft, &calculatedPosRight);

            mermaid::svg::ByteDiagramFieldLabelDrawInfo fldi;
            fldi.simpleSvg  = false;
            fldi.arraySize  = arraySize;
            fldi.indexFirst = idx;
            fldi.indexLast  = idx;
            fldi.posLeft    = calculatedPosLeft ;
            fldi.posRight   = calculatedPosRight;
            fldi.posY       = posY;
            fldi.fieldName  = label;
            fldi.svgText    = ss.str();
            fieldLabelsDrawInfo.emplace_back(fldi);

            begin = drawEnd;
            curLineBytePos += int(numBytesToDraw);
        }

        if (curLineBytePos>=displayWidth)
        {
            incPosY();
        }

        checkMaxXY();
    };

    checkMaxXY();


    std::string svgAllText;

    if (!diagram.title.empty() && showTitle && titleOnTop)
    {
        std::stringstream oss;
        mermaid::svg::drawText( oss, sizeX/2, posY, diagram.title, "mdppPacketDiaTitle", "hanging", "middle");
        posY += titleLineHeight;
        checkMaxXY();
        svgAllText = oss.str();
    }


    std::size_t secCnt = std::size_t(-1);
    for(const auto &sectionInfo : diagramViewData.sections)
    {
        //bool vertPosSectionIncremented = false;
        ++secCnt;

        //if (diagramViewData.sections.size()>1)
        {
            auto secStr = sectionInfo.addressStr;
            if (diagram.testDisplayOption(umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::hexPrefixSection))
                secStr = umba::tokenizer::mermaid::utils::addNumberPrefix(secStr, "0x");

            auto savedCurLineBytePos = curLineBytePos;

            if (!sectionInfo.name.empty())
            {
                if (!secStr.empty())
                    secStr += " - " + sectionInfo.name;
                else 
                    secStr = sectionInfo.name;
            }

            if (!secStr.empty())
            {
                // if (posX!=0) // не было перехода на новую строку байт
                if (curLineBytePos!=0) // не было перехода на новую строку байт
                {
                    //vertPosSectionIncremented = true;
                    incPosY();
                    checkMaxXY();
                }

                std::stringstream oss;
                mermaid::svg::drawText( oss, left /* +posX */ , posY, secStr, "mdppPacketDiaOrgLabel", "hanging" /* "middle" */, "start" /* hAlign */);
                mermaid::svg::ByteDiagramFieldLabelDrawInfo  fldi;
                fldi.simpleSvg  = true;
                fldi.svgText    = oss.str();
                fieldLabelsDrawInfo.emplace_back(fldi);
                checkMaxXY();
            }

            curLineBytePos = savedCurLineBytePos;
        }

        // plainIncPosY();
        // posY += lineGap;
        if (curLineBytePos==0) // не было перехода на новую строку байт
        {
            incPosY();
            checkMaxXY();
        }
        curLineBytePos = int(sectionInfo.linearAddress%std::uint64_t(displayWidth));

        //if (secCnt)
        {
        //    plainIncPosY();
        }

        if (curLineBytePos!=0)
        {
            plainIncPosY();
        }


        for(const auto &entry: sectionInfo.entries)
        {
            auto entryName = entry.name;
            // if (entry.bArray)
            //     entryName += "[" + std::to_string(entry.data.size()) + "]";

            if (!entry.bArray)
            {
                UMBA_ASSERT(entry.data.size()==1);
                drawByteEntry(entry.data[0], entryName, 0, std::size_t(-1));
            }
            else // array
            {
                std::size_t idx = std::size_t(-1);
                for(const auto &data: entry.data)
                {
                    ++idx;
                    drawByteEntry(data, entryName, idx, entry.data.size());
                }
            }

            checkMaxXY();
        }
    }

    // Склеиваем fieldLabelDrawInfo
    {
        std::vector<mermaid::svg::ByteDiagramFieldLabelDrawInfo> tmp;

        for(const auto &fldi : fieldLabelsDrawInfo)
        {
            if (tmp.empty())
            {
                tmp.emplace_back(fldi);
            }
            else if (fldi.simpleSvg!=tmp.back().simpleSvg)
            {
                tmp.emplace_back(fldi);
            }
            else if (fldi.fieldName!=tmp.back().fieldName)
            {
                tmp.emplace_back(fldi);
            }
            else if (fldi.posY!=tmp.back().posY)
            {
                tmp.emplace_back(fldi);
            }
            else
            {
                // У нас одна строка, и одно поле
                // надо мержить
                auto &prev = tmp.back();

                UMBA_ASSERT(prev.arraySize==fldi.arraySize);

                // prev.indexFirst // не меняем
                prev.indexLast = fldi.indexLast; // Обновляем индекс последнего элемента
                // prev.posLeft // не меняем
                prev.posRight  = fldi.posRight ; // Обновляем правую позицию
                // prev.posY // не трогаем
                // prev.fieldName // не трогаем
                prev.svgText += fldi.svgText;

            }
        }

        swap(tmp, fieldLabelsDrawInfo);

    }

    std::vector< std::pair<std::string,std::string> > footnotes;
    std::string footnotesSvg;

    if (showFieldLabels)
    {
        int nFootnote = 0;
        //std::stringstream ftss;


        for(auto &fldi : fieldLabelsDrawInfo)
        {
            UMBA_USED(fldi);

            std::stringstream ss;
            auto labelPosY = fldi.posY-6; // +2; // +4; // -0; // 8;
            if (showFieldIndex)
            {
                labelPosY -= indexLabelLineHeight; // рисуем выше меток индекса
                labelPosY += 4;
            }

            const auto centerX    = (fldi.posLeft+fldi.posRight)/2;
            std::string labelText = fldi.fieldName;

            if (fldi.arraySize!=std::size_t(-1)) // array?
            {
                labelText += "[" + std::to_string(fldi.indexFirst) + "-" + std::to_string(fldi.indexLast) + "]";
            }

            // 19 символов - это 442-328+1=115 
            // 6.05 пикселей на символ в среднем
            const auto fieldWidth = fldi.posRight-fldi.posLeft-6;
            auto allowedNumChars = std::size_t(fieldWidth/(fieldLabelAvgSymbolWidth-0u));
            if (allowedNumChars<labelText.size())
            {
                std::string newText =  /* std::string("*") +  */ std::to_string(++nFootnote);
                footnotes.emplace_back(std::make_pair(newText, labelText));

                labelText = std::string("*") + newText;

            }

            mermaid::svg::drawText( ss, centerX, labelPosY
                                  , labelText
                                  , bSmallFonts ? "mdppPacketDiaLabelSmall" : "mdppPacketDiaLabel"
                                  , "auto", "middle"
                                  );

            fldi.svgText = ss.str() + fldi.svgText;
            
        }

    }

    if (showFieldLabels && !footnotes.empty())
    {
        if (curLineBytePos!=0) // не было перехода на новую строку байт
        {
            incPosY();
            checkMaxXY();
        }

        std::stringstream oss;

        posY += fieldLabelLineHeight/2;

        const auto footnotesStartY = posY;
        const auto deltaX = sizeX/3;
        auto footnoteX = 0;
        const auto footnotesSize = footnotes.size(); // -1u
        const std::size_t colHeight = footnotesSize/3u + ((footnotesSize%3u)==0 ? 0u : 1u);
        std::size_t printedInCol = 0;
        posY -= fieldLabelLineHeight;
        auto ftMaxPosY = posY;
        for(std::size_t i=0u; i!=footnotes.size(); ++i, ++printedInCol)
        {
            if (printedInCol >= colHeight)
            {
                posY = footnotesStartY;
                footnoteX += deltaX;
                printedInCol = 0;
            }
            else
            {
                posY += fieldLabelLineHeight;
            }

            checkMaxXY();
            ftMaxPosY = std::max(ftMaxPosY, posY);

            mermaid::svg::drawText( oss, footnoteX+footnoteLeft, posY, footnotes[i].first
                                  , "mdppPacketDiaLabel" // bSmallFonts ? "mdppPacketDiaLabelSmall" : "mdppPacketDiaLabel"
                                  , "auto", "start"
                                  );
            mermaid::svg::drawText( oss, footnoteX+footnoteTextLeft, posY, "- "+footnotes[i].second
                                  , "mdppPacketDiaLabel" // bSmallFonts ? "mdppPacketDiaLabelSmall" : "mdppPacketDiaLabel"
                                  , "auto", "start"
                                  );

        }

        footnotesSvg = oss.str();
        posY = ftMaxPosY;

    }


    for(const auto &fldi : fieldLabelsDrawInfo)
    {
        svgAllText += "<g>\n";
        svgAllText += fldi.svgText;
        svgAllText += "</g>\n";
    }

    svgAllText += "<g>\n";
    svgAllText += footnotesSvg;
    svgAllText += "</g>\n";

    if (!diagram.title.empty() && showTitle && !titleOnTop)
    {
        std::stringstream oss;
        posY += titleLineHeight;
        mermaid::svg::drawText( oss, sizeX/2, posY, diagram.title, "mdppPacketDiaTitle", "hanging", "middle");
        posY += titleLineHeight;
        checkMaxXY();
        svgAllText += oss.str();
    }


    mermaid::svg::writeSvg(std::cout, maxPosX+1, maxPosY, style, svgAllText);

    return 0;
}


