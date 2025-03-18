/*! \file
    \brief SVG-хелперы
 */

#pragma once

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#include "umba/tokenizer/parsers/marmaid_packet_diagram_parser_types.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType>
void writeSvg( StreamType &oss
             , int viewSizeX, int viewSizeY
             , const std::string &style
             , const std::string &text
             )
{
    oss << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%\" viewBox=\"0 0 " << viewSizeX << " " << viewSizeY << "\" style=\"max-width: 1026px;\">\n";
    oss << style << "\n";
    oss << text << "\n";
    oss << "</svg>\n";
}
// <svg id="mermaid-22" width="100%" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1026 423" 
//      style="max-width: 1026px;" role="graphics-document document" aria-roledescription="packet"
//      >
//  
// </svg>

//----------------------------------------------------------------------------
template<typename StreamType>
void drawRect( StreamType &oss
             , int  posX , int posY
             , int  sizeX, int sizeY
             , const std::string &itemClass
             , bool roundLeft
             , bool roundRight
             , int  r
             )
{
    // https://stackoverflow.com/questions/34923888/rounded-corner-only-on-one-side-of-svg-rect
    // https://medium.com/@dennismphil/one-side-rounded-rectangle-using-svg-fb31cf318d90

    // https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke-linejoin

    // https://developer.mozilla.org/en-US/docs/Web/CSS/stroke-linejoin

    if (roundLeft && roundRight) // Both rounds
    {
        oss << "<rect x=\"" << posX << "\" y=\"" << posY << "\" width=\"" << sizeX << "\" height=\"" << sizeY << "\" rx=\"" << r << "\" ry=\"" << r << "\" class=\"" << itemClass << "\" />\n";
    }
    else if (!roundLeft && !roundRight) // No rounds at all
    {
        oss << "<rect x=\"" << posX << "\" y=\"" << posY << "\" width=\"" 
            << sizeX << "\" height=\"" << sizeY << "\" class=\"" << itemClass << "\" />\n";
    }
    else if (roundLeft)
    {
        oss << "<path class=\"" << itemClass 
            << "\" d=\"M" << posX+sizeX << "," << posY // +sizeY 
            << " v" << (sizeY)
            << " h-" << (sizeX-r) 
            << " q-"<<  r   << ",0" << " "  << -r << ",-" << r 
            << " v-" << (sizeY-2*r)
            << " q" << "0,-" << r   << " "  <<  r << ",-" << r // 0,-5 5,-5
            //<< " h" << (sizeX-r) 
            << " z" << "\"" << " />\n";
    }
    else // roundRight
    {
        oss << "<path class=\"" << itemClass 
            << "\" d=\"M" << posX << "," << posY 
            << " h" << (sizeX-r) 
            << " q" << r << ",0 " << r << "," << r 
            << " v" << (sizeY-2*r)
            << " q" << "0," << r << " -" << r << "," << r 
            << " h-" << (sizeX-r) 
            << " z" << "\"" << " />\n";
    }

}

//----------------------------------------------------------------------------
template<typename StreamType>
void drawLine( StreamType &oss
             , int  startX, int startY
             , int  endX  , int endY
             , const std::string &lineClass
             )
{
    oss << "<line x1=\"" << startX << "\" y1=\"" << startY << "\" x2=\"" << endX << "\" y2=\"" << endY << "\" class=\"" << lineClass << "\"/>\n";
}

//----------------------------------------------------------------------------
template<typename StreamType>
void drawText( StreamType &oss
             , int  posX, int posY
             , const std::string &text
             , const std::string &textClass
             , const std::string &baseLine  = "auto" // auto|middle|hanging - https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/dominant-baseline
             , const std::string &hAlign    = "start" // start|middle|end   - https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/text-anchor
             )
{
    oss << "<text x=\"" << posX << "\" y=\"" << posY << "\" class=\"" << textClass << "\" dominant-baseline=\"" << baseLine << "\" text-anchor=\"" << hAlign << "\">" << text << "</text>\n";
}
// <text x="398.5" y="172" class="packetLabel" dominant-baseline="middle" text-anchor="middle">PSH</text>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
using byte_vector_t = umba::tokenizer::marmaid::utils::byte_vector_t;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
byte_vector_t makeNumbersVector(std::uint8_t from, std::uint8_t to)
{
    byte_vector_t bv;

    if (from>to)
    {
        for(; from>=to; --from)
            bv.push_back(from);
    }
    else
    {
        for(; from<=to; ++from)
            bv.push_back(from);
    }

    return bv;
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








