
#include "math.h" //// temp
#include "main.h"
#include "scene.h"
#include "image.h"
#include "opengl.h"
#include "common.h"
#include "mainwindow.h"
#include "viewport.h"
#include "shader.h"


BEGIN_EVENT_TABLE(CViewport, wxGLCanvas)
 EVT_PAINT(CViewport::OnPaint)
 EVT_SIZE(CViewport::OnResize)
 EVT_MOUSE_EVENTS(CViewport::OnMouse)
 EVT_KEY_DOWN(CViewport::OnKeyDown)
 EVT_KEY_UP(CViewport::OnKeyUp)
END_EVENT_TABLE()


const int glconf[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };


// constructor
CViewport::CViewport(wxWindow *parent) : 
           wxGLCanvas(parent, wxID_ANY, glconf, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
 //printf(">>> CViewport::CViewport()\n");
 active = true;
 glcontext = NULL;
 shader = NULL;
 
 drag = false;
 dragx = 0;
 dragy = 0;
 mousex = 0;
 mousey = 0;
 
 autoZoomOnFirstRedraw = false;
 
 //SetBackgroundColour( wxSystemSettingsNative::GetColour(wxSYS_COLOUR_APPWORKSPACE) );
 SetBackgroundColour( wxColor(g_bkg.r*255, g_bkg.g*255, g_bkg.b*255, 0.0) );
 
 // ...
 Init();
}


// delay this shit, or wxWidgets will unnecissarily raise Win32 errors due to
// XFCE fuckup (show window != window shown), and probably also some X11 shittyness.
void CViewport::Init()
{
 if (glcontext) return;
 
 // create OpenGL context
 glcontext = new wxGLContext(this, NULL);
 assert(glcontext != NULL);
 
 // make GL context current
 this->SetCurrent(*glcontext);
 
 // initialize GLEW
 if (glewInit() != GLEW_OK) {
  wxMessageBox( "GLEW initialization failed.", "CViewport", wxOK | wxICON_ERROR);
  printf(">>> GLEW init FAILED!\n");
 }
 
 // set default OpenGL states
 glEnable(GL_CULL_FACE);
 
 // get hardware capabilities
 glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat*)&g_maxaniso);
 
 // load shader
 char vertfile[128];
 char fragfile[128];
 sprintf(vertfile, "%s/vert.glsl", g_datadir.c_str() );
 sprintf(fragfile, "%s/frag.glsl", g_datadir.c_str() );
 shader = LoadShader(vertfile, fragfile);
 shader->Bind();
 //printf(">>> CViewport::CViewport() OK\n");
}


// updates shader uniforms
void CViewport::UpdateShaderUniforms()
{
 if (!shader) return;
 shader->SetUniform1i("colorMode", g_colormode );
 shader->SetUniform1i("alphaMode", g_alphamode );
 shader->SetUniform1i("invertMode", g_invert );
 shader->SetUniform2f("imageSize", Image.width, Image.height );
 shader->SetUniform3fv("gridColor", g_gridColor );
 shader->SetUniform3fv("checkColor1", g_checkColor1 );
 shader->SetUniform3fv("checkColor2", g_checkColor2 );
 shader->SetUniform1f("checkSize", (1.0 / (float)g_checkSize) );
 shader->SetUniform1i("showGrid", g_grid );
 shader->SetUniform1i("showRamp", g_ramp );
}


// viewport resize event
void CViewport::OnResize(wxSizeEvent &evt)
{
 int w,h;
 GetClientSize(&w,&h);
 
 // ignore if smaller than 1px
 if (w < 1 || h < 1) return;
 
 // nothing changes
 if (w == g_width && h == g_height) return;
 
 // update viewport size
 g_width = w;
 g_height = h;
 
 //printf(">>> resize %i,%i\n", g_width, g_height);
 
 // workaround for all kinds of wxShit
 if (autoZoomOnFirstRedraw) {
  
  // workaround for workaround
  // It appears we are resizing an absurd number of times at startup, and thus
  // our workaround won't actually work. So we add some hueristics to try and
  // figure out if the window is visible and we've resized to a more or less
  // final window size yet. No doubt unreliable and probably breaks other shit.
  // But that's life in wxLand.
  if (g_width > 200 && g_height > 200) {
   
   // workaround for workaround for workaround
   // Ok, turns out that doesn't work well enough. wxWidgets makes windows show first,
   // then later, if the window system is actually paying attention, maximizes it.
   // Sort of. Maybe. So we'll need some more heuristics to detect what state this
   // elusive invisible window is in. Let's pretend wxWidgets knows what it is doing.
   
   // Jesus step aside! Here's a real miracle. This code actually works!
   if (( (wxTopLevelWindow*)GetParent() )->IsMaximized()) {
    autoZoomOnFirstRedraw = false;
    AutoZoom( false );
   }
   
  }
 }
 
 // trigger redraw
 Redraw();
}


// zooms image to fit viewport
void CViewport::AutoZoom(bool zoomin)
{
 // auto-zoom
 if (!Image.loaded) return;
 
 // compute zoom level to fit viewport
 const float sx = (float)g_width  / (float)(Image.width);
 const float sy = (float)g_height / (float)(Image.height);
 float s = sx;
 if (sy < sx) s = sy;
 
 // zoom out to fit
 if (s < 1) {
  g_zoom = 1;
  g_panx = 0;
  g_pany = 0;
  Zoom(s, false);
 }
 
 // zoom in to fit
 if (zoomin) {
  if (s > 1) {
   g_zoom = 1;
   g_panx = 0;
   g_pany = 0;
   Zoom(s, false);
  }
 }
 
 //printf(">>> autozoom %.4f @ %i,%i\n", z, g_width, g_height);
 
 // trigger redraw
 //Redraw(); // can't do that here, recursive! Call from button click event etc.
}


