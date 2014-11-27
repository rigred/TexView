
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/artprov.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/filename.h>
#include "icon.h"
#include "image.h"
#include "scene.h"
#include "browse.h"
#include "common.h"
#include "filehandler.h"
#include "mainwindow.h"


// menu ID enums
enum {
 mnuNone = 0,
 
 // file
 mnuFileOpen,
 mnuFileReload,
 mnuFileClose,
 mnuFileExit,
 
 // view
 mnuViewZoomIn,
 mnuViewZoomOut,
 mnuViewZoomSmooth,
 
 mnuViewAlpha,
 mnuViewFilter,
 mnuViewLighting,
 mnuViewTile,
 mnuViewGrid,
 mnuViewRamp,
 mnuViewInvert,
 mnuViewInfo,
 mnuViewColorR,
 mnuViewColorG,
 mnuViewColorB,
 mnuViewColorA,
 
 mnuViewRotCW,
 mnuViewRotCCW,
 mnuViewMirrorX,
 mnuViewMirrorY,
 
 mnuViewPrev,
 mnuViewNext,
 
 mnuViewAlphaNone,
 mnuViewAlphaBlend,
 mnuViewAlphaTest,
 
 mnuViewZoomReset,
 mnuViewZoomAuto,
 
 // help
 mnuHelpAbout,
};


// event table
BEGIN_EVENT_TABLE( CMainWindow, wxFrame )
 EVT_SIZE( CMainWindow::OnResize )
 EVT_ACTIVATE( CMainWindow::OnActivate )
 EVT_MENU( wxID_ANY, CMainWindow::OnMenuClick )
 EVT_UPDATE_UI( wxID_ANY, CMainWindow::OnMenuUpdate )
 EVT_CLOSE( CMainWindow::OnCloseWindow )
 EVT_COMBOBOX( wxID_ANY, CMainWindow::OnComboBox )
END_EVENT_TABLE()


CMainWindow *MainWindow = NULL;


