#version 330

in vec2 UV;

out vec3 color;

uniform sampler2D fbo_texture;

void main() {
	color = texture(fbo_texture, UV).xyz ;
}