/*! \file
    \brief 
 */

/*

У нас есть текст, по которому мы можем итерироваться однонаправленными итераторами только вперёд.

*/

#include "umba/umba.h"
#include "umba/debug_helpers.h"
//

#include "marty_utf/utf.h"
#include "marty_csv/marty_csv.h"

//
#include <iostream>
#include <string>
#include <iterator>
#include <iomanip>
#include <utility>

//
#include "marty_bigint/undef_min_max.h"


enum class ParseErrorType
{
    UnclosedQuote,
    InvalidCharAfterQuote,
    InconsistentColumns,
    InvalidQuoteUsage
};

inline
std::string to_string(ParseErrorType peType)
{
    switch(peType)
    {
        case ParseErrorType::UnclosedQuote        : return "UnclosedQuote";
        case ParseErrorType::InvalidCharAfterQuote: return "InvalidCharAfterQuote";
        case ParseErrorType::InconsistentColumns  : return "InconsistentColumns";
        case ParseErrorType::InvalidQuoteUsage    : return "InvalidQuoteUsage";
        default: return "Unknown";
    }
}

struct ParseError
{
    ParseErrorType type;
    std::string message;
    size_t line;
    size_t position;
};

template<typename StreamType>
void printError(StreamType &oss, const ParseError &pe)
{
    using std::to_string;

    oss << pe.line << ":" << pe.position << ": " << to_string(pe.type) << ": " << pe.message << "\n";
}

struct ParseResult
{
    std::vector<std::vector<std::string>> data;
    std::vector<ParseError>               errors;
};


class CsvParser
{
    char m_delimiter      = 0;
    char m_quot           = 0;
    bool m_strictMode     = false;
    size_t m_currentLine  = 1;
    size_t m_currentPos   = 0;
    size_t m_lineStartPos = 0;

    void addError(ParseResult& result, ParseErrorType type, const std::string& msg)
    {
        using std::to_string;

        auto linePos = m_currentPos - m_lineStartPos + 1;

        result.errors.push_back(
            {
                type,
                msg // + " (line: " + to_string(m_currentLine) + 
                    // ", pos: " + to_string(linePos) + ")"
                    ,
                m_currentLine,
                linePos
            });
    }

    static
    std::string trimRight(const std::string &str)
    {
        size_t lastNonSpace = str.find_last_not_of(" \t");
        if (lastNonSpace!=std::string::npos)
            return str.substr(0, lastNonSpace + 1);
        return str;
    }

    static
    std::string trimLeft(const std::string &str)
    {
        size_t firstrNonSpace = str.find_first_not_of(" \t");
        if (firstrNonSpace!=std::string::npos)
            return str.substr(firstrNonSpace, str.npos);
        return std::string(); // Непробельные символы не найдены - у нас пустая строка
    }

    static
    std::string trim(const std::string &str)
    {
        return trimLeft(trimRight(str));
    }


public:

    CsvParser(char delim=',', char quot='\"', bool strict=true)
    : m_delimiter(delim)
    , m_quot(quot)
    , m_strictMode(strict)
          // m_currentLine(1),
          // m_currentPos(0)
    {
    }

