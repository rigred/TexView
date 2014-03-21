
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "opengl.h"


// image class
class CImage
{
private:
 
 GLuint texname;                            // GL texture handle 
 GLenum target;                             // GL texture target (1D, 2D, 3D etc)
 GLint intformat;                           // GL internal texture format
 GLenum format;                             // GL texture format
 GLenum type;                               // GL texture data type (byte, int, float etc)
 
public:
 
 std::string filename;                      // file name
 int width;                                 // length of pixel row
 int height;                                // number of pixel rows
 int frags;                                 // number of color channels 
 bool loaded;                               // whether image is loaded
 bool mipmaps;                              // whether image has mipmaps
 
 // constructor/destructor
 CImage();
 ~CImage();
 
 // misc
 bool Load(const char *image);
 void Refresh();
 bool Reload();
 void Unload();
 
 void Bind();
 void Unbind();
 
};


extern CImage Image;


#endif
