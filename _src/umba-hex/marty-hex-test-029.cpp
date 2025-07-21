/*! \file
    \brief Тестируем разбор CSV (marty::csv)
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
    

    auto allQuotes = std::string("\"\'`");
    auto allSeps   = std::string("\t;,:|#");

    auto csvQuot = marty::csv::detectQuotes(data, allSeps, allQuotes);
    if (csvQuot==0)
    {
        std::cout << "No quote char found\n\n";
        csvQuot = '\"';
    }
    else
    {
        std::cout << "Found quote char: " << csvQuot << "\n\n";
    }

    char csvSep = marty::csv::detectSeparators(data, allSeps, csvQuot);

    if (!csvSep)
        csvSep = ';';


    auto res = marty::csv::parse(data, csvSep, csvQuot);

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
            std::cout << marty::csv::utils::expandStr(line[idx], colMaxLen[idx]+2u);
        }

        std::cout << "\n";
    }


    return 0;
}
