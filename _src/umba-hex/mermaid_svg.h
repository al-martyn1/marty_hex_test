/*! \file
    \brief SVG-хелперы
 */

#pragma once

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#include "umba/tokenizer/parsers/mermaid_packet_diagram_parser_types.h"
#include "marty_svg/marty_svg.h"

//----------------------------------------------------------------------------

// mermaid::svg::
namespace mermaid {
namespace svg {


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
// Для одного элемента даиграммы
struct ByteDiagramViewEntryData
{
    std::uint64_t   linearAddress;
    std::string     addressStr   ;
    byte_vector_t   bytes        ;
    bool            bFlatByteArray = false; // flat array of single bytes - show it as single entry
    std::uint64_t   idxFirst       = 0; // Индекс хвоста получаем по размеру вектора bytes
    //std::uint64_t   idxLast        = 0;
    bool            arrayFirst = false; // first element
    bool            arrayLast  = false; // last element
};

// Поле, одиночное или массив/диапазон
struct ByteDiagramViewEntry
{
    std::uint64_t                            linearAddress ;
    std::string                              addressStr    ; // Совпадает с тем, что в первом элементе в data
    std::string                              name          ;
    std::vector<ByteDiagramViewEntryData>    data          ;
    bool                                     bArray = false; // or range
    //bool                                     bFlat  = false; // flat array of single bytes - show it as single entry
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

//----------------------------------------------------------------------------
struct ByteDiagramFieldLabelDrawInfo
{
    std::string  fieldName;

    bool        simpleSvg = true;
    std::size_t arraySize = std::size_t(-1); // Not an array
    std::size_t indexFirst;
    std::size_t indexLast ;
    int posLeft  = 0;
    int posRight = 0;
    int posY     = 0;

