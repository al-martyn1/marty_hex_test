/*! \file
    \brief Тестируем marty_expressions
 */

#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"

//
#include "umba/tokenizer.h"
//
#include "umba/tokenizer/utils.h"
//
#include "umba/tokenizer/tokenizer_log.h"
#include "umba/tokenizer/tokenizer_log_console.h"
#include "umba/tokenizer/token_collection.h"
//
#include "umba/tokenizer/lang/cpp.h"
//

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"

//
#include "marty_expressions/marty_expressions.h"
//
#include "marty_expressions/tokenizer_helpers.h"
//
#include "marty_expressions/enum_descriptions.h"
//
#include "marty_expressions/sample_op_traits.h"



// using TokenType = umba::tokenizer::payload_type; // OperatorTokenTypeT
using OperatorTraitsType = marty::expressions::SampleOperatorTraits< umba::tokenizer::payload_type >;
// PositionInfoType




using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{
    MARTY_ARG_USED(argc);
    MARTY_ARG_USED(argv);

    std::vector<std::string> data;

    // std::string data;

    if (umba::isDebuggerPresent())
    {
        data = std::vector<std::string>
        { "42u+12*37|2*5-10"
        //, "+42u+12*37"
        //, "/42u+12*37"
        //, "42u+12*37+::::std::sqrt(64)"
        };
    }
    else
    {
        data.emplace_back(std::string(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>()));
    }
    
    for(auto &d : data)
        d = marty_cpp::normalizeCrLfToLf(d);

    using TokenizerBuilderType          = decltype(umba::tokenizer::cpp::makeTokenizerBuilder<char>());
    using TokenizerType                 = decltype(TokenizerBuilderType().makeTokenizer());
    using TokenizerPayloadType          = umba::tokenizer::payload_type;
    using TokenizerIteratorType         = typename TokenizerType::iterator_type;
    using TokenizerTokenParsedDataType  = typename TokenizerType::token_parsed_data_type;
    using TokenCollectionType           = umba::tokenizer::TokenCollection<TokenizerType>;
    using InputIteratorType             = typename TokenizerType::iterator_type;
    using MessagesStringType            = typename TokenizerType::messages_string_type;
    using TokenParsedDataType           = typename TokenizerType::token_parsed_data_type;
    //using ParserType                    = umba::tokenizer::mermaid::PacketDiagramParser<TokenizerType>;

    auto tokenizerBuilder = umba::tokenizer::cpp::makeTokenizerBuilder<char>();

    using PositionInfoType = std::size_t;

    using ExpressionInputItem = marty::expressions::ExpressionInputItem<PositionInfoType, TokenizerPayloadType>;

    std::vector<ExpressionInputItem> inputItems;


    auto tokenHandler = [&](auto& tokenizer
        , bool bLineStart, umba::tokenizer::payload_type tokenType
        , InputIteratorType b, InputIteratorType e
        , TokenParsedDataType parsedData // std::basic_string_view<tokenizer_char_type> parsedData
        , MessagesStringType& errMsg
        ) -> bool
    {
        UMBA_USED(tokenizer);
        UMBA_USED(bLineStart);
        UMBA_USED(parsedData);
        UMBA_USED(errMsg);

#if defined(_DEBUG)

        auto dataPtr   = b != e ? static_cast<const char*>(b) : static_cast<const char*>(0);
        auto tokenText = b != e ? umba::tokenizer::utils::makeTokenText(tokenType, b, e) : std::string();

        UMBA_USED(dataPtr);
        UMBA_USED(tokenText);

#endif

        if (tokenType == UMBA_TOKENIZER_TOKEN_CTRL_RST || tokenType == UMBA_TOKENIZER_TOKEN_CTRL_FIN)
            return true;

        if (tokenType & UMBA_TOKENIZER_TOKEN_CTRL_FLAG)
        {
            return true; // Управляющий токен, не надо выводить, никакой нагрузки при нём нет
        }

        using TokenizerT = std::decay_t<decltype(tokenizer)>;

        std::size_t posInfoIdx = 0;
        using PositionInfoType = std::decay_t<decltype(posInfoIdx)>;

        auto positionInfoGenerator = [&](InputIteratorType b, InputIteratorType e)
        {
            UMBA_USED(b); UMBA_USED(e);
            return std::size_t(0);
        };

        using OperatorTokenType = umba::tokenizer::payload_type;
        using IntegerType       = typename TokenizerT::integer_type;
        using FloatingPointType = typename TokenizerT::floating_point_type;
        using StringType        = typename TokenizerT::string_type;

        return marty::expressions::tokenizer_helpers::convertTokenizerEvent<TokenizerT>(inputItems, tokenType, b, e, parsedData, positionInfoGenerator);

    };



    auto tokenizer = umba::tokenizer::cpp::makeTokenizer( tokenizerBuilder
                                                      , tokenHandler
                                                      );

    tokenizer.unexpectedHandler = [&](auto &tokenizer, InputIteratorType it, InputIteratorType itEnd, const char* srcFile, int srcLine) -> bool
                             {
                                 UMBA_USED(tokenizer); UMBA_USED(it); UMBA_USED(itEnd); UMBA_USED(srcFile); UMBA_USED(srcLine);
                                 // printError(std::cout, inputFilename, UMBA_TOKENIZER_TOKEN_UNEXPECTED, it, itEnd, srcFile, srcLine);
                                 //ctxBasic.hasErrors = true;
                                 return true;
                             };
     
    tokenizer.reportUnknownOperatorHandler = [&](auto &tokenizer, InputIteratorType b, InputIteratorType e)
                             {
                                 //cout << "Possible unknown operator: '" << umba::iterator::makeString(b, e) << "'\n";
                                 UMBA_USED(tokenizer); UMBA_USED(b); UMBA_USED(e);
                                 //ctxBasic.hasErrors = true;
                             };
     
    tokenizer.reportStringLiteralMessageHandler = [&](auto &tokenizer, bool bErr, InputIteratorType it, const MessagesStringType &msg)
                             {
                                 UMBA_USED(tokenizer); UMBA_USED(bErr); UMBA_USED(it); UMBA_USED(msg);
                                 //ctxBasic.hasErrors = true;
                             };


    auto opTraits = OperatorTraitsType();

    #if 0
    TokenType bracketTokens[] =
    { UMBA_TOKENIZER_TOKEN_CURLY_BRACKET_OPEN 
    , UMBA_TOKENIZER_TOKEN_CURLY_BRACKET_CLOSE
    , UMBA_TOKENIZER_TOKEN_ROUND_BRACKET_OPEN 
    , UMBA_TOKENIZER_TOKEN_ROUND_BRACKET_CLOSE
    , UMBA_TOKENIZER_TOKEN_ANGLE_BRACKET_OPEN 
    , UMBA_TOKENIZER_TOKEN_ANGLE_BRACKET_CLOSE
    , UMBA_TOKENIZER_TOKEN_SQUARE_BRACKET_OPEN 
    , UMBA_TOKENIZER_TOKEN_SQUARE_BRACKET_CLOSE
    , UMBA_TOKENIZER_TOKEN_DBLSQUARE_BRACKET_OPEN 
    , UMBA_TOKENIZER_TOKEN_DBLSQUARE_BRACKET_CLOSE
    , UMBA_TOKENIZER_TOKEN_SPACE
    , UMBA_TOKENIZER_TOKEN_IDENTIFIER
    };

    std::cout << "Brackets kind:\n";

    for(auto br: bracketTokens)
    {
        std::cout << br << ": " << enum_serialize(opTraits.getBracketKind(br)) << ", " << enum_serialize_flags(opTraits.getOperatorType(br)) << "\n";
    }
    #endif

    using ExpressionParserType = marty::expressions::ExpressionParser<OperatorTraitsType, PositionInfoType, umba::tokenizer::payload_type >;

    auto exprParser = ExpressionParserType( opTraits
                                          , marty::expressions::OperatorFeatures::regular
                                          | marty::expressions::OperatorFeatures::groupping
                                          | marty::expressions::OperatorFeatures::functionCall
                                          | marty::expressions::OperatorFeatures::indexation
                                         //|| marty::expressions::OperatorFeatures::
                                          );

    auto testExpressionParsing = [&](const std::string &expr, bool mergeNames) -> int
    {
        inputItems.clear();

        auto itBegin = InputIteratorType(expr.data(), expr.size());
        auto itEnd   = InputIteratorType();
    
        std::cout << "Expression " << expr << "\n";
    
        if (!umba::tokenizer::utils::tokenizeInput(tokenizer, itBegin, itEnd))
        {
            std::cerr << "Tokenization error\n";
            return -1;
        }
    
        if (mergeNames)
        {
            auto inputItemsMerged = marty::expressions::tokenizer_helpers::mergeFullQualifiedIdents(inputItems, UMBA_TOKENIZER_TOKEN_OPERATOR_SCOPE_RESOLUTION, std::string("/"));
            UMBA_USED(inputItemsMerged);
            swap(inputItemsMerged, inputItems);
        }

        auto res = exprParser.initialize();
        if (res!=marty::expressions::Error::ok)
        {
            std::cerr << "Error: " << exprParser.getErrorMessage(res) << "\n\n";
            return 0;
        }
    
        for(const auto &tk: inputItems)
        {
            res = exprParser.parse(tk);
            if (res!=marty::expressions::Error::ok)
            {
                std::cerr << "\n";
                exprParser.dump(std::cerr);
                std::cerr << "\n";
                std::cerr << "Error: " << exprParser.getErrorMessage(res) << "\n\n";
                return 0;
            }
        }
    
        res = exprParser.finalize();
        if (res!=marty::expressions::Error::ok)
        {
            std::cerr << "\n";
            exprParser.dump(std::cerr);
            std::cerr << "\n";
            std::cerr << "Error: " << exprParser.getErrorMessage(res) << "\n\n";
            return 0;
        }

        std::cout << "\n";
        exprParser.dump(std::cout);
        std::cout << "\n";

        return 1;
    
    };


    std::size_t totalCnt  = 0;
    std::size_t passedCnt = 0;

    bool mergeNames = true;
    for(const auto& d : data)
    {
        ++totalCnt;

        auto res = testExpressionParsing(d, mergeNames);
        if (res<0)
        {
            std::cerr << "Fatal error, exiting\n";
            return 1;
        }

        if (res)
            ++passedCnt;
    }

    std::cout << "Total tests: " << totalCnt << ", passed: " << passedCnt << ", failed: " << (totalCnt-passedCnt) << "\n";

    //std::vector<std::string> data;

    // UMBA_USED(exprParser);



    return 0;
}

