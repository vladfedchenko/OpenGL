#version 330 core

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;

out vec3 vs_fs_normal;
out vec4 vs_fs_color;

void main()
{
	vs_fs_color = color;
	vs_fs_normal = mat3(view_matrix) * normal;
	gl_Position = projection_matrix * view_matrix * position;
}
