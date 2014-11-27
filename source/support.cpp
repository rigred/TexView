
#include <stdio.h>
#include <string.h>

struct supinfo {
 const char *ext;
 const char *full;
};


supinfo supext[] =
{
 "bmp",  "Windows Bitmap",
 "cut",  "Dr. Halo",
 "dcx",  "Multi-PCX",
 "dicom","Dicom",
 "dcm",  "Dicom",
 "dds",  "DirectDraw Surface",
 "exr",  "OpenEXR",
 "fits", "Flexible Image Transport System",
 "fit",  "Flexible Image Transport System",
 "ftx",  "Heavy Metal: FAKK 2",
 "hdr",  "Radiance High Dynamic",
 "icns", "Macintosh icon",
 "ico",  "Windows icon",
 "cur",  "Windows cursor",
 "iff",  "Interchange File Format",
 "iwi",  "Infinity Ward Image",
 "gif",  "Graphics Interchange Format",
 "jpg",  "JPEG",
 "jpe",  "JPEG",
 "jpeg", "JPEG",
 "jp2",  "JPEG 2000",
 "lbm",  "Interlaced Bitmap",
 "lif",  "Homeworld texture",
 "mdl",  "Half-Life Model",
 "mp3",  "MPEG-1 Audio Layer 3",
 "pal",  "Palette",
 "pcd",  "Kodak PhotoCD",
 "pcx",  "ZSoft PCX",
 "pic",  "Softimage PIC",
 "png",  "Portable Network Graphics",
 "pbm",  "Portable Anymap",
 "pgm",  "Portable Anymap",
 "pnm",  "Portable Anymap",
 "pnm",  "Portable Anymap",
 "pix",  "Alias Wavefront",
 "psd",  "Adobe PhotoShop",
 "psp",  "PaintShop Pro",
 "pxr",  "Pixar",
 "raw",  "Raw",
 "rot",  "Homeworld 2 Texture",
 "sgi",  "Silicon Graphics",
 "bw",   "Silicon Graphics BW",
 "rgb",  "Silicon Graphics RGB",
 "rgba", "Silicon Graphics RGBA",
 "texture","Creative Assembly Texture",
 "tga",  "Truevision Targa",
 "tif",  "Tagged Image File Format",
 "tpl",  "Gamecube Texture",
 "utx",  "Unreal Texture",
 "wal",  "Quake 2 Texture",
 "vtf",  "Valve Texture Format",
 "wdp",  "HD Photo",
 "hdp",  "HD Photo",
 "xpm",  "X Pixel Map",
 NULL,NULL
};


// returns true if file type is supported
bool IsSupportedExt(const char *ext)
{
 // no extension
 if (!ext) return false;
 
 // match against list
 supinfo *info = supext;
 while (info->ext)
 {
  if (strcasecmp(info->ext, ext) == 0) return true;
  info++;
 }
 
 // not supported
 return false;
}


// returns true if file extension is supported
bool IsSupportedFile(const char *fname)
{
 if (!fname) return false;
 
 // ignore Apple/OSX metadata files
 if (strncmp(fname,"._",2) == 0) return false;
 
 // get file extension
 const char *ext = strrchr(fname,'.');
 if (!ext) return NULL;
 
 // check extension
 return IsSupportedExt(ext+1);
}
