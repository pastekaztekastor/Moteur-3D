#version 450

layout( location = 0 ) out vec4 fragColor;

in vec3 inVertexNormal;
in vec3 inVertexPosition;
in vec3 inLumierPosition;

uniform vec3 uDiffuseColor;
uniform vec3 uAmbientColor;
uniform vec3 uSpecularColor;
uniform float uShininess;

vec3 vecN;
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
	theta = dot(normalize(vecLi),vecN);
	vec3 DiffuseColor = uDiffuseColor * max(0,theta);

	// SPECULAIRE
	vec3 specularColor = uSpecularColor * pow( max (0, cos( dot( normalize( reflect( normalize(-vecLi),vecN)), normalize(vecLo)))),uShininess) ; 

	fragColor =	vec4(uAmbientColor , 1.f ) + vec4(DiffuseColor , 1.f ) + vec4(specularColor , 1.f );
}
