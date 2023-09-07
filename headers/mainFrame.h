#pragma once

#include "database.h"

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/statline.h>

#include <memory>

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

private:

    /// Variables ///
    std::string search;

    // Main panels
    wxTextCtrl* inputBar;
    wxChoice* choice;

    // Sizers
    wxBoxSizer* bottomSizer;
    wxBoxSizer* scrolledSizer;
    wxScrolled<wxPanel>* scrolledPanel;

    // Database
    std::unique_ptr<Database> db{std::make_unique<Database>()};

    // Functions
    void init();
    void OnTextEnter(wxCommandEvent& event);


    wxDECLARE_EVENT_TABLE();
};

