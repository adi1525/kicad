/*
 * This program source code file is part of KICAD, a free EDA CAD application.
 *
 * Copyright (C) 2011 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2010 Kicad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */


#ifndef SCH_PART_H_
#define SCH_PART_H_

#include <sch_lib.h>


//-----<temporary home for PART sub objects, move after stable>------------------

#include <wx/gdicmn.h>
#include <deque>
#include <vector>
#include <sweet_lexer.h>


namespace SCH {

class PART;
class SWEET_PARSER;
class PROPERTY;

};


class POINT : public wxPoint
{
public:
    POINT( int x, int y ) :
        wxPoint( x, y )
    {}

    POINT() :
        wxPoint()
    {}
};

typedef float   ANGLE;


namespace SCH {

class FONT
{
    friend class PART;
    friend class SWEET_PARSER;

protected:
    wxString        name;       ///< name or other id such as number, TBD
    wxSize          size;
    bool            italic;
    bool            bold;

public:
    FONT() :
        italic( false ),
        bold( false )
    {}
};


struct TEXT_EFFECTS
{
    POINT       pos;
    ANGLE       angle;
    FONT        font;
    bool        isVisible;

    PROPERTY*   property;       ///< only used from a COMPONENT, specifies PROPERTY in PART
    wxString    propName;       ///< only used from a COMPONENT, specifies PROPERTY in PART

    TEXT_EFFECTS() :
        angle( 0 ),
        isVisible( false ),
        property( 0 )
    {}
};


class BASE_GRAPHIC
{
    friend class PART;
    friend class SWEET_PARSER;

protected:
    PART*   owner;

public:
    BASE_GRAPHIC( PART* aOwner ) :
        owner( aOwner )
    {}

    virtual ~BASE_GRAPHIC() {}
};

typedef std::deque<POINT>  POINTS;

class POLY_LINE : public BASE_GRAPHIC
{
    friend class PART;
    friend class SWEET_PARSER;

protected:
    double      lineWidth;
    int         fillType;       // T_none, T_filled, or T_transparent
    POINTS      pts;

public:
    POLY_LINE( PART* aOwner ) :
        BASE_GRAPHIC( aOwner )
    {}
};

class BEZIER : public POLY_LINE
{
    friend class PART;
    friend class SWEET_PARSER;

public:
    BEZIER( PART* aOwner ) :
        POLY_LINE( aOwner )
    {}
};

class RECTANGLE : public BASE_GRAPHIC
{
    friend class PART;
    friend class SWEET_PARSER;

protected:
    double      lineWidth;
    int         fillType;       // T_none, T_filled, or T_transparent
    POINT       start;
    POINT       end;

public:
    RECTANGLE( PART* aOwner ) :
        BASE_GRAPHIC( aOwner )
    {}
};


class CIRCLE : public BASE_GRAPHIC
{
    friend class PART;
    friend class SWEET_PARSER;

protected:
    POINT       center;
    int         radius;
    double      lineWidth;
    int         fillType;       // T_none, T_filled, or T_transparent

public:
    CIRCLE( PART* aOwner ) :
        BASE_GRAPHIC( aOwner )
    {}
};


class ARC : public BASE_GRAPHIC
{
    friend class PART;
    friend class SWEET_PARSER;

protected:
    POINT       pos;
    double      lineWidth;
    int         fillType;       // T_none, T_filled, or T_transparent
    int         radius;
    POINT       start;
    POINT       end;

public:
    ARC( PART* aOwner ) :
        BASE_GRAPHIC( aOwner )
    {}
};


class GR_TEXT : public BASE_GRAPHIC
{
    friend class PART;
    friend class SWEET_PARSER;

protected:
    POINT       pos;
    ANGLE       angle;

    int         fillType;       ///< T_none, T_filled, or T_transparent
    int         hjustify;       ///< T_center, T_right, or T_left
    int         vjustify;       ///< T_center, T_top, or T_bottom

    bool        isVisible;
    wxString    text;
    FONT        font;

public:
    GR_TEXT( PART* aOwner ) :
        BASE_GRAPHIC( aOwner ),
        angle( 0 ),
        fillType( PR::T_filled ),
        hjustify( PR::T_left ),
        vjustify( PR::T_bottom ),
        isVisible( true )
    {}
};


class PROPERTY : public BASE_GRAPHIC
{
    friend class PART;
    friend class SWEET_PARSER;

protected:
    PART*           birthplace;     ///< at which PART in inheritance chain was this PROPERTY added
    wxString        name;
    wxString        text;
    TEXT_EFFECTS    effects;

public:
    PROPERTY( PART* aOwner, const wxChar* aName = wxT( "" ) ) :
        BASE_GRAPHIC( aOwner ),
        birthplace( aOwner ),
        name( aName )
    {}
};


struct PINTEXT
{
    wxString    text;
    FONT        font;
    bool        isVisible;