// creates the main window
CMainWindow::CMainWindow() : wxFrame(NULL, wxID_ANY, g_appname, wxDefaultPosition, wxSize(700, 500))
{
 MainWindow = this;
 viewport = NULL;
 cbbZoom = NULL;
 
 // set minimum size
 SetMinSize( wxSize(400,300) );
 
 // center window and maximize
 Centre();
 //Maximize();
 
 // set drag and drop target
 dropTgt = new CDropTarget();
 SetDropTarget( dropTgt );
 
 // --- menu -------------------------------------------------------------------
 
 // create file menu
 wxMenu *mnuFile = new wxMenu();
 mnuFile->Append( mnuFileOpen, "&Open...", "Open file");
 mnuFile->Append( mnuFileClose, "&Close", "Close file");
 mnuFile->AppendSeparator();
 mnuFile->Append( mnuFileExit, "&Exit", "Close " + g_appname);
 
 // create view menu
 wxMenu *mnuView = new wxMenu();
 mnuView->Append( mnuViewFilter, "&Filter", "Toggles interpolation", wxITEM_CHECK);
 //mnuView->Append( mnuViewTile, "&Tile", "Toggles image tiling", wxITEM_CHECK);
 mnuView->Append( mnuViewGrid, "&Grid", "Toggles pixel grid", wxITEM_CHECK);
 mnuView->Append( mnuViewRamp, "&Ramp", "Toggles intensity ramp", wxITEM_CHECK);
 mnuView->Append( mnuViewInvert, "&Invert", "Invert Image", wxITEM_CHECK);
 
 // create view > alpha sub menu
 wxMenu *mnuViewAlpha = new wxMenu();
 mnuViewAlpha->Append( mnuViewAlphaNone, "&None", "No transparancy", wxITEM_CHECK);
 mnuViewAlpha->Append( mnuViewAlphaBlend, "&Alpha Blend", "Alpha blended transparancy", wxITEM_CHECK);
 mnuViewAlpha->Append( mnuViewAlphaTest, "&Alpha Test", "Alpha tested transparancy", wxITEM_CHECK);
 mnuView->AppendSubMenu( mnuViewAlpha, "&Alpha Mode", "Select alpha mode");
 
 // create help menu
 wxMenu *mnuHelp = new wxMenu();
 mnuHelp->Append( mnuHelpAbout, "&About...", "Shows information about this software");
 
 // create menu bar
 wxMenuBar *menubar = new wxMenuBar();
 menubar->Append( mnuFile, "&File");
 menubar->Append( mnuView, "&View");
 menubar->Append( mnuHelp, "&Help");
 
 // add menu bar to window
 SetMenuBar(menubar);
 
 // --- toolbar ----------------------------------------------------------------
 
 {
  // create toolbar
  wxToolBar *toolbar = CreateToolBar();
  //toolbar->SetMargins( wxSize(2,2) );
  //toolbar->SetToolPacking(2);
  //toolbar->SetToolSeparation(2);
  toolbar->SetToolBitmapSize( wxSize(16,16) );
  
  // add buttons
  //toolbar->AddTool(mnuFileNew, "New", bmFileNew, wxNullBitmap, wxITEM_NORMAL, "Create new workspace file");
  toolbar->AddTool(mnuFileOpen, "Open", Icon("file_open"), wxNullBitmap, wxITEM_NORMAL, "Open workspace file");
  //toolbar->AddTool(mnuFileSave, "Save", bmFileSave, wxNullBitmap, wxITEM_NORMAL, "Save workspace file");
  toolbar->AddSeparator();
  
  const int znum = 6;
  const wxString zstr[znum] = { "25%", "50%", "100%", "200%", "400%", "800%" };
  cbbZoom = new wxComboBox(toolbar, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, znum, zstr, wxCB_DROPDOWN);
  toolbar->AddControl( cbbZoom, "Zoom" );
  toolbar->AddTool(mnuViewZoomIn, "Zoom In", Icon("zoomin"), wxNullBitmap, wxITEM_NORMAL, "Zoom in");
  toolbar->AddTool(mnuViewZoomOut, "Zoom Out", Icon("zoomout"), wxNullBitmap, wxITEM_NORMAL, "Zoom out");
  
  toolbar->AddSeparator();
  toolbar->AddTool(mnuViewInfo, "Info", Icon("info"), wxNullBitmap, wxITEM_NORMAL, "Displays image info");
  toolbar->AddTool(mnuViewPrev, "Previous", Icon("prev"), wxNullBitmap, wxITEM_NORMAL, "Loads previous image in folder");
  toolbar->AddTool(mnuFileReload, "Reload", Icon("refresh"), wxNullBitmap, wxITEM_NORMAL, "Reloads image from disk");
  toolbar->AddTool(mnuViewNext, "Next", Icon("next"), wxNullBitmap, wxITEM_NORMAL, "Loads next image in folder");
  
  toolbar->AddSeparator();
  toolbar->AddTool(mnuViewColorR, "Red"  , Icon("color_r"), wxNullBitmap, wxITEM_CHECK, "Show red channel");
  toolbar->AddTool(mnuViewColorG, "Green", Icon("color_g"), wxNullBitmap, wxITEM_CHECK, "Show green channel");
  toolbar->AddTool(mnuViewColorB, "Blue" , Icon("color_b"), wxNullBitmap, wxITEM_CHECK, "Show blue channel");
  toolbar->AddTool(mnuViewColorA, "Alpha", Icon("color_a"), wxNullBitmap, wxITEM_CHECK, "Show alpha channel");
  toolbar->AddSeparator();
  //toolbar->AddTool(mnuViewLighting, "Lighting", bmViewLighting, wxNullBitmap, wxITEM_CHECK, "Toggle Lighting");
  //toolbar->AddTool(mnuViewTexture, "Texture", bmViewTexture, wxNullBitmap, wxITEM_CHECK, "Toggle Texture");
  toolbar->AddTool(mnuViewFilter, "Filter", Icon("filter"), wxNullBitmap, wxITEM_CHECK, "Toggle interpolation");
  toolbar->AddTool(mnuViewGrid, "Grid", Icon("grid"), wxNullBitmap, wxITEM_CHECK, "Show Pixel Grid");
  toolbar->AddTool(mnuViewRamp, "Ramp", Icon("ramp"), wxNullBitmap, wxITEM_CHECK, "Show Color Ramp");
  toolbar->AddTool(mnuViewInvert, "Invert", Icon("invert"), wxNullBitmap, wxITEM_CHECK, "Invert Channels");
  toolbar->AddSeparator();
  toolbar->AddTool(mnuViewRotCCW, "Rotate -90", Icon("rotccw"), wxNullBitmap, wxITEM_NORMAL, "Rotate Counter clock-wise");
  toolbar->AddTool(mnuViewRotCW, "Rotate +90", Icon("rotcw"), wxNullBitmap, wxITEM_NORMAL, "Rotate Clock-wise");
  toolbar->AddTool(mnuViewMirrorX, "Mirror Horizontally", Icon("mirrorx"), wxNullBitmap, wxITEM_CHECK, "Mirror Horizontally");
  toolbar->AddTool(mnuViewMirrorY, "Mirror Vertically", Icon("mirrory"), wxNullBitmap, wxITEM_CHECK, "Mirror Vertically");
  toolbar->AddSeparator();
  toolbar->AddTool(mnuViewZoomReset, "Reset Zoom", Icon("zoomreset"), wxNullBitmap, wxITEM_NORMAL, "Reset zoom");
  toolbar->AddTool(mnuViewZoomAuto, "Zoom to fit", Icon("zoomfit"), wxNullBitmap, wxITEM_NORMAL, "Zoom to fit");
  
  // refresh
  toolbar->Realize();
 }
 
 // --- viewport ---------------------------------------------------------------
 
 // bug workaround: window must be visible or wxGL chokes on the big one
 Show();
 wxYield();
 
 viewport = new CViewport(this);
 viewport->SetFocus();
 viewport->Hide();
 
 // --- create error message ---------------------------------------------------
 
 errPanel = new wxPanel(this, wxID_ANY, wxPoint(0,0), wxSize(400,100) );
 //errPanel->CenterOnParent();
 
 errText = new wxStaticText( errPanel, wxID_ANY, "<error message>" );
 
 wxBitmap icon = wxArtProvider::GetBitmap(wxART_WARNING, wxART_MESSAGE_BOX, wxDefaultSize );
 errIcon = new wxStaticBitmap( errPanel, wxID_ANY, icon, wxPoint(0,0), wxDefaultSize );
 
 // wxSizers are full of shit!! We'll do it manually...
 //wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
 //box->Add( errIcon, 1, wxALIGN_CENTRE_HORIZONTAL | wxBOTTOM | wxALL, 20 );
 //box->Add( errText, 1, wxALIGN_CENTRE_HORIZONTAL | wxTOP | wxALL, 20 );
 //errPanel->SetSizer(box);
 
 errPanel->Hide();
 
 // --- status bar -------------------------------------------------------------
 
 int w[10] = { 200, // 0  status
               100, // 1  image dimensions
               100, // 2  format
                50, // 3  zoom
               100, // 4  cursor X,Y
               100, // 5  cursor U,V
                50, // 6  RGBA (byte range)
                50, // 7  RGBA (float range)
                50, // 8  color HEX
                -1
              };
 
 CreateStatusBar(1);
 stsStatus = GetStatusBar();
 stsStatus->SetFieldsCount(10, w);
 
 // --- misc -------------------------------------------------------------------
 
 // show window
 Show();
 
 // attempt to trigger resize event
// SetSize( GetSize().x, GetSize().y+1 );
 // Doesn't work, I guess because window is invisible, despite being 'wxShown',
 // (aka 'fuck you, I will remain invisible until I care to fucking show up some day').
 // And while we're at it, XFCE and X11 suck dick. What kind of bullshit UI system
 // can't guarantee a window to become visible when you call the designated API
 // function designed for that specific purpose!!!
 // wxWidgets + XFCE + X11 = clusterfuck!!
}


