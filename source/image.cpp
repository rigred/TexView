
#include <IL/il.h>
#include <assert.h>
#include "common.h"
#include "image.h"


CImage Image;


// constructor
CImage::CImage()
{
 width = 0;
 height = 0;
 frags = 0;
 loaded = false;
 mipmaps = false;
 
 texname = 0;
 target = 0;
 intformat = 0;
 format = 0;
 type = 0;
}


// loads image
bool CImage::Load(const char *fname)
{
 assert(fname != NULL);
 
 //printf(">>> %s\n", fname);
 
 // unload old image
 Unload();
 
 // init devil
 ilInit();
 
 // create image
 ILuint img;
 ilGenImages(1, &img);
 ilBindImage(img);
 
 // correct origin
 ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
 ilEnable(IL_ORIGIN_SET);
 
 // convert palette
 ilEnable(IL_CONV_PAL);
 //ilutEnable(ILUT_OPENGL_CONV);
 
 // load image
 if (!ilLoadImage(fname)) {
  ilDeleteImages(1, &img);
  g_errmsg = "Format not recognised or file corrupted.";
  return false;
 } else {
  
  // copy info
  width = ilGetInteger(IL_IMAGE_WIDTH);
  height = ilGetInteger(IL_IMAGE_HEIGHT);
  frags = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
  
  
  GLint maxsize;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxsize);
  if (width > maxsize || height > maxsize) {
   g_errmsg = "Image dimension larger than ";
   char shitstream[10];
   sprintf(shitstream, "%i", (int)maxsize );
   g_errmsg += shitstream;
   g_errmsg += " not supported by graphics driver!";
   return false;
  }
  
  //printf(">>> %ix%i*%i\n", width, height, frags);
  
  int  ilfmt;
  switch (frags) {
  case 1:  ilfmt = IL_LUMINANCE;       break;
  case 2:  ilfmt = IL_LUMINANCE_ALPHA; break;
  case 3:  ilfmt = IL_RGB;             break;
  case 4:  ilfmt = IL_RGBA;            break;
  default: ilfmt = IL_RGB;             break;
  }
  
  // convert
  if (!ilConvertImage(ilfmt, IL_UNSIGNED_BYTE)) {
   ilDeleteImages(1, &img);
   g_errmsg = "Failed to convert image!";
   return false;
  }
  
  // opengl texture
  target = GL_TEXTURE_2D;
  //target = GL_TEXTURE_RECTANGLE_ARB; // todo: for compat mode?
  intformat = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
  format = ilGetInteger(IL_IMAGE_FORMAT);
  type = GL_UNSIGNED_BYTE;
 }
 
 // create texture
 glGenTextures(1, &texname);
 glBindTexture(target, texname);
 
 // load image
 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 glTexImage2D(target, 0, intformat, width, height, 0, format, type, ilGetData() );
 
 // generate mipmaps
 if (g_genmipmaps) {
  glGenerateMipmap(GL_TEXTURE_2D);
  mipmaps = true;
 }
 
 // set texture params
 Refresh();
 
 // unload image
 ilDeleteImages(1, &img);
 
 /*
 // determine target & images
 int images = 0;
 if (cubemap) {
  target = GL_TEXTURE_CUBE_MAP;
 } else {
  target = GL_TEXTURE_2D;
 }
 
 // determine texture format
 GLint intformat;
 switch (frags)
 {
  case 1:
   format = GL_ALPHA;
   intformat = GL_ALPHA8;
   break;
  case 3:
   format = GL_BGR;
   intformat = GL_RGB8;
   break;
  case 4:
   format = GL_BGRA;
   intformat = GL_RGBA8;
   break;
 }
 
  // mipmapping
  if (mipmap) {
   glTexParameteri(target, GL_GENERATE_MIPMAP, GL_TRUE);
  } else {
   glTexParameteri(target, GL_GENERATE_MIPMAP, GL_FALSE);
  }
  
  // determine upload target
  int uptarget = 0;
  if (cubemap) {
   if (i == 5) uptarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
   if (i == 4) uptarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
   if (i == 2) uptarget = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
   if (i == 3) uptarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
   if (i == 1) uptarget = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
   if (i == 0) uptarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
  } else {
   uptarget = GL_TEXTURE_2D;
  }
  
  // upload texture image
  glTexImage2D(uptarget, 0, intformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
 }
 */
 
 // copy filename
 filename = fname;
 
 // set flag
 loaded = true;
 
 // success
 return true;
}


void CImage::Bind()
{
 glBindTexture(target, texname);
}
void CImage::Unbind()
{
 glBindTexture(target, 0);
}


// refreshes parameters
void CImage::Refresh()
{
 // set texture wrapping
 GLint wrapmodex = GL_CLAMP_TO_EDGE;
 GLint wrapmodey = GL_CLAMP_TO_EDGE;
 if (!g_cubemap) {
  if (g_tilex) wrapmodex = GL_REPEAT;
  if (g_tiley) wrapmodey = GL_REPEAT;
 }
 glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapmodex);
 glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapmodey);

 // minification filter
 GLint min;
 if (g_filter) {
  if (mipmaps) {
   min = GL_LINEAR_MIPMAP_LINEAR;
  } else {
   min = GL_LINEAR;
  }
 } else {
  if (mipmaps) {
   min = GL_LINEAR_MIPMAP_LINEAR;
  } else {
   min = GL_NEAREST;
  }
 }
 
 // magnification filter
 GLint mag;
 if (g_filter) {
  mag = GL_LINEAR;
 } else {
  mag = GL_NEAREST;
 }
 
 glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
 glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);

 // anisotropic filtering
 if (g_filter && g_maxaniso > 1) {
  glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_maxaniso);
 } else {
  glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
 }
}


// reloads image
bool CImage::Reload()
{
 if (!loaded) return true;
 const std::string fname = filename;
 Unload();
 return Load(fname.c_str());
}


// unloads image
void CImage::Unload()
{
 loaded = false;
 filename.clear();
 if (texname) {
  glDeleteTextures(1, &texname);
  texname = 0;
 }
 width = 0;
 height = 0;
 frags = 0;
}


// destructor
CImage::~CImage()
{
 Unload();
}
