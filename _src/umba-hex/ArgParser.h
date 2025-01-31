#pragma once

#include <stack>

//#include "app_config.h"
#include "umba/cmd_line.h"
#include "umba/cli_tool_helpers.h"
#include "umba/shellapi.h"
#include "umba/string_plus.h"
#include "AppVerConfig.h"

//
#include "marty_cpp/src_normalization.h"



// AppConfig    appConfig;

//extern umba::SimpleFormatter umbaLogStreamMsg;


template<typename StringType>
struct ArgParser
{

    static
    const std::set<std::string>& getWarnOptsSet()
    {
        const static std::set<std::string> s {}; // {"img-copy", "img-copy-exist", "same-file", "plantuml", "graphviz"};
        return s;
    }

    static
    const std::set<std::string>& getInfoOptsSet()
    {
        const static std::set<std::string> s {}; // /* = */ {"snippet-lookup", "plantuml", "graphviz", "opt-files", "config", "strip-extentions", "page-index", "meta-tags", "auto-url"};
        return s;
    }


// 0 - ok, 1 normal stop, -1 - error
template<typename ArgsParser>
int operator()( const StringType                                &a           //!< строка - текущий аргумент
              , umba::command_line::CommandLineOption           &opt         //!< Объект-опция, содержит разобранный аргумент и умеет отвечать на некоторые вопросы
              , ArgsParser                                      &argsParser  //!< Класс, который нас вызывает, содержит некоторый контекст
              , umba::command_line::ICommandLineOptionCollector *pCol        //!< Коллектор опций - собирает инфу по всем опциям и готов вывести справку
              , bool fBuiltin
              , bool ignoreInfos
              )
{
    //using namespace marty::clang::helpers;

    UMBA_USED(fBuiltin);
    UMBA_USED(a);

    std::string dppof = "Don't parse predefined options from ";

    if (opt.isOption())
    {
        std::string errMsg;
        std::string strVal;
        //int intVal;
        //unsigned uintVal;
        std::size_t szVal;
        bool boolVal;

        UMBA_USED(szVal);
        UMBA_USED(boolVal);

        if (opt.name.empty())
        {
            LOG_ERR<<"invalid (empty) option name\n";
            return -1;
        }

        if (opt.isOption("quet") || opt.isOption('q') || opt.setDescription("Operate quetly"))  // . Short alias for '--verbose=quet'
        {
            argsParser.quet = true;
            //appConfig.setOptQuet(true);
        }

        #if 1
        else if ( opt.setParam("info-type1[,+info-type2,-info-type]",umba::command_line::OptionType::optString)
               || opt.isOption("info")
               // || opt.setParam("VAL",true)
               || opt.setDescription("Make info messages enabled/disabled, '+' (or nothing) - enable message, '-' - disable it. Type is one of: " + umba::log::makeAllWarnInfoLogOptionsString(getInfoOptsSet())
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;
        
            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            std::string unknownOpt;
            if (!umba::log::addRemoveInfoOptions(getInfoOptsSet(), strVal, unknownOpt))
            {
                LOG_ERR<<"Unknown info type: '" << unknownOpt << "' (--info)\n";
                return -1;
            }

            return 0;
        }
        #endif

        #if 1
        else if ( opt.setParam("warn-type1[,+warn-type2,-warn-type]",umba::command_line::OptionType::optString)
               || opt.isOption("warning")
               // || opt.setParam("VAL",true)
               || opt.setDescription("Make warning messages enabled/disabled, '+' (or nothing) - enable message, '-' - disable it. Type is one of: " + umba::log::makeAllWarnInfoLogOptionsString(getWarnOptsSet())
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;
        
            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            std::string unknownOpt;
            if (!umba::log::addRemoveWarningOptions(getWarnOptsSet(), strVal, unknownOpt))
            {
                LOG_ERR<<"Unknown warning type: '" << unknownOpt << "' (--warning)\n";
                return -1;
            }

            return 0;
        }
        #endif

        else if (opt.isOption("home") || opt.setDescription("Open homepage"))
        {
            if (argsParser.hasHelpOption) return 0;
            umba::shellapi::openUrl(appHomeUrl);
            return 1;
        }

        else if ( opt.isBuiltinsDisableOptionMain  ()
               // || opt.setDescription( dppof + "main distribution options file `" + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::appGlobal   ) + "`"))
               || opt.setDescription( dppof + "main distribution options file"))
        { } // simple skip - обработка уже сделана

        else if ( opt.isBuiltinsDisableOptionCustom()
               //|| opt.setDescription( dppof + "custom global options file `"     + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::customGlobal) + "`"))
               || opt.setDescription( dppof + "custom global options file"))
        { } // simple skip - обработка уже сделана

        else if ( opt.isBuiltinsDisableOptionUser  ()
               //|| opt.setDescription( dppof + "user local options file `"        + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::userLocal   ) + "`"))
               || opt.setDescription( dppof + "user local options file"))
        { } // simple skip - обработка уже сделана

        else if (opt.isOption("version") || opt.isOption('v') || opt.setDescription("Show version number"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                umba::cli_tool_helpers::printOnlyVersion(std::cout);
                return 1;
            }
        }

        else if (opt.isOption("version-info") || opt.setDescription("Show version info - app name, version, host platform, build date and time"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                if (argsParser.getPrintHelpStyle()!=umba::command_line::PrintHelpStyle::md)
                {
                    umba::cli_tool_helpers::printNameVersion(std::cout);
                }
                else
                {
                    umba::cli_tool_helpers::printNameVersion(std::cout);
                }

                return 1;
            }
        }

        else if (opt.isOption("builtin-options-info") || opt.setDescription("Show builtin options files location"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                argsParser.printBuiltinFileNames(std::cout);
                return 1;
            }
        }

        else if (opt.isOption("where") || opt.setDescription("Show where the executable file is"))
        {
            if (argsParser.hasHelpOption) return 0;

            LOG_MSG << argsParser.programLocationInfo.exeFullName << "\n";
            return 0;
        }

        else if (opt.setParam("CLR", 0, "no/none/file|"
                                        "ansi/term|"
                                        #if defined(WIN32) || defined(_WIN32)
                                        "win32/win/windows/cmd/console"
                                        #endif
                             )
              || opt.setInitial(-1) || opt.isOption("color")
              || opt.setDescription("Force set console output coloring")
              /* ", can be:\nno, none, file - disable coloring\nansi, term - set ansi terminal coloring\nwin32, win, windows, cmd, console - windows console specific coloring method" */
              )
        {
            if (argsParser.hasHelpOption) return 0;

            umba::term::ConsoleType res;
            auto mapper = [](int i) -> umba::term::ConsoleType
                          {
                              switch(i)
                              {
                                  case 0 : return umba::term::ConsoleType::file;
                                  case 1 : return umba::term::ConsoleType::ansi_terminal;
                                  case 2 : return umba::term::ConsoleType::windows_console;
                                  default: return umba::term::ConsoleType::file;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            coutWriter.forceSetConsoleType(res);
            cerrWriter.forceSetConsoleType(res);

            return 0;
        }

        #if 0
        else if ( opt.setParam("?MODE",true)
               || opt.isOption("verbose")
               // || opt.setParam("VAL",true)
               || opt.setDescription("Verbose mode on/off."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            argsParser.quet       = !boolVal;
            appConfig.verboseMode =  boolVal;
            return 0;
        }
        #endif

        #if 0
        else if ( opt.setParam("?MODE",true)
               || opt.isOption("overwrite") || opt.isOption('Y')
               // || opt.setParam("VAL",true)
               || opt.setDescription("Allow overwrite existing file."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            bOverwrite = boolVal;
            return 0;
        }
        #endif

        #if 0
        else if ( opt.setParam("LINEFEED",umba::command_line::OptionType::optString)
               || opt.isOption("linefeed") || opt.isOption("LF") || opt.isOption('L')
               // || opt.setParam("VAL",true)
               || opt.setDescription("Output linefeed. LINEFEED is one of: `CR`/`LF`/`CRLF`/`LFCR`/`DETECT`."
                                     #if defined(WIN32) || defined(_WIN32)
                                     "Default is `CRLF`."
                                     #else
                                     "Default is `LF`."
                                     #endif
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            marty_cpp::ELinefeedType tmp = marty_cpp::enum_deserialize( strVal, marty_cpp::ELinefeedType::invalid );
            if (tmp== marty_cpp::ELinefeedType::invalid)
            {
                LOG_ERR<<"Invalid linefeed option value: "<<strVal<<"\n";
                return -1;
            }

            appConfig.outputLinefeed = tmp;

            return 0;
        }
        #endif

        #if 0
        else if ( opt.setParam("VAR:VAL")
               || opt.isOption("set-var") || opt.isOption("set-condition-var") || opt.isOption('C')
               || opt.setDescription("Set variable valie for conditions and substitutions"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR<<"Setting condition variable requires argument (--set-condition-var)\n";
                return -1;
            }

            auto optArg = opt.optArg;
            if (!appConfig.addConditionVar(optArg))
            {
                LOG_ERR<<"Setting condition variable failed, invalid argument: '" << optArg << "' (--set-condition-var)\n";
                return -1;
            }

            return 0;
        }
        #endif

        else if ( opt.isOption("gcc")
               || opt.setDescription("GCC messages format instead of MSVC format")
                )
        {
            if (argsParser.hasHelpOption) return 0;

            umbaLogGccFormat = true;

            return 0;
        }

        else if ( opt.isOption("autocomplete-install")
               || opt.setDescription("Install autocompletion to bash"
                                     #if defined(WIN32) || defined(_WIN32)
                                         "/clink(cmd)"
                                     #endif
                                    )
               )
        {
            if (argsParser.hasHelpOption) return 0;

            //return autocomplete(opt, true);
            return umba::command_line::autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), true, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR : LOG_MSG; } );
        }

        else if ( opt.isOption("autocomplete-uninstall")
               || opt.setDescription("Remove autocompletion from bash"
                                     #if defined(WIN32) || defined(_WIN32)
                                         "/clink(cmd)"
                                     #endif
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            //return autocomplete(opt, false);
            return umba::command_line::autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), false, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR : LOG_MSG; } );
        }

        else if (opt.isHelpStyleOption())
        {
            // Job is done in isHelpStyleOption
        }

        else if (opt.isHelpOption()) // if (opt.infoIgnore() || opt.isOption("help") || opt.isOption('h') || opt.isOption('?') || opt.setDescription(""))
        {
            if (!ignoreInfos)
            {
                if (pCol && !pCol->isNormalPrintHelpStyle())
                    argsParser.quet = true;
                //printNameVersion();
                if (!argsParser.quet)
                {
                    umba::cli_tool_helpers::printNameVersion(std::cout);
                    //umba::cli_tool_helpers::printBuildDateTime();
                    umba::cli_tool_helpers::printCommitHash(std::cout);
                    std::cout<<"\n";
                //printHelp();
                }

                if (pCol && pCol->isNormalPrintHelpStyle() && argsParser.argsNeedHelp.empty())
                {
                    //argsParser.printHelpPage( std::cout, "[OPTIONS] input_file [output_file]", "If output_file not taken, STDOUT used", helpText );
                    auto helpText = opt.getHelpOptionsString();
                    std::cout << "Usage: " << argsParser.programLocationInfo.exeName
                              << " [OPTIONS] input_file [output_file]\n"
                              << "  If output_file not taken, STDOUT used\n"
                              << "\nOptions:\n\n"
                              << helpText;
                              //<< " [OPTIONS] input_file [output_file]\n\nOptions:\n\n"<<helpText;
                }

                if (pCol) // argsNeedHelp
                {
                    argsParser.printHelpPage( std::cout
                                            , "[OPTIONS] input_file [output_file]"
                                            , "If output_file not taken, STDOUT used"
                                            , pCol->makeText( 78, &argsParser.argsNeedHelp )
                                            );
                    // std::cout<<pCol->makeText( 78, &argsParser.argsNeedHelp );
                }

                return 1;

            }

            return 0; // simple skip then parse builtins
        }

        else
        {
            LOG_ERR<<"unknown option: "<<opt.argOrg<<"\n";
            return -1;
        }

        return 0;

    } // if (opt.isOption())

    else if (opt.isResponseFile())
    {
        //std::string

        StringType optName;
        umba::utfToStringTypeHelper(optName, opt.name);
        auto optFileName = argsParser.makeAbsPath(optName);

        if (!argsParser.quet)
        {
            #if !defined(NDEBUG)
            std::cout << "Processing options file: " << optFileName << "\n";
            #endif
        }

        argsParser.pushOptionsFileName(optFileName);
        auto parseRes = argsParser.parseOptionsFile( optFileName );
        argsParser.popOptionsFileName();

        if (!parseRes)
            return -1;

        if (argsParser.mustExit)
            return 1;

        return 0;

    }

    // Process non-option args here

    #if 0
    if (inputFilename.empty())
    {
        inputFilename = argsParser.makeAbsPath(a);
    }
    else
    {
        outputFilename = argsParser.makeAbsPath(a);
    }
    #endif

    return 0;

}

}; // struct ArgParser



class CommandLineOptionCollector : public umba::command_line::CommandLineOptionCollectorImplBase
{
protected:
    virtual void onOptionDup( const std::string &opt ) override
    {
        LOG_ERR<<"Duplicated option key - '"<<opt<<"'\n";
        throw std::runtime_error("Duplicated option key");
    }

};



