
#ifndef DROPTARGET_H
#define DROPTARGET_H

#define wxUSE_DRAG_AND_DROP 1
//#include <wx/wx.h>
#include <wx/dnd.h>


// drag and drop target
class CDropTarget: public wxFileDropTarget
{
private:
 
 bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames);
 
public:
 
 // constructor/destructor
 CDropTarget();
 ~CDropTarget();
 
};

#endif
