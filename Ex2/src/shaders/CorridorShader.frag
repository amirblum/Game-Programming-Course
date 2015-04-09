#version 330

// Texture
uniform samplerCube textureSampler;
uniform samplerCube bumpSampler;
uniform float texturePercent;

// Bump
uniform bool bumpMappingOn;

// Light
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform float lightCutoff;
uniform float lightDarkenStart;
uniform float lightDarkenEnd;

in vec3 myWorldPosition;
in vec3 texcoords;
out vec4 outColor;

#define EPS (0.01)
#define DARKNESS_COMPONENT (0.25f)
#define BUMP_SCALE (2.0f)

void main()
{
    // Set correct sample coords
    vec3 sampleCoords;
    {
        sampleCoords = texcoords;
        float texZ = sampleCoords.z;
        
        // First handle overflow, so (-1.5,1.5)->(-0.5,0.5)
        if (texZ > 0.5f) {
            texZ -= 1.0f;
        } else if (texZ < -0.5f) {
            texZ += 1.0f;
        }
        
        // Now bring to (0,1) scale
        float texZDistance = texZ + 0.5f;
        
        // Basically modulo...just don't know if modulo works with floats and don't feel like finding out
        int texturePatternNum = int(texZDistance / texturePercent);
        float offset = texZDistance - (texturePercent * texturePatternNum);
        
        // Bring back into (-0.5,0.5) coords
        offset = (offset / texturePercent) - 0.5f;
        sampleCoords.z = offset;
    }
    
    // Calculate normal
    vec3 normal = vec3(0.0f);
    {
        float texturePixelRatio = 1 / 512.0f;
        vec3 right = vec3(1.0f, 0.0f, 0.0f);
        vec3 up = vec3(0.0f, 1.0f, 0.0f);
        vec3 forward = vec3(0.0f, 0.0f, 1.0f);
        
        // First get face normal (without bump)
        float magX = abs(sampleCoords.x);
        float magY = abs(sampleCoords.y);
        float magZ = abs(sampleCoords.z);
        
        vec3 bumpRotation = vec3(1.0f);
        
        if (magX > magY && magX > magZ) {
            normal = right;
            if (sampleCoords.x > 0) {
                normal *= -1.0f;
            }
            
            float heightCurrent = texture(bumpSampler, sampleCoords).x;
            float uHeightAfter = texture(bumpSampler, sampleCoords + forward * texturePixelRatio).x;
            float vHeightAfter = texture(bumpSampler, sampleCoords + up * texturePixelRatio).x;
            
            float du = uHeightAfter - heightCurrent;
            float dv = vHeightAfter - heightCurrent;
            
            bumpRotation = vec3(0.0f, dv, du);
        } else if (magY > magX && magY > magZ) {
            normal = up;
            if (sampleCoords.y > 0) {
                normal *= -1.0f;
            }
            
            float heightCurrent = texture(bumpSampler, sampleCoords).x;
            float uHeightAfter = texture(bumpSampler, sampleCoords + forward * texturePixelRatio).x;
            float vHeightAfter = texture(bumpSampler, sampleCoords + right * texturePixelRatio).x;
            
            float du = uHeightAfter - heightCurrent;
            float dv = vHeightAfter - heightCurrent;
            
            bumpRotation = vec3(dv, 0.0f, du);
        }
        
        if (bumpMappingOn) {
            normal = normalize(normal - bumpRotation * BUMP_SCALE);
        }
    }
    
    // Calculate darkening:
    float darkenRatio;
    {
        darkenRatio = DARKNESS_COMPONENT;
        if (-myWorldPosition.z > lightDarkenStart) {
            // Start darkening more after lightDarkenStart
            float darkenSectionLength = lightDarkenEnd - lightDarkenStart;
            float distanceIntoDarkenSection = -myWorldPosition.z - lightDarkenStart;
            darkenRatio *= exp(-((distanceIntoDarkenSection / darkenSectionLength) * 2));
        }
        
        // Check if visible by flashlight
        {
            // Get vector to light
            vec3 vecToLight = myWorldPosition - lightPos;
            
            // Check angle
            vec3 lightDirN = normalize(lightDir);
            vec3 vecToLightN = normalize(vecToLight);
            
            float cosAngle = dot(vecToLightN, lightDirN);
            
            if (cosAngle > lightCutoff) {
                // In flashlight, reset darkenRatio. Don't use DARKNESS_COMPONENT, instead start it a little brighter
                float defaultDarkenRatio = darkenRatio;
                darkenRatio = 2.5f;
                
                // Start darkening from halfway to lightDarkenStart
                if (-myWorldPosition.z > lightDarkenStart/2) {
                    float darkenSectionLength = lightDarkenEnd - lightDarkenStart/2;
                    float distanceIntoDarkenSection = -myWorldPosition.z - lightDarkenStart/2;
                    darkenRatio *= exp(-((distanceIntoDarkenSection / darkenSectionLength) * 6));
                }
                
                // Soften edges of light + achieve stronger-round-the-center flashlight effect
                if (cosAngle < lightCutoff + EPS) {
                    darkenRatio *= (cosAngle - lightCutoff) / (EPS);
                }
                
                // Don't darken more than the default
                darkenRatio = max(defaultDarkenRatio, darkenRatio);
            }
        }
    }
    
    vec3 lVector = normalize(lightPos - myWorldPosition);
    
    vec3 diffuseCoefficient = texture(textureSampler, sampleCoords).xyz;
    vec3 diffuseComponent = max(diffuseCoefficient * dot(lVector, normal), 0);
    
    outColor = vec4(diffuseComponent, 1.0f) * darkenRatio;
}
