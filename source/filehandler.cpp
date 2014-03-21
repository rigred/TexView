
#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <string.h>
#include "image.h"


// This is the only way to convert wxString to char, but without guarantee.
// wxString::mb_str() is completely unsafe, typically the returned pointer
// becomes invalidated immediately after ::mb_str() returns.
// The code below simply relies on good luck, and only works statistically,
// presumably due to memory temporal coherence. YMMV.
const char *FixString(const wxString &str)
{
 static char buffer[1024];
 int len = str.Len();
 if (len > 0) {
  strncpy(buffer, str.mb_str(), len);
 }
 buffer[len] = '\0';
 return buffer;
}


// shows browse dialog
const char *BrowseFile(wxWindow *parent)
{
 wxString fname;
 const char *strPtr = NULL;
 
 wxStandardPathsBase &stdp = wxStandardPaths::Get();
 
 const wxString filterList = 
  "BMP (*.bmp)|*.bmp|"
  "CUR (*.cur)|*.cur|"
  "DDS (*.dds)|*.dds|"
  "EXR (*.exr)|*.exr|"
  "GIF (*.gif)|*.gif|"
  "HDR (*.hdr)|*.hdr|"
  "ICO (*.ico)|*.ico|"
  "IWI (*.iwi)|*.iwi|"
  "JPG (*.jpg)|*.jpg|"
  "PCX (*.pcx)|*.pcx|"
  "PNG (*.png)|*.png|"
  "PSD (*.psd)|*.psd|"
  "PSP (*.psp)|*.psp|"
  "PXR (*.pxr)|*.pxr|"
  "RAW (*.raw)|*.raw|"
  "TGA (*.tga)|*.tga|"
  "TIF (*.tif)|*.tif|"
  "UTX (*.utx)|*.utx|"
  "WAL (*.wal)|*.wal|"
  "VTF (*.vtf)|*.vtf|"
  "XPM (*.xpm)|*.xpm|"
  "All Files (*.*)|*.*";
 const int filterIndex = 21;
 
 wxString defaultdir = ""; //stdp.GetDataDir(); //stdp.GetDataDir();
 wxString defaultfile = "";
 int flags = wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR;
 
 
 //wxMessageBox(defaultdir);
 
 // create dialog
 wxFileDialog *cdlFile = new wxFileDialog(parent, "Open File", defaultdir, defaultfile, filterList, flags);
 cdlFile->SetFilterIndex( filterIndex );
 
 // show dialog
 int r = cdlFile->ShowModal();
 if (r == wxID_OK) {
  fname = cdlFile->GetFilename();
  //strPtr = fname.mb_str(); // bug: WX utterly fails!
  strPtr = FixString( fname );
  //wxMessageBox( browseFilename );
  //Console.Print( strPtr );
 }
 delete cdlFile;
 
 // return
 return strPtr;
}


/*
 if (fname.IsOk() && fname.FileExists()) {
 fname.GetFullPath().c_str()
*/
