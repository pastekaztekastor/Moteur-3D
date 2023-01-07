#version 450

layout( location = 0 ) in vec3 aVertexPosition;

out float varSortieVS;

// Je ne sais pas si il faut mettre un variable de controle

void main() {
	gl_Position = vec4(aVertexPosition, 1.f);
}
