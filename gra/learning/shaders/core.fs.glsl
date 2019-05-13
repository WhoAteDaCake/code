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

  // Sometimes we might not to supply both of these textures
  bool has_diffuse;
  bool has_specular;
  bool show_color;
  bool ignore_light;
	float shininess;

};

struct Light {
	  /**
   * 0 - Directional
   * 1 - Point light
   * 2 - Spotlight
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

#define LIGHT_COUNT 5

uniform Light lights[LIGHT_COUNT];

uniform Material material;
// Because 130 doesn't support samplers inside structs
uniform sampler2D mat_diffuse_tex;
uniform sampler2D mat_specular_tex;

uniform vec3 camera_pos;

vec3 get_diffuse_tex();
vec3 get_specular_tex();
vec3 calc_dir_light(Light light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(Light light, vec3 normal, vec3 view_dir);
vec3 calc_spot_light(Light light, vec3 normal, vec3 view_dir);

void main() {
	vec3 normal = normalize(vs_normal);
	vec3 view_dir = normalize(camera_pos - vs_position);
	vec3 result = vec3(0.f, 0.f, 0.f);
  if (!material.ignore_light) {
    for (int i = 0; i < LIGHT_COUNT; i += 1) {
      Light light = lights[i];
      if (light.type == 0) {
      	result += calc_dir_light(light, normal, view_dir);
      } else if (light.type == 1) {
      	result += calc_point_light(light, normal, view_dir);
      } else if (light.type == 2) {
      	result += calc_spot_light(light, normal, view_dir);
      }
    }
  } else {
    result = vec3(1.f);
  }
	

  vec3 color = vec3(1.f);
  if (material.show_color) {
    color = vs_color;
  }

	fs_color = vec4(result * vs_color, 1.f);
}

vec3 get_diffuse_tex() {
	if (material.has_diffuse) {
		return texture(mat_diffuse_tex, vs_texcoord).rgb;
	} else {
		return vec3(1.f, 1.f, 1.f);
	}
}

vec3 get_specular_tex() {
	if (material.has_specular) {
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
  float spec = pow(max(dot(normal, reflect_dir), 0.0), material.shininess);
  // combine results
  vec3 ambient = material.ambient * light.ambient * get_diffuse_tex();
  vec3 diffuse = material.diffuse * light.diffuse * diff * get_diffuse_tex();
  vec3 specular = material.specular * light.specular * spec * get_specular_tex();
  return ambient + diffuse + specular;
  // return vec3(1.f, 1.f, 1.f);
}

vec3 calc_point_light(Light light, vec3 normal, vec3 view_dir) {
  vec3 light_dir = normalize(light.position - vs_position);
  // diffuse shading
  float diff = max(dot(normal, light_dir), 0.0);
  // specular shading
  vec3 reflect_dir = reflect(-light_dir, normal);
  // Change dot(normal) to view_dir if you want to follow the camera;
  float spec = pow(max(dot(normal, reflect_dir), 0.0), material.shininess);
  // attenuation
  float distance = length(light.position - vs_position);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
  // combine results
  vec3 ambient = material.ambient * light.ambient * get_diffuse_tex();
  vec3 diffuse = material.diffuse * light.diffuse * diff * get_diffuse_tex();
  vec3 specular = material.specular * light.specular * spec * get_specular_tex();
  // TODO re-enable when we have multiple lights
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  // return vec3(1.f, 0.f, 0.f);
  return (ambient + diffuse + specular);
}


vec3 calc_spot_light(Light light, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(light.position - vs_position);
  // diffuse shading
  float diff = max(dot(normal, light_dir), 0.0);
  // specular shading
  vec3 reflect_dir = reflect(-light_dir, normal);
  // Change to view dir to follow camera
  float spec = pow(max(dot(normal, reflect_dir), 0.0), material.shininess);
  // attenuation
  float distance = length(light.position - vs_position);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
  // spotlight intensity
  float theta = dot(light_dir, normalize(-light.direction)); 
  float epsilon = light.cut_off - light.outer_cut_off;
  float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
  // combine results
  vec3 ambient = material.ambient * light.ambient * get_diffuse_tex();
  vec3 diffuse = material.diffuse * light.diffuse * diff * get_diffuse_tex();
  vec3 specular = material.specular * light.specular * spec * get_specular_tex();
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  return (ambient + diffuse + specular);
}