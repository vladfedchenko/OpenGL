#version 330 core

uniform sampler2D text1;

layout (location = 0) out vec4 color;

in vec2 vs_tex_coord;

void main(void)
{	
    vec4 base_color = texture(text1, vs_tex_coord);
    //if (base_color.a == 0)
    //{
    //	color = vec4(1.0, 0.0, 0.0, 1.0);
    //}
    //else
    //{
    //	color = vec4(0.0, 0.0, 1.0, 1.0);
    //}
    color = base_color;
    //color = vec4(vs_tex_coord, 0.5, 1.0);
}
