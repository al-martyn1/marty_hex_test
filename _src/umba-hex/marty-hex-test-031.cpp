/*! \file
    \brief Тестируем marty_expressions - простой парсинг булевых выражений
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
#include "umba/tokenizer/lexers/cpp.h"
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
#include "marty_expressions/simple_bool_expressions.h"
//
#include "marty_expressions/tokenizer_helpers.h"
//
#include "marty_expressions/enum_descriptions.h"
//
#include "marty_expressions/sample_op_traits.h"
//
#include "marty_expressions/utils.h"
//
#include "marty_expressions/truth_table.h"




// using TokenType = umba::tokenizer::payload_type; // OperatorTokenTypeT
//using OperatorTraitsType = marty::expressions::SampleOperatorTraits< umba::tokenizer::payload_type >;
using OperatorTraitsType = marty::expressions::SimpleBoolExpressionOperatorTraits< umba::tokenizer::payload_type >;
// PositionInfoType


enum Action
{
    doConstantAbsorption          ,
    doMultiAry                    ,
    doCollapseParentheses         ,
    doRemoveParentheses           ,
    doPromoteNegations            ,
    doRemoveUnnecessaryParentheses,
    doCollapseSameVars            ,
    doAddParentheses              ,
    doConvertNegativeOperands     ,
    doSort                        ,
    doSimplify
};

// bool bShowLabels    = false;

inline
std::string getActionName(Action a)
{
    switch(a)
    {
        case doConstantAbsorption          : return "ConstantAbsorption ";
        case doMultiAry                    : return "MultiAry           ";
        case doCollapseParentheses         : return "CollapseParentheses";
        case doRemoveParentheses           : return "RemoveParentheses  ";
        case doPromoteNegations            : return "PromoteNegations   ";
        case doRemoveUnnecessaryParentheses: return "RemoveUnnecessary()";
        case doCollapseSameVars            : return "CollapseSameVars   ";
        case doAddParentheses              : return "AddParentheses     ";
        case doConvertNegativeOperands     : return "ConvertNegatives   ";
        case doSort                        : return "Sort               ";
        case doSimplify                    : return "Simplify           ";
        default: return "<UNKNOWN>";
    }
}

inline
std::string getActionNameShort(Action a)
{
    switch(a)
    {
        case doConstantAbsorption          : return "CA";
        case doMultiAry                    : return "MA";
        case doCollapseParentheses         : return "CP";
        case doRemoveParentheses           : return "RP";
        case doPromoteNegations            : return "PN";
        case doRemoveUnnecessaryParentheses: return "RU";
        case doCollapseSameVars            : return "CS";
        case doAddParentheses              : return "AP";
        case doConvertNegativeOperands     : return "CN";
        case doSort                        : return "SR";
        case doSimplify                    : return "SM";
        default: return "UN";
    }
}


using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{
    MARTY_ARG_USED(argc);
    MARTY_ARG_USED(argv);

    struct TestDataItem
    {
        std::string  expressionString;
        int          expectedResult  ; // -1 - tokenization error, 0 - expression parsing error, 1 - Ok
    };

    // std::vector<std::string> data;

    // std::string data;

    bool bShowLabels       = false;
    bool bShowLabelsShort  = false;
    bool bShowFinalLabel   = false;
    bool bFail             = false;
    bool bUseNamedOps      = false;
    bool bPrintTruthTable  = false;
    std::vector<Action> actions;


    std::vector<TestDataItem> data;

    if (umba::isDebuggerPresent())
    {
        data = std::vector<TestDataItem>
        { { "(h&g&f&e)|q|p|o|(a1 or !(!(b1&c))|a1) & not !((a2||b2|a2)) | c&!true | e&!(((r|t|!(!y&u&!u))|q|e)&h&g|f)", 1 }
             
        //, { "(a1 or b1) & not (a2||b2) | c&!d", 1 }
        //, { "a1 | b1 & (a2|b2) | c&!d", 1 }
        //, { "a1 | b1 & a2 | b2 | c & !d", 1 }
        //, { "a1 | b1 & (a2|b2 | c&!d", 0 }

        //, "+42u+12*37"
        //, "/42u+12*37"
        //, "42u+12*37+::::std::sqrt(64)"
        };

        // -p -M -C -N

        //bOptimize                     = true;
        bUseNamedOps     = true;
        bPrintTruthTable = true;
        bShowFinalLabel  = true;

        #if 1
        // CA  MA  CP  PN  RU  CS  CA  CN
        actions.push_back(doConstantAbsorption          );
        actions.push_back(doMultiAry                    );
        actions.push_back(doCollapseParentheses         );
        actions.push_back(doPromoteNegations            );
        actions.push_back(doRemoveUnnecessaryParentheses);
        actions.push_back(doCollapseSameVars            );
        actions.push_back(doConvertNegativeOperands     );
        actions.push_back(doAddParentheses              );
        actions.push_back(doSort                        );
        #else
        actions.push_back(doSimplify                    );
        #endif

    }
    else
    {
        int argIdx = 1;
        while(argIdx<argc)
        {
            auto arg = std::string(argv[argIdx]);
            if (arg.empty() || arg[0]!='-')
                break;

            if (std::string(argv[argIdx])=="-f")       { bFail = true;  ++argIdx;  }
            else if (std::string(argv[argIdx])=="-n")  { bUseNamedOps = true; ++argIdx; }
            else if (std::string(argv[argIdx])=="-l")  { bShowLabels = true; ++argIdx; }
            else if (std::string(argv[argIdx])=="-s")  { bShowLabelsShort = true; ++argIdx; }
            else if (std::string(argv[argIdx])=="-t")  { bPrintTruthTable = true; ++argIdx; }
            else if (std::string(argv[argIdx])=="-a")  { bShowFinalLabel = true; ++argIdx; }

            else if (std::string(argv[argIdx])=="-A")  { actions.push_back(doConstantAbsorption); ++argIdx; }
            else if (std::string(argv[argIdx])=="-M")  { actions.push_back(doMultiAry); ++argIdx; }
            else if (std::string(argv[argIdx])=="-C")  { actions.push_back(doCollapseParentheses); ++argIdx; }
            else if (std::string(argv[argIdx])=="-R")  { actions.push_back(doRemoveParentheses); ++argIdx; }
            else if (std::string(argv[argIdx])=="-N")  { actions.push_back(doPromoteNegations); ++argIdx; }
            else if (std::string(argv[argIdx])=="-U")  { actions.push_back(doRemoveUnnecessaryParentheses); ++argIdx; }
            else if (std::string(argv[argIdx])=="-L")  { actions.push_back(doCollapseSameVars); ++argIdx; }
            else if (std::string(argv[argIdx])=="-P")  { actions.push_back(doAddParentheses); ++argIdx; }
            else if (std::string(argv[argIdx])=="-G")  { actions.push_back(doConvertNegativeOperands); ++argIdx; }
            else if (std::string(argv[argIdx])=="-O")  { actions.push_back(doSort); ++argIdx; }

            else if (std::string(argv[argIdx])=="-S")  { actions.push_back(doSimplify); ++argIdx; }

            else
            {
                break;
            }
        }

        if (argIdx<argc)
        {
            data.emplace_back(TestDataItem{std::string(argv[argIdx]), (bFail?0:1) });
        }
        else
        {
            data.emplace_back(TestDataItem{std::string(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>()), (bFail?0:1) });
        }
    }
    
    for(auto &d : data)
        d.expressionString = marty_cpp::normalizeCrLfToLf(d.expressionString);

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
    tokenizerBuilder.addOperator(umba::string::make_string<typename TokenizerType::string_type>("**"), UMBA_TOKENIZER_TOKEN_OPERATOR_POWER);

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
    tokenizer = umba::tokenizer::cpp::TokenizerConfigurator()(tokenizer, true /* suffixGluing */ , false /* preprocessorFilter */ );


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


    auto opTraits = OperatorTraitsType{ UMBA_TOKENIZER_TOKEN_ROUND_BRACKET_OPEN 
                                      , UMBA_TOKENIZER_TOKEN_ROUND_BRACKET_CLOSE
                                      , UMBA_TOKENIZER_TOKEN_OPERATOR_LOGICAL_NOT // UMBA_TOKENIZER_TOKEN_OPERATOR_BITWISE_NOT
                                      , UMBA_TOKENIZER_TOKEN_OPERATOR_LOGICAL_AND // UMBA_TOKENIZER_TOKEN_OPERATOR_BITWISE_AND
                                      // , UMBA_TOKENIZER_TOKEN_OPERATOR_BITWISE_XOR
                                      , UMBA_TOKENIZER_TOKEN_OPERATOR_LOGICAL_OR  // UMBA_TOKENIZER_TOKEN_OPERATOR_BITWISE_OR
                                      };


    using ExpressionParserType = marty::expressions::SimpleBoolExpressionParser<OperatorTraitsType, PositionInfoType, umba::tokenizer::payload_type >; // (opTraits);
                                                  // ExpressionParser<OperatorTraitsType, PositionInfoType, umba::tokenizer::payload_type >;

    using EvaluatorType        = marty::expressions::SimpleBoolExpressionEvaluator<OperatorTraitsType, PositionInfoType, umba::tokenizer::payload_type >;


    auto exprParser = ExpressionParserType(opTraits  
                                          /*, marty::expressions::OperatorFeatures::regular
                                          | marty::expressions::OperatorFeatures::groupping
                                          | marty::expressions::OperatorFeatures::functionCall
                                          | marty::expressions::OperatorFeatures::indexation
                                         //|| marty::expressions::OperatorFeatures::
                                           */ 
                                          );
    auto evaluator = EvaluatorType(opTraits);
    //auto evaluator = exprParser.getEvaluator();

    using TruthTable = marty::expressions::TruthTable<typename EvaluatorType::StringType>;

    TruthTable ttMerged;



    auto testExpressionParsing = [&](const std::string &expr, bool mergeNames) -> int
    {
        //UMBA_USED(mergeNames);

        inputItems.clear();

        auto itBegin = InputIteratorType(expr.data(), expr.size());
        auto itEnd   = InputIteratorType();
    
        std::cerr << "Expression: " << expr << "\n";
    
        if (!umba::tokenizer::utils::tokenizeInput(tokenizer, itBegin, itEnd))
        {
            std::cerr << "Tokenization error\n";
            return -1;
        }

        if (mergeNames)
        {
            auto inputItemsMerged = marty::expressions::tokenizer_helpers::mergeFullQualifiedIdents(inputItems, UMBA_TOKENIZER_TOKEN_OPERATOR_SCOPE_RESOLUTION, std::string("/"));
            // UMBA_USED(inputItemsMerged);
            swap(inputItemsMerged, inputItems);
        }

        // Можно использовать символы как логических, так и битовых операторов
        // Символ '~' часто используется при записи логических выражений для отрицаний, как и символ '!'.
        // Для унификации с ЯВУ, такими, как, например, C++, позволяем использовать '&&'/'||', так и '&'/'|'.
        marty::expressions::tokenizer_helpers::replaceOperatorTokenInplace( inputItems, UMBA_TOKENIZER_TOKEN_OPERATOR_BITWISE_NOT, UMBA_TOKENIZER_TOKEN_OPERATOR_LOGICAL_NOT);
        marty::expressions::tokenizer_helpers::replaceOperatorTokenInplace( inputItems, UMBA_TOKENIZER_TOKEN_OPERATOR_BITWISE_AND, UMBA_TOKENIZER_TOKEN_OPERATOR_LOGICAL_AND);
        marty::expressions::tokenizer_helpers::replaceOperatorTokenInplace( inputItems, UMBA_TOKENIZER_TOKEN_OPERATOR_BITWISE_OR , UMBA_TOKENIZER_TOKEN_OPERATOR_LOGICAL_OR );

    
        #if 1
        auto res = exprParser.initialize();
        if (res!=marty::expressions::Error::ok)
        {
            std::cerr << "Init error: " << exprParser.getErrorMessage(res) << "\n\n";
            std::cerr << "Error: " << exprParser.getErrorMessage(res) << "\n\n";
            
            return 0;
        }
    
        for(const auto &tk: inputItems)
        {
            res = exprParser.parse(tk);
            if (res!=marty::expressions::Error::ok)
            {
                //std::cerr << "\n";
                //exprParser.dump(std::cerr);
                //std::cerr << "\n";
                std::cerr << "Error: " << exprParser.getErrorMessage(res) << "\n\n";
                std::cerr << marty::expressions::getExpressionItemTypeName(getExpressionItemType(tk)) << ": " << marty::expressions::utils::expressionItemToString(tk) << "\n";
                return 0;
            }
        }
    
        res = exprParser.finalize();
        if (res!=marty::expressions::Error::ok)
        {
            // std::cerr << "\n";
            // exprParser.dump(std::cerr);
            // std::cerr << "\n";
            std::cerr << "Error (fini): " << exprParser.getErrorMessage(res) << "\n\n";
            // std::cerr << marty::expressions::getExpressionItemTypeName(getExpressionItemType(tk)) << ": " << marty::expressions::utils::expressionItemToString(tk) << "\n";
            return 0;
        }

        auto exprTree   = exprParser.getExpression();
        auto strExprTree   = evaluator.toString(exprTree  , bUseNamedOps);


        auto exprVars = evaluator.getExpressionVars(exprTree);
        //auto truthTable = TruthTable(exprVars.begin(), exprVars.end());

        std::cerr << "\n";
        std::cerr << "\n";


        std::string label;
        if (bShowLabels)
            label = "SRC: " + expr;

        auto appendLabel = [&](auto a, auto e)
        {
            if (bShowLabels)
            {
                label += "\\n";
                label += getActionName(a);
                label += ": ";
                label += evaluator.toString(e, bUseNamedOps);
            }
            else if (bShowLabelsShort)
            {
                if (!label.empty())
                   label += "  ";
                label += getActionNameShort(a);
            }
        };

        std::vector<std::string> allVars;
        evaluator.getExpressionVars(exprTree, allVars);

        for(auto a: actions)
        {
            switch(a)
            {
                case doConstantAbsorption          : 
                    exprTree = evaluator.performConstantAbsorption(exprTree);
                    break;
                case doMultiAry                    :
                    exprTree = evaluator.makeMultiAry(exprTree);
                    break;
                case doCollapseParentheses         :
                    exprTree = evaluator.collapseNestedParentheses(exprTree);
                    break;
                case doRemoveParentheses           :
                    exprTree = evaluator.removeAllParentheses(exprTree);
                    break;
                case doPromoteNegations            :
                    exprTree = evaluator.promoteNegations(exprTree);
                    break;
                case doRemoveUnnecessaryParentheses:
                    exprTree = evaluator.removeUnnecessaryParentheses(exprTree);
                    break;
                case doCollapseSameVars            :
                    exprTree = evaluator.collapseSameVarsAndObvioslyConstants(exprTree);
                    break;
                case doAddParentheses              :
                    exprTree = evaluator.addRequiredParentheses(exprTree);
                    break;
                case doConvertNegativeOperands     :
                    exprTree = evaluator.convertNegativeOperands(exprTree);
                    break;
                case doSort                        :
                    exprTree = evaluator.sortArguments(exprTree);
                    break;
                case doSimplify                    :
                    exprTree = evaluator.simplify(exprTree);
                    break;
                default: {}
            }

            appendLabel(a, exprTree); 
        }

        if (bShowFinalLabel)
        {
            if (!label.empty())
                label += "\\n";
            label += evaluator.toString(exprTree, bUseNamedOps);
        }

        if (bPrintTruthTable)
        {
            auto truthTable = evaluator.makeTruthTable(exprTree);
            truthTable.print(std::cerr, allVars) << "\n";
        }

//addRequiredParentheses
        
        // auto truthTable = evaluator.makeTruthTable(exprTree);
        // auto maxVarLen  = truthTable.varGetMaxLen();
        // if (maxVarLen<3)
        //     maxVarLen = 3;

        #if 0
        std::cerr << "Truth table:\n";
        for(std::size_t varIdx=0; varIdx!=truthTable.varGetNumber(); ++varIdx)
        {
            auto varName = truthTable.varGetName(varIdx);
            std::cerr << "    " << varName << truthTable.varGetNameFill(varName, maxVarLen) << " :" << " ";
            TruthTable::VarsStateType vsMax = truthTable.varsStateMax();
            for(TruthTable::VarsStateType vs=0u; vs!=vsMax; ++vs)
                std::cerr << (truthTable.varGetValue(varIdx, vs));
            std::cerr << "\n";
        }

        // std::cerr << "\n";
        {
            auto fnNameStr = std::string("Fn");
            TruthTable::VarsStateType vsMax = truthTable.varsStateMax();
            std::cerr << "    " << fnNameStr << truthTable.varGetNameFill(fnNameStr.c_str(), maxVarLen) << " :" << " ";
            for(TruthTable::VarsStateType vs=0u; vs!=vsMax; ++vs)
                std::cerr << std::string(1, truthTable.resultGetChar(vs));
        }

        std::cerr << "\n";
        std::cerr << "\n";


        ttMerged |= truthTable;

        std::cerr << "Truth table (merged):\n";
        for(std::size_t varIdx=0; varIdx!=ttMerged.varGetNumber(); ++varIdx)
        {
            auto varName = ttMerged.varGetName(varIdx);
            std::cerr << "    " << varName << ttMerged.varGetNameFill(varName, maxVarLen) << " :" << " ";
            TruthTable::VarsStateType vsMax = ttMerged.varsStateMax();
            for(TruthTable::VarsStateType vs=0u; vs!=vsMax; ++vs)
                std::cerr << (ttMerged.varGetValue(varIdx, vs));
            std::cerr << "\n";
        }

        //std::cerr << "\n";
        {
            auto fnNameStr = std::string("Fn");
            TruthTable::VarsStateType vsMax = ttMerged.varsStateMax();
            std::cerr << "    " << fnNameStr << ttMerged.varGetNameFill(fnNameStr.c_str(), maxVarLen) << " :" << " ";
            for(TruthTable::VarsStateType vs=0u; vs!=vsMax; ++vs)
                std::cerr << std::string(1, ttMerged.resultGetChar(vs));
        }
        #endif

        

        std::cerr << "---\n";
        evaluator.gvGraph(std::cout, exprTree, label);
        std::cerr << "---\n";

        return 1;
        #endif

        // return 0;
    };


    std::size_t totalCnt  = 0;
    std::size_t passedCnt = 0;

    bool mergeNames = true;
    for(const auto& d : data)
    {
        ++totalCnt;

        // std::string  expressionString;
        // int          expectedResult  ; // -1 - tokenization error, 0 - expression parsing error, 1 - Ok

        auto res = testExpressionParsing(d.expressionString, mergeNames);
        if (res==d.expectedResult)
        {
            ++passedCnt;
        }
        // if (res<0)
        // {
        //     std::cerr << "Fatal error, exiting\n";
        //     return 1;
        // }
        //  
        // if (res)
        //     ++passedCnt;
    }

    std::cerr << "Total tests: " << totalCnt << ", passed: " << passedCnt << ", failed: " << (totalCnt-passedCnt) << "\n";

    //std::vector<std::string> data;

    // UMBA_USED(exprParser);



    return 0;
}

