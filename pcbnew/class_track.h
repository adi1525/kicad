/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 1992-2011 KiCad Developers, see AUTHORS.txt for contributors.
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

/**
 * @file class_track.h
 * @brief Definitions for tracks, vias and zones.
 */

#ifndef CLASS_TRACK_H
#define CLASS_TRACK_H


#include <class_board_item.h>
#include <class_board_connected_item.h>
#include <PolyLine.h>


class TRACK;
class D_PAD;
class MSG_PANEL_ITEM;


// Via attributes (m_Shape parameter)
#define VIA_THROUGH                3       /* Always a through hole via */
#define VIA_BLIND_BURIED           2       /* this via can be on internal layers */
#define VIA_MICROVIA               1       /* this via which connect from an external layer
                                            * to the near neighbor internal layer */
#define VIA_NOT_DEFINED            0       /* not yet used */

#define UNDEFINED_DRILL_DIAMETER  -1       //< Undefined via drill diameter.


/**
 * Function GetTrace
 * is a helper function to locate a trace segment having an end point at \a aPosition
 * on \a aLayerMask starting at \a aStartTrace and end at \a aEndTrace.
 * <p>
 * The segments of track that are flagged as deleted or busy are ignored.  Layer
 * visibility is also ignored.
 * </p>
 * @param aStartTrace A pointer to the TRACK object to begin searching.
 * @param aEndTrace A pointer to the TRACK object to stop the search.  A NULL value
 *                  searches to the end of the list.
 * @param aPosition A wxPoint object containing the position to test.
 * @param aLayerMask A layer or layers to mask the hit test.  Use -1 to ignore
 *                   layer mask.
 * @return A TRACK object pointer if found otherwise NULL.
 */
extern TRACK* GetTrace( TRACK* aStartTrace, TRACK* aEndTrace, const wxPoint& aPosition,
                        int aLayerMask );


class TRACK : public BOARD_CONNECTED_ITEM
{
    // make SetNext() and SetBack() private so that they may not be called from anywhere.
    // list management is done on TRACKs using DLIST<TRACK> only.
private:
    void SetNext( EDA_ITEM* aNext )       { Pnext = aNext; }
    void SetBack( EDA_ITEM* aBack )       { Pback = aBack; }


public:
    int         m_Width;            // Thickness of track, or via diameter
    wxPoint     m_Start;            // Line start point
    wxPoint     m_End;              // Line end point
    int         m_Shape;            // vias: shape and type, Track = shape..

protected:
    int         m_Drill;            // for vias: via drill (- 1 for default value)

public:
    BOARD_CONNECTED_ITEM* start;    // pointers to a connected item (pad or track)
    BOARD_CONNECTED_ITEM* end;

    double      m_Param;            // Auxiliary variable ( used in some computations )

public:
    TRACK( BOARD_ITEM* aParent, KICAD_T idtype = PCB_TRACE_T );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    TRACK* Next() const { return (TRACK*) Pnext; }
    TRACK* Back() const { return (TRACK*) Pback; }

    virtual void Move( const wxPoint& aMoveVector )
    {
        m_Start += aMoveVector;
        m_End   += aMoveVector;
    }

    virtual void Rotate( const wxPoint& aRotCentre, double aAngle );

    virtual void Flip( const wxPoint& aCentre );

    void SetPosition( const wxPoint& aPos )     { m_Start = aPos; }     // was overload
    const wxPoint& GetPosition() const          { return m_Start; }     // was overload

    void SetWidth( int aWidth )                 { m_Width = aWidth; }
    int GetWidth() const                        { return m_Width; }

    void SetEnd( const wxPoint& aEnd )          { m_End = aEnd; }
    const wxPoint& GetEnd() const               { return m_End; }

    void SetStart( const wxPoint& aStart )      { m_Start = aStart; }
    const wxPoint& GetStart() const             { return m_Start; }

    EDA_RECT GetBoundingBox() const;

    /**
     * Function GetBestInsertPoint
     * searches the "best" insertion point within the track linked list.
     * The best point is the begging of the corresponding net code section.
     * (The BOARD::m_Track and BOARD::m_Zone lists are sorted by netcode.)
     * @param aPcb The BOARD to search for the insertion point.
     * @return TRACK* - the item found in the linked list (or NULL if no track)
     */
    TRACK* GetBestInsertPoint( BOARD* aPcb );

    /* Search (within the track linked list) the first segment matching the netcode
     * ( the linked list is always sorted by net codes )
     */
    TRACK* GetStartNetCode( int NetCode );

    /* Search (within the track linked list) the last segment matching the netcode
     * ( the linked list is always sorted by net codes )
     */
    TRACK* GetEndNetCode( int NetCode );

