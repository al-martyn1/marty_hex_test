/*! \file
    \brief Тестим функцию форматирования с аргументами в виде marty::format::Args
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

//
#include "log.h"
//
#include "AppConfig.h"

AppConfig appConfig;

#include "ArgParser.h"



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




// , typename ArgumentVariantType=FormatArgumentVariant
// , typename VectorType=std::vector<ArgumentVariantType>
// , typename MapType=std::unordered_map<StringType, std::size_t>
// , typename ArgsType = Args<ArgumentVariantType, VectorType, MapType>
//  
//  
// using Args
// marty::format::Args<ArgumentVariantType, VectorType, MapType>

inline
std::string testFormatString(int nTest, const std::string& fmt, const marty::format::Args &args)
{
    out << "\n";
    out << "Test #" << nTest << "\n" << umba::omanip::flush;
    out << "InputStr : \"" << fmt << "\"\n" << umba::omanip::flush;
    out << "Args: ";
    for(auto && valToFormat : args)
    {
        auto val = std::visit( [](auto && a) -> std::string
                               {
                                   return marty::format::martyFormatSimpleConvertToString<std::string>(a);
                               }
                             , valToFormat
                             );
        out << val << ", ";
    }
    out << "\n";

    auto resStr = marty::format::formatMessage( fmt, args );

    out << "OutputStr: \"" << resStr << "\"\n";
    out << "-----\n\n";

    return resStr;
}

template<typename T>
void printSignedUnsignedTypeInfo()
{
    std::cout << "Signed size  : " << sizeof(T) << "\n";
    std::cout << "Unsigned size: " << sizeof(std::make_unsigned_t<T>) << "\n";
}

int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);


    using namespace marty::format;

    //----------------------------------------------------------------------------
    // using MyVariant = std::variant<
    //     int,
    //     BasicFormatValueFilter<const int*, int*>,
    //     double,
    //     BasicFormatValueFilter<char*, std::string::iterator>,
    //     std::vector<float>
    // >;

    using MyFilter = marty::format::FormatValueFilter;
	
    std::cout << std::boolalpha;

    for(unsigned i=0; i<=100; ++i)
    {
        std::cout << i << ": " << marty::format::utils::formatRomanInteger(i, true, false) << "\n";
        std::cout << i << ": " << marty::format::utils::formatRomanInteger(i, true, true ) << "\n";
    }

    std::cout << "-------\n";


    // Проверка наличия фильтров
    #if 0
    std::cout << "Variant contains filters: " 
              << variant_has_filter_v<MyFilter> << "\n";
    std::cout << "Number of filters: " 
              << variant_filter_count_v<MyFilter> << "\n\n";

    // Печать информации о фильтрах
    variant_filter_traits<MyFilter>::print();

    // Работа с параметрами
    if constexpr (variant_has_filter_v<MyFilter>) {
        using Params = variant_filter_params_t<MyFilter>;
        
        // Первый фильтр
        using FirstIn = std::tuple_element_t<0, Params>;
        using FirstOut = std::tuple_element_t<1, Params>;
        
        std::cout << "\nFirst filter:\n";
        std::cout << "Input:  " << typeid(FirstIn).name() << "\n";
        std::cout << "Output: " << typeid(FirstOut).name() << "\n";
    }
    #endif

    // std::cout << "Variant contains filters: " 
    //           << variant_filter_traits<FormatArgumentVariant>::has_filter << "\n";
    // std::cout << "Number of filters: " 
    //           << variant_filter_traits<FormatArgumentVariant>::count << "\n\n";

    // Печать информации о фильтрах
    // variant_filter_traits<FormatArgumentVariant>::print();

    std::cout << "signed char sizes:\n";
    printSignedUnsignedTypeInfo<signed char>();
    std::cout << "\n\n";

    std::cout << "signed short sizes:\n";
    printSignedUnsignedTypeInfo<signed short>();
    std::cout << "\n\n";

    std::cout << "signed int sizes:\n";
    printSignedUnsignedTypeInfo<signed int>();
    std::cout << "\n\n";

    std::cout << "signed long sizes:\n";
    printSignedUnsignedTypeInfo<signed long>();
    std::cout << "\n\n";

    std::cout << "signed long long sizes:\n";
    printSignedUnsignedTypeInfo<signed long long>();
    std::cout << "\n\n";

    std::cout << "Variant contains filters: " 
              << std::boolalpha 
              << variant_filter_traits<FormatArgumentVariant>::has_filter 
              << "\n\n";

    // Получение информации о первом фильтре
    using Traits = variant_filter_traits<FormatArgumentVariant>;
    
    if constexpr (Traits::has_filter) {
        // Тип первого фильтра
        using FirstFilter = Traits::first_filter;
        std::cout << "First filter type: " << typeid(FirstFilter).name() << "\n";
        
        // Типы аргументов первого фильтра
        using FirstFilterArgs = Traits::first_filter_args;
        std::cout << "Input iterator type:  " 
                  << typeid(typename FirstFilterArgs::input_type).name() << "\n";
        std::cout << "Output iterator type: " 
                  << typeid(typename FirstFilterArgs::output_type).name() << "\n";
        
        // Пример использования типов
        typename FirstFilterArgs::input_type input_it;
        typename FirstFilterArgs::output_type output_it;
        
        std::cout << "Successfully created iterator objects\n";

        if constexpr (marty::format::utils::has_ctor_require_two_pointers<typename FirstFilterArgs::input_type>)
           std::cout << "Iterator has require_pointers_pair\n";
        else
           std::cout << "Iterator has NO require_pointers_pair\n";

    } else {
        std::cout << "No filters found in variant\n";
    }




    //----------------------------------------------------------------------------

    out << "Test initializer_list\n";
    out << marty::format::formatMessage( std::string("{} {0} {} {}"), {0, 1, 2, 3} );
    out << "\n";
    out << "\n";


    using Args = marty::format::Args;

    int nTest = 0;
    // );
    // , ignoreErrors);
    testFormatString(++nTest, "{:{}.{}f}", Args().arg(1.5f).arg(2).arg(3) );            // #1
    testFormatString(++nTest, "{0:}", Args().arg(10));                                  // #2
#if 1
    testFormatString(++nTest, "{:10.5f}", Args().arg(31.4159));                         // #3
    testFormatString(++nTest, "Текст и {:6}", Args().arg("строка"));                    // #4
    testFormatString(++nTest, "{:6}", Args().arg(L"L\"Широкая строка\""));              // #5
    testFormatString(++nTest, "{:*<{}}", Args().arg(99).arg(3)); // Ширину поля задаём аргументом  // #6
    testFormatString(++nTest, "{:*>6}", Args().arg(99));                                // #7
    testFormatString(++nTest, "{:*^6}", Args().arg(99));                                // #8
    testFormatString(++nTest, "{:6d}", Args().arg(99));                                 // #9
    testFormatString(++nTest, "{:6}", Args().arg(99));                                  // #10
    testFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", Args().arg(99));                // #11
    testFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", Args().arg(99));                // #12
    testFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", Args().arg(99));                // #13
    testFormatString(++nTest, "{0:},{0:+},{0:-},{0: }", Args().arg(99));                // #14
    testFormatString(++nTest, "{:+06d}", Args().arg(99));                               // #15
    testFormatString(++nTest, "{:#06x}", Args().arg(99));                               // #16
    testFormatString(++nTest, "{:<06}", Args().arg(99));                                // #17
    testFormatString(++nTest, "{:10f}", Args().arg(99.9));                              // #18
    testFormatString(++nTest, "{:{}f}", Args().arg(99.9).arg(3));                       // #19
    testFormatString(++nTest, "{:.5f}", Args().arg(99.9));                              // #20
    testFormatString(++nTest, "{:.{}f}", Args().arg(99.9).arg(3));                      // #21
    testFormatString(++nTest, "{:{}f}", Args().arg(99.9).arg(3));                       // #22
    testFormatString(++nTest, "{:.^5s}", Args().arg("123"));                            // #23
    testFormatString(++nTest, "{:.5s}", Args().arg("123"));                             // #24
    testFormatString(++nTest, "{:.<5.5s}", Args().arg("1234567"));                      // #25
    testFormatString(++nTest, "{:?}", Args().arg("123456")); // !!! Тут надо потом вставить непечатные символы, чтобы проверить escape
    testFormatString(++nTest, "Coordinates: {latitude}, {longitude}", Args().arg("latitude", "59.958516").arg("longitude", "30.312167"));   // 59.958516, 30.312167
    testFormatString(++nTest, "{:<30}", Args().arg(123)); // Ширина 30?                 // #28
    testFormatString(++nTest, "{:>30}", Args().arg("1234567"));                         // #29
    testFormatString(++nTest, "{:^30}", Args().arg(99.9));                              // #30
    testFormatString(++nTest, "{:*^30}", Args().arg(999));                              // #31
    testFormatString(++nTest, "{:+f}; {:+f}", Args().arg(1.1).arg(2.2));                // #32
    testFormatString(++nTest, "{: f}; {: f}", Args().arg(1.1).arg(2.2));                // #33
    testFormatString(++nTest, "{:-f}; {:-f}", Args().arg(1.1).arg(2.2));                // #34
    testFormatString(++nTest, "int: {0:d};  hex: {0:x};  oct: {0:o};  bin: {0:b}", Args().arg(12345));
    testFormatString(++nTest, "int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", Args().arg(12345));
    testFormatString(++nTest, "{:,}", Args().arg(1234567));                             // #37
    testFormatString(++nTest, "{:_}", Args().arg(1234567));                             // #38
    testFormatString(++nTest, "{:'}", Args().arg(1234567));                             // #39
    testFormatString(++nTest, "{:'}", Args().arg(marty::BigInt("12142345234654765678768")));
    testFormatString(++nTest, "{:'}", Args().arg(marty::Decimal("12142345234654765678768.12142345234654765678768")));
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 0));                     // #42
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 1));                     // #43
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 2));                     // #44
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 3));                     // #45
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 4));                     // #46
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 9));                     // #47
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 13));                    // #48
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 18));                    // #49
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 29));                    // #50
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 46));                    // #51
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 63));                    // #52
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 72));                    // #53
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 96));                    // #54
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 127));                   // #55
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 192));                   // #56
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 352));                   // #57
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 768));                   // #58
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 1024));                  // #59
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 2048));                  // #60
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 4096));                  // #61
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 9003));                  // #62
    testFormatString(++nTest, "{n:d} - {n:R}", Args().arg("n", 10999));                 // #63
    testFormatString(++nTest, "Correct answers: {:.2%}", Args().arg(0.75));             // #64
    testFormatString(++nTest, "{:02X}{:02X}{:02X}{:02X}", Args().arg(123).arg(456).arg(789).arg(912));  // #65

    // testFormatString(++nTest, "", Args().arg());
    // testFormatString(++nTest, "", Args().arg());
    // testFormatString(++nTest, "", Args().arg());
#endif

    return 0;
}


