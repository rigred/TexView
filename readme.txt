TexView
=======

NOTE
TexView is currently in a poor state because of a very annoying wxWidgets bug,
for which there is no workaround. Until this is fixed, TexView should not be
considered ready for everyday use, but is otherwise fully functional and safe
to use.
A binary will be posted once all critical bugs are fixed.


DESCRIPTION

TexView is a cross-platform texture image viewer, with support for a wide range
of image types.

TexView is intended to replace outdated, but still much-used texture viewers
such as DxTex (Microsoft), Windows Texture Viewer (Tomas Blaho and included in
NVidia toolkit) and ddsview (amnoid.de). It also has some standard features one
would expect to find in your typical (basic) image viewers, so that you can use
it as your default image viewer if you prefer.

Features:
* very fast launch time (probably the quickest image viewer on Linux)
* GPU accelerated image display (fast at any zoom level)
* supports wide variety of image formats (courtesey of DevIL library)
* traditional and intuitive user interface
* displays important image format details

Limitations:
* image dimensions larger than the maximum supported GPU textures size is
  currently not supported
* see issues on GitHub: https://github.com/ByteHazard/TexView/issues


BUG REPORTS

Please file report bugs at the TexView GitHub:
 https://github.com/ByteHazard/TexView/issues

If you don't have a GitHub account, bug reports can be mailed to:
 martijn@bytehazard.com


ACKNOWLEDGEMENTS

TexView has dependencies on the following libraries:
* wxWidgets > cross platform user interface
* DevIL > image loader
* OpenGL > GPU acceleration