    ParseResult parse(const std::string& content)
    {
        using std::to_string;

        ParseResult result;
        std::vector<std::string> currentRow;
        std::string field;
        bool inQuotes = false;
        bool wasQuoted = false;
        size_t columnsCount = 0;
        bool lastCharDelimiter = false;


        // m_currentLine = 1;
        // m_currentPos = 0;
        //size_t 
        m_lineStartPos = 0;

        auto addCol = [&]()
        {
            if (!wasQuoted)
                field = trim(field);
            currentRow.push_back(field);
            field.clear();
            inQuotes = false;
            wasQuoted = false;
        };

        auto handleRowEnd = [&](bool force = false)
        {
            if (force || !inQuotes)
            {
                if (wasQuoted)
                {
                    // size_t lastNonSpace = field.find_last_not_of(" \t");
                    // if (lastNonSpace != std::string::npos)
                    // {
                    //     field = field.substr(0, lastNonSpace + 1);
                    // }

                    field = trimRight(field);
                }

                if (!field.empty() || lastCharDelimiter || wasQuoted)
                {
                    // if (!wasQuoted)
                    //     field = trim(field);
                    // currentRow.push_back(field);
                    addCol();
                }

                if (!currentRow.empty())
                {
                    if (columnsCount == 0)
                    {
                        columnsCount = currentRow.size();
                    }
                    else if (m_strictMode && currentRow.size() != columnsCount)
                    {
                        addError(result, ParseErrorType::InconsistentColumns,
                            "Columns count mismatch. Expected: " + 
                            to_string(columnsCount) + 
                            ", got: " + to_string(currentRow.size()));
                    }
                    result.data.push_back(currentRow);
                }

                currentRow.clear();
                field.clear();
                inQuotes = false;
                wasQuoted = false;
                m_currentLine++;
                m_lineStartPos = m_currentPos + 1;
                return true;
            }
            return false;
        };

        for (m_currentPos = 0; m_currentPos < content.size(); ++m_currentPos)
        {
            char c = content[m_currentPos];
            
            if (inQuotes)
            {
                if (c == m_quot)
                {
                    if (m_currentPos + 1 < content.size() && content[m_currentPos + 1] == m_quot)
                    {
                        field += m_quot;
                        m_currentPos++;
                    }
                    else
                    {
                        inQuotes = false;
                        wasQuoted = true;
                        
                        size_t end = m_currentPos + 1;
                        while (end < content.size() && 
                               content[end] != m_delimiter && 
                               content[end] != '\r' && 
                               content[end] != '\n')
                        {
                            if (content[end] != ' ' && content[end] != '\t')
                            {
                                addError(result, ParseErrorType::InvalidCharAfterQuote, "Invalid character after closing quote");
                                while (end < content.size() && 
                                       content[end] != m_delimiter && 
                                       content[end] != '\n' && 
                                       content[end] != '\r')
                                {
                                    end++;
                                }
                                m_currentPos = end - 1;
                                break;
                            }
                            end++;
                        }
                        m_currentPos = end - 1;
                    }
                }
                else
                {
                    field += c;
                }
            }
            else
            {
                if (c == m_quot)
                {
                    if (!field.empty() && field != std::string(field.size(), ' '))
                    {
                        addError(result, ParseErrorType::InvalidQuoteUsage, "Quote appears in middle of field");
                        field += c;
                    }
                    else
                    {
                        inQuotes = true;
                        wasQuoted = true;
                        field.clear();
                    }

                    lastCharDelimiter = false;
                }
                else if (c == m_delimiter)
                {
                    // currentRow.push_back(field);
                    // field.clear();
                    // wasQuoted = false;
                    addCol();
                    lastCharDelimiter = true;
                }
                else if (c == '\r' || c == '\n')
                {
                    bool lineEndProcessed = handleRowEnd();
                    
                    while (m_currentPos + 1 < content.size() && 
                          (content[m_currentPos + 1] == '\r' || 
                           content[m_currentPos + 1] == '\n'))
                    {
                        m_currentPos++;
                    }
                    
                    if (!lineEndProcessed)
                    {
                        addError(result, ParseErrorType::UnclosedQuote, "Unclosed quotes in field. Attempting recovery");
                        handleRowEnd(true);
                    }

                    lastCharDelimiter = false;
                }
                else
                {
                    field += c;
                    lastCharDelimiter = false;
                }
            }
        }

        if (!field.empty() || lastCharDelimiter || wasQuoted || !currentRow.empty())
        {
            if (inQuotes)
            {
                addError(result, ParseErrorType::UnclosedQuote, "Unclosed quotes at end of input");
            }
            handleRowEnd(true);
        }

        return result;
    }
};


inline
std::string expandStr(std::string str, std::size_t size)
{
    str.resize(size, ' ');
    return str;
}