    std::string  svgText;
};


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
             , const ByteDiagramViewEntryData &entryData
             , std::size_t startIdx
             , int posX, int posY
             , const std::string &rectClass
             , const std::string &lineClass
             , const std::string &arrayBoundsLineClass
             , const std::string &byteNumClass
             , const std::string &indexNumClass
             // , const std::string &fieldLabelsClass
             // , const std::string &fieldLabel
             , int byteWidth
             , int lineHeigh
             // , int indexLabelLineHeight
             , int r
             , int endsSpacing
             , bool singleByteBlock
             , bool compactMode
             , std::size_t first=0, std::size_t last=std::size_t(-1)
             , umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags packetDiagramDisplayOptionFlags=umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::none
             , int *pCalculatedPosLeft  = 0
             , int *pCalculatedPosRight = 0
             )
{

    // UMBA_USED(fieldLabelsClass    );
    // UMBA_USED(fieldLabel          );
    // UMBA_USED(indexLabelLineHeight);

    // UMBA_USED(pCalculatedPosLeft );
    // UMBA_USED(pCalculatedPosRight);

    if (pCalculatedPosLeft)
       *pCalculatedPosLeft = posX;

    //int rightPos;

    const byte_vector_t &bv = entryData.bytes;

    if (bv.empty())
        return;

    if (last==std::size_t(-1))
        last = bv.size()-1;

    bool roundLeft  = first==0;
    bool roundRight = bv.size()==(last+1);
    std::size_t len = last-first+1;

    bool drawByteNumbers              = (packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::byteNumbers)!=0;
    bool drawByteNumbersForSingleByte = (packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::singleByteNumbers)!=0;
    bool drawSingleByteBlockNumbers   = (packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::singleByteBlockNumbers)!=0;
    bool splitWordsToByte             = (packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::splitWordsToBytes)!=0;
    bool showFieldIndex               = (packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::showFieldIndex)!=0;
    bool showSingleByteFieldIndex     = (packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::showSingleByteFieldIndex)!=0;
    bool showArrayBounds              = (packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::showArrayBounds)!=0;
    

    if (pCalculatedPosRight)
       *pCalculatedPosRight = int(posX+byteWidth*len);

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

    if (showArrayBounds)
    {
        auto deltaX       = 2;
        auto deltaTopY    = 1;
        auto deltaBottomY = 3;
        auto topY     = posY-deltaTopY;
        //auto btmY     = posY+lineHeigh+2;
        auto hgt      = lineHeigh+deltaTopY+deltaBottomY;

        if (entryData.arrayFirst && first==0)
        {
            #if 1
            auto earsPosX = posX+deltaX+1;

            marty::svg::pathStart(oss, earsPosX, topY, arrayBoundsLineClass, true /* bAbs */);
            marty::svg::pathHorzLineTo(oss, -deltaX);
            marty::svg::pathVertLineTo(oss, hgt);
            marty::svg::pathHorzLineTo(oss, +deltaX);
            marty::svg::pathEnd(oss, false /* !closePath */ );
            #endif
        }

        if (entryData.arrayLast && last==bv.size()-1)
        {
            // auto earsPosX = posX+delta-1;
            auto earsPosX = int(posX+(last-first+1)*byteWidth)-deltaX-1;

            marty::svg::pathStart(oss, earsPosX, topY, arrayBoundsLineClass, true /* bAbs */);
            marty::svg::pathHorzLineTo(oss, +deltaX);
            marty::svg::pathVertLineTo(oss, hgt);
            marty::svg::pathHorzLineTo(oss, -deltaX);
            marty::svg::pathEnd(oss, false /* !closePath */ );
        }
    }


    auto makeIndexStr = [&](std::uint64_t v)
    {
        if ((packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::hexFieldIndex)==0)
            return std::to_string(v); // Возвращаем десятичное представление

        auto strHex = umba::tokenizer::mermaid::utils::makeHexString(v);

        if ((packetDiagramDisplayOptionFlags&umba::tokenizer::mermaid::PacketDiagramDisplayOptionFlags::hexPrefixFieldIndex)==0)
            return strHex; // Префикс HEX не надо добавлять

        return umba::tokenizer::mermaid::utils::addNumberPrefix(strHex, "0x");
    };

    {
        auto chunkSize = last-first+1u;
        for(auto i=first; i<=last; ++i)
        {
            auto tx = int(posX+(i-first)*byteWidth+byteWidth/2);

            if (i==first)
                tx += endsSpacing/2;
            else if (i==last)
                tx -= endsSpacing/2;

            if ( (singleByteBlock && drawSingleByteBlockNumbers)
              || (bv.size()<2     && drawByteNumbersForSingleByte)
              || (bv.size()>1     && drawByteNumbers)
               )
            drawText( oss, tx, int(posY+lineHeigh/2+1)
                    , std::to_string(bv[i]), byteNumClass, "middle", "middle"
                    );

        }

        if (showFieldIndex)
        {
            auto tx = int( posX +  /* (first)*byteWidth */ +endsSpacing);
            if (bv.size()>1 || showSingleByteFieldIndex)
            {
                drawText( oss, tx, int(posY-2)
                        , makeIndexStr(std::size_t(startIdx+first)), indexNumClass, "auto", "start"
                        );
            }
            std::size_t sizeForLeftLabelLim = compactMode ? 2u : 1u;
            if (chunkSize>sizeForLeftLabelLim)
            {
                tx = int( posX + ( /* first+ */ chunkSize)*byteWidth-endsSpacing);
                drawText( oss, tx, int(posY-2)
                        , makeIndexStr(std::size_t(startIdx+first+chunkSize-1)), indexNumClass, "auto", "end"
                        );
            }
        }
            //indexNumClass


    }

}

// Размер байта по ширине вычисляем как 
// style="max-width: 1026px;" - 1024/кол-во байт в строке - 8, 16, 32, 64
// 1024/64=16 px - не много, при высоте в 26.
// 1024/32=32



} // namespace svg
} // namespace mermaid
// mermaid::svg::









