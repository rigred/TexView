
#ifndef COMMON_H
#define COMMON_H

#include <string>
class CShader;


struct color {
 float r;
 float g;
 float b;
 float a;
 color() {
  r = 1.0;
  g = 1.0;
  b = 1.0;
  a = 1.0;
 };
 color(float _r, float _g, float _b) {
  r = _r;
  g = _g;
  b = _b;
  a = 1.0;
 };
 color(float _r, float _g, float _b, float _a) {
  r = _r;
  g = _g;
  b = _b;
  a = _a;
 };
 operator const float *() const { return (float*)this; }
};


extern const std::string g_appname;       // application title
extern std::string g_datadir;             // application resource directory
extern int g_width;                       // viewport width
extern int g_height;                      // viewport height
extern float g_maxaniso;                  // max texture anisotropy

extern color g_bkg;                 // background color
extern int g_mipmap;                // mipmap level
extern int g_colormode;             // color channel (0=all, 1=R, 2=G, 3=B, 4=A)
extern int g_alphamode;             // alpha display mode (0=none, 1=blend, 2=alphatest)
extern float g_zoom;                // zoom scale
extern float g_panx;                // pan x offset
extern float g_pany;                // pan y offset
extern float g_rot;                 // rotation
extern float g_cursorx;             // cursor position in image space
extern float g_cursory;             // cursor position in image space
extern int g_mipmaplevel;           // mipmap level to display
extern bool g_mirrorx;              // mirror horizontally
extern bool g_mirrory;              // mirror vertically
extern bool g_tilex;                // tile texture
extern bool g_tiley;                // tile texture
extern bool g_grid;                 // pixel grid
extern bool g_ramp;                 // height map ramp
extern bool g_invert;               // inverts image
extern bool g_bump;                 // bump map
extern bool g_filter;               // filters image
extern bool g_normalize;            // normalizes normal map
extern bool g_cubemap;              // display as cubemap
extern bool g_zoomsmooth;           // smooth zooming
extern bool g_startupresize;        // automatically resizes window on startup
extern bool g_genmipmaps;           // generates mipmaps
extern std::string g_errmsg;        // image error message

// configuration
extern float g_gridColor[3];        // pixel grid color
extern float g_checkColor1[3];      // checker color
extern float g_checkColor2[3];      // checker color
extern int g_checkSize;             // checker color


#endif
