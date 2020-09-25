#version 430

out vec4 color;

void main(void)
{
    float x = gl_FragCoord.z;//(gl_FragCoord.z+0.8)/1.5;
    color = vec4(x, x, x, 1.0);
}