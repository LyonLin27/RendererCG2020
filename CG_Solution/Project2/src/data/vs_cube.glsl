#version 430

layout (location=0) in vec3 position;

uniform mat4 mv_mat;
uniform mat4 p_mat;

out vec4 _color;

void main(void)
{
	gl_Position = p_mat * mv_mat * vec4(position,1.0);
	_color = vec4(position,1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
} 
