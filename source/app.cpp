#include "../headers/app.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Library");
    frame->SetClientSize(800,600);
    frame->Center();
    frame->Show(true);

    return true;
}
