/*! \file
    \brief Тестим умножение marty::BigInt на скорость с чанком std::uint8_t
 */

#ifndef MARTY_BIGINT_FORCE_NUMBER_UNDERLYING_TYPE
    #define MARTY_BIGINT_FORCE_NUMBER_UNDERLYING_TYPE std::uint8_t
#endif

#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <utility>

//
#include "marty_bigint/marty_bigint.h"

#include <windows.h>

#include "marty_bigint/undef_min_max.h"



//----------------------------------------------------------------------------
inline
std::uint32_t getMillisecTick()
{
    #if defined(WIN32) || defined(_WIN32)

        // https://devblogs.microsoft.com/cppblog/visual-c-compiler-version/
        // https://learn.microsoft.com/en-us/cpp/overview/compiler-versions?view=msvc-170

        #if defined(_MSC_VER) && _MSC_VER>1929
            #pragma warning(push)
            #pragma warning(disable:28159) // - warning C28159: Consider using 'GetTickCount64' instead of 'GetTickCount'
        #endif

        return (std::uint32_t)GetTickCount();

        #if defined(_MSC_VER) && _MSC_VER>1929
            #pragma warning(pop)
        #endif

    #else // Linups users can add native millisec counter getter or new std chrono fns

        return 0;

    #endif
}

struct TickElapsedPrinter
{
    std::uint32_t  startTick = 0;
    std::string    msg;

    TickElapsedPrinter(const std::string &m) : startTick(getMillisecTick()), msg(m) {}
    ~TickElapsedPrinter()
    {
        //#if defined(DEBUG) || defined(_DEBUG)
        if (!msg.empty())
            std::cout << msg << ": " << (getMillisecTick()-startTick) << "\n" << std::flush;
        //#else
        //std::cout << "+" << std::flush;
        //#endif
    }

}; // struct TickElapsedPrinter





using marty::BigInt;

#if defined(DEBUG) || defined(_DEBUG)
std::size_t numInputs =     10000;
#else
std::size_t numInputs = 100000000;
#endif

std::vector<BigInt>  baseVals1;
std::vector<BigInt>  baseVals2;
std::vector<BigInt>  baseVals3;
// std::vector<BigInt>  baseVals4;


std::vector< std::vector<BigInt> > vals;


int unsafeMain(int argc, char* argv[]);


int main(int argc, char* argv[])
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cerr << "unknown error\n";
        return 2;
    }

}


void findSizeMinMax(const std::vector<BigInt> &vec, std::size_t &minSize, std::size_t &maxSize)
{
    minSize = std::size_t(-1);
    maxSize = std::size_t( 0);

    for(auto &&v : vec)
    {
        minSize = std::min(minSize, v.size());
        maxSize = std::max(minSize, v.size());
    }

}

inline void reduceNumInputs(std::size_t d)
{
    if (d==0)
        return;
#if defined(DEBUG) || defined(_DEBUG)
    if (numInputs>100)
#else
    if (numInputs>10000)
#endif
        numInputs /= d;
}