// centers error message, wxWidgets sizers suck to much
void CMainWindow::ResizeStuff()
{
 errPanel->SetSize( GetClientRect() );
 
 wxSize s = errPanel->GetSize();
 
 int totw = errIcon->GetSize().x + 20 + errText->GetSize().x;
 
 int ctrX = s.x / 2;
 int ctrY = s.y / 2;
 
 errIcon->SetPosition( wxPoint(ctrX - (totw/2) , ctrY - errIcon->GetSize().y/2 ) );
 errText->SetPosition( wxPoint(ctrX - (totw/2) + errIcon->GetSize().x+20, ctrY - errText->GetSize().y/2 ) );
}


// viewport resize event
void CMainWindow::OnResize(wxSizeEvent &evt)
{
 //if (!g_startupresize) {
  
  if (viewport) {
   viewport->SetSize( GetClientRect() );
  }
  
  if (errPanel) {
   ResizeStuff();
  }
  
 //}
 
 UpdateViewport();
}


// toggles boolean
void Toggle(bool &v)
{
 v = !v;
}


// opens file
bool CMainWindow::OpenFile(const char *filename)
{
 if (!filename) return false;
 
 //printf(">>> OpenFile %s\n", filename);
 
 // close file
 CloseFile();
 
 // set current file/folder path strings
 wxFileName fname( filename );
 fname.Normalize(wxPATH_NORM_ALL);
 curpath = fname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR, wxPATH_NATIVE);
 curfile = fname.GetFullName();
 curfilepath = curpath + curfile;
 
 //printf(">>> curpath: %s\n", (const char*)curpath.mb_str() );
 //printf(">>> curfile: %s\n", (const char*)curfile.mb_str() );
 
 // set title
 SetTitle(g_appname + " - [" + curfilepath + "]");
 
 // load file
 SetStatusText("Loading file...", 0);
 if (!Image.Load( curfilepath.mb_str() )) {
  // todo: display warning in viewport
  
  wxString str;
  str += "Failed to load \"";
  str += curfilepath;
  str += "\".";
  str += "\n\n";
  str += g_errmsg;
  
  errText->SetLabel( str );
  ResizeStuff();
  errPanel->Show();
  viewport->Hide();
  SetStatusText("", 0);
  
  return false;
 }
 SetStatusText("", 0);
 
 // don't reset zoom if size is the same
 bool resetZoom = true;
 if ((curwidth == Image.width) && (curheight == Image.height)) resetZoom = false;
 
 // remember size
 curwidth = Image.width;
 curheight = Image.height;
 
 // size
 char str[64];
 sprintf(str, "%ix%i", curwidth, curheight);
 SetStatusText(str, 1);

 // format
 switch (Image.frags) {
 case 1: SetStatusText("Grayscale", 2); break;
 case 2: SetStatusText("Grayscale Alpha", 2); break;
 case 3: SetStatusText("RGB", 2); break;
 case 4: SetStatusText("RGBA", 2); break;
 }
 
 // resize window to fit image
 if (g_startupresize) {

  // check if image is bigger than viewport
  const int dx = g_width - curwidth;
  const int dy = g_height - curheight;

  //printf(">>> g_size %i,%i\n", g_width, g_height);
  //printf(">>> autosize %i,%i\n", dx, dy);

  if (dx < 0 || dy < 0) {
   
   // get viewport size
   wxSize s = GetSize();
   //printf(">>> getsize %i,%i\n", s.x, s.y);
   
   // adjust size and add some padding
   s.x += -dx+20;
   s.y += -dy+20;
   
   // get desktop size
   int dw,dh;
   ::wxDisplaySize(&dw,&dh);
   //printf(">>> wxDisplaySize %i,%i\n", dw, dh);
   
   // maximize if nearly fullscreen
   if ((s.x > dw-100) || (s.y > dh-100)) {
    Maximize();
   } else {
    
    // resize
    SetSize(s);
    Centre();
   }
   
  }
 }
 
 // reset zoom
 if (resetZoom) {
  ResetZoom();
 }
 
 // spank UI
 viewport->Show();
 UpdateUI();
 UpdateViewport();
 
 // auto-zoom
 //viewport->AutoZoom(); // wx just doesn't care do do shit
 viewport->autoZoomOnFirstRedraw = true;
 
 // success
 return true;
}


