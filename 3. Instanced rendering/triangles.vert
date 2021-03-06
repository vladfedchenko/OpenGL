#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out vec4 vs_fs_color;

void main()
{
	vs_fs_color = color;
	gl_Position = vec4(position.x + (gl_InstanceID * (0.8 / 3.0)), position.y, position.z, position.w);
}
