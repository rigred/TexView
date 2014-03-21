
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "droptarget.h"
#include <wx/wx.h>
#include "viewport.h"


// main window class
class CMainWindow: public wxFrame
{
private:
 
 DECLARE_EVENT_TABLE()
 
 CViewport *viewport;
 wxStatusBar *stsStatus;
 wxComboBox *cbbZoom;
 CDropTarget *dropTgt;
 
 wxPanel *errPanel;
 wxStaticBitmap *errIcon;
 wxStaticText *errText;
 
 wxString curpath;         // full current path
 wxString curfile;         // current file+ext
 wxString curfilepath;     // curpath+curfile
 int curwidth;
 int curheight;
 
 void OnMenuClick(wxCommandEvent &evt);
 void OnMenuUpdate(wxUpdateUIEvent &evt);
 void OnActivate(wxActivateEvent &evt);
 void OnCloseWindow(wxCloseEvent &evt);
 void OnComboBox(wxCommandEvent &evt);
 void OnResize(wxSizeEvent &evt);
 
 void ResizeStuff();
 
public:
 
 CMainWindow();
 
 void UpdateViewport();
 void UpdateUI();
 
 bool OpenFile(const char *filename);
 void ReloadFile();
 void CloseFile();
 
};


extern CMainWindow *MainWindow;


#endif
