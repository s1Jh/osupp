#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
uniform mat3 transform;
uniform mat3 shape;
uniform mat3 camera;
uniform float z;

out vec2 UV;

void main()
{
    UV = aUV;
    gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);
}

    #shader fragment
    #version 330 core

uniform vec4 fill;
uniform bool useTexture;
uniform int blendMode;
uniform mat3 textureTransform;
uniform sampler2D fillTexture;

in vec2 UV;

out vec4 FragColor;

void main()
{
    if (!useTexture)
    {
        FragColor = fill;
        return;
    }

    FragColor = texture(fillTexture, (textureTransform * vec3(UV, 1.0)).xy);
    switch (blendMode)
    {
        case 0:
        break;
        case 1:
        FragColor *= fill;
        break;
        case 2:
        FragColor += fill;
        break;
        case 3:
        FragColor -= fill;
        break;
    }
}
