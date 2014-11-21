///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_EXPORT_VRML_BASE_H__
#define __DIALOG_EXPORT_VRML_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class DIALOG_SHIM;

#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_EXPORT_3DFILE_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_EXPORT_3DFILE_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		enum
		{
			ID_USE_ABS_PATH = 1000
		};
		
		wxStaticText* m_staticText1;
		wxFilePickerCtrl* m_filePicker;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_SubdirNameCtrl;
		wxCheckBox* m_cbCopyFiles;
		wxCheckBox* m_cbUseRelativePaths;
		wxRadioBox* m_rbSelectUnits;
		wxStaticLine* m_staticline1;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCancelClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOkClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DIALOG_EXPORT_3DFILE_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("VRML Export Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DIALOG_EXPORT_3DFILE_BASE();
	
};

#endif //__DIALOG_EXPORT_VRML_BASE_H__
