#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame* mainFrame = new MainFrame("Scheduling Simulator");

	mainFrame->SetClientSize(925, 600);

	mainFrame->Center();

	mainFrame->Show();

	return true;
}