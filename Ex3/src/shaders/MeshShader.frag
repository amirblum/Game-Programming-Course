#version 330

// Texture
uniform sampler2D textureSampler;

in vec3 myWorldPosition;
in vec2 myTexcoords;
in vec3 myNormal;
out vec4 outColor;

#define EPS (0.01)

vec3 light1Pos = vec3(-5.0f, 3.0f, -5.0f);
//vec3 light2Pos = vec3(-1.0f, -10.0f, 0.0f);
vec3 light3Pos = vec3(1.0f, 0.0f, 3.0f);

void main()
{
    vec3 l1Vector = normalize(light1Pos - myWorldPosition);
//    vec3 l2Vector = normalize(light2Pos - myWorldPosition);
    vec3 l3Vector = normalize(light3Pos - myWorldPosition);

    vec3 diffuseCoefficient = texture(textureSampler, myTexcoords).xyz;
    vec3 normal = normalize(myNormal);
    vec3 diffuseComponent = max(diffuseCoefficient * dot(l1Vector, normal), 0.0f);
//    diffuseComponent += max(diffuseCoefficient * dot(l2Vector, normal), 0.0f);
    diffuseComponent += max(diffuseCoefficient * dot(l3Vector, normal), 0.0f);

    outColor = vec4(diffuseComponent, 1.0f);
}
