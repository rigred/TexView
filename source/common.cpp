
#include "shader.h"
#include "common.h"

const std::string g_appname = "TexView 0.1.0";
std::string g_datadir = "data";
int g_width = 10;
int g_height = 10;
float g_asp = 1.0;
float g_maxaniso = 1.0;

color g_bkg = color(0.2, 0.2, 0.2);
int g_mipmap = 0;
int g_colormode = 0;
int g_alphamode = 2;
float g_zoom = 1.0;
float g_panx = 0.0;
float g_pany = 0.0;
float g_rot = 0.0;
float g_cursorx = 0.0;
float g_cursory = 0.0;
int g_mipmaplevel = 0;
bool g_tilex = false;
bool g_tiley = false;
bool g_mirrorx = false;
bool g_mirrory = false;
bool g_grid = false;
bool g_ramp = false;
bool g_invert = false;
bool g_bump = false;
bool g_filter = true;
bool g_normalize = false;
bool g_cubemap = false;
bool g_zoomsmooth = false;
bool g_startupresize = false;
bool g_genmipmaps = true;
std::string g_errmsg;


float g_gridColor[3] = { 0,0,0 };
float g_checkColor1[3] = { 0.45, 0.45, 0.45 };
float g_checkColor2[3] = { 0.55, 0.55, 0.55 };
int g_checkSize = 16;
