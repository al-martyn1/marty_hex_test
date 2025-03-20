/*! \file
    \brief SVG-хелперы
 */

#pragma once

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#include "umba/tokenizer/parsers/mermaid_packet_diagram_parser_types.h"
#include "marty_svg/marty_svg.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
using marty::svg::writeSvg;
using marty::svg::drawRect;
using marty::svg::drawLine;
using marty::svg::drawText;
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
using byte_vector_t = umba::tokenizer::mermaid::utils::byte_vector_t;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
byte_vector_t makeNumbersVector(std::uint8_t from, std::uint8_t to)
{
    return umba::tokenizer::mermaid::utils::makeByteVector(from, to);
}

//----------------------------------------------------------------------------
template<typename StreamType>
void drawWord( StreamType &oss
             , const byte_vector_t &bv
             , int posX, int posY
             , const std::string &rectClass
             , const std::string &lineClass
             , const std::string &byteNumClass
             , int byteWidth
             , int lineHeigh
             , int r
             , int endsSpacing
             , std::size_t first=0, std::size_t last=std::size_t(-1)
             , bool drawByteNumbers=true
             , bool drawByteNumbersForSingleByte=true
             , bool splitWordsToByte=true
             )
{
    if (bv.empty())
        return;

    if (last==std::size_t(-1))
        last = bv.size()-1;

    bool roundLeft  = first==0;
    bool roundRight = bv.size()==(last+1);
    std::size_t len = last-first+1;

    drawRect( oss, posX+endsSpacing, posY
            , int(byteWidth*len-2*endsSpacing), lineHeigh
            , rectClass
            , roundLeft, roundRight
            , r
            );

    if (splitWordsToByte)
    {
        for(auto i=first+1; i<=(last /* +1 */ ); ++i)
        {
            auto vlX = int(posX+(i-first)*byteWidth);
            drawLine(oss, vlX, posY, vlX, posY+lineHeigh, lineClass);
        }
    }

    bool drawSingleByteLabel = drawByteNumbersForSingleByte || bv.size()>1;
    if (drawSingleByteLabel && drawByteNumbers)
    {
        for(auto i=first; i<=last; ++i)
        {
            auto tx = int(posX+(i-first)*byteWidth+byteWidth/2);

            if (i==first)
                tx += endsSpacing/2;
            else if (i==last)
                tx -= endsSpacing/2;

            drawText( oss, tx, int(posY+lineHeigh/2+1)
                    , std::to_string(bv[i]), byteNumClass, "middle", "middle"
                    );
        }
    }

}

// Размер байта по ширине вычисляем как 
// style="max-width: 1026px;" - 1024/кол-во байт в строке - 8, 16, 32, 64
// 1024/64=16 px - не много, при высоте в 26.
// 1024/32=32




//----------------------------------------------------------------------------
// Для одного элемента
struct ByteDiagramViewEntryData
{
    std::uint64_t   linearAddress;
    std::string     addressStr   ;
    byte_vector_t   bytes        ;
};

// Поле, одиночное или массив/диапазон
struct ByteDiagramViewEntry
{
    std::uint64_t                            linearAddress ;
    std::string                              addressStr    ; // Совпадает с тем, что в первом элементе в data
    std::string                              name          ;
    std::vector<ByteDiagramViewEntryData>    data          ;
    bool                                     bArray = false; // or range
    bool                                     bFlat  = false; // flat array of single bytes - show it as single entry
};

// Секция (org)
struct ByteDiagramViewSectionData
{
    std::uint64_t                            linearAddress;
    std::string                              addressStr   ;
    std::string                              name         ;
    std::vector<ByteDiagramViewEntry>        entries      ;
};

// Вся диаграмма
struct ByteDiagramViewData
{
    std::string                              title   ;
    std::vector<ByteDiagramViewSectionData>  sections;
};








