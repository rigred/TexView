
#include <IL/il.h>
#include <wx/stdpaths.h>
#include "common.h"
#include "mainwindow.h"
#include "main.h"


IMPLEMENT_APP(CMain)

wxApp *appHandle = NULL;


// app init
bool CMain::OnInit()
{
 // set app handle
 appHandle = this;
 
 // get executable directory
 const std::string exepath( ::wxStandardPaths::Get().GetExecutablePath() );
 g_datadir = exepath.substr(0, exepath.find_last_of('/') ) + "/data";
 //printf(">>> g_datadir: %s\n", g_datadir.c_str() );
 
 // check devil version
 if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
  const char *errstr = "DevIL version mismatch!";
  printf("%s\n", errstr);
  wxMessageBox(errstr, "Fatal Error", wxOK | wxICON_ERROR);
  return false;
 }
 
 // init png loader for icons
 wxImage::AddHandler(new wxPNGHandler);
 
 // create main window
 CMainWindow *win = new CMainWindow();
 
 // command line arguments
 if (this->argc >= 2) {
  g_startupresize = true;
  MainWindow->OpenFile( argv[1] );
  g_startupresize = false;
 }
 
 // success
 return true;
}


// app exit
int CMain::OnExit()
{
 //
 return 0;
}
