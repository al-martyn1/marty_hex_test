/*! \file
    \brief Рисуем граф UFSM в SVG ручками
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


    if (umba::isDebuggerPresent())
    {
        // std::string cwd;
        // std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        // // std::cout 
        // LOG_MSG << "App Root Path: " << rootPath << "\n";
        // // std::cout 
        // LOG_MSG << "Working Dir  : " << cwd << "\n";
        //  
        // //inputFilename = rootPath + "_libs/umba_tokenizer/inc/umba/tokenizer/parsers/ufsm/samples/http.ufsm";
        // inputFilename = rootPath + "_libs/umba_tokenizer/inc/umba/tokenizer/parsers/ufsm/samples/traffic_lights.ufsm";
        // //fsmNameStr    = "samples//traffic_lights::TrafficLightRoad\\\\";
        // fsmNameStr    = "samples//traffic_lights::TrafficLightRoad";

    } // if (umba::isDebuggerPresent())
    else
    {
        // if (argc>1)
        //     inputFilename = argv[1];
        // if (argc>2)
        //     fsmNameStr    = argv[2];
        // if (argc>3)
        //     useTransitionPoint = true;
    }

    // if (inputFilename.empty())
    // {
    //     LOG_ERR << "No input file taken\n";
    //     return 2;
    // }
    //  
    // if (fsmNameStr.empty())
    // {
    //     LOG_ERR << "No FSM name taken\n";
    //     return 2;
    // }


    return 0;
}




