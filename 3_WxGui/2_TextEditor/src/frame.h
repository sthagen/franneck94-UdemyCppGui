#pragma once

#include <wx/wx.h>
#include <wx/filedlg.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

    void OnOpenFile(wxCommandEvent &event);
    void OnSaveFile(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);

private:
    wxTextCtrl *m_textCtrl;
    wxDECLARE_EVENT_TABLE();
};

enum class FileOperations
{
    ID_OpenFile = 1,
    ID_SaveFile,
    ID_Exit
};
