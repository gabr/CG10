// Arkadiusz Gabrys qe83mepi
// Agnieszka Zacher by57zeja

#version 330

uniform vec4 Color;  // updated each draw call
uniform sampler2D Texture;
uniform sampler2D Mask;
uniform sampler2D Texture_Night;
uniform sampler2D Texture_Normal;

in vec3 V;
in vec3 L;
in vec2 TexCoord;

uniform vec4 LightSource;  // updated each draw call
 
void main() {

    vec3 N = normalize(texture2D(Texture_Normal, TexCoord).rgb * 2.0 - 1.0);
    vec3 R = reflect(L, N);

    // ambient from texture
    vec4 ambient = texture2D(Texture, TexCoord);
    vec4 mask = texture2D(Mask, TexCoord);
    vec4 night = texture2D(Texture_Night, TexCoord);

    // diff
    vec4 diff;
    float dotPr = dot(N, L);
    if (dotPr < 0.0)
    {
        diff = night * -dotPr * 0.8;
    }
    else
    {
        diff = ambient * dotPr;
    }

    // spec
    float s = 10;
    dotPr = dot(V, R);
    if (dotPr < 0.0)
    {
        dotPr = 0.0;
    }

    vec4 spec = vec4(1.0) * pow(dotPr, s);

    vec4 color =  diff; // no ambient for plantes

    if (mask.x > 0.0)
    {
        color += 0.5 * spec;
    }

    gl_FragColor = color;

}