using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{
    MARTY_ARG_USED(argc);
    MARTY_ARG_USED(argv);

    std::string data;

    if (umba::isDebuggerPresent())
    {
        // data = "A,B,C\n1,,3\n,5,";
        // data = "Name#Age#City\nJohn#30#New York";
        // data = "\"Name\",\" Age \",\"Address\"\n\"John, Smith\",25,\"123 Main St, Apt 5\"";
        // data = "Name,\" Age \",Country\nJohn,25,USA";
        // data = "Product|Price|Quantity\nMonitor\\|27\"|$199.99|50";
        data = "\"Name\",\"Age\",\"Address\"\r\n\"John, Smith\",25,\"123 Main St, Apt 5\"";
        // data = "Name;Age;Country\nMaria;30;Germany";

    }
    else
    {
        data = std::string(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>());
    }
    

    //auto outIt = marty::utf::UtfOutputIterator<char>(strRes);
    // umba::TextPositionCountingIterator
    // auto b = marty::utf::UtfInputIterator<char>(data);
    // auto e = marty::utf::UtfInputIterator<char>();

    // for(; b!=e; ++b)
    // {
    //  
    // }

    auto allQuotes = std::string("\"\'`");
    auto allSeps   = std::string("\t;,:|#");

    auto csvQuot = marty::csv::detectQuotes(data.begin(), data.end(), allSeps, allQuotes);
    if (csvQuot==0)
        std::cout << "No quote char found\n\n";
    else
        std::cout << "Found quote char: " << csvQuot << "\n\n";

    char csvSep = 0;

    {

        std::vector<std::string> lines;
        marty::csv::details::splitToLines(data.begin(), data.end(), lines, csvQuot);

        // marty::csv::details::CLineCharCounts charsMathEV  ;
        // marty::csv::details::CLineCharCounts charsVariance;
        // marty::csv::details::calcVariance(lines, charsMathEV, charsVariance);
        //  
        // for(std::size_t idx=0; idx!=127; ++idx)
        // {
        //     if (idx>0x20u)
        //         std::cout << idx << " (" << char(idx) << "): ";
        //     else
        //         std::cout << idx << "    : ";
        //  
        //     std::cout << charsMathEV.counts[idx] << "   " << charsVariance.counts[idx] << "\n";
        // }
        //  
        // std::cout << "\n";


        std::vector< marty::csv::details::CVarianceItem > variances;
        marty::csv::details::getCsvSeparatorVariances( variances, lines, "\t;,:|#");
         
        std::cout<<"CSV stat:\n";
        std::vector< marty::csv::details::CVarianceItem >::const_iterator vvIt = variances.begin();
        for(; vvIt != variances.end(); ++vvIt)
        {
            if (vvIt->ch<=32) 
            {
                 switch(vvIt->ch)
                    {
                     case 9:   std::cout << "\\t : ";   break;
                     default:  std::cout << vvIt->ch << " : ";
                    }
            }
            else
            {
                std::cout << "'" << (char)vvIt->ch << "': ";
            }
            std::cout << "M: " << vvIt->M << ", ";
            std::cout << "D: " << vvIt->D << ", ";
            // std::cout<<"T: "<<vvIt->tag<<"\n";
            std::cout << "\n";
        }
    
        std::cout<<"\n";

        if (!variances.empty())
            csvSep = char(variances[0].ch);
        
    }

    if (!csvSep)
        csvSep = ';';

    std::cout << "------------------\n";
    std::cout << data << "\n\n";

    std::cout << "------------------\n";

// template<typename LinesVectorType>
// void calcVariance( const LinesVectorType &lines
//                  , CLineCharCounts       &charsMathEV
//                  , CLineCharCounts       &charsVariance
//                  )




// template<typename InputIter, typename StringType>
// char detectQuotes(InputIter b, InputIter e, const StringType &seps, const StringType &quotes)

    // CsvParser(char delim=',', char quot='\"', bool strict=true)
    auto parser = CsvParser(csvSep, csvQuot); // (char delim = ',', bool strict = true)

    auto res = parser.parse(data);

    for(auto &&e : res.errors)
    {
        printError(std::cerr, e);
    }

    std::cerr << std::flush;

    std::vector<std::size_t> colMaxLen;

    for(auto && line : res.data)
    {
        std::size_t idx = 0u;
        for(; idx!=line.size(); ++idx)
        {
            const auto & col = line[idx];
            if (idx<colMaxLen.size())
            {
                colMaxLen[idx] = std::max(col.size(), colMaxLen[idx]);
            }
            else
            {
                colMaxLen.push_back(col.size());
            }
        }
    }

    for(auto && line : res.data)
    {
        std::size_t idx = 0u;
        for(; idx!=line.size(); ++idx)
        {
            std::cout << expandStr(line[idx], colMaxLen[idx]+2u);
        }

        std::cout << "\n";
    }

// struct ParseResult
// {
//     vector<vector<std::string>> data;
//     vector<ParseError> errors;
// };



    return 0;
}
