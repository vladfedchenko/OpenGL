#version 330 core

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 ToLightDir;
uniform vec3 HalfVector;
uniform float Shininess;
uniform float ReflStrength;

layout (location = 0) out vec4 color;

in vec3 vs_fs_normal;
in vec2 vs_tex_coord;

void main(void)
{
	float difuse = max(0.0, dot(vs_fs_normal, ToLightDir));
	float specular = max(0.0, dot(vs_fs_normal, HalfVector));
	
	if (difuse == 0.0)
	{
		specular = 0.0;
	}
	else
	{
		specular = pow(specular, Shininess);
	}
	
	vec3 scattered = Ambient + difuse * LightColor;
	vec3 reflected = ReflStrength * specular * LightColor;
	
	vec4 base_col = vec4(vs_tex_coord.x, vs_tex_coord.y, 0.5, 1.0);
	
	vec3 col_rgb = min((base_col.rgb * scattered + reflected), vec3(1.0));
	
    color = vec4(col_rgb, base_col.a);
    
    //color = vec4(vs_tex_coord.x, vs_tex_coord.y, 0.5, 1.0);
}
