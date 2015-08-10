#version 330

// Texture
uniform sampler2D textureSampler1;
uniform bool viewable;

in vec2 texcoords;
out vec4 outColor;

void main()
{
    vec4 texel = texture(textureSampler1, texcoords);
    
    if (texel.a < 0.1f) {
        discard;
    }
    
//    outColor = texel;
    outColor = vec4(1.0f, 0.0f, 0.0f, 0.7f);
}
