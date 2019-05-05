#version 130
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;


struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
// Because 130 doesn't support samplers inside structs
uniform sampler2D mat_diffuse_tex;
uniform sampler2D mat_specular_tex;


uniform vec3 light_pos0;
uniform vec3 camera_pos;

vec4 get_ambient(Material material) {
	return vec4(material.ambient, 1.f);
}

vec4 get_diffuse(Material material, vec3 position, vec3 normal, vec3 light_pos) {
	vec3 pos_to_light=normalize(position-light_pos);
	float dmod=clamp(dot(pos_to_light,normal),0.f,1.f);
	vec3 diffuse=material.diffuse*dmod;
	return vec4(diffuse, 1.f);
}

vec4 get_specular(Material material, vec3 position, vec3 normal, vec3 light_pos, vec3 camera_pos) {
	vec3 light_to_pos=normalize(light_pos-position);
	vec3 reflect_dir=normalize(reflect(light_to_pos,normalize(normal)));
	vec3 pos_to_view=normalize(position-camera_pos);
	float specular_const=pow(max(dot(pos_to_view,reflect_dir),0),30);
	vec3 specular=material.specular*specular_const;
	return vec4(specular, 1.f);
}

void main()
{

	// Output
	vec4 light_final=
		get_ambient(material)+
		get_diffuse(material, vs_position, vs_normal, light_pos0)+
		get_specular(material, vs_position, vs_normal, light_pos0, camera_pos);
	fs_color=texture(mat_diffuse_tex,vs_texcoord)* vec4(vs_color,1.f)*light_final;
	// fs_color=texture(texture0,vs_texcoord)*light_final;
}