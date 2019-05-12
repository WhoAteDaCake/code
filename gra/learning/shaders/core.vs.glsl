#version 130


in vec3 vertex_position;
in vec3 vertex_color;
in vec2 vertex_texcoord;
in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

// mat4 m_inverse(mat4 matrix) {
// 	mat4 result;
// 	mat3 m = transpose(mat3(matrix));
// 	vec3 v = vec3(matrix[3]);
// 	result = mat4(m);
// 	result[3] = vec4(-m * v, 1.f);
// 	return result;
// }

void main()
{
	// Because vs_pos is v3
	// vs_position=vec4(model_matrix*vec4(vertex_position,1.f)).xyz;
	vs_position = vec3(model_matrix * vec4(vertex_position, 1.f));
	vs_color=vertex_color;
	
	// Image has to be flipped before loaded
	vs_texcoord=vec2(vertex_texcoord.x,vertex_texcoord.y*-1.f);
	
	vs_normal=mat3(model_matrix)*vertex_normal;
	
	gl_Position=projection_matrix*view_matrix*model_matrix*vec4(vertex_position,1.f);
}