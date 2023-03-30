#include <wx/wx.h>

#include "main.h"
#include "frame.h"

#define WinMain main
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    auto *frame = new MyFrame(wxT("Simple"));
    frame->Show(true);

    return true;
}
