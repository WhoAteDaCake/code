#version 130

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuse_tex;
	sampler2D specular_tex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;


uniform sampler2D texture0;
uniform vec3 light_pos0;
uniform vec3 camera_pos;

void main()
{
	// Ambient (natural light)
	vec3 ambient=material.ambient;
	// Diffuse
	vec3 diffuse_color=vec3(1.f,1.f,1.f);
	vec3 pos_to_light=normalize(vs_position-light_pos0);
	float dmod=clamp(dot(pos_to_light,vs_normal),0.f,1.f);
	vec3 diffuse=material.diffuse*dmod;
	
	// Specular
	vec3 light_to_pos=normalize(light_pos0-vs_position);
	vec3 reflect_dir=normalize(reflect(light_to_pos,normalize(vs_normal)));
	vec3 pos_to_view=normalize(vs_position-camera_pos);
	float specular_const=pow(max(dot(pos_to_view,reflect_dir),0),30);
	vec3 specular=material.specular*specular_const;
	// Attenuation
	
	// Output
	vec4 light_final=vec4(ambient,1.f)+vec4(diffuse,1.f)+vec4(specular,1.f);
	fs_color=texture(material.diffuse_tex,vs_texcoord)*vec4(vs_color,1.f)*light_final;
}