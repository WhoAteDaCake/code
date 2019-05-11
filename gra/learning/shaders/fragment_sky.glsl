#version 130

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    // FragColor = texture(skybox, TexCoords);
  FragColor = vec4(1.f);
}