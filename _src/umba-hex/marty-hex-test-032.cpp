/*! \file
    \brief Тестируем лексер UFSM
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
#include "umba/tokenizer/lexers/ufsm.h"
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

#include "marty_containers/insertion_ordered_map.h"



// using TokenType = umba::tokenizer::payload_type; // OperatorTokenTypeT
using OperatorTraitsType = marty::expressions::SampleOperatorTraits< umba::tokenizer::payload_type >;
// PositionInfoType




using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{
    MARTY_ARG_USED(argc);
    MARTY_ARG_USED(argv);

    using insertion_ordered_map = marty::containers::insertion_ordered_map<std::string, std::size_t>;

    insertion_ordered_map iom
    // { { "First" , 1u }
    // , { "Second", 2u }
    // , { "Third" , 3u }
    // , { "Fourth", 4u }
    // , { "Fifth" , 5u }
    // };
    { { "1" , 1u }
    , { "2" , 2u }
    , { "3" , 3u }
    , { "4" , 4u }
    , { "5" , 5u }
    };

    for(const auto &kv : iom)
        std::cout << "\"" << kv.first << "\"" << " = " << kv.second << "\n";
    std::cout << "\n";

    std::cout << "Erase \"2\"\n";
    iom.erase("2");

    for(const auto &kv : iom)
        std::cout << "\"" << kv.first << "\"" << " = " << kv.second << "\n";
    std::cout << "\n";

    std::cout << "Set \"6\"\n";
    iom["6"] = 6u;

    for(const auto &kv : iom)
        std::cout << "\"" << kv.first << "\"" << " = " << kv.second << "\n";
    std::cout << "\n";

    std::cout << "Set \"0\"\n";
    iom["0"] = 0u;

    for(const auto &kv : iom)
        std::cout << "\"" << kv.first << "\"" << " = " << kv.second << "\n";
    std::cout << "\n";

    std::cout << "------------\n";



    std::string data;

    if (umba::isDebuggerPresent())
    {
        data = "42u+12*37|2*5-10";
    }
    else
    {
        data = std::string(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>());
    }
    
    // for(auto &d : data)
    //     d = marty_cpp::normalizeCrLfToLf(d);

    data = marty_cpp::normalizeCrLfToLf(data);

    using TokenizerBuilderType          = decltype(umba::tokenizer::ufsm::makeTokenizerBuilder<char>());
    using TokenizerType                 = decltype(TokenizerBuilderType().makeTokenizer());
    using TokenizerPayloadType          = umba::tokenizer::payload_type;
    using TokenizerIteratorType         = typename TokenizerType::iterator_type;
    using TokenizerTokenParsedDataType  = typename TokenizerType::token_parsed_data_type;
    using TokenCollectionType           = umba::tokenizer::TokenCollection<TokenizerType>;
    using InputIteratorType             = typename TokenizerType::iterator_type;
    using MessagesStringType            = typename TokenizerType::messages_string_type;
    using TokenParsedDataType           = typename TokenizerType::token_parsed_data_type;
    //using ParserType                    = umba::tokenizer::mermaid::PacketDiagramParser<TokenizerType>;

    auto tokenizerBuilder = umba::tokenizer::ufsm::makeTokenizerBuilder<char>();
    //tokenizerBuilder.addOperator(umba::string::make_string<typename TokenizerType::string_type>("**"), UMBA_TOKENIZER_TOKEN_OPERATOR_POWER);

    // using PositionInfoType = std::size_t;
    //  
    // using ExpressionInputItem = marty::expressions::ExpressionInputItem<PositionInfoType, TokenizerPayloadType>;
    //  
    // std::vector<ExpressionInputItem> inputItems;


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
        UMBA_USED(b);
        UMBA_USED(e);

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

        return true; // marty::expressions::tokenizer_helpers::convertTokenizerEvent<TokenizerT>(inputItems, tokenType, b, e, parsedData, positionInfoGenerator);

    };


    auto tokenizer = umba::tokenizer::ufsm::makeTokenizer( tokenizerBuilder
                                                         , tokenHandler
                                                         );
    //tokenizer = umba::tokenizer::ufsm::TokenizerConfigurator()(tokenizer, true /* suffixGluing */ , false /* preprocessorFilter */ );


    tokenizer.unexpectedHandler = [&](auto &tokenizer, InputIteratorType it, InputIteratorType itEnd, const char* srcFile, int srcLine) -> bool
                             {
                                 UMBA_USED(tokenizer); UMBA_USED(it); UMBA_USED(itEnd); UMBA_USED(srcFile); UMBA_USED(srcLine);
                                 // printError(std::cout, inputFilename, UMBA_TOKENIZER_TOKEN_UNEXPECTED, it, itEnd, srcFile, srcLine);
                                 //ctxBasic.hasErrors = true;
                                 std::cerr << "Error: unexpected\n";
                                 return true;
                             };
     
    tokenizer.reportUnknownOperatorHandler = [&](auto &tokenizer, InputIteratorType b, InputIteratorType e)
                             {
                                 //cout << "Possible unknown operator: '" << umba::iterator::makeString(b, e) << "'\n";
                                 UMBA_USED(tokenizer); UMBA_USED(b); UMBA_USED(e);
                                 std::cerr << "Error: unknown operator\n";
                                 //ctxBasic.hasErrors = true;
                             };
     
    tokenizer.reportStringLiteralMessageHandler = [&](auto &tokenizer, bool bErr, InputIteratorType it, const MessagesStringType &msg)
                             {
                                 UMBA_USED(tokenizer); UMBA_USED(bErr); UMBA_USED(it); UMBA_USED(msg);
                                 //ctxBasic.hasErrors = true;
                                 std::cerr << "Warning: something wrong with string literal\n";
                             };


    auto opTraits = OperatorTraitsType();


    auto itBegin = InputIteratorType(data.data(), data.size());
    auto itEnd   = InputIteratorType();
    
    // std::cout << "Expression " << expr << "\n";
    
    if (!umba::tokenizer::utils::tokenizeInput(tokenizer, itBegin, itEnd))
    {
        std::cerr << "Tokenization error\n";
        return -1;
    }
    else
    {
        std::cerr << "Tokenization passed\n";
    }
    

    return 0;
}

