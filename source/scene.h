
#ifndef SCENE_H
#define SCENE_H

#include "math.h"


// scene class
class CScene
{
public:
 
 void Draw() const;
 
};


extern CScene Scene;


float2 TFi(const float2 &v);

void Zoom(float tgt);
void Zoom(float val, bool incr);
void Zoom(float x, float y, float tgt);
void Zoom(float x, float y, float val, bool incr);
void ResetZoom();
bool PointTest(float x, float y);


#endif
