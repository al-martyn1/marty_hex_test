/*! \file
    \brief Тестируем marty::gdl::BitMatrix
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
#include "marty_svg/marty_svg.h"
#include "marty_gdl/marty_gdl.h"


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

#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/rule_of_five.h"

//
#include "marty_gdl/bit_matrix.h"



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
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        //std::cout 
        std::cerr << "Unknown error\n";
        return 2;
    }

}



int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    using std::cout;

    auto bm1 = marty::gdl::BitMatrix(5, 10, false);
    cout << bm1 << "\n\n";

    bm1.set(3, 6, true);
    cout << bm1 << "\n\n";


    return 0;
}




