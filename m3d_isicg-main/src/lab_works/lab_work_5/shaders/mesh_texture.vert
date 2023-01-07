#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

uniform mat4 uMVPMatrix; // Projection * View * Model
uniform mat4 uMVMatrix;  // Pour la oVertexPosition
uniform mat4 uNormalMatrix;

out vec3 inVertexNormal;
out vec3 inVertexPosition;
out vec3 inLumierPosition;
out vec2 inFragTexCoords;

void main()
{
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );

	vec4 normal = uNormalMatrix * normalize( vec4 (aVertexNormal, 1.f));

	inVertexNormal	= normal.xyz;
	inVertexPosition = (uMVMatrix * vec4(aVertexPosition,1.f)).xyz;
	inLumierPosition = vec3(0,0,0);
	inFragTexCoords = aVertexTexCoords;
}
