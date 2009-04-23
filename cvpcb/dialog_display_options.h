/////////////////////////////////////////////////////////////////////////////
// Name:        dialog_display_options.h
// Purpose:     
// Author:      jean-pierre Charras
// Modified by: 
// Created:     17/02/2006 17:47:55
// RCS-ID:      
// Copyright:   License GNU
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 17/02/2006 17:47:55

#ifndef _DIALOG_DISPLAY_OPTIONS_H_
#define _DIALOG_DISPLAY_OPTIONS_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dialog_display_options.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/valgen.h"
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG 10000
#define EDGE_SELECT 10001
#define TEXT_SELECT 10002
#define PADFILL_OPT 10003
#define PADNUM_OPT 10004
#define ID_STATICLINE1 10006
#define SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_STYLE wxDEFAULT_DIALOG_STYLE|MAYBE_RESIZE_BORDER
#define SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_TITLE _("Display Options")
#define SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_IDNAME ID_DIALOG
#define SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_SIZE wxSize(400, 300)
#define SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * WinEDA_FootprintDisplayOptionsFrame class declaration
 */

class WinEDA_FootprintDisplayOptionsFrame: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WinEDA_FootprintDisplayOptionsFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WinEDA_FootprintDisplayOptionsFrame( );
    WinEDA_FootprintDisplayOptionsFrame( WinEDA_BasePcbFrame* parent, wxWindowID id = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_IDNAME, const wxString& caption = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_TITLE, const wxPoint& pos = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_POSITION, const wxSize& size = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_SIZE, long style = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_IDNAME, const wxString& caption = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_TITLE, const wxPoint& pos = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_POSITION, const wxSize& size = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_SIZE, long style = SYMBOL_WINEDA_FOOTPRINTDISPLAYOPTIONSFRAME_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin WinEDA_FootprintDisplayOptionsFrame event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
    void OnApplyClick( wxCommandEvent& event );

////@end WinEDA_FootprintDisplayOptionsFrame event handler declarations

////@begin WinEDA_FootprintDisplayOptionsFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WinEDA_FootprintDisplayOptionsFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	void UpdateObjectSettings(void);
	
////@begin WinEDA_FootprintDisplayOptionsFrame member variables
    wxBoxSizer* OuterBoxSizer;
    wxBoxSizer* MainBoxSizer;
    wxRadioBox* m_EdgesDisplayOption;
    wxRadioBox* m_TextDisplayOption;
    wxBoxSizer* ColumnBoxSizer;
    wxCheckBox* m_IsShowPadFill;
    wxCheckBox* m_IsShowPadNum;
    wxButton* m_CancelButton;
////@end WinEDA_FootprintDisplayOptionsFrame member variables

	WinEDA_BasePcbFrame * m_Parent;

};

#endif
    // _DIALOG_DISPLAY_OPTIONS_H_