    /**
     * Function GetLength
     * returns the length of the track using the hypotenuse calculation.
     * @return double - the length of the track
     */
    double GetLength() const
    {
        double dx = m_Start.x - m_End.x;
        double dy = m_Start.y - m_End.y;

        return hypot( dx, dy );
    }

    /* Display on screen: */
    void Draw( EDA_DRAW_PANEL* panel, wxDC* DC,
               GR_DRAWMODE aDrawMode, const wxPoint& aOffset = ZeroOffset );

    /* divers */
    int GetShape() const { return m_Shape & 0xFF; }
    void SetShape( int aShape ) { m_Shape = aShape; }

    /**
     * Function TransformShapeWithClearanceToPolygon
     * Convert the track shape to a closed polygon
     * Used in filling zones calculations
     * Circles (vias) and arcs (ends of tracks) are approximated by segments
     * @param aCornerBuffer = a buffer to store the polygon
     * @param aClearanceValue = the clearance around the pad
     * @param aCircleToSegmentsCount = the number of segments to approximate a circle
     * @param aCorrectionFactor = the correction to apply to circles radius to keep
     * clearance when the circle is approximated by segment bigger or equal
     * to the real clearance value (usually near from 1.0)
     */
    void TransformShapeWithClearanceToPolygon( std::vector <CPolyPt>& aCornerBuffer,
                                               int                    aClearanceValue,
                                               int                    aCircleToSegmentsCount,
                                               double                 aCorrectionFactor );
    /**
     * Function SetDrill
     * sets the drill value for vias.
     * @param aDrill is the new drill diameter
    */
    void SetDrill( int aDrill )             { m_Drill = aDrill; }

    /**
     * Function GetDrill
     * returns the local drill setting for this VIA.  If you want the calculated value,
     * use GetDrillValue() instead.
     */
    int GetDrill() const                    { return m_Drill; }

    /**
     * Function GetDrillValue
     * "calculates" the drill value for vias (m-Drill if > 0, or default
     * drill value for the board.
     * @return real drill_value
    */
    int GetDrillValue() const;

    /**
     * Function SetDrillDefault
     * sets the drill value for vias to the default value #UNDEFINED_DRILL_DIAMETER.
    */
    void SetDrillDefault()      { m_Drill = UNDEFINED_DRILL_DIAMETER; }

    /**
     * Function IsDrillDefault
     * @return true if the drill value is default value (-1)
    */
    bool IsDrillDefault()       { return m_Drill <= 0; }

    /**
     * Function ReturnMaskLayer
     * returns a "layer mask", which is a bitmap of all layers on which the
     * TRACK segment or SEGVIA physically resides.
     * @return int - a layer mask, see pcbstruct.h's LAYER_BACK, etc.
     */
    int ReturnMaskLayer() const;

    /**
     * Function IsPointOnEnds
     * returns STARTPOINT if point if near (dist = min_dist) start point, ENDPOINT if
     * point if near (dist = min_dist) end point,STARTPOINT|ENDPOINT if point if near
     * (dist = min_dist) both ends, or 0 if none of the above.
     * if min_dist < 0: min_dist = track_width/2
     */
    int IsPointOnEnds( const wxPoint& point, int min_dist = 0 );

    /**
     * Function IsNull
     * returns true if segment length is zero.
     */
    bool IsNull();

