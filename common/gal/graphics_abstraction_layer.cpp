/*
 * This program source code file is part of KICAD, a free EDA CAD application.
 *
 * Copyright (C) 2012 Torsten Hueter, torstenhtr <at> gmx.de
 * Copyright (C) 2012 Kicad Developers, see change_log.txt for contributors.
 *
 * Graphics Abstraction Layer (GAL) - base class
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

#include <wx/log.h>

#include <gal/graphics_abstraction_layer.h>
#include <gal/definitions.h>


using namespace KiGfx;

GAL::GAL() :
        strokeFont( this )
{
    // Set the default values for the internal variables
    SetIsFill( false );
    SetIsStroke( true );
    SetFillColor( COLOR4D( 0.0, 0.0, 0.0, 0.0 ) );
    SetStrokeColor( COLOR4D( 1.0, 1.0, 1.0, 1.0 ) );
    SetZoomFactor( 1.0 );
    SetDepthRange( VECTOR2D( -2048, 2047 ) );
    SetFlip( false, false );
    SetLineWidth( 1.0 );

    // Set grid defaults
    SetGridVisibility( true );
    SetGridStyle( GRID_STYLE_LINES );
    SetCoarseGrid( 10 );
    SetGridLineWidth( 0.5 );

    // Initialize the cursor shape
    SetCursorColor( COLOR4D( 1.0, 1.0, 1.0, 1.0 ) );
    SetCursorSize( 20 );
    SetCursorEnabled( true );

    strokeFont.LoadNewStrokeFont( newstroke_font, newstroke_font_bufsize );
}


GAL::~GAL()
{
}


void GAL::SetTextAttributes( const EDA_TEXT* aText )
{
    strokeFont.SetGlyphSize( VECTOR2D( aText->GetSize() ) );
    strokeFont.SetHorizontalJustify( aText->GetHorizJustify() );
    strokeFont.SetVerticalJustify( aText->GetVertJustify() );
    strokeFont.SetBold( aText->IsBold() );
    strokeFont.SetItalic( aText->IsItalic() );
    strokeFont.SetMirrored( aText->IsMirrored() );
}


void GAL::ComputeWorldScreenMatrix()
{
    ComputeWorldScale();

    worldScreenMatrix.SetIdentity();

    MATRIX3x3D translation;
    translation.SetIdentity();
    translation.SetTranslation( 0.5 * screenSize );

    MATRIX3x3D scale;
    scale.SetIdentity();
    scale.SetScale( VECTOR2D( worldScale, worldScale ) );

    MATRIX3x3D flip;
    flip.SetIdentity();
    flip.SetScale( VECTOR2D( flipX, flipY ) );

    MATRIX3x3D lookat;
    lookat.SetIdentity();
    lookat.SetTranslation( -lookAtPoint );

    worldScreenMatrix = translation * flip * scale * lookat * worldScreenMatrix;
    screenWorldMatrix = worldScreenMatrix.Inverse();
}


void GAL::DrawGrid()
{
    if( !gridVisibility )
        return;

    SetTarget( TARGET_NONCACHED );

    // Draw the origin marker
    double origSize = static_cast<double>( gridOriginMarkerSize ) / worldScale;
    SetLayerDepth( 0.0 );
    SetIsFill( false );
    SetIsStroke( true );
    SetStrokeColor( COLOR4D( 1.0, 1.0, 1.0, 1.0 ) );
    SetLineWidth( gridLineWidth / worldScale );
    DrawLine( gridOrigin + VECTOR2D( -origSize, -origSize ),
              gridOrigin + VECTOR2D( origSize, origSize ) );
    DrawLine( gridOrigin + VECTOR2D( -origSize, origSize ),
              gridOrigin + VECTOR2D( origSize, -origSize ) );
    DrawCircle( gridOrigin, origSize * 0.7 );

    // Draw the grid
    // For the drawing the start points, end points and increments have
    // to be calculated in world coordinates
    VECTOR2D    worldStartPoint = screenWorldMatrix * VECTOR2D( 0.0, 0.0 );
    VECTOR2D    worldEndPoint   = screenWorldMatrix * screenSize;

    int gridScreenSizeDense  = round( gridSize.x * worldScale );
    int gridScreenSizeCoarse = round( gridSize.x * static_cast<double>( gridTick ) * worldScale );

    // Compute the line marker or point radius of the grid
    double marker = 2.0 * gridLineWidth / worldScale;
    double doubleMarker = 2.0 * marker;

    // Check if the grid would not be too dense
    if( std::max( gridScreenSizeDense, gridScreenSizeCoarse ) > gridDrawThreshold )
    {
        // Compute grid variables
        int gridStartX = round( worldStartPoint.x / gridSize.x );
        int gridEndX = round( worldEndPoint.x / gridSize.x );
        int gridStartY = round( worldStartPoint.y / gridSize.y );
        int gridEndY = round( worldEndPoint.y / gridSize.y );

        // Swap the coordinates, if they have not the right order
        SWAP( gridEndX, <, gridStartX );
        SWAP( gridEndY, <, gridStartY );

        // Correct the index, else some lines are not correctly painted
        gridStartX  -= 1;
        gridStartY  -= 1;
        gridEndX    += 1;
        gridEndY    += 1;

        // Draw the grid behind all other layers
        SetLayerDepth( depthRange.y * 0.75 );

        if( gridStyle == GRID_STYLE_LINES )
        {
            SetIsFill( false );
            SetIsStroke( true );
            SetStrokeColor( gridColor );

            // Now draw the grid, every coarse grid line gets the double width
            for( int j = gridStartY; j < gridEndY; j += 1 )
            {
                if( j % gridTick == 0 && gridScreenSizeDense > gridDrawThreshold )
                    SetLineWidth( doubleMarker );
                else
                    SetLineWidth( marker );

                if( ( j % gridTick == 0 && gridScreenSizeCoarse > gridDrawThreshold )
                    || gridScreenSizeDense > gridDrawThreshold )
                {
                    drawGridLine( VECTOR2D( gridStartX * gridSize.x, j * gridSize.y ),
                                  VECTOR2D( gridEndX * gridSize.x,   j * gridSize.y ) );
                }
            }

            for( int i = gridStartX; i < gridEndX; i += 1 )
            {
                if( i % gridTick == 0 && gridScreenSizeDense > gridDrawThreshold )
                    SetLineWidth( doubleMarker );
                else
                    SetLineWidth( marker );

                if( ( i % gridTick == 0 && gridScreenSizeCoarse > gridDrawThreshold )
                    || gridScreenSizeDense > gridDrawThreshold )
                {
                    drawGridLine( VECTOR2D( i * gridSize.x, gridStartY * gridSize.y ),
                                  VECTOR2D( i * gridSize.x, gridEndY * gridSize.y ) );
                }
            }
        }
        else    // Dotted grid
        {
            bool tickX, tickY;
            SetIsFill( true );
            SetIsStroke( false );
            SetFillColor( gridColor );

            for( int j = gridStartY; j < gridEndY; j += 1 )
            {
                if( j % gridTick == 0 && gridScreenSizeDense > gridDrawThreshold )
                    tickY = true;
                else
                    tickY = false;

                for( int i = gridStartX; i < gridEndX; i += 1 )
                {
                    if( i % gridTick == 0 && gridScreenSizeDense > gridDrawThreshold )
                        tickX = true;
                    else
                        tickX = false;

                    if( tickX || tickY || gridScreenSizeDense > gridDrawThreshold )
                    {
                        double radius = ( tickX && tickY ) ? doubleMarker : marker;
                        DrawRectangle( VECTOR2D( i * gridSize.x - radius,
                                                 j * gridSize.y - radius ),
                                       VECTOR2D( i * gridSize.x + radius,
                                                 j * gridSize.y + radius ) );
                    }
                }
            }
        }
    }
}


VECTOR2D GAL::GetGridPoint( VECTOR2D aPoint ) const
{
    VECTOR2D pointWorld = ToWorld( aPoint );

    pointWorld.x = round( pointWorld.x / gridSize.x ) * gridSize.x;
    pointWorld.y = round( pointWorld.y / gridSize.y ) * gridSize.y;

    return ToScreen( pointWorld );
}