// reloads file
void CMainWindow::ReloadFile()
{
 if (!Image.Reload()) {
  CloseFile();
 }
}


// closes file
void CMainWindow::CloseFile()
{
 Image.Unload();
 
 //SetTitle(g_appname);
 SetStatusText("", 0);
 SetStatusText("", 1);
 SetStatusText("", 2);
 SetStatusText("", 3);
 
 //ResetZoom();
 UpdateUI();
 UpdateViewport();
 
 // hide error panel
 errPanel->Hide();
 //viewport->Hide();
}


// selects color channel to display
void SelChan(int n)
{
 if (g_colormode == n) {
  g_colormode = 0;
 } else {
  g_colormode = n;
 }
}


// when menu item is clicked
void CMainWindow::OnMenuClick(wxCommandEvent &evt)
{
 switch (evt.GetId())
 {
  // file
  case mnuFileOpen:
   {
    const char *fname = BrowseFile( this );
    if (fname) OpenFile( fname );
   }
   break;
   
  case mnuFileReload:
   ReloadFile();
   break;
   
  case mnuFileClose:
   CloseFile();
   viewport->Hide();
   SetTitle(g_appname);
   break;
   
  case mnuFileExit:
   Close(true);
   break;
   
  // view
  case mnuViewZoomIn:
   Zoom( 2.0, false );
   break;
  case mnuViewZoomOut:
   Zoom( 0.5, false );
   break;
  case mnuViewZoomReset:
   ResetZoom();
   //g_zoom = 1;
   //g_panx = 0;
   //g_pany = 0;
   break;
  case mnuViewZoomAuto:
   viewport->AutoZoom(true);
   break;
  case mnuViewPrev:
   OpenFolderFile(curpath, curfilepath, -1);
   break;
  case mnuViewNext:
   OpenFolderFile(curpath, curfilepath, 1);
   break;
  case mnuViewFilter:
   Toggle( g_filter );
   Image.Bind();
   Image.Refresh();
   Image.Unbind();
   break;
  case mnuViewGrid:
   Toggle( g_grid );
   break;
  case mnuViewRamp:
   Toggle( g_ramp );
   break;
  case mnuViewInvert:
   Toggle( g_invert );
   break;
  case mnuViewTile:
   Toggle( g_tilex );
   Toggle( g_tiley );
   break;
  
  // view > alpha
  case mnuViewAlphaNone:
   g_alphamode = 0;
   break;
  case mnuViewAlphaTest:
   g_alphamode = 1;
   break;
  case mnuViewAlphaBlend:
   g_alphamode = 2;
   break;
   
  case mnuViewColorR: SelChan(1); break;
  case mnuViewColorG: SelChan(2); break;
  case mnuViewColorB: SelChan(3); break;
  case mnuViewColorA: SelChan(4); break;
  
  case mnuViewRotCW:  g_rot += 90; break;
  case mnuViewRotCCW: g_rot -= 90; break;
  
  case mnuViewMirrorX: g_mirrorx = !g_mirrorx; break;
  case mnuViewMirrorY: g_mirrory = !g_mirrory; break;
  
  // help
  case mnuHelpAbout:
   wxMessageBox(g_appname + "\nMartijn Buijs, 2013\nwww.bytehazard.com", "About", wxOK | wxICON_INFORMATION );
   break;
 }
 UpdateUI();
 UpdateViewport();
}


