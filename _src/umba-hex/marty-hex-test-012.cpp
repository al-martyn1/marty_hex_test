/*! \file
    \brief Рисуем SVG, drawWord и прочие тесты примитивов.
 */


/*

Destination Port - 16 символов. На бит диаграмме mermaid packet diagram (MPD)занимает немного места внутри прямоугольника пакета.
MPD в ширину 32 бита. "Destination Port" 4 раза влезает в половину ширины.

Hardware Revision Minor - 23 символа

Ширина байтовой диаграммы - 8, 16, 32, 64, 128, 256 байт.

Каждый байт/слово/и тп отображаем прямоугольником соответсвующего кратного размера.
Что делать, если поле размером больше байта не влезает в строку?
Можно нормальные границы байт/слов/и тп отображать закруглёнными, а разрыв - прямыми углами, 
типа: 
```
(  )(  )(  )(      )(  ]
[          )(  )(      )
```

Внутри каждого поля пишем его название.

Как отображать массивы?
Как отдельные байты/слова/и тп? А как показать, что это массив?

Как показывать порядок байт в словах? Внутри прямоугольника слова
разделить его на байты, и там проставить номера (0 - самый младший?)

А надписи с именами полей расположить не внутри, а над?
Как отображать смещение поля от начала/адрес поля?

? Отдельные поля рисуем так, чтобы между ними были широкие пробелы,
  а между эелементами массива - без пробелов или с минимальными

? Или отдельные поля рисуем без разделения?

Нам надо отобразить слово/дв. слово/и тп как прямоугольник с закруглениями.
Его надо разбить на байты, в каждом байте проставить его номер.
Нумерация байт - от 0 до N, нулевой байт - LSB, N - MSB. На диаграмме
так и  рисуем - "0 (LSB)", "N (MSB)".
Для одиночных байт номер не рисуем.

Размер байта по ширине вычисляем как 
style="max-width: 1026px;" - 1024/кол-во байт в строке - 8, 16, 32, 64
1024/64=16 px - не много, при высоте в 26.
1024/32=32




Consistent Overhead Byte Stuffing (COBS) - https://blog.mbedded.ninja/programming/serialization-formats/consistent-overhead-byte-stuffing-cobs/


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
#include <sstream>

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

#include "umba/tokenizer/parsers/mermaid/packet_diagram_parser.h"



//#define MERMAID_TYPE_UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST

//struct

#include "svg-style.h"

// https://en.wikipedia.org/wiki/List_of_monospaced_typefaces

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

    inputText = marty_cpp::normalizeCrLfToLf(inputText);
    std::unordered_map<std::string, std::string> frontMatterTags;
    umba::tokenizer::utils::prepareTextForParsing(inputText, 0 /* pStyle */ , &frontMatterTags );

    TokenizerBuilderType tokenizerBuilder = umba::tokenizer::mermaid::packet_diagram::makeTokenizerBuilder<char>();
    auto pTokenCollection = std::make_shared<TokenCollectionType>( tokenizerBuilder.makeTokenizer()
                                                                 , umba::tokenizer::mermaid::packet_diagram::TokenizerConfigurator()
                                                                 , pParserLog
                                                                 , inputText
                                                                 , pFilenameSet->addFile(inputFilename)
                                                                 );
    ParserType parser = ParserType(pTokenCollection, pFilenameSet, pParserLog);
    parser.setDiagramTitle(frontMatterTags["title"]);

    if (!parser.parse())
    {
        LOG_ERR << "there is some errors\n";
        return 4;
    }


    std::stringstream oss;

    #if 0
    marty::svg::drawRect(oss, 50,  50, 200, 26, "mdppPacketDiaBlock", false, false, 8);
    marty::svg::drawRect(oss, 50, 100, 200, 26, "mdppPacketDiaBlock", false, true , 8);
    marty::svg::drawRect(oss, 50, 150, 200, 26, "mdppPacketDiaBlock", true , false, 8);
    marty::svg::drawRect(oss, 50, 200, 200, 26, "mdppPacketDiaBlock", true , true , 8);

    mermaid::svg::ByteDiagramViewEntryData  ebv4;
    mermaid::svg::ByteDiagramViewEntryData  ebv8;
    ebv4.bytes = mermaid::svg::makeNumbersVector(0, 3);
    ebv8.bytes = mermaid::svg::makeNumbersVector(0, 7);

    #if 0
    // Хз что это такое, не помню уже
    mermaid::svg::drawWord( oss, ebv4, ebv4.idxFirst, 50, 250
                          , "mdppPacketDiaBlock"
                          , "mdppPacketDiaBlock"
                          , "mdppArrayBoundsLine"
                          , "mdppPacketDiaByteNumberLabel"
                          , "mdppPacketDiaIndexLabel"
                          , 32, 26 // byte width, line height
                          , 8, 4   // r, indent
                          , false // entryData.bFlatByteArray
                          , false // bSmallFonts
                          , 0, 3   // range
                          );

    mermaid::svg::drawWord(oss, ebv8, ebv8.idxFirst, 50, 250
                         , "mdppPacketDiaBlock"
                         , "mdppPacketDiaBlock"
                         , "mdppArrayBoundsLine"
                         , "mdppPacketDiaByteNumberLabel"
                         , "mdppPacketDiaIndexLabel"
                         , 32, 26 // byte width, line height
                         , 8, 4   // r, indent
                         , false // entryData.bFlatByteArray
                         , false // bSmallFonts
                         , 0, 3   // range
                         );
    #endif
    #endif


    //mermaid::svg::byte_vector_t bv4 = mermaid::svg::makeNumbersVector(0, 3);
    //mermaid::svg::byte_vector_t bv8 = mermaid::svg::makeNumbersVector(0, 7);

    std::string strBlack     = "black";
    std::string strGreen     = "green";
    std::string strBlue      = "blue" ;
    std::string strRed       = "red"  ;
    std::string strFireBrick = "#B22222"  ; // #B22222 fireBrick
    std::string strAqua      = "aqua" ;
    
    std::string strEmpty;
 
    #if 0
    // хз чо
    marty::svg::drawRectEx( oss,  50, 250, 200, 45
                          , 10 // r
                          , 3 // strokeWidth
                          , strGreen // strokeColor
                          , strAqua // strRed
                          , marty::svg::RoundRectFlags::round
                          );

    // Полукруг
    auto roundLeftRightTop = marty::svg::RoundRectFlags::roundLeftTop | marty::svg::RoundRectFlags::roundRightTop;
    marty::svg::drawRectEx( oss, 50, 350, 90, 45
                          , 45 // r
                          , 3 // strokeWidth
                          , strGreen // strokeColor
                          , strEmpty // "red" //
                          , roundLeftRightTop
                          );

    // Справа
    marty::svg::drawRectEx( oss, 260, 250, 200, 45
                          , 10 // r
                          , 3 // strokeWidth
                          , strGreen // strokeColor
                          , strEmpty // "red" //
                          , marty::svg::RoundRectFlags::none
                          );

    // Снизу
    marty::svg::drawRectEx( oss, 50, 300, 200, 45
                          , 10 // r
                          , 3 // strokeWidth
                          , strGreen // strokeColor
                          , strEmpty // "red" //
                          , marty::svg::RoundRectFlags::none
                          );

    // Снизу справа
    marty::svg::drawRectEx( oss, 260, 300, 200, 45
                          , 10 // r
                          , 3 // strokeWidth
                          , strGreen // strokeColor
                          , strEmpty // "red" //
                          , marty::svg::RoundRectFlags::roundLeftTop | marty::svg::RoundRectFlags::roundRightBottom
                          );
    #endif


    // Пример корпуса с крышкой люка

    // Корпус
    int baseX = 100;
    int baseY = 100;
    int rackSizeX  = 400;
    int rackSizeY  = 160;
    int slotSizeX  = 100;
    int slotSizeY  =  80;
    int slotShiftY =  10;
    int latchSize  =  10;
    int latchShift =  25;
    marty::svg::drawRectEx( oss
                          , baseX, baseY
                          , rackSizeX, rackSizeY
                          , 10 // r
                          , 2 // strokeWidth
                          , strBlack // strokeColor
                          , strFireBrick //
                          , marty::svg::RoundRectFlags::noRound
                          );

    // Откидная крышка
    marty::svg::drawRectEx( oss
                          , baseX+rackSizeX/2-slotSizeX/2, baseY+rackSizeY/2-slotSizeY/2+slotShiftY
                          , slotSizeX, slotSizeY
                          , 20 // r
                          , 2 // strokeWidth
                          , strBlack // strokeColor
                          , strEmpty // strRed //
                          , marty::svg::RoundRectFlags::roundTop
                          );

    marty::svg::drawRectEx( oss
                          , baseX+rackSizeX/2-latchSize, baseY+rackSizeY/2-latchSize/2-latchShift
                          , latchSize*2, latchSize
                          , latchSize // r
                          , 2 // strokeWidth
                          , strBlack // strokeColor
                          , strEmpty // strRed //
                          , marty::svg::RoundRectFlags::roundBottom
                          );

    // Снизу
    #if 0
    marty::svg::drawRectEx( oss, 310, 300, 200, 45
                          , 100 // r
                          , strGreen // strokeColor
                          , 3 // strokeWidth
                          , strEmpty // "red" //
                          , marty::svg::RoundRectFlags::none
                          );
    #endif

    // std::string strGreen = "green";
    // std::string strBlue  = "blue" ;
    // std::string strRed   = "red"  ;
    // std::string strEmpty;


    mermaid::svg::writeSvg(std::cout, 1026, 423, style, oss.str());

 
    return 0;
}