int unsafeMain(int argc, char* argv[])
{
    MARTY_ARG_USED(argc);
    MARTY_ARG_USED(argv);

    bool bCsv = argc>1;

    if (!bCsv)
    {
        #if defined(WIN32) || defined(_WIN32)
            #if defined(WIN64) || defined(_WIN64)
                std::cout<<"Platform: x64" << std::endl << std::flush;
            #else
                std::cout<<"Platform: x86" << std::endl << std::flush;
            #endif
        #else
                std::cout<<"Platform: unknown" << std::endl << std::flush;
        #endif
    
        std::cout << "BigInt chunk size: " << sizeof(marty::BigInt::chunk_type) << "\n" << std::flush;
        std::cout << "-------------------------\n\n" << std::flush;
    }

    for(std::size_t i=0u; i!=numInputs; ++i)
        baseVals1.emplace_back(std::rand());

    for(std::size_t i=0u; i!=numInputs; ++i)
        baseVals2.emplace_back(std::rand());

    for(std::size_t i=0u; i!=numInputs; ++i)
        baseVals3.emplace_back(std::rand());

    // for(std::size_t i=0u; i!=numInputs; ++i)
    //     baseVals4.emplace_back(std::rand());

    auto printLastSize = [&]()
    {
        #if defined(DEBUG) || defined(_DEBUG)
        // std::cout << "Total vals: " << vals.size() << ", last size: " << vals.back().size() << "\n\n" << std::flush;
        #endif
    };


    // Первый вектор кладём как есть сгенерированное рандомом
    vals.emplace_back(baseVals1); // #0
    printLastSize();


    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#1");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back(((baseVals1[i]+baseVals2[i])<<16)+baseVals3[i]);
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#2");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back(((vals[1][i]+baseVals2[i]<<32)+(baseVals3[i]<<16)+baseVals1[i]));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(5);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#3");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[2][i]<<(std::rand()%0x3F+0x3F)) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    //reduceNumInputs(4);
    reduceNumInputs(2);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#4");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[3][i]<<(std::rand()%0x3F+0x3F)) + (baseVals2[i])+(baseVals3[i]<<6)+(baseVals1[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    // reduceNumInputs(5);
    // numInputs *= 2;

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#5");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[4][i]<<(std::rand()%0x3F+0x3F)) + (baseVals3[i])+(baseVals1[i]<<6)+(baseVals2[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    //reduceNumInputs(2);
    reduceNumInputs(5);
    //reduceNumInputs(10);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#6");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[5][i]<<512) + vals[4][i] + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    //reduceNumInputs(5);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#7");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[6][i]<<512) + vals[5][i] + (baseVals2[i])+(baseVals3[i]<<6)+(baseVals1[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    //reduceNumInputs(4);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#8");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[7][i]<<512) + vals[6][i] + (baseVals3[i])+(baseVals1[i]<<6)+(baseVals2[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#9");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[8][i]<<512) + vals[7][i] + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#10");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[9][i]<<512) + vals[8][i] + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    //reduceNumInputs(5);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#11");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[10][i]<<512) + vals[9][i] + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    //reduceNumInputs(5);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#12");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[11][i]<<1024) + (vals[10][i]<<512) + (vals[9][i]<<256) + (vals[8][i]<<64) + (vals[7][i]<<24) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    //reduceNumInputs(5);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#13");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[12][i]<<2048) + (vals[11][i]<<1024) + (vals[10][i]<<512) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    //reduceNumInputs(5);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#14");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[13][i]<<4096) + (vals[12][i]<<2048) + (vals[11][i]<<1024) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    //reduceNumInputs(5);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#15");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[14][i]<<8192) + (vals[13][i]<<4096) + (vals[12][i]<<2048) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);
    //reduceNumInputs(5);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#16");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[15][i]<<16384) + (vals[14][i]<<8192) + (vals[13][i]<<4096) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#17");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[16][i]<<32768) + (vals[15][i]<<16384) + (vals[14][i]<<8192) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#18");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[17][i]<<65536) + (vals[16][i]<<32768) + (vals[15][i]<<16384) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();

    reduceNumInputs(2);

    std::cout /*<< "\n"*/ << std::flush;
    {
        #if defined(DEBUG) || defined(_DEBUG)
        auto tkprn = TickElapsedPrinter("#19");
        #endif
        std::vector<BigInt> v;
        for(std::size_t i=0u; i!=numInputs; ++i)
            v.emplace_back((vals[18][i]<<131072) + (vals[17][i]<<65536) + (vals[16][i]<<32768) + (baseVals1[i])+(baseVals2[i]<<6)+(baseVals3[i]<<12));
        vals.emplace_back(v);
    }
    printLastSize();


    if (!bCsv)
    {
        std::cout /*<< "\n"*/ << std::flush;
        std::cout << "\n\n" << std::flush;
    }

    auto totalTicksCounter = TickElapsedPrinter(bCsv ? std::string() : std::string("Total elapsed"));

    std::array<BigInt::MultiplicationMethod, 4> mMethods = { BigInt::MultiplicationMethod::auto_, BigInt::MultiplicationMethod::school, BigInt::MultiplicationMethod::karatsuba, BigInt::MultiplicationMethod::furer };

    std::array<bool, 4> excluded = { false,false,false,false };
    

    std::size_t idx = 0;
    for(auto &v: vals)
    {
        std::size_t minSize = 0u;
        std::size_t maxSize = 0u;
        findSizeMinMax(v, minSize, maxSize);
        minSize /= CHAR_BIT;
        maxSize /= CHAR_BIT;

        auto strIdx = std::to_string(idx++);
        if (!bCsv)
        {
            while(strIdx.size()<2) strIdx.append(1,' ');
            std::cout << "#" << strIdx 
                      << " min: " << minSize << " bytes (#chunks: " << minSize/sizeof(marty::BigInt::chunk_type) << "),"
                      << " max: " << maxSize << " bytes (#chunks: " << maxSize/sizeof(marty::BigInt::chunk_type) << ")\n   "
                      << " Size of data vector: " << v.size() << " - " << std::flush; //  << "\n";
        }
        else
        {
            std::cout << strIdx  << ";" 
                      << minSize/sizeof(marty::BigInt::chunk_type) << "-" << maxSize/sizeof(marty::BigInt::chunk_type) << ";"
                      << minSize << "-" << maxSize << ";"
                      << v.size()<< ";" << std::flush
                      ;
        }

        std::array<std::uint32_t, 4> ticksElapsed = { 0,0,0,0 };
        std::array<BigInt::chunk_type, 4> calculatedChunks = { 0,0,0,0 };
        std::array<unsigned, 4> calculatedPercents = { 0,0,0,0 };
        

        std::size_t methodIdx = 0;
        for(; methodIdx!=mMethods.size(); ++methodIdx)
        {
            auto mm = mMethods[methodIdx];

            BigInt::setMultiplicationMethod(mm);

            // BigInt::chunk_type counter = 0;

            if (excluded[methodIdx])
            {
                calculatedPercents[methodIdx] = unsigned(-1);
                continue;
            }
            
            std::uint32_t startTick = getMillisecTick();
            std::vector<BigInt>::const_iterator it1 = v.begin();
            std::vector<BigInt>::const_iterator it2 = v.begin(); ++it2; // у нас вектора большие, проблем нет
            for(; it2!=v.end(); ++it1, ++it2)
            {
                calculatedChunks[methodIdx] += (*it1 * *it2).getHighChunk();
            }

            auto elapsed = getMillisecTick() - startTick;
            ticksElapsed[methodIdx] = elapsed;

            if (!bCsv)
            {
                // std::cout << "    ";
                std::cout << "+" << std::flush;
            }

            if (methodIdx==0)
            {
                // std::cout << BigInt::getMultiplicationMethodName() << ": 100% (" << ticksElapsed[methodIdx] << ")  " << std::flush;
                calculatedPercents[methodIdx] = 100;
            }
            else
            {
                if (!ticksElapsed[0])
                {
                    calculatedPercents[methodIdx] = unsigned(-1);
                }
                else
                {
                    std::uint64_t elapsed64 = elapsed;
                    elapsed64 *= 100;
                    calculatedPercents[methodIdx] = unsigned(elapsed64/ticksElapsed[0]);
                }

                // std::cout << BigInt::getMultiplicationMethodName() << ": ";
                // std::uint64_t elapsed64 = elapsed;
                // elapsed64 *= 100;
                // if (!ticksElapsed[0])
                //     std::cout << "INF% (" << ticksElapsed[methodIdx] << ")  " << std::flush;
                // else
                //     std::cout << (elapsed64/ticksElapsed[0]) << "% (" << ticksElapsed[methodIdx] << ")  " << std::flush;
            }

        }

        if (!bCsv)
        {
            std::cout << "\n" << std::flush;
        }

        auto minIt  = std::min_element( calculatedPercents.begin(), calculatedPercents.end() );
        auto minIdx = std::size_t(std::distance(calculatedPercents.begin(), minIt));

        //unsigned(-1)

        // std::size_t 
        methodIdx = 0;
        for(; methodIdx!=mMethods.size(); ++methodIdx)
        {
            if (!bCsv)
            {
                std::cout << "    ";
            }
            if (minIdx==methodIdx)
                std::cout << "! ";

            if (!bCsv)
            {
                std::cout << BigInt::getMultiplicationMethodName(mMethods[methodIdx]) << ": ";
            }

            if (excluded[methodIdx])
            {
                std::cout << "-";
            }
            else
            {
                if (calculatedPercents[methodIdx]==unsigned(-1))
                {
                    std::cout << "INF";
                }
                else
                {
                    std::cout << calculatedPercents[methodIdx];
                    if (calculatedPercents[methodIdx]>1000)
                        excluded[methodIdx] = true;
                }
    
                std::cout << "%";
                std::cout << " (" << ticksElapsed[methodIdx] << ")";
            }

            if (!bCsv)
            {
                std::cout << "  ";
            }

            if (methodIdx!=(mMethods.size()-1))
            {
                if (bCsv)
                   std::cout << ";";
            }
            else // last in line
            {
                if (bCsv)
                    std::cout << "\n";
            }

            std::cout << std::flush;

        }

        if (!bCsv)
        {
            std::cout << "\n\n" << std::flush;
        }

        v.clear();
        v.shrink_to_fit();
        

// class BigInt
// {
//  
// public: // types
//  
//     enum MultiplicationMethod
//     {
//         auto_ = 0,
//         school,
//         karatsuba,
//         furer
//     };

// const char* BigInt::getMultiplicationMethodName()
// {
//     switch(s_multiplicationMethod)
//     {
//         case MultiplicationMethod::school:
//              return "school"
//  
//         case MultiplicationMethod::karatsuba:
//              return "karatsuba";
//  
//         case MultiplicationMethod::furer:
//              return "furer";
//  



    }



    return 0;
}


