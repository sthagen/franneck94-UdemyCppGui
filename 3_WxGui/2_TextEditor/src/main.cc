#include <wx/wx.h>
#include <wx/filedlg.h>

#include "main.h"
#include "frame.h"

#define WinMain main
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit() {
    MainFrame* frame = new MainFrame("Text Editor");
    frame->Show(true);
    return true;
}
