// Arkadiusz Gabrys qe83mepi
// Agnieszka Zacher by57zeja

#version 120

varying float height;
varying vec3 v_normal;
varying vec3 v_viewingVec;

void main() {

 	/**
		HINT: You can use your own sun-shaders from previous tasks!
	*/

    vec4 yellow = vec4(1.0, (1.0 - dot(v_normal, v_viewingVec)), 0.0, 1.0);

    vec4 color = vec4(1.0, height, 0.0, 1.0);
 
    gl_FragColor = color + 0.8 * yellow;
}
