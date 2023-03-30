#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title);

private:
    void OnButtonClick(wxCommandEvent &event);
};
