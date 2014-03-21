
#include <math.h>
#include "math.h"
#include "image.h"
#include "opengl.h"
#include "common.h"
#include "scene.h"


CScene Scene;


// rotates point
float2 ROT(const float2 &v, float rot)
{
 const float s = sin(rot);
 const float c = cos(rot);
 return float2( (c * v.x - s * v.y) ,
                (s * v.x + c * v.y) );
}

/*
// transform
float TFX(float x) { return (x * g_zoom) + g_panx + ((g_width /2)-(Image.width /2)); }
float TFY(float y) { return (y * g_zoom) + g_pany + ((g_height/2)-(Image.height/2)); }

// inverse transform
float TFXi(float x) { return (x - g_panx - ((g_width /2)-(Image.width /2)) ) / g_zoom; }
float TFYi(float y) { return (y - g_pany - ((g_height/2)-(Image.height/2)) ) / g_zoom; }

float TF2X(float x) { return (x * g_zoom) + g_panx + ((g_width /2)-(Image.width /2)); }
float TF2Y(float y) { return (y * g_zoom) + g_pany + ((g_height/2)-(Image.height/2)); }
float TF2Xi(float x) { return (x - g_panx + ((g_width /2)-(Image.width /2)) ) / g_zoom; }
float TF2Yi(float y) { return (y - g_pany + ((g_height/2)-(Image.height/2)) ) / g_zoom; }
*/


// transforms image coord to view coord
float2 TF(const float2 &v)
{
 float2 q = v;
 
 // center
 float sx = (float)Image.width  / 2.0;
 float sy = (float)Image.height / 2.0;
 q.x -= sx;
 q.y -= sy;
 
 // transform
 q = ROT(q, g_rot * DEGTORAD);
 if (g_mirrorx) q.x = -q.x;
 if (g_mirrory) q.y = -q.y;
 
 // uncenter
 q.x += sx;
 q.y += sy;
 
 float2 t;
 t.x = (q.x * g_zoom) + g_panx + ( (g_width /2) - (Image.width /2) );
 t.y = (q.y * g_zoom) + g_pany + ( (g_height/2) - (Image.height/2) );
 
 return t;
}


// transforms view coord to image coord
float2 TFi(const float2 &v)
{
 static float2 t;
 t.x = (v.x - g_panx - ( (g_width /2)-(Image.width /2) ) ) / g_zoom;
 t.y = (v.y - g_pany - ( (g_height/2)-(Image.height/2) ) ) / g_zoom;
 
 //t = ROT(t, -g_rot*DEGTORAD); // looks like not needed?
 
 return t;
}


// draws scene
void CScene::Draw() const
{
 if (!Image.loaded) return;
 
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0, g_width, g_height, 0, -1.0, 1.0);
 
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 
 bool compatmode = true;
 if (compatmode) {
  
  const float w = Image.width;
  const float h = Image.height;
  
  float2 v1 = TF( float2(0,0) );
  float2 v2 = TF( float2(0,h) );
  float2 v3 = TF( float2(w,h) );
  float2 v4 = TF( float2(w,0) );
  
  /*
  // background
  if (Image.frags == 4) {
   
   // draw background
   glColor4f(0, 1, 1, 1);
   glBegin(GL_QUADS);
    glTexCoord2f(0,1); glVertex2fv(v1);
    glTexCoord2f(0,0); glVertex2fv(v2);
    glTexCoord2f(1,0); glVertex2fv(v3);
    glTexCoord2f(1,1); glVertex2fv(v4);
   glEnd();
   
   // don't do transparancy if we're showing one channel
   if (g_chan == 0) {
    
    // alpha-blended transparancy
    if (g_alphamode == 1) {
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glEnable(GL_BLEND);
    }
    
    // alpha-tested transparancy
    if (g_alphamode == 2) {
     glAlphaFunc(GL_GREATER, 0.5);
     glEnable(GL_ALPHA_TEST);
    }
    
   }
  }
  */
  
  // determine vertex color
  color c(0,0,0,0);
  c.r = (g_colormode==1) ? 1:0;
  c.g = (g_colormode==2) ? 1:0;
  c.b = (g_colormode==3) ? 1:0;
  //c.a = g_chan_a ? 1:0;
  if (g_colormode==0) c = color(1,1,1,1);
  glColor4fv(c);
  
  // draw image as texture rectangle
  Image.Bind();
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
   glTexCoord2f(0,1); glVertex2fv(v1);
   glTexCoord2f(0,0); glVertex2fv(v2);
   glTexCoord2f(1,0); glVertex2fv(v3);
   glTexCoord2f(1,1); glVertex2fv(v4);
  glEnd();
  glEnable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);
  Image.Unbind();
  
  /*
  // disable transparancy
  if (Image.frags == 4) {
   if (g_alphamode == 1) glDisable(GL_BLEND);
   if (g_alphamode == 2) glDisable(GL_ALPHA_TEST);
  }
  */
  
 } else {
  /*
  const float sw = (float)g_width;
  const float sh = (float)g_height;
  
  const float w = (float)Image.width;
  const float h = (float)Image.height;
  
  const float sx = sw / w;
  const float sy = sh / h;
  
  //float2 v1(TFXi(0)/w * sx, TFYi(0)/h * sy);
  //float2 v2(TFXi(0)/w * sx, TFYi(h)/h * sy);
  //float2 v3(TFXi(w)/w * sx, TFYi(h)/h * sy);
  //float2 v4(TFXi(w)/w * sx, TFYi(0)/h * sy);
  
  float2 v1(TF2Xi(0), TF2Yi(0));
  float2 v2(TF2Xi(0), TF2Yi(h));
  float2 v3(TF2Xi(w), TF2Yi(h));
  float2 v4(TF2Xi(w), TF2Yi(0));
  
  glColor4f(1.0, 1.0, 1.0, 1.0);
  Image.Bind();
  glEnable(GL_TEXTURE_2D);
  
  glBegin(GL_QUADS);
   glTexCoord2fv(v1); glVertex2f(0,      0       );
   glTexCoord2fv(v2); glVertex2f(0,      g_height);
   glTexCoord2fv(v3); glVertex2f(g_width,g_height);
   glTexCoord2fv(v4); glVertex2f(g_width,0       );
  glEnd();
  
  glDisable(GL_TEXTURE_2D);
  Image.Unbind();
  */
 }
 
}


