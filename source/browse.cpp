
#include <wx/dir.h>
#include <wx/filename.h>
#include "image.h"
#include "support.h"
#include "mainwindow.h"


// desktop/window managers
enum denv {
 de_unknown = 0,
 de_xfce,
 de_unity,
};

// file managers
enum fman {
 fm_unknown = 0,
 fm_thunar,
 fm_nautilus,
 fm_nemo,
 fm_pcmanfm,
};


// read INI file
bool Read(const char *filename)
{
 // close file
 FILE *fp = fopen(filename, "rb");
 if (!fp) return false;
 
 // read
 
 
 // close file
 fclose(fp);
}


void GetFileManagerSettings()
{
#ifdef WIN32
 
 // todo: get info from registry?
 
#else
 
 // --- detect window manager --------------------------------------------------
 
 // get desktop environment string
 const char *deStr = getenv("XDG_CURRENT_DESKTOP");
 if (!deStr) return;
 printf(">>> wm: %s\n", deStr);
 
 // identify desktop environment
 denv de = de_unknown;
 if (strcasecmp(deStr,"xfce")==0) de = de_xfce;
 if (strcasecmp(deStr,"unity")==0) de = de_unity;
 
 // --- detect file manager ----------------------------------------------------
 
 // XFCE
 if (de == de_xfce) {
  /*
  file: ~/.config/xfce4/helpers.rc
  line: FileManager=
   Thunar
   nautilus
   pcmanfm
   Nemo?
  */
 }
 
 // Thunar
 /*
 file: ~/.config/Thunar/thunarrc
 line: LastSortColumn=
  THUNAR_COLUMN_NAME
  THUNAR_COLUMN_SIZE
  THUNAR_COLUMN_TYPE
  THUNAR_COLUMN_DATE_MODIFIED
 */
 
 #endif
}


// ...
void OpenFolderFile(const wxString &path, const wxString &curfile, int step)
{
 if (path.empty()) return;
 
 //printf(">>> file: %s\n", FixString(fname.GetFullPath()) );
 //printf(">>> path: %s\n", (const char*)path.mb_str() );
 
 // open folder
 wxDir dir( path );
 if (!dir.IsOpened()) return;
 
 // list of supported files in folder
 wxArrayString list;
 
 // loop files
 wxString filespec;
 wxString filename;
 bool gotItem = dir.GetFirst(&filename, filespec, wxDIR_FILES | wxDIR_HIDDEN);
 while (gotItem)
 {
  //printf(">>> %s\n", (const char*)filename.mb_str() );
  
  // FUCK wxWidgets, there's no way to know if the item is a symlink or file!
  // But IsSupportedFile will reject symlinks because they have no extension.
  
  // ignore if not supported extension
  if (IsSupportedFile( filename.mb_str() )) {
   
   // add to list
   list.push_back(path + filename);
  }
  
  // next item
  gotItem = dir.GetNext(&filename);
 }
 
 // sort alphabetically
 list.Sort();
 // todo: sort by size,dimensions,date,type etc
 //GetFileManagerSettings();
 
 /*
 //// temp: display list
 for (int i=0; i<list.GetCount(); i++)
 {
  printf(">>> [%i][%s]\n", i, (const char*)list[i].mb_str() );
 }
 //// temp
 */
 
 // get current file index
 const int curIndex = list.Index(curfile, true, false);
 if (curIndex == wxNOT_FOUND) return;
 
 // next/previous file index
 const int num = list.GetCount();
 int next = curIndex + step;
 if (next < 0) next = num-1;
 if (next >= num) next = 0;
 
 // ignore if nothing changed
 if (next == curIndex) return;
 
 // load next file
 MainWindow->OpenFile( list[next].mb_str() );
}
