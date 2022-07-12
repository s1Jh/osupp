#shader vertex
#version 330 core
layout (location=0) in vec2 aPos;

uniform mat3 camera;
uniform mat3 transform;
uniform mat3 shape;

uniform float z;

void main()
{
    gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);
    gl_Position.z = z;
}

    #shader fragment
    #version 330 core

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform vec2 resolution;

uniform vec2 A;
uniform vec2 B;

uniform mat3 camera;
uniform mat3 transform;

uniform float thickness;
uniform vec4 fill;

out vec4 FragColor;

float aspectRatio = resolution.x / resolution.y;

float drawLine(vec2 p1, vec2 p2, float thickness) {
    vec2 uv = gl_FragCoord.xy / resolution.xy;

    uv *= 2.0f;
    uv -= 1.0f;

    float a = abs(distance(p1, uv));
    float b = abs(distance(p2, uv));
    float c = abs(distance(p1, p2));

    if (a >= c || b >= c) return 0.0;

    float p = (a + b + c) * 0.5;

    // median to (p1, p2) vector
    float h = 2 / c * sqrt(p * (p - a) * (p - b) * (p - c));

    return mix(1.0, 0.0, smoothstep(0.5 * thickness, 1.5 * thickness, h));
}

void main()
{
    vec2 tA = vec3(camera * transform * vec3(A, 1.0f)).xy;
    vec2 tB = vec3(camera * transform * vec3(B, 1.0f)).xy;
    float factor = drawLine(tA, tB, thickness);
    if (factor <= 0.1f)
    discard;
    gl_FragColor = fill * factor;
}

