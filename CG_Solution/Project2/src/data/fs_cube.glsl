#version 430

in vec4 _color;
out vec4 color;

uniform mat4 mv_mat;
uniform mat4 p_mat;

void main(void)
{	color = _color;
}
