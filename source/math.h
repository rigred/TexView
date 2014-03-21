
#ifndef MATH_H
#define MATH_H

#define PI 3.14159265
#define RADTODEG (180 / PI)
#define DEGTORAD (PI / 180)


// 2d vector
struct float2 {
 float x;
 float y;
 float2() {
  x = 0.0;
  y = 0.0;
 }
 float2(float a, float b) {
  x = a;
  y = b;
 }
 float2(const float2 &v) {
  x = v.x;
  y = v.y;
 }
 operator const float *() const { return (float*)this; }
};


#endif
