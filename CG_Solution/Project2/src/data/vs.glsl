#version 430

uniform float offset;

mat4 buildRotateX(float rad){
  mat4 xrot = mat4( 1.0,      0.0,       0.0, 0.0, 
                    0.0, cos(rad), -sin(rad), 0.0, 
                    0.0, sin(rad),  cos(rad), 0.0, 
                    0.0,       0.0,      0.0, 1.0);

  return xrot;
}

mat4 buildRotateZ(float rad){
  mat4 zrot = mat4( cos(rad), -sin(rad), 0.0, 0.0, 
                    sin(rad),  cos(rad), 0.0, 0.0, 
                    0.0,       0.0,      1.0, 0.0, 
                    0.0,       0.0,      0.0, 1.0);

  return zrot;
}

void main(void){
  mat4 rotz = buildRotateZ(10.0 * cos(offset*3.0));
  mat4 rotx = buildRotateX(10.0 * cos(offset*3.0));
  mat4 rot45 = rotz * rotx;
  if (gl_VertexID == 0) gl_Position = rot45*vec4( 0.25,-0.25, 0.0, 1.0);
  else if (gl_VertexID == 1) gl_Position = rot45*vec4(-0.25,-0.25, 0.0, 1.0);
  else gl_Position = rot45*vec4( 0.25, 0.25, 0.0, 1.0);
}