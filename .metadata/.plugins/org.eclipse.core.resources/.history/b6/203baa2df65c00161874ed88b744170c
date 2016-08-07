#version 330 core

uniform sampler2D tex;
in vec2 vs_tex_coord;

layout (location = 0) out vec4 color;

void main(void)
{
    color = texture(tex, vs_tex_coord);
    //color = vec4(vs_tex_coord.x, vs_tex_coord.y, 0.0, 1.0);
}
