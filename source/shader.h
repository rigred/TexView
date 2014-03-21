
#ifndef SHADER_H
#define SHADER_H


// shader class
class CShader
{
private:
 
 unsigned int vertshader;
 unsigned int fragshader;
 unsigned int program;
 mutable bool bound;
 
public:
 
 // constructor/destructor
 CShader();
 ~CShader();
 
 // core
 bool Create(const char *vert, const char *frag);
 void Bind() const;
 void Unbind() const;
 void Destroy();
 
 // misc
 void SetUniform1i(const char *name, int v) const;
 void SetUniform1f(const char *name, float v) const;
 void SetUniform2f(const char *name, float v1, float v2) const;
 void SetUniform3f(const char *name, float v1, float v2, float v3) const;
 void SetUniform4f(const char *name, float v1, float v2, float v3, float v4) const;
 void SetUniform2fv(const char *name, const float *v) const;
 void SetUniform3fv(const char *name, const float *v) const;
 void SetUniform4fv(const char *name, const float *v) const;
};


CShader *LoadShader(const char *, const char *);


#endif