    void GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList );

    /**
     * Function GetMsgPanelInfoBase
     * Display info about the track segment only, and does not calculate the full track length
     * @param aList A list of #MSG_PANEL_ITEM objects to add status information.
     */
    void GetMsgPanelInfoBase( std::vector< MSG_PANEL_ITEM >& aList );

    /**
     * Function ShowWidth
     * returns the width of the track in displayable user units.
     */
    wxString ShowWidth() const;

    SEARCH_RESULT Visit( INSPECTOR* inspector, const void* testData,
                         const KICAD_T scanTypes[] );


    virtual bool HitTest( const wxPoint& aPosition );

    virtual bool HitTest( const EDA_RECT& aRect ) const;

    /**
     * Function GetVia
     * finds the first SEGVIA object at \a aPosition on \a aLayer starting at the trace.
     *
     * @param aPosition The wxPoint to HitTest() against.
     * @param aLayerMask The layer to match, pass -1 for a don't care.
     * @return A pointer to a SEGVIA object if found, else NULL.
     */
    TRACK* GetVia( const wxPoint& aPosition, int aLayerMask = -1 );

    /**
     * Function GetVia
     * finds the first SEGVIA object at \a aPosition on \a aLayer starting at the trace
     * and ending at \a aEndTrace.
     *
     * @param aEndTrace Pointer to the last TRACK object to end search.
     * @param aPosition The wxPoint to HitTest() against.
     * @param aLayerMask The layers to match, pass -1 for a don't care.
     * @return A pointer to a SEGVIA object if found, else NULL.
     */
    TRACK* GetVia( TRACK* aEndTrace, const wxPoint& aPosition, int aLayerMask );

    /**
     * Function GetTrace
     * return the trace segment connected to the segment at \a aEndPoint from \a
     * aStartTrace to \a aEndTrace.
     *
     * @param aStartTrace A pointer to the TRACK object to begin searching.
     * @param aEndTrace A pointer to the TRACK object to stop the search.  A NULL value
     *                  searches to the end of the list.
     * @param aEndPoint The start or end point of the segment to test against.
     * @return A TRACK object pointer if found otherwise NULL.
     */
    TRACK* GetTrace( TRACK* aStartTrace, TRACK* aEndTrace, int aEndPoint );

    /**
     * Function GetEndSegments
     * get the segments connected to the end point of the track.
     *  return 1 if OK, 0 when a track is a closed loop
     *  and the beginning and the end of the track in *StartTrack and *EndTrack
     *  Modify *StartTrack en *EndTrack  :
     *  (*StartTrack)->m_Start coordinate is the beginning of the track
     *  (*EndTrack)->m_End coordinate is the end of the track
     *  Segments connected must be consecutive in list
     */
    int GetEndSegments( int NbSegm, TRACK** StartTrack, TRACK** EndTrack );

    wxString GetClass() const
    {
        return wxT( "TRACK" );
    }

     /**
     * Function GetClearance
     * returns the clearance in internal units.  If \a aItem is not NULL then the
     * returned clearance is the greater of this object's clearance and
     * aItem's clearance.  If \a aItem is NULL, then this objects clearance
     * is returned.
     * @param aItem is another BOARD_CONNECTED_ITEM or NULL
     * @return int - the clearance in internal units.
     */
    virtual int GetClearance( BOARD_CONNECTED_ITEM* aItem = NULL ) const;

    virtual wxString GetSelectMenuText() const;

    virtual BITMAP_DEF GetMenuImage() const { return  showtrack_xpm; }

    virtual EDA_ITEM* Clone() const;

#if defined (DEBUG)

    void Show( int nestLevel, std::ostream& os ) const;     // overload

    /**
     * Function ShowState
     * converts a set of state bits to a wxString
     * @param stateBits Is an OR-ed together set of bits like BUSY, EDIT, etc.
     */
    static wxString ShowState( int stateBits );

#endif
};


class SEGZONE : public TRACK
{
public:
    SEGZONE( BOARD_ITEM* aParent );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    wxString GetClass() const
    {
        return wxT( "ZONE" );
    }


    SEGZONE* Next() const { return (SEGZONE*) Pnext; }

    wxString GetSelectMenuText() const;

    BITMAP_DEF GetMenuImage() const { return  add_zone_xpm; }

    EDA_ITEM* Clone() const;
};


class SEGVIA : public TRACK
{
public:
    SEGVIA( BOARD_ITEM* aParent );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    void Draw( EDA_DRAW_PANEL* panel, wxDC* DC,
               GR_DRAWMODE aDrawMode, const wxPoint& aOffset = ZeroOffset );

    bool IsOnLayer( int aLayer ) const;

    /**
     * Function SetLayerPair
     * set the .m_Layer member param:
     *  For a via m_Layer contains the 2 layers :
     * top layer and bottom layer used by the via.
     * The via connect all layers from top layer to bottom layer
     * 4 bits for the first layer and 4 next bits for the second layer
     * @param top_layer = first layer connected by the via
     * @param bottom_layer = last layer connected by the via
     */
    void SetLayerPair( int top_layer, int bottom_layer );

    /**
     * Function ReturnLayerPair
     * Return the 2 layers used by  the via (the via actually uses
     * all layers between these 2 layers)
     *  @param top_layer = pointer to the first layer (can be null)
     *  @param bottom_layer = pointer to the last layer (can be null)
     */
    void ReturnLayerPair( int* top_layer, int* bottom_layer ) const;

    const wxPoint& GetPosition() const  {  return m_Start; }       // was overload
    void SetPosition( const wxPoint& aPoint ) { m_Start = aPoint;  m_End = aPoint; }    // was overload

    wxString GetClass() const
    {
        return wxT( "VIA" );
    }

    wxString GetSelectMenuText() const;

    BITMAP_DEF GetMenuImage() const { return  via_sketch_xpm; }

    EDA_ITEM* Clone() const;

#if defined (DEBUG)
    void Show( int nestLevel, std::ostream& os ) const;     // overload
#endif
};


#endif /* CLASS_TRACK_H */