const float MINZOOM = 1.0 / 4096.0;
const float MAXZOOM = 1024.0;



unsigned NextPow2( unsigned x )
{
 --x;
 x |= x >> 1;
 x |= x >> 2;
 x |= x >> 4;
 x |= x >> 8;
 x |= x >> 16;
 return ++x;
}


// zooms to target scale
void Zoom(float x, float y, float tgt)
{
 if (!Image.loaded) return;
 
 // get mouse coords to image space
 float2 c = TFi( float2(x,y) );
 
 // change zoom
 g_zoom = tgt;
 if (g_zoom < MINZOOM) g_zoom = MINZOOM;
 if (g_zoom > MAXZOOM) g_zoom = MAXZOOM;
 
 // get mouse coords in image space again
 const float2 nc = TFi( float2(x,y) );
 
 // pan to new zoom target by zoom difference
 g_panx += (nc.x - c.x) * g_zoom;
 g_pany += (nc.y - c.y) * g_zoom;
 
 //// hack: clamp pan coordinates for pixel alignment
 if (g_zoom == 1) { // todo: for all powers of two
  g_panx = round(g_panx);
  g_pany = round(g_pany);
 }
 //// hack
}


// zooms at center to target
void Zoom(float tgt)
{
 const float x = g_width  / 2;
 const float y = g_height / 2;
 Zoom(x,y,tgt);
}


// zoom at center by increment/scale
void Zoom(float val, bool incr)
{
 const float x = g_width  / 2;
 const float y = g_height / 2;
 Zoom(x,y,val,incr);
}


// zoom at coordinate by increment/scale
void Zoom(float x, float y, float val, bool incr)
{
 float tgt = 1.0;
 if (incr) {
  tgt = g_zoom + (val * g_zoom);   // zoom increment
 } else {
  tgt = g_zoom * val;              // zoom scale
 }
 
 // determine zoom position target
 float2 p(x,y);
 
 // if outside zoom at target
 if (!PointTest(x,y)) {
 // p = TF( float2(Image.width/2, Image.height/2) );
 }
 
 Zoom(p.x, p.y, tgt);
}


// resets zoom
void ResetZoom()
{
 g_zoom = 1.0;
 g_panx = 0;
 g_pany = 0;
 g_rot = 0;
 g_mirrorx = false;
 g_mirrory = false;
}


// returns true if inside image
bool PointTest(float x, float y)
{
 if (!Image.loaded) return false;
 float2 p = TFi( float2(x,y) );
 if (p.x < 0) return false;
 if (p.y < 0) return false;
 if (p.x >= Image.width) return false;
 if (p.y >= Image.height) return false;
 return true;
}
