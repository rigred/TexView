
#include <stdio.h> //// temp
#include <string.h>
#include <assert.h>
#include "opengl.h"
#include "shader.h"


// constructor
CShader::CShader()
{
 vertshader = 0;
 fragshader = 0;
 program = 0;
 bound = false;
}


// creates shader
bool CShader::Create(const char *vertsrc, const char *fragsrc)
{
 // verify arguments
 assert(vertsrc != NULL);
 assert(fragsrc != NULL);
 
 // ensure not yet in use
 assert(program == 0);
 
 GLint r;
 
 // create shaders
 vertshader = glCreateShader(GL_VERTEX_SHADER);
 fragshader = glCreateShader(GL_FRAGMENT_SHADER);
 
 // load shader source
 glShaderSource(vertshader, 1, &vertsrc, NULL);
 glShaderSource(fragshader, 1, &fragsrc, NULL);
 
 // compile vertex shader
 glCompileShader(vertshader);
 glGetShaderiv(vertshader, GL_COMPILE_STATUS, &r);
 if (r != GL_TRUE) {
  printf(">>> glCompileShader failed (vert)\n");
  Destroy();
  return false;
 }
 
 // compile fragment shader
 glCompileShader(fragshader);
 glGetShaderiv(fragshader, GL_COMPILE_STATUS, &r);
 if (r != GL_TRUE) {
  printf(">>> glCompileShader failed (frag)\n");
  Destroy();
  return false;
 }
 
 // create program
 program = glCreateProgram();
 
 // attach shaders to program
 glAttachShader(program, vertshader);
 glAttachShader(program, fragshader);
 
 // link program
 glLinkProgram(program);
 glGetProgramiv(program, GL_LINK_STATUS, &r);
 if (r != GL_TRUE) {
  printf(">>> glLinkProgram failed\n");
  Destroy();
  return false;
 }
 
 // success
 return true;
}


// binds shader
void CShader::Bind() const
{
 assert(bound == false);
 glUseProgram(program);
 bound = true;
}


// unbinds shader
void CShader::Unbind() const
{
 assert(bound == true);
 glUseProgram(0);
 bound = false;
}


// sets shader uniform
void CShader::SetUniform1i(const char *name, int v) const
{
 assert(bound == true);
 GLint loc = glGetUniformLocation(program, name);
 if (loc == -1) return;
 glUniform1i(loc, v);
}
void CShader::SetUniform1f(const char *name, float v) const
{
 assert(bound == true);
 GLint loc = glGetUniformLocation(program, name);
 if (loc == -1) return;
 glUniform1f(loc, v);
}
void CShader::SetUniform2f(const char *name, float v1, float v2) const
{
 assert(bound == true);
 GLint loc = glGetUniformLocation(program, name);
 if (loc == -1) return;
 glUniform2f(loc, v1, v2);
}
void CShader::SetUniform3f(const char *name, float v1, float v2, float v3) const
{
 assert(bound == true);
 GLint loc = glGetUniformLocation(program, name);
 if (loc == -1) return;
 glUniform3f(loc, v1, v2, v3);
}
void CShader::SetUniform4f(const char *name, float v1, float v2, float v3, float v4) const
{
 assert(bound == true);
 GLint loc = glGetUniformLocation(program, name);
 if (loc == -1) return;
 glUniform4f(loc, v1, v2, v3, v4);
}
void CShader::SetUniform2fv(const char *name, const float *v) const
{
 assert(bound == true);
 GLint loc = glGetUniformLocation(program, name);
 if (loc == -1) return;
 glUniform2fv(loc, 1, v);
}
void CShader::SetUniform3fv(const char *name, const float *v) const
{
 assert(bound == true);
 GLint loc = glGetUniformLocation(program, name);
 if (loc == -1) return;
 glUniform3fv(loc, 1, v);
}
void CShader::SetUniform4fv(const char *name, const float *v) const
{
 assert(bound == true);
 GLint loc = glGetUniformLocation(program, name);
 if (loc == -1) return;
 glUniform4fv(loc, 1, v);
}


// destroys shader
void CShader::Destroy()
{
 if (program) {
  glDeleteProgram(program);
  program = 0;
 }
 if (vertshader) {
  glDeleteShader(vertshader);
  vertshader = 0;
 }
 if (fragshader) {
  glDeleteShader(fragshader);
  fragshader = 0;
 }
}


// destructor
CShader::~CShader()
{
 Destroy();
}


#include <stdio.h>


// loads text from file to buffer
bool LoadTextFile(const char *filename, char **bufferptr)
{
 assert(filename != NULL);
 assert(*bufferptr == NULL);
 
 // open file
 FILE *fp = fopen(filename,"rb");
 if (!fp) {
  printf(">>> file %s not found\n", filename);
  return false;
 }
 
 // get size
 fseek(fp, 0, SEEK_END);
 const int size = ftell(fp);
 fseek(fp, 0, SEEK_SET);
 
 // return if file size is zero bytes
 if (size == 0) return false;
 
 // allocate string buffer
 char *buffer = new char[size+1];
 
 // read entire file
 fread(buffer,size,1,fp);
 
 // close file
 fclose(fp);
 
 // terminate string buffer
 buffer[size] = '\0';
 
 // set pointer
 *bufferptr = buffer;
 
 // success
 return true;
}


// loads shader from file
CShader *LoadShader(const char *vertFName, const char *fragFName)
{
 CShader *sh = new CShader();
 char *vertStr = NULL;
 char *fragStr = NULL;
 
 // load files
 bool vertOk = LoadTextFile(vertFName, &vertStr);
 bool fragOk = LoadTextFile(fragFName, &fragStr);
 
 // check errors
 if (!vertOk) printf("Vertex shader file not found!\n");
 if (!fragOk) printf("Fragment shader file not found!\n");
 
 // create shader
 sh->Create( vertStr, fragStr );
 
 // delete code buffers
 delete [] vertStr;
 delete [] fragStr;
 
 // return shader
 return sh;
}
