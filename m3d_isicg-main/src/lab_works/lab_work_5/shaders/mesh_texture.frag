#version 450

layout( location = 0 ) out vec4 fragColor;

in vec3 inVertexNormal;
in vec3 inVertexPosition;
in vec3 inLumierPosition;
in vec2 inFragTexCoords;

uniform vec3 uDiffuseColor;
uniform vec3 uAmbientColor;
uniform vec3 uSpecularColor;
uniform float uShininess;

uniform bool uHasDiffuseMap;
layout( binding = 1 ) uniform sampler2D uDiffuseMap;

vec3 vecN;
vec4 DiffuseColor;
float theta;

void main()
{	
	vec3 vecLi = normalize(inLumierPosition - inVertexPosition);
	vec3 vecLo = normalize(vec3(0,0,0)-inVertexPosition);

	if (dot(normalize(vecLo),normalize(inVertexNormal)) < 0){
		vecN = normalize(vec3(0,0,0)-inVertexNormal);
	}
	else {
		vecN = normalize(inVertexNormal);
	}
		
	// DIFFUSE
	if  (uHasDiffuseMap){
		DiffuseColor = texture(uDiffuseMap, inFragTexCoords);
	}
	else {
		theta = dot(normalize(vecLi),vecN);
		DiffuseColor = vec4((uDiffuseColor * max(0,theta)) , 1f);
	}
	// SPECULAIRE
	vec3 specularColor = uSpecularColor * pow( max (0, cos( dot( normalize( reflect( normalize(-vecLi),vecN)), normalize(vecLo)))),uShininess) ; 

	fragColor =	vec4(uAmbientColor , 1.f ) + DiffuseColor + vec4(specularColor , 1.f );
}