// paint event
void CViewport::OnPaint(wxPaintEvent &evt)
{
 //Init();
 if (!glcontext) return;
 
 // select context
 this->SetCurrent(*glcontext);
 
 // avoid flicker
 wxPaintDC(this);
 
 // clear
 glViewport(0, 0, g_width, g_height);
 glClearColor(g_bkg.r, g_bkg.g, g_bkg.b, g_bkg.a);
 glClear(GL_COLOR_BUFFER_BIT);
 
 // draw scene
 Scene.Draw();
 
 //////////////////////////////////////
 /*
 float2 TF(const float2 &v);
 float2 TFi(const float2 &v);
 
 glPointSize(3);
 
 glColor3f(1,1,0);
 glBegin(GL_POINTS);
  //glVertex2f(mousex,mousey);
  glVertex2fv( TFi(float2(mousex,mousey)) );
 glEnd();
 
 glColor3f(1,0,0);
 glBegin(GL_POINTS);
  //glVertex2f(dragx,dragy);
  glVertex2fv( TFi(float2(dragx,dragy)) );
 glEnd();
 */
 //////////////////////////////////////////
 
 // swap buffers
 glFinish();
 this->SwapBuffers();
}


// triggers redraw if window is active
void CViewport::Redraw()
{
 if (active) {
  Refresh(false);
 }
}


// mouse movement event
void CViewport::OnMouse(wxMouseEvent &evt)
{
 // update button states
 const bool mb_left = evt.LeftIsDown();
 const bool mb_right = evt.RightIsDown();
 const bool mb_middle = evt.MiddleIsDown();
 
 // update mouse wheel vector
 float mousewheel = 0;
 const int delta = evt.GetWheelDelta();
 if (delta > 0) {
  mousewheel = (float)evt.GetWheelRotation() / (float)delta;
 }
 
 // update mouse viewport position
 const float mx = evt.GetX();
 const float my = evt.GetY();
 mousex = mx;
 mousey = my;
 
 // compute mouse motion vector
 static int oldx = evt.GetX();
 static int oldy = evt.GetY();
 const float vecx = mx - oldx;
 const float vecy = my - oldy;
 
 // remember current coordinates
 oldx = mx;
 oldy = my;
 
 // drag
 if (mb_right || mb_left || mb_middle) {
  if (!drag) {
   dragx = mx;
   dragy = my;
  }
  drag = true;
 } else {
  drag = false;
 }
 
 // focus workaround
 if (mb_middle || mb_right) {
  SetFocus();
 }
 
 // set cursor
 if (mb_left  ) {
  SetCursor( wxCURSOR_HAND ); // pan
 } else if (mb_middle) {
  SetCursor( wxCURSOR_HAND ); // pan
 } else if (mb_right ) {
  SetCursor( wxCURSOR_MAGNIFIER ); // zoom
 } else {
  SetCursor( wxCURSOR_ARROW ); // default
 }
 
 // middle mouse button panning
 if (mb_middle || mb_left) {
  //this->SetCursor( wxCURSOR_HAND );
  g_panx += vecx;
  g_pany += vecy;
  Redraw();
 }
 
 // right mouse button zooming
 static bool wb = false;
 if (mb_right) {
  //this->SetCursor( wxCURSOR_MAGNIFIER );
  Zoom(dragx, dragy, -vecy * 0.01, true);
  Redraw();
 }
 
 // mouse wheel zooming
 if (delta > 0) {
  if (g_zoomsmooth) {
   Zoom(mx, my, mousewheel * 0.1, true );
  } else {
   //if (mousewheel > 0) Zoom(mx, my, 2.0, false );
   //if (mousewheel < 0) Zoom(mx, my, 0.5, false );
   if (mousewheel > 0) Zoom(2.0, false );
   if (mousewheel < 0) Zoom(0.5, false );
  }
  Redraw();
 }
 
 // update stuff
 MainWindow->UpdateUI();
 
 // claim all input
 evt.Skip();
}


// activates/deactivates viewport
void CViewport::SetActive(bool v)
{
 active = v;
}


// key down event
void CViewport::OnKeyDown(wxKeyEvent &evt)
{
 const int key = evt.GetKeyCode();
 
 switch (key) {
 case WXK_SPACE:
  void ResetZoom();
  ResetZoom();
  break;
 case WXK_ADD:
 case WXK_NUMPAD_ADD:
 case WXK_PAGEDOWN:
 case WXK_DOWN:
  Zoom( 2.0, false );
  break;
 case WXK_SUBTRACT:
 case WXK_NUMPAD_SUBTRACT:
 case WXK_PAGEUP:
 case WXK_UP:
  Zoom( 0.5, false );
  break;
 case WXK_LEFT:
  g_mipmap--;
  break;
 case WXK_RIGHT:
  g_mipmap++;
  break;
 }
 
 Redraw();
 
 // update stuff
 MainWindow->UpdateUI();
}


// key up event
void CViewport::OnKeyUp(wxKeyEvent &evt)
{
 //
}


// returns mouse coordinates in viewport space
int CViewport::GetMouseX() const
{
 return mousex;
}
int CViewport::GetMouseY() const
{
 return mousey;
}


// destructor
CViewport::~CViewport()
{
 // destroy shader
 if (shader) {
  delete shader;
 }
 
 // destroy GL context
 if (glcontext) {
  delete glcontext;
 }
}
