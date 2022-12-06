#version 450

in vec3 color;

layout ( location = 0 ) out vec4 fragColor;

uniform float uAlphaCanal;

void main() {
	fragColor = vec4(color.x*uAlphaCanal, color.y*uAlphaCanal, color.z*uAlphaCanal ,1.f);
}
