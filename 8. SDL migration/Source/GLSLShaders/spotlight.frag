#version 330 core

uniform sampler2D tex;
uniform bool use_texture;

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform float Shininess;
uniform float ReflStrength;

uniform vec3 EyeDirection;
uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;
uniform float DistanceScaleCoof;

uniform vec3 ConeDirection;
uniform float SpotCosCutoff;
uniform float SpotExponent;

layout (location = 0) out vec4 color;

in vec3 vs_fs_normal;
in vec2 vs_tex_coord;
in vec4 world_pos;

void main(void)
{
	vec3 lightDirection = LightPosition - vec3(world_pos);
	float lightDistance = length(lightDirection);
	lightDirection = lightDirection / lightDistance;
	lightDistance *= DistanceScaleCoof;
	
	float spotCos = dot(lightDirection, -ConeDirection);
	
	float attenuation = 0.0;
	if (spotCos > SpotCosCutoff)
	{
		attenuation = 1.0 /
			(ConstantAttenuation + 
			LinearAttenuation * lightDistance +
			QuadraticAttenuation * lightDistance * lightDistance);
		attenuation *= pow(spotCos, SpotExponent);
	}
		
	vec3 halfVector = normalize(lightDirection + EyeDirection);

	float difuse = max(0.0, dot(vs_fs_normal, lightDirection));
	float specular = max(0.0, dot(vs_fs_normal, halfVector));
	
	if (difuse == 0.0)
	{
		specular = 0.0;
	}
	else
	{
		specular = pow(specular, Shininess);
	}
	
	vec3 scattered = Ambient + attenuation * difuse * LightColor;
	vec3 reflected = ReflStrength * attenuation * specular * LightColor;
	
	vec4 base_col;
	if (use_texture)
	{
		base_col = texture(tex, vs_tex_coord);
	}
	else
	{
		base_col = vec4(vs_tex_coord.x, vs_tex_coord.y, 0.5, 1.0);
	}
	
	vec3 col_rgb = min((base_col.rgb * scattered + reflected), vec3(1.0));
	
    color = vec4(col_rgb, base_col.a);
}
