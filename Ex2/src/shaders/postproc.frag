#version 330

uniform float uStep;
uniform float vStep;

uniform mat3 ppKernel;

in vec2 UV;
out vec3 color;

uniform sampler2D fbo_texture;

void main() {
    vec3 outColor = vec3(0.0f);
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            // Get corresponding texture data
            vec2 sampleUV = UV + vec2(uStep * (i - 1), vStep * (j - 1));
            // Edge handling (extend)
            sampleUV.x = clamp(sampleUV.x, 0.0f, 1.0f);
            sampleUV.y = clamp(sampleUV.y, 0.0f, 1.0f);
            // Sample
            vec3 textureSample = texture(fbo_texture, sampleUV).xyz;
            
            // Convolute
            outColor += ppKernel[i][j] * textureSample;
        }
    }
    
	color = outColor;    
}