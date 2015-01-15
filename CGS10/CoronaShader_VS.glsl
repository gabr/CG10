// Arkadiusz Gabrys qe83mepi
// Agnieszka Zacher by57zeja

#version 330
 
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec4 vTex;

layout(location = 3) in vec4 vColor; // vertex based Color (no uniform!)

uniform mat4 P;
uniform mat4 MV;
uniform mat4 MVP;  // updated each draw call
 
out vec4 Color;



void main() 
{
    float l = length(MV * vec4(0.0, 0.0, 0.0, 0.0));
    gl_Position = P * (MV * vec4(0.0, 0.0, 0.0, 1.0) + vec4(vPosition.x, vPosition.y, l, 0.0));

	Color = vColor;
}