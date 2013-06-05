/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2011 Jean-Pierre Charras, <jp.charras@wanadoo.fr>
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

/*
 * @file footprint_info.h
 */

#ifndef FOOTPRINT_INFO_H_
#define FOOTPRINT_INFO_H_

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include <kicad_string.h>


class FP_LIB_TABLE;


/*
 * Class FOOTPRINT_INFO
 * is a helper class to handle the list of footprints available in libraries. It stores
 * footprint names, doc and keywords
 */
class FOOTPRINT_INFO
{
public:
    wxString  m_libName;    ///< Name of the library containing this module excluding path and ext.
    wxString  m_libPath;    ///< The full library name and path associated the footprint.
    wxString  m_Module;     ///< Module name.
    int       m_Num;        ///< Order number in the display list.
    wxString  m_Doc;        ///< Footprint description.
    wxString  m_KeyWord;    ///< Footprint key words.
    unsigned  m_padCount;   ///< Number of pads

    FOOTPRINT_INFO()
    {
        m_Num = 0;
        m_padCount = 0;
    }

    const wxString& GetFootprintName() const { return m_Module; }

    void SetLibraryName( const wxString& aLibName ) { m_libName = aLibName; }
    const wxString& GetLibraryName() const { return m_libName; }

    void SetLibraryPath( const wxString& aLibPath ) { m_libPath = aLibPath; }
    const wxString& GetLibraryPath() const { return m_libPath; }
};


class FOOTPRINT_LIST
{
public:
    boost::ptr_vector< FOOTPRINT_INFO > m_List;
    wxString m_filesNotFound;
    wxString m_filesInvalid;

public:

    /**
     * Function GetCount
     * @return the number of items stored in list
     */
    unsigned GetCount() const { return m_List.size(); }

    /**
     * Function GetModuleInfo
     * @return the item stored in list if found
     * @param aFootprintName = the name of item
     */
    FOOTPRINT_INFO * GetModuleInfo( const wxString & aFootprintName )
    {
        BOOST_FOREACH( FOOTPRINT_INFO& footprint, m_List )
        {
            if( aFootprintName.CmpNoCase( footprint.m_Module ) == 0 )
                return &footprint;
        }
        return NULL;
    }

    /**
     * Function GetItem
     * @return the aIdx item in list
     * @param aIdx = index of the given item
     */
    FOOTPRINT_INFO & GetItem( unsigned aIdx )
    {
        return m_List[aIdx];
    }

    /**
     * Function AddItem
     * add aItem in list
     * @param aItem = item to add
     */
    void AddItem( FOOTPRINT_INFO* aItem )
    {
        m_List.push_back( aItem );
    }

    /**
     * Function ReadFootprintFiles
     * Read the list of libraries (*.mod files) and populates m_List ( list of available
     * modules in libs ).
     * for each module, are stored
     *      the module name
     *      documentation string
     *      associated keywords
     *      library name
     * Module description format:
     *   $MODULE c64acmd                    First line of module description
     *   Li c64acmd DIN connector           Library reference
     *   Cd Europe 96 AC male vertical      documentation string
     *   Kw PAD_CONN DIN                    associated keywords
     *   ...... other data (pads, outlines ..)
     *   $Endmodule
     *
     * @param aFootprintsLibNames = an array string giving the list of libraries to load
     */
    bool ReadFootprintFiles( wxArrayString& aFootprintsLibNames );

    bool ReadFootprintFiles( FP_LIB_TABLE& aTable );
};

/// FOOTPRINT object list sort function.
inline bool operator<( const FOOTPRINT_INFO& item1, const FOOTPRINT_INFO& item2 )
{
    return StrNumCmp( item1.m_Module, item2.m_Module, INT_MAX, true ) < 0;
}

#endif  // FOOTPRINT_INFO_H_
