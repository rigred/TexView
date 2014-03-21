
#include <wx/wx.h>
#include "mainwindow.h"
#include "droptarget.h"


// constructor
CDropTarget::CDropTarget()
{
 //
}


// drag and drop
bool CDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames)
{
 if (!MainWindow) return false;
 
 // open file
 if (filenames.size() >= 1) {
  MainWindow->OpenFile( filenames[0] );
 }
 
 // accepted
 return true;
}


// destructor
CDropTarget::~CDropTarget()
{
 //
}
