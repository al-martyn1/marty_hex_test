/*! \file
    \brief Пробуем в разбор строк, задающих наборы символов
 */

#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"

#include "umba/char_class.h"

//
//
#include "umba/tokenizer/parsers/ufsm/enums.h"
//
#include <cstdint>
#include <string>
#include <vector>
#include <utility>



//----------------------------------------------------------------------------




using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{
    MARTY_ARG_USED(argc);
    MARTY_ARG_USED(argv);

    std::vector<std::string> inputStrings;

    if (umba::isDebuggerPresent())
    {
        inputStrings.emplace_back("`\\``");
        inputStrings.emplace_back("\\*-[\\d]"); // все кроме цифр, как \D
        inputStrings.emplace_back("\\D");
        inputStrings.emplace_back("\\d");
        inputStrings.emplace_back("\\xd");
        inputStrings.emplace_back("\\xD");
        inputStrings.emplace_back("\\x21");
        inputStrings.emplace_back("\\s");
        inputStrings.emplace_back("\\S");
        inputStrings.emplace_back("\\w");
        inputStrings.emplace_back("\\W");
        inputStrings.emplace_back("\\*");
        inputStrings.emplace_back("\\n");
        inputStrings.emplace_back("\\c?");
    }
    else
    {
        for(auto i=1; i<argc; ++i)
            inputStrings.emplace_back(argv[i]);
    }
    
    std::set<std::uint8_t>  charsSet;
    charsSet.insert(std::uint8_t('0'));
    charsSet.insert(std::uint8_t('1'));
    charsSet.insert(std::uint8_t('2'));
    charsSet.insert(std::uint8_t('3'));
    charsSet.insert(std::uint8_t('4'));
    //charsSet.insert(std::uint8_t('5'));
    charsSet.insert(std::uint8_t('6'));
    charsSet.insert(std::uint8_t('7'));
    charsSet.insert(std::uint8_t('8'));
    charsSet.insert(std::uint8_t('9'));
    charsSet.insert(std::uint8_t('a'));
    charsSet.insert(std::uint8_t('b'));
    charsSet.insert(std::uint8_t('c'));
    charsSet.insert(std::uint8_t('d'));
    // charsSet.insert(std::uint8_t('e'));
    // charsSet.insert(std::uint8_t('f'));
    charsSet.insert(std::uint8_t('A'));
    charsSet.insert(std::uint8_t('B'));
    charsSet.insert(std::uint8_t('C'));
    charsSet.insert(std::uint8_t('D'));
    charsSet.insert(std::uint8_t('E'));
    charsSet.insert(std::uint8_t('F'));
    //charsSet.insert(std::uint8_t(''));

    charsSet.insert(std::uint8_t('\n'));
    charsSet.insert(std::uint8_t('\r'));

    cout << "Testing char string generation\n";
    cout << umba::tokenizer::char_class::utils::makePredicateCharClassString(charsSet);
    cout << "\n\n";


    for(auto s : inputStrings)
    {
        cout << "---------\n";
        std::unordered_set<std::uint8_t> resSet;
        std::size_t errPos;
        cout << "String: " << s << "\n";
        if (!umba::tokenizer::char_class::utils::parseCharClassDefinition(s, resSet, &errPos))
        {
            cout << "Error at pos: " << errPos << "\n";
        }
        else
        {
            cout << "Result: " << umba::tokenizer::char_class::utils::makePredicateCharClassString(resSet) << "\n";
        }

        cout << "\n";
    }


    return 0;
}

