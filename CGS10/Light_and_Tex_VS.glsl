// Arkadiusz Gabrys qe83mepi
// Agnieszka Zacher by57zeja

#version 330
 
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec4 vTex;

uniform mat4 MVP;  // updated each draw call
uniform mat4 MV;  // updated each draw call 
uniform mat3 NormalMatrix;  // updated each draw call
 
uniform vec4 LightSource;  // updated each draw call

varying vec3 N;
varying vec4 V;
varying vec2 TexCoord;

void main() { 

	// TODO:
	// reuse your phong vs code
	// interpolate tex coordinates over the surface using the provided varying

    TexCoord = vTex.st;
    N = normalize(NormalMatrix * vNormal.xyz);
	V = MV * vPosition;

	gl_Position = MVP * vPosition;
}