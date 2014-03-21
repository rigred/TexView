
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <wx/glcanvas.h>
class CShader;


// viewport class
class CViewport: public wxGLCanvas
{
private:
 
 // properties
 wxGLContext *glcontext;
 bool active;
 
 bool drag;
 float dragx;
 float dragy;
 float mousex;
 float mousey;
 
 const CShader *shader;
 
 // internal
 void Init();
 void Redraw();
 
 // events
 void Paint(wxPaintEvent &evt);
 void OnResize(wxSizeEvent &evt);
 void OnKeyDown(wxKeyEvent &evt);
 void OnKeyUp(wxKeyEvent &evt);
 void Mouse(wxMouseEvent &evt);
 
public:
 
 bool autoZoomOnFirstRedraw;     // triggers autozoom on first redraw to workaround wxBullshit
 
 // constructor
 CViewport(wxWindow *parent);
 ~CViewport();
 
 // misc
 void SetActive(bool v);
 int GetMouseX() const;
 int GetMouseY() const;
 void AutoZoom(bool zoomin);
 void UpdateShaderUniforms();
 
 DECLARE_EVENT_TABLE()
};


#endif
