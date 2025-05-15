/*! \file
    \brief Тестим LocaleInfo
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

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "marty_format/marty_format.h"
#include "marty_format/locale_info.h"

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
#include <utility>

#include "encoding/encoding.h"


//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

umba::SimpleFormatter &out = umbaLogStreamMsg;

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

bool bOverwrite         = false;

#if 1
//
#include "log.h"
//
#include "AppConfig.h"

AppConfig appConfig;

#include "ArgParser.h"
#endif


int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        LOG_ERR << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        LOG_ERR << "unknown error\n";
        return 2;
    }

}


int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);


    marty::Decimal d = "3.14159";

    std::cout << "d   : " << d.toString() << "\n";               // 3.14159
    std::cout << "dE+6: " << d.powered10(+6).toString() << "\n"; // 3141590
    std::cout << "dE-6: " << d.powered10(-6).toString() << "\n"; // 0.00000314159
    std::cout << "dE+3: " << d.powered10(+3).toString() << "\n"; // 3141.59
    std::cout << "dE-3: " << d.powered10(-3).toString() << "\n"; // 0.00314159

    d = "30000";
    std::cout << "d   : " << d.toString() << "\n";               // 30000
    std::cout << "dE+6: " << d.powered10(+6).toString() << "\n"; // 30000000000
    std::cout << "dE-6: " << d.powered10(-6).toString() << "\n"; // 0.03
    std::cout << "dE+3: " << d.powered10(+3).toString() << "\n"; // 30000000
    std::cout << "dE-3: " << d.powered10(-3).toString() << "\n"; // 30


    return 0;
}


