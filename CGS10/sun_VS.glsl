// Arkadiusz Gabrys qe83mepi
// Agnieszka Zacher by57zeja

#version 120

uniform float Time;  // updated each frame by the application

uniform mat4 MVP;  // updated each draw call
uniform mat4 MV;  // updated each draw call
uniform mat3 NormalMatrix;  // updated each draw call

varying float height; // handed to the fragment shader
varying vec3 v_normal;
varying vec3 v_viewingVec;

float rand(vec2 n)
{
    return 0.5 + 0.5 * fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453) * 100;
}

void main()
{
	/**
		HINT: You can use your own sun-shaders from previous tasks!
	*/
 
    v_normal = normalize(NormalMatrix * gl_Normal);
    v_viewingVec = normalize(vec4(0.0) - (MV * gl_Vertex)).xyz;

    vec3 shiftTmp = gl_Normal * sin(rand(gl_Normal.xy) + Time/50) * 1.2;
    vec4 phase = abs(vec4(shiftTmp.x, shiftTmp.y, shiftTmp.z, 0.0));
    height = length(phase);
 
    gl_Position = MVP * (gl_Vertex + phase);
}
