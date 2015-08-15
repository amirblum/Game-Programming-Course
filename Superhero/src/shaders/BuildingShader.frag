#version 330

// Texture
uniform samplerCube cubeSampler1;

in vec3 texcoords;

out vec4 outColor;

void main()
{
    outColor = texture(cubeSampler1, texcoords);
//    outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
