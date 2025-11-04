/*! \file
    \brief Делаем граф UFSM через graphviz
 */


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
#include "marty_dot/marty_dot.h"

//
#include "umba/tokenizer.h"
//
#include "umba/tokenizer/tokenizer_log.h"
#include "umba/tokenizer/tokenizer_log_console.h"
#include "umba/tokenizer/token_collection.h"
//
#include "umba/tokenizer/parsers/ufsm/parser.h"
//
#include "umba/tokenizer/parsers/ufsm/inserters.h"

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
        // std::cout 
        LOG_ERR << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        //std::cout 
        LOG_ERR << "Unknown error\n";
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

    using TokenizerBuilderType          = decltype(umba::tokenizer::ufsm::makeTokenizerBuilder<char>());
    using TokenizerType                 = decltype(TokenizerBuilderType().makeTokenizer());
    using TokenizerPayloadType          = umba::tokenizer::payload_type;
    using TokenizerIteratorType         = typename TokenizerType::iterator_type;
    using TokenizerTokenParsedDataType  = typename TokenizerType::token_parsed_data_type;
    using TokenCollectionType           = umba::tokenizer::TokenCollection<TokenizerType>;
    using ParserType                    = umba::tokenizer::ufsm::Parser<TokenizerType>;


    auto pFilenameSet = std::make_shared<FilenameSetType>();
    auto pParserLog   = std::make_shared<ParserConsoleErrorLog>(pFilenameSet);


    std::string inputFilename;
    std::string fsmNameStr;
    bool useTransitionPoint = false;

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
        // std::cout 
        LOG_MSG << "App Root Path: " << rootPath << "\n";
        // std::cout 
        LOG_MSG << "Working Dir  : " << cwd << "\n";

        //inputFilename = rootPath + "_libs/umba_tokenizer/inc/umba/tokenizer/parsers/ufsm/samples/http.ufsm";
        inputFilename = rootPath + "_libs/umba_tokenizer/inc/umba/tokenizer/parsers/ufsm/samples/traffic_lights.ufsm";
        //fsmNameStr    = "samples//traffic_lights::TrafficLightRoad\\\\";
        fsmNameStr    = "samples//traffic_lights::TrafficLightRoad";

    } // if (umba::isDebuggerPresent())
    else
    {
        if (argc>1)
            inputFilename = argv[1];
        if (argc>2)
            fsmNameStr    = argv[2];
        if (argc>3)
            useTransitionPoint = true;
    }

    if (inputFilename.empty())
    {
        LOG_ERR << "No input file taken\n";
        return 2;
    }

    if (fsmNameStr.empty())
    {
        LOG_ERR << "No FSM name taken\n";
        return 2;
    }

    std::string inputText;
    if (!umba::filesys::readFile(inputFilename, inputText))
    {
        LOG_ERR << "failed to read file: '" << inputFilename << "'\n";
        return 3;
    }

    try
    {
        std::unordered_map<std::string, std::string> frontMatterTags;

        auto textPreparator   = [&](std::string &t) { umba::tokenizer::utils::prepareTextForParsing(t, 0 /* pStyle */ , &frontMatterTags ); };
        auto parserPreparator = [&](auto& p)        { UMBA_USED(p); /* p.setDiagramTitle(frontMatterTags["title"]); */ };

        auto diagram = umba::tokenizer::parserParseData<ParserType>
            ( umba::tokenizer::ufsm::makeTokenizerBuilder<char>() // TokenizerBuilderType()
            , umba::tokenizer::ufsm::TokenizerConfigurator()
            , pFilenameSet
            , pParserLog
            , inputFilename
            , inputText
            , textPreparator
            , parserPreparator
            );

        UMBA_USED(diagram);

        //umba::tokenizer::ufsm::print(std::cout, diagram /* , std::size_t indendSize=0u */ );
        // diagram.print(std::cout);

        // LOG_MSG << "File processed: '" << inputFilename << "'\n\n";
    
        // LOG_MSG << "C/C++ structs:\n\n";
    
        //umba::tokenizer::mermaid::cpp::printCppPacketDiagram( std::cout, diagram );

        using umba::tokenizer::ufsm::FullQualifiedName;
        using umba::tokenizer::ufsm::NamespaceEntry;
        using umba::tokenizer::ufsm::NamespaceEntryKind;
        using umba::tokenizer::ufsm::StateMachineDefinition;

        //auto fsmName = FullQualifiedName("samples/traffic_lights/TrafficLightRoad", "/");
        auto fsmName = FullQualifiedName(fsmNameStr, {"/", "\\", "::", "."});

        NamespaceEntry *pe;
        if (diagram.findEntry(fsmName, &pe))
        {
            LOG_MSG << "Found entry\n";

            auto kind = getNamespaceEntryKind(*pe);
            if (kind!=NamespaceEntryKind::fsmDefinition)
            {
                LOG_MSG << "Found entry has invalid kind, not an fsmDefinition\n";
                return 1;
            }

            StateMachineDefinition *pFsm = std::get_if<StateMachineDefinition>(pe);
            if (!pFsm)
            {
                LOG_MSG << "Something goes wrong while calling get\n";
                return 1;
            
            }
            try
            {
                // Shapes
                // https://graphviz.org/doc/info/shapes.html#polygon
                // FSM exqample https://graphviz.org/Gallery/directed/fsm.html
                // label - https://graphviz.org/docs/attrs/label/
                // html label - https://graphviz.org/doc/info/shapes.html#html
                // xlabel - https://graphviz.org/docs/attrs/xlabel/

                pFsm->assignStateIds();

                std::cerr << "------------\n";
                auto trVec = pFsm->makeExpandedTransitionsForGraph();
                std::cerr << "------------\n";
                for(const auto &transition : trVec)
                    std::cerr << transition << "\n";

                std::cerr << "------------\n";

                std::cout << "digraph " << pFsm->name << "\n{\n";
                std::cout << "  fontname=\"Helvetica,Arial,sans-serif\"\n"
                          << "  node [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                          << "  edge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                          << "  rankdir=LR;\n";

                for(const auto &kv : pFsm->states)
                {
                    std::cout << "  " << kv.second.id << " [label=<" << marty::dot::ecapeHtmlLabelString(kv.second.name) << ">";
                    if (kv.second.isError())
                    {
                        std::cout << " shape=Mcircle"; // doublecircle
                    }
                    else if (kv.second.isInitial() || kv.second.isFinal())
                    {
                        std::cout << " shape=doublecircle"; // doublecircle
                    }
                    else
                    {
                        std::cout << " shape=ellipse"; // oval circle point
                    }

                    std::cout << "]\n";

                    if (kv.second.isInitial())
                    {
                        std::cout << "  e" << kv.second.id << " [shape=point]\n";
                        std::cout << "  e" << kv.second.id << " -> " << kv.second.id << "\n";
                    }

                }

    // Выносим петлю подальше
    // A -> P [arrowhead=none, color=black, penwidth=1, minlen=2];
    // P -> A [color=black, penwidth=1];
    // P [shape=point, width=0, height=0, penwidth=0.7, color=black];

    // // Создаём метки как отдельные невидимые узлы
    // L1 [label="Петля 1", shape=plaintext, width=0, height=0];
    // L2 [label="Петля 2", shape=plaintext, width=0, height=0];
    //  
    // // Привязываем метки к вершине без видимых рёбер
    // A -> L1 [style=invis, constraint=false];
    // A -> L2 [style=invis, constraint=false];
    //  
    // // Основная петля
    // A -> A [style=dot, weight=100];
    //  
    // // Размещаем метки вокруг вершины
    // {rank=same; A; L1; L2}
    // L1 -> A [style=invis];
    // A -> L2 [style=invis];



                auto transitions = pFsm->makeExpandedTransitionsForGraph();
                std::size_t id = 0;
                for(auto &&transition : transitions)
                {
                    transition.id = ++id;
                    auto sourceStateName = transition.getSourceState();
                    auto targetStateName = transition.getTargetState();
                    if (transition.isSelfTarget())
                        targetStateName = sourceStateName;

                    auto sourceStateIt = pFsm->states.find(sourceStateName);
                    if (sourceStateIt==pFsm->states.end())
                    {
                        throw std::runtime_error("Unknown source state " + sourceStateName);
                    }

                    auto targetStateIt = pFsm->states.find(targetStateName);
                    if (targetStateIt==pFsm->states.end())
                    {
                        throw std::runtime_error("Unknown target state " + targetStateName);
                    }

                    std::string transitionLabelStr;
                    transitionLabelStr.append(transition.eventsAsString());
                    if (transition.hasAdditionalCondition())
                    {
                        transitionLabelStr.append("\n");
                        transitionLabelStr.append("[");
                        transitionLabelStr.append(transition.additionalConditionAsString());
                        transitionLabelStr.append("]");
                    }

                    if (transition.isSelfTarget())
                    {
                        if (useTransitionPoint)
                        {
                            std::cout << "  " << sourceStateIt->second.id << " -> " << "t" << transition.id << " [arrowhead=none];\n";
                            std::cout << "  " << "t" << transition.id << " -> " << targetStateIt->second.id << ";\n"; //  "[arrowhead=none];\n"
                            std::cout << "  " << "t" << transition.id << " [shape=point, width=0, height=0, penwidth=0.7];\n";
                            std::cout << "  " << "l" << transition.id << " [label=<" << marty::dot::ecapeHtmlLabelString(transitionLabelStr) << ">, shape=plaintext, width=0, height=0];\n";
                            std::cout << "  " << "l" << transition.id << " -> " << "t" << transition.id << " [style=invis];\n";
                            std::cout << "  " << "{rank=same; " << sourceStateIt->second.id << "; l" << transition.id << "; " << "t" << transition.id << "}\n";
                        }
                        else // Обычный алгоритм
                        {
                            std::cout << "  " << sourceStateIt->second.id << " -> " << targetStateIt->second.id; // << "\n";
                            std::cout << " [label=<" << marty::dot::ecapeHtmlLabelString(transitionLabelStr) << ">];\n";
                        }
                    }
                    else
                    {
                        std::cout << "  " << sourceStateIt->second.id << " -> " << targetStateIt->second.id; // << "\n";
                        std::cout << " [label=<" << marty::dot::ecapeHtmlLabelString(transitionLabelStr) << ">];\n";
                    }

                }

                std::cout << "}\n";


            }
            catch(const std::exception &e)
            {
                LOG_ERR << "Error: " << e.what() << "\n";
                return 4;
            }
            catch(...)
            {
                LOG_ERR << "there is some errors\n";
                return 4;
            }
        }

    }
    catch(...)
    {
        LOG_ERR << "there is some errors\n";
        return 4;
    }

    return 0;
}




