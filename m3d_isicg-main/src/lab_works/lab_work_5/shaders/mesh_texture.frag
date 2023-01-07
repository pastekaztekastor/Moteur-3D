#version 450

layout( location = 0 ) out vec4 fragColor;

in vec3 inVertexNormal;
in vec3 inVertexPosition;
in vec3 inLumierPosition;
in vec2 inFragTexCoords;

uniform float uShininess;

uniform vec3 uDiffuseColor;
uniform bool uHasDiffuseMap;
layout( binding = 1 ) uniform sampler2D uDiffuseMap;
vec4 diffuseColor;

uniform vec3 uAmbientColor;
uniform bool uHasAmbientMap;
layout( binding = 2 ) uniform sampler2D uAmbientMap;
vec4 ambientColor;

uniform vec3 uSpecularColor;
uniform bool uHasSpecularMap;
layout( binding = 3 ) uniform sampler2D uSpecularMap;
vec4 specularColor;


void main()
{	
	vec3 vecLi = normalize(inLumierPosition - inVertexPosition);
	vec3 vecLo = normalize(vec3(0,0,0)-inVertexPosition);
	vec3 vecN;

	if (dot(normalize(vecLo),normalize(inVertexNormal)) < 0){
		vecN = normalize(vec3(0,0,0)-inVertexNormal);
	}
	else {
		vecN = normalize(inVertexNormal);
	}

	// AMBIENT
	if  (uHasAmbientMap){
		ambientColor = texture(uAmbientMap, inFragTexCoords);
	}
	else {
		ambientColor = vec4(uAmbientColor , 1f);
	}

	// DIFFUSE
	float theta;
	if  (uHasDiffuseMap){
		diffuseColor = texture(uDiffuseMap, inFragTexCoords);
	}
	else {
		theta = dot(normalize(vecLi),vecN);
		diffuseColor = vec4((uDiffuseColor * max(0,theta)) , 1f);
	}

	// SPECULAIRE
	if  (uHasSpecularMap){
		specularColor = texture(uSpecularMap, inFragTexCoords);
	}
	else {
		specularColor = vec4(uSpecularColor * pow( max (0, cos( dot( normalize( reflect( normalize(-vecLi),vecN)), normalize(vecLo)))),uShininess),1f) ; 
	}


	fragColor =	ambientColor + diffuseColor + specularColor;
}
