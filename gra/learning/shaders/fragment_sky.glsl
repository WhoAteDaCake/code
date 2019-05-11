#version 130

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    // FragColor = textureCube(skybox, TexCoords);
  FragColor = vec4(vec3(1.f, 0.f, 0.f), 1.f);
}