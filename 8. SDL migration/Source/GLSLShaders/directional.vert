#version 330 core

uniform mat4 vertex_mvp_matr;
uniform mat3 normal_transform_matr;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 in_tex_coord;

out vec3 vs_fs_normal;
out vec2 vs_tex_coord;

void main()
{
	vs_tex_coord = in_tex_coord;
	vs_fs_normal = normal_transform_matr * normal;
	gl_Position = vertex_mvp_matr * position;
}
