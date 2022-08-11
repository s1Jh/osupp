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

vec2 normal(vec2 inputVector) {
    return vec2(inputVector.y, -inputVector.x);
}

float aspectRatio = resolution.x / resolution.y;

float drawLine(vec2 p1, vec2 p2, float thickness) {
    // our fragment positioned in normalized space
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv *= 2.0f;
    uv -= 1.0f;
    uv.x *= aspectRatio;

    // we need the normal vector to calculate the distance of this point to the drawn line
    vec2 normalVector = normal(p1 - p2);
    // the intersection line is normal to the original drawn line
    vec2 intersectNormal = p1 - p2;

    // drawn line
    float a1 = normalVector.x;
    float b1 = normalVector.y;
    float c1 = -(a1*p1.x + b1 * p1.y);

    // perpendicular line going through UV
    float a2 = intersectNormal.x;
    float b2 = intersectNormal.y;
    float c2 = -(a2*uv.x + b2 * uv.y);

    vec2 intersect;

    intersect.x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);
    intersect.y = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);

    float smallerX = min(p1.x, p2.x);
    float smallerY = min(p1.y, p2.y);
    float biggerX = max(p1.x, p2.x);
    float biggerY = max(p1.y, p2.y);

    float factor = 1.0f;

    if (((smallerX <= intersect.x) && (intersect.x <= biggerX)) &&
        ((smallerY <= intersect.y) && (intersect.y <= biggerY))) {
        // IN RANGE

        factor = abs(a1 * uv.x + b1 * uv.y + c1) / sqrt(pow(a1, 2) + pow(b1, 2));
    } else {
        if (intersect.x < smallerX) {
            if (intersect.y < smallerY) {
                factor = distance(uv, vec2(smallerX, smallerY));
            } else {
                factor = distance(uv, vec2(smallerX, biggerY));
            }
        } else {
            if (intersect.y < smallerY) {
                factor = distance(uv, vec2(biggerX, smallerY));
            } else {
                factor = distance(uv, vec2(biggerX, biggerY));
            }
        }
    }

    if (factor > thickness)
        return -1.0f;
    return factor;
}

void main()
{
//    gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec2 tA = vec3(transform * vec3(A, 1.0f)).xy;
    vec2 tB = vec3(transform * vec3(B, 1.0f)).xy;
    float factor = drawLine(tA, tB, thickness);
    if (factor < 0.0f)
        discard;
    gl_FragColor = fill * mix(1.0, 0.0, smoothstep(0.5 * thickness, 1.5 * thickness, factor));
}

