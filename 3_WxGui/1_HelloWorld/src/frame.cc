#include <wx/wx.h>

#include "frame.h"

MyFrame::MyFrame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)) {
    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *button = new wxButton(panel, wxID_ANY, wxT("Click me!"));
    sizer->Add(button, 0, wxALL, 10);
    Connect(button->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnButtonClick));

    panel->SetSizer(sizer);
    Centre();
}

void MyFrame::OnButtonClick(wxCommandEvent &event) {
    wxMessageBox("Hello, wxWidgets!", "Message", wxOK | wxICON_INFORMATION, this);
}
