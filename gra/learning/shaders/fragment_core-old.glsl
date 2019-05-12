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

struct Light {
	  /**
   * 0 - Directional NOT_DONE
   * 1 - Point light NOT_DONE
   * 2 - Spotlight NOT_DONE
   */
	int type;

  vec3 position;
	vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

	// For point light
	float constant;
  float linear;
  float quadratic;
	// For spotlight
	float cut_off;
};

uniform Light light; 

uniform Material material;
// Because 130 doesn't support samplers inside structs
uniform sampler2D mat_diffuse_tex;
uniform sampler2D mat_specular_tex;
// Sometimes we might not to supply both of these textures
uniform int mat_has_diffuse;
uniform int mas_has_specular;

uniform int show_color;

uniform vec3 camera_pos;

float get_attenuation(Light light) {
	float dist = length(light.position - vs_position);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));   
	return attenuation; 
}

vec3 get_ambient(Material material, Light light) {
	return material.ambient * light.ambient;
}

vec3 get_diffuse(Material material, Light light, vec3 position, vec3 normal) {
	if (light.type == 2) {
		vec3 light_dir = normalize(light.position- position);
		vec3 norm = normalize(normal);
		float diff = max(dot(norm, light_dir), 0.f);
		// TODO check if need to modify by material diffuse
		return material.diffuse * light.diffuse * diff;
	}

	vec3 pos_to_light;
	if (light.type == 1){
		pos_to_light = normalize(light.position - position);
	} else if (light.type == 0) {
		pos_to_light = normalize(-light.direction);
	}
	// TODO check what to do for type 2
	float dmod=clamp(dot(pos_to_light,normal),0.f,1.f);
	vec3 diffuse=material.diffuse* light.diffuse * dmod;
	return diffuse;
}

vec3 get_specular(Material material, Light light,vec3 position, vec3 normal, vec3 camera_pos) {
	vec3 reflect_dir;
	float shiness = 32;

	if (light.type == 1) {
		vec3 light_to_pos=normalize(position - light.position);
		reflect_dir = normalize(reflect(light_to_pos,normalize(normal)));
	} else if (light.type == 0) {
		reflect_dir = normalize(reflect(-light.direction, normal));
	} else if (light.type == 2) {
		// TODO, check if can reuse type 0
		reflect_dir = reflect(-light.direction, normal);
	}

	vec3 pos_to_view=normalize(camera_pos - position);
	float specular_const=pow(max(dot(pos_to_view,reflect_dir),0),shiness);
	// And check for optional specular textures
	vec3 tex_color = vec3(1.f);
	if (mas_has_specular == 1) {
		tex_color = texture(mat_specular_tex, vs_texcoord).rgb;
	}

	vec3 specular=material.specular*specular_const * tex_color * light.specular;
	return specular;
}

void main()
{
	vec3 texture_color = vec3(1.f);
	if (mat_has_diffuse == 1) {
		texture_color = texture(mat_diffuse_tex,vs_texcoord).rgb;
	}
	vec4 color = vec4(1.f);
	if (show_color == 1) {
		color = vec4(vs_color,1.f);
	}

	// Output
	// vec4 light_final=
	// TODO check if ambient should be multplied by tex_color
	vec3 ambient = get_ambient(material, light);
	vec3 diffuse = get_diffuse(material, light, vs_position, vs_normal);
	vec3 specular = get_specular(material, light,vs_position, vs_normal, camera_pos);

	if (light.type == 1) {
		float att = get_attenuation(light);
		// ambient *= att;
		diffuse *= att;
		specular *= att;
	}
	if (light.type == 2) {
		vec3 light_to_pos=normalize(vs_position - light.position);
		float theta = dot(light.direction, light_to_pos);

		if (theta > light.cut_off) {
			float att = get_attenuation(light);
			// ambient *= att;
			diffuse *= att;
			specular *= att;
		} else {
			/*
				Null specular and diffuse.
				We will only use ambient
				*/
			diffuse = vec3(0.f);
			specular = vec3(0.f);
			ambient *= texture_color;
		}

	}

	vec4 light_final = vec4(ambient + diffuse + specular, 1.f);
	fs_color= color * light_final * vec4(texture_color, 1.f);
}