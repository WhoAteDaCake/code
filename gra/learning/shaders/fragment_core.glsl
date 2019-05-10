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
// Sometimes we might not to supply both of these textures
uniform int mat_has_diffuse;
uniform int mas_has_specular;

uniform int show_color;


uniform vec3 light_pos0;
uniform vec3 camera_pos;

vec4 get_ambient(Material material) {
	return vec4(material.ambient, 1.f);
}

vec4 get_diffuse(Material material, vec3 position, vec3 normal, vec3 light_pos) {
	vec3 pos_to_light=normalize(light_pos - position);
	float dmod=clamp(dot(pos_to_light,normal),0.f,1.f);
	vec3 diffuse=material.diffuse*dmod;
	return vec4(diffuse, 1.f);
}

vec4 get_specular(Material material, vec3 position, vec3 normal, vec3 light_pos, vec3 camera_pos) {
	vec3 light_to_pos=normalize(position - light_pos);
	vec3 reflect_dir=normalize(reflect(light_to_pos,normalize(normal)));
	vec3 pos_to_view=normalize(camera_pos - position);
	float specular_const=pow(max(dot(pos_to_view,reflect_dir),0),30);
	// TODO use light color
	// And check for optional specular textures
	vec3 tex_color = vec3(1.f);
	if (mas_has_specular == 1) {
		tex_color = texture(mat_specular_tex, vs_texcoord).rgb;
	}

	vec3 specular=material.specular*specular_const * tex_color;
	return vec4(specular, 1.f);
}

void main()
{
	vec4 texture_color = vec4(1.f);
	if (mat_has_diffuse == 1) {
		texture_color = texture(mat_diffuse_tex,vs_texcoord);
	}
	vec4 color = vec4(1.f);
	if (show_color == 1) {
		color = vec4(vs_color,1.f);
	}

	// fs_color = vec4(vec3(1.f, 0.f, 0.f), 1.f);
	// Output
	vec4 light_final=
		get_ambient(material)+
		get_diffuse(material, vs_position, vs_normal, light_pos0)+
		get_specular(material, vs_position, vs_normal, light_pos0, camera_pos);
	fs_color=
		texture_color *
		color*
		light_final;
}