    PINTEXT() :
        isVisible( true )
    {}
};


class PIN : public BASE_GRAPHIC
{
    friend class PART;
    friend class SWEET_PARSER;

public:
    PIN( PART* aOwner ) :
        BASE_GRAPHIC( aOwner ),
        birthplace( aOwner ),
        angle( 0 ),
        connectionType( PR::T_input ),
        shape( PR::T_line ),
        length( 0 ),
        isVisible( true )
    {}

protected:
    PART*       birthplace;         ///< at which PART in inheritance chain was this PIN added
    POINT       pos;
    ANGLE       angle;

    PINTEXT     padname;
    PINTEXT     signal;

    int         connectionType;     ///< T_input, T_output, T_bidirectional, T_tristate, T_passive, T_unspecified,
                                    ///< T_power_in, T_power_out, T_open_collector, T_open_emitter, or T_unconnected.

    int         shape;              ///< T_none, T_line, T_inverted, T_clock, T_inverted_clk, T_input_low, T_clock_low,
                                    ///< T_falling_edge, T_non_logic.

    int         length;             ///< length of pin in internal units
    bool        isVisible;          ///< pin is visible

};


}  // namespace SCH


//-----</temporary home for PART sub objects, move after stable>-----------------


namespace SCH {

typedef std::vector< BASE_GRAPHIC* >    GRAPHICS;
typedef std::vector< PIN* >             PINS;
typedef std::vector< PROPERTY* >        PROPERTIES;

class LPID;
class SWEET_PARSER;


/**
 * Class PART
 * will have to be unified with what Wayne is doing.  I want a separate copy

 * here until I can get the state management correct.  Since a PART only lives
 * within a cache called a LIB, its constructor is private (only a LIB
 * can instantiate one), and it exists in various states of freshness and
 * completeness relative to the LIB_SOURCE within the LIB.
 */
class PART
{
    friend class LIB;           // is the owner of all PARTS, afterall
    friend class SWEET_PARSER;

protected:      // not likely to have C++ descendants, but protected none-the-less.

    /// a protected constructor, only a LIB can instantiate a PART.
    PART( LIB* aOwner, const STRING& aPartNameAndRev );

    /**
     * Function destroy
     * clears out this object, deleting all graphics, all fields, all properties,
     * etc.
     */
    void clear();

    /**
     * Function inherit
     * is a specialized assignment function that copies a specific subset, enough
     * to fulfill the requirements of the Sweet s-expression language.
     */
    void inherit( const PART& aBasePart );

    POINT           anchor;

    //PART( LIB* aOwner );

    LIB*            owner;      ///< which LIB am I a part of (pun if you want)
    int             contains;   ///< has bits from Enum PartParts

    STRING          partNameAndRev;   ///< example "passives/R[/revN..]", immutable.

    LPID*           extends;    ///< of base part, NULL if none, otherwise I own it.
    PART*           base;       ///< which PART am I extending, if any.  no ownership.

    /// encapsulate the old version deletion, take ownership of @a aLPID
    void setExtends( LPID* aLPID );

    /// s-expression text for the part, initially empty, and read in as this part
    /// actually becomes cached in RAM.
    STRING          body;

    // mandatory properties
    PROPERTY        reference;      ///< prefix only, only components have full references
    PROPERTY        value;
    PROPERTY        footprint;
    PROPERTY        model;
    PROPERTY        datasheet;

    // separate lists for speed:

    /**
     * Member properties
     * holds the non-mandatory properties.
     */
    PROPERTIES      properties;

    /**
     * Member graphics
     * owns : POLY_LINE, RECTANGLE, CIRCLE, ARC, BEZIER, and GR_TEXT objects.
     */
    GRAPHICS        graphics;

    /**
     * Member pins
     * owns all the PINs in pins.
     */
    PINS            pins;

    /// Alternate body forms.
    //ALTERNATES  alternates;

    wxString        keywords;


public:

    virtual ~PART();

    PART& operator=( const PART& other );

    /**
     * Function Owner
     * returns the LIB* owner of this part.
     */
    LIB* Owner()  { return owner; }

    /**
     * Function Parse
     * translates a Sweet string into a binary form that is represented
     * by the normal fields of this class.  Parse is expected to call Inherit()
     * if this part extends any other.
     *
     * @param aParser is an instance of SWEET_PARSER, rewound at the first line.
     *
     * @param aLibTable is the LIB_TABLE "view" that is in effect for inheritance,
     *  and comes from the big containing SCHEMATIC object.
     */
    void Parse( SWEET_PARSER* aParser, LIB_TABLE* aLibTable ) throw( IO_ERROR, PARSE_ERROR );

/*
    void SetValue( const wxString& aValue )
    {
        value = aValue;
    }
    const wxString& GetValue()
    {
        return value;
    }

    void SetFootprint( const wxString& aFootprint )
    {
        footprint = aFootprint;
    }
    const wxString& GetFootprint()
    {
        return footprint;
    }

    void SetModel( const wxString& aModel )
    {
        model = aModel;
    }
    const wxString& GetModel()
    {
        return model;
    }
*/

/*
    void SetBody( const STR_UTF& aSExpression )
    {
        body = aSExpression;
    }
*/
};

}   // namespace PART

#endif  // SCH_PART_
