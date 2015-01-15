// Arkadiusz Gabrys qe83mepi
// Agnieszka Zacher by57zeja

#version 330
 
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec4 vTex;
layout(location = 3) in vec4 vTangent;

uniform mat4 MVP;  // updated each draw call
uniform mat4 MV;  // updated each draw call 
uniform mat3 NormalMatrix;  // updated each draw call
 
uniform vec4 LightSource;  // updated each draw call

out vec3 V;
out vec3 L;
out vec2 TexCoord;

void main() {

    vec3 n = normalize(NormalMatrix * vNormal.xyz);
    vec3 t = normalize(NormalMatrix * vTangent.xyz);
    vec3 b = cross (n, t);
    
    vec3 eyeDir = (MV * vPosition).xyz;
    vec3 lightDir = (LightSource - (MV * vPosition)).xyz;
    
    V.x = dot (eyeDir, t);
    V.y = dot (eyeDir, b);
    V.z = dot (eyeDir, n);
    V = normalize(V);

    L.x = dot (lightDir, t);
    L.y = dot (lightDir, b);
    L.z = dot (lightDir, n);
    L = normalize(L);

    TexCoord = vTex.st;

	gl_Position = MVP * vPosition;

}