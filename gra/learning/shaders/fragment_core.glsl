#version 130

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture0;
uniform vec3 light_pos0;

void main()
{
	// Ambient (natural light)
	vec3 ambient=vec3(.1f,.1f,.1f);
	// Diffuse
	vec3 diffuse_color=vec3(1.f,1.f,1.f);
	vec3 pos_to_light=normalize(vs_position-light_pos0);
	float dmod=clamp(dot(pos_to_light,vs_normal),0.f,1.f);
	vec3 diffuse=diffuse_color*dmod;
	
	vec4 light_final=vec4(ambient,1.f)+vec4(diffuse,1.f);
	fs_color=texture(texture0,vs_texcoord)*vec4(vs_color,1.f)*light_final;
}