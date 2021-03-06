#version 330 core

uniform mat4 mvp_matrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 in_tex_coord;

out vec2 vs_tex_coord;

void main()
{
	gl_Position = mvp_matrix * position;
	vs_tex_coord = in_tex_coord;
}
