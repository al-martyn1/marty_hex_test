/*! \file
    \brief Тестим UMBA_RULE_OF_FIVE
 */


#include "umba/rule_of_five.h"
//
#include <iostream>

using std::cout;
using std::cerr;


struct TestStruct
{
    UMBA_RULE_OF_FIVE(TestStruct, delete, delete, delete, delete, delete);
};




int main(int  /* argc */ , char*  /* argv */ [])
{
    cout << "sizeof(TestStruct): " << sizeof(TestStruct) << "\n";
    return 0;
}