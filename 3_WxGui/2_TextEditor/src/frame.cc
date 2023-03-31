#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/menu.h>
#include <wx/menuitem.h>
#include <wx/textctrl.h>
#include <wx/filedlg.h>

#include "frame.h"

MainFrame::MainFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    wxMenu *fileMenu = new wxMenu("FileMenu");
    fileMenu->Append(static_cast<int>(FileOperations::ID_OpenFile), "&Open...\tCtrl-O", "Open a file");
    fileMenu->Append(static_cast<int>(FileOperations::ID_SaveFile), "&Save...\tCtrl-S", "Save the file");
    fileMenu->AppendSeparator();
    fileMenu->Append(static_cast<int>(FileOperations::ID_Exit), "E&xit\tAlt-X", "Quit the application");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    m_textCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
}

void MainFrame::OnOpenFile(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this,
                                _("Open file"),
                                "",
                                "",
                                "Text files (*.txt)|*.txt|All files (*.*)|*.*",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxFileInputStream input(openFileDialog.GetPath());

    if (!input.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }

    m_textCtrl->LoadFile(openFileDialog.GetPath());
}

void MainFrame::OnSaveFile(wxCommandEvent &event)
{
    wxFileDialog saveFileDialog(this,
                                _("Save file"),
                                "",
                                "",
                                "Text files (*.txt)|*.txt|All files (*.*)|*.*",
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxFileOutputStream output(saveFileDialog.GetPath());

    if (!output.IsOk())
    {
        wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        return;
    }

    m_textCtrl->SaveFile(saveFileDialog.GetPath());
}

void MainFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame) \
    EVT_MENU(static_cast<int>(FileOperations::ID_OpenFile), MainFrame::OnOpenFile) \
    EVT_MENU(static_cast<int>(FileOperations::ID_SaveFile), MainFrame::OnSaveFile) \
    EVT_MENU(static_cast<int>(FileOperations::ID_Exit), MainFrame::OnExit) \
wxEND_EVENT_TABLE()