// when menu item is evaluated
void CMainWindow::OnMenuUpdate(wxUpdateUIEvent &evt)
{
 switch (evt.GetId())
 {
  case mnuViewFilter: evt.Check( g_filter ); break;
  case mnuViewTile:   evt.Check( g_tilex  ); break;
  case mnuViewGrid:   evt.Check( g_grid   ); break;
  case mnuViewRamp:   evt.Check( g_ramp   ); break;
  case mnuViewInvert: evt.Check( g_invert ); break;
  
  case mnuViewColorR: evt.Check( g_colormode==1 ); break;
  case mnuViewColorG: evt.Check( g_colormode==2 ); break;
  case mnuViewColorB: evt.Check( g_colormode==3 ); break;
  case mnuViewColorA: evt.Check( g_colormode==4 ); break;
  
  case mnuViewAlphaNone : evt.Check( g_alphamode == 0 ); break;
  case mnuViewAlphaTest : evt.Check( g_alphamode == 1 ); break;
  case mnuViewAlphaBlend: evt.Check( g_alphamode == 2 ); break;
  
  case mnuViewMirrorX: evt.Check( g_mirrorx ); break;
  case mnuViewMirrorY: evt.Check( g_mirrory ); break;
 }
}


// combo box event
void CMainWindow::OnComboBox(wxCommandEvent &evt)
{
 assert(cbbZoom != NULL);
 //wxMessageBox( cbbZoom->GetValue() );
 
 float tgt = (float)atoi( cbbZoom->GetValue().mb_str() ) / 100.0;
 Zoom(tgt);
 
 UpdateUI();
 UpdateViewport();
}


