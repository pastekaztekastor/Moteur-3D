#version 450

in vec3 color;

layout ( location = 0 ) out vec4 fragColor;

void main() {
	fragColor = vec4(color.x, color.y, color.z ,1.f);
}
