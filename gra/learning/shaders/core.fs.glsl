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
	float shininess;
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
	float outer_cut_off;
};

uniform Light light; 

uniform Material material;
// Because 130 doesn't support samplers inside structs
uniform sampler2D mat_diffuse_tex;
uniform sampler2D mat_specular_tex;
// Sometimes we might not to supply both of these textures
uniform int mat_has_diffuse;
uniform int mat_has_specular;

uniform int show_color;

uniform vec3 camera_pos;

vec3 get_diffuse_tex();
vec3 get_specular_tex();
vec3 calc_dir_light(Light light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(Light light, vec3 normal, vec3 view_dir);
vec3 calc_spot_light(Light light, vec3 normal, vec3 view_dir);

void main() {
	vec3 normal = normalize(vs_normal);
	vec3 view_dir = normalize(camera_pos - vs_position);
	vec3 result;
	if (light.type == 0) {
		result = calc_dir_light(light, normal, view_dir);
	} else if (light.type == 1) {
		result = calc_point_light(light, normal, view_dir);
	} else if (light.type == 2) {
		result = calc_spot_light(light, normal, view_dir);
	}
	fs_color = vec4(result, 1.f);
}

vec3 get_diffuse_tex() {
	if (mat_has_diffuse == 1) {
		return texture(mat_diffuse_tex, vs_texcoord).rgb;
	} else {
		return vec3(1.f, 1.f, 1.f);
	}
}

vec3 get_specular_tex() {
	if (mat_has_specular == 1) {
		return texture(mat_specular_tex, vs_texcoord).rgb;
	} else {
		return vec3(1.f, 1.f, 1.f);
	}
}

vec3 calc_dir_light(Light light, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(-light.direction);
  // diffuse shading
  float diff = clamp(dot(normal, light_dir), 0.0, 1.f);
  // specular shading
  vec3 reflect_dir = normalize(reflect(-light_dir, normal));
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  // combine results
  vec3 ambient = light.ambient;
  vec3 diffuse = light.diffuse * diff * get_diffuse_tex();
  vec3 specular = light.specular * spec * get_specular_tex();
  return ambient + diffuse + specular;
}

vec3 calc_point_light(Light light, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(light.position - vs_position);
  // diffuse shading
  float diff = max(dot(normal, light_dir), 0.0);
  // specular shading
  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  // attenuation
  float distance = length(light.position - vs_position);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
  // combine results
  vec3 ambient = light.ambient * get_diffuse_tex();
  vec3 diffuse = light.diffuse * diff * get_diffuse_tex();
  vec3 specular = light.specular * spec * get_specular_tex();
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}


vec3 calc_spot_light(Light light, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(light.position - vs_position);
  // diffuse shading
  float diff = max(dot(normal, light_dir), 0.0);
  // specular shading
  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  // attenuation
  float distance = length(light.position - vs_position);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
  // spotlight intensity
  float theta = dot(light_dir, normalize(-light.direction)); 
  float epsilon = light.cut_off - light.outer_cut_off;
  float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
  // combine results
  vec3 ambient = light.ambient * get_diffuse_tex();
  vec3 diffuse = light.diffuse * diff * get_diffuse_tex();
  vec3 specular = light.specular * spec * get_specular_tex();
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  return (ambient + diffuse + specular);
}