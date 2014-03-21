
#include <vector>
#include "common.h"
#include "icon.h"


std::vector <wxBitmap*> icons;


// loads icons from file
void LoadIcons()
{
 
}


// returns icon by name
const wxBitmap &Icon(const char *name)
{
 char fname[128];
 sprintf(fname, "%s/%s.png", g_datadir.c_str(), name);
 
 wxBitmap *bm = new wxBitmap;
 if (!bm->LoadFile( fname, wxBITMAP_TYPE_PNG)) {
  
  //char errstr[128];
  //sprintf(errstr, "Icon \"%s\" is missing!", fname);
  //wxMessageBox(errstr, wxOK | wxICON_ERROR);
 }
 return *bm;
}
