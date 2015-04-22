#version 330

// Glow
uniform sampler2D glowTexture;
uniform bool applyGlow;

// Convolution
uniform float uStep;
uniform float vStep;

uniform mat3 ppKernel;

// PP
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
    
    if (applyGlow) {
        vec3 glow = vec3(0.0f);
        
        // Compute basic box blur of glowTexture
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                // Get corresponding texture data
                vec2 sampleUV = UV + vec2(uStep * (i - 1), vStep * (j - 1));
                // Edge handling (extend)
                sampleUV.x = clamp(sampleUV.x, 0.0f, 1.0f);
                sampleUV.y = clamp(sampleUV.y, 0.0f, 1.0f);
                // Sample
                vec3 textureSample = texture(glowTexture, sampleUV).xyz;
                
                // Convolute
                glow += textureSample / 9;
            }
        }
        
        // Only apply if there is something to add
        if (dot(glow, glow) > 0) {
            outColor = outColor * 0.9 + glow * 0.1;
        }
    }
    
	color = outColor;
}