// focus change event
void CMainWindow::OnActivate(wxActivateEvent &evt)
{
 if (viewport) {
  viewport->SetActive( evt.GetActive() );
 }
 evt.Skip();
}


// redraws viewport
void CMainWindow::UpdateViewport()
{
 if (!viewport) return;
 viewport->Refresh(false);
}


// updates UI elements
void CMainWindow::UpdateUI()
{
 // bugfix
 if (!MainWindow) return;
 
 // update shader params
 if (viewport) {
  viewport->UpdateShaderUniforms();
 }
 
 // update zoom dropdown
 if (cbbZoom) {
  if (Image.loaded) {
   char str[32];
   sprintf(str,"%i%%", (int)(g_zoom*100));
   cbbZoom->SetStringSelection(str);
  } else {
   cbbZoom->SetStringSelection("");
  }
 }
 
 // update statusbar
 if (stsStatus) {
  if (Image.loaded) {
   char str[32];
   
   // zoom level
   sprintf(str,"%i%%", (int)(g_zoom*100));
   stsStatus->SetStatusText(str, 3);
   
   // get viewport mouse coordinates
   const int px = viewport->GetMouseX();
   const int py = viewport->GetMouseY();
   
   // project to pixel coordinates
   const float2 pc = TFi( float2(px,py) );
   
   // convert to UV space
   const float2 uv = float2( pc.x/(float)Image.width, pc.y/(float)Image.height );
   
   // mouse coords
   sprintf(str,"XY %i,%i", (int)pc.x, (int)pc.y );
   stsStatus->SetStatusText(str, 4 );
   
   // UV coords
   sprintf(str,"UV %.3f,%.3f", uv.x, 1.0-uv.y );
   stsStatus->SetStatusText(str, 5 );
   
  } else {
   
   // clear statusbar
   for (int i=0; i<10; i++)
   {
    stsStatus->SetStatusText("", i);
   }
   
  }
 }
 
}


// close window
void CMainWindow::OnCloseWindow(wxCloseEvent &evt)
{
 Destroy();
 MainWindow = NULL;
}
