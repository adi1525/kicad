///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_PLOT_BASE_H__
#define __DIALOG_PLOT_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checklst.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_PLOT_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_PLOT_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		enum
		{
			ID_ALLOW_PRINT_PAD_ON_SILKSCREEN = 1000,
			ID_PRINT_REF,
			ID_MIROR_OPT,
			ID_CREATE_DRILL_FILE
		};
		
		wxBoxSizer* m_MainSizer;
		wxStaticText* m_staticText121;
		wxChoice* m_plotFormatOpt;
		wxStaticText* m_staticTextDir;
		wxTextCtrl* m_outputDirectoryName;
		wxButton* m_browseButton;
		wxStaticBoxSizer* m_LayersSizer;
		wxCheckListBox* m_layerCheckListBox;
		wxBoxSizer* m_PlotOptionsSizer;
		wxCheckBox* m_plotSheetRef;
		wxCheckBox* m_plotPads_on_Silkscreen;
		wxCheckBox* m_plotModuleValueOpt;
		wxCheckBox* m_plotModuleRefOpt;
		wxCheckBox* m_plotTextOther;
		wxCheckBox* m_plotInvisibleText;
		wxCheckBox* m_plotNoViaOnMaskOpt;
		wxCheckBox* m_excludeEdgeLayerOpt;
		wxCheckBox* m_plotMirrorOpt;
		wxCheckBox* m_plotPSNegativeOpt;
		wxStaticText* m_staticText11;
		wxChoice* m_drillShapeOpt;
		wxStaticText* m_staticText12;
		wxChoice* m_scaleOpt;
		wxStaticText* m_staticText13;
		wxChoice* m_plotModeOpt;
		wxStaticText* m_textDefaultPenSize;
		wxTextCtrl* m_linesWidth;
		wxStaticText* m_SolderMaskMarginLabel;
		wxStaticText* m_SolderMaskMarginCurrValue;
		wxStaticText* m_solderMaskMinWidthLabel;
		wxStaticText* m_SolderMaskMinWidthCurrValue;
		wxStaticBoxSizer* m_GerberOptionsSizer;
		wxCheckBox* m_useGerberExtensions;
		wxCheckBox* m_subtractMaskFromSilk;
		wxCheckBox* m_useAuxOriginCheckBox;
		wxStaticBoxSizer* m_HPGLOptionsSizer;
		wxStaticText* m_textPenSize;
		wxTextCtrl* m_HPGLPenSizeOpt;
		wxStaticText* m_textPenOvr;
		wxTextCtrl* m_HPGLPenOverlayOpt;
		wxStaticBoxSizer* m_PSOptionsSizer;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_fineAdjustXscaleOpt;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_fineAdjustYscaleOpt;
		wxStaticText* m_textPSFineAdjustWidth;
		wxTextCtrl* m_PSFineAdjustWidthOpt;
		wxCheckBox* m_forcePSA4OutputOpt;
		wxTextCtrl* m_messagesBox;
		wxButton* m_plotButton;
		wxButton* m_buttonDrill;
		wxButton* m_buttonQuit;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void SetPlotFormat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOutputDirectoryBrowseClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetScaleOpt( wxCommandEvent& event ) { event.Skip(); }
		virtual void Plot( wxCommandEvent& event ) { event.Skip(); }
		virtual void CreateDrillFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DIALOG_PLOT_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Plot"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DIALOG_PLOT_BASE();
	
};

#endif //__DIALOG_PLOT_BASE_H__
