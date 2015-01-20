
uniform sampler2D tex;
uniform vec2 imageSize;
uniform vec2 gridSize;
uniform int colorMode;
uniform int alphaMode;
uniform int invertMode;
uniform int showGrid;
uniform int showRamp;
uniform vec2 cursorPos;
uniform vec3 gridColor;
uniform vec3 checkColor1;
uniform vec3 checkColor2;
uniform float checkSize;

varying vec2 uv;


// checker texture
vec3 Checker()
{
 // todo: make anti-aliased
 vec2 check = uv * (imageSize * checkSize);
 if (mod( floor(check.x) + floor(check.y), 2.0) < 1.0) {
  return checkColor1; //vec3(0,1,1); // turquiose
 } else {
  return checkColor2; //vec3(1,0,1); // magenta
 }
}


// ramp color
vec3 RampColor(float value)
{
 float v = 4.0 * value;
 float r = min(v - 1.5, -v + 4.5);
 float g = min(v - 0.5, -v + 3.5);
 float b = min(v + 0.5, -v + 2.5);
 return clamp( vec3(r, g, b), 0.0, 1.0 );
}


// fragment shader
void main()
{
 vec3 frag;
 
 // sample texture
 vec4 tex = texture2D(tex, uv);
 
 // invert
 if (invertMode == 1) {
  if (colorMode == 0) {
   tex.rgb = vec3(1.0) - tex.rgb;
  } else {
   tex = vec4(1.0) - tex;
  }
 }
 
 // color mode
 if (colorMode == 0) {
  if (showRamp == 1) {
   
   frag = RampColor( (tex.r+tex.g+tex.b) / 3.0 );
   
  } else {
   frag = tex.rgb;
   
   // alphaTest
   if (alphaMode == 1) {
    if (tex.a < 0.5) {
     frag = Checker();
    }
   }
   
   // alphaBlend
   if (alphaMode == 2) { 
    frag = mix(Checker(), tex.rgb, tex.a);
   }
   
   // alphaToCoverage
   if (alphaMode == 3) {
    // todo
   }
  }
 }
 if (colorMode > 0) {
  float value = 1.0;
  if (colorMode == 1) value = tex.r;
  if (colorMode == 2) value = tex.g;
  if (colorMode == 3) value = tex.b;
  if (colorMode == 4) value = tex.a;
  
  // color ramp
  if (showRamp == 1) {
   frag = RampColor( value );
  } else {
   frag = vec3( value );
  }
 }
 
 // pixel grid
 if (showGrid == 1) {
  
  // todo fade by zoom level
  float fade = 1.0;
  
  // pixel highlight
  float hilite = 0.0;
  vec2 loc = uv * gridSize;
  vec2 cur = floor(cursorPos * gridSize);
  if ( (loc.x>cur.x) && (loc.y>cur.y) && (loc.x<cur.x+1.0) && (loc.y<cur.y+1.0) ) hilite = 0.25;
  frag.rgb = mix(frag.rgb, vec3(3.0, 0.0, 0.0), hilite * fade);
  
  // grid
  vec2 p = uv * gridSize + vec2(0.5);
  vec2 f = abs( fract(p) - 0.5);
  vec2	fdx = abs( dFdx(p) );
  vec2	fdy = abs( dFdy(p) );
  vec2 df =	sqrt(fdx*fdx + fdy*fdy);
  vec2 g = clamp(f / df, 0.0, 1.0);
  float c = g.x * g.y;
  frag = mix(gridColor, frag, (c * fade + 1.0-fade )*0.4+0.6 );
 }
 
 // output
 gl_FragColor = vec4(frag,1.0);
}

