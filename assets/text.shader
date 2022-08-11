#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
uniform mat3 transform;
uniform mat3 shape;
uniform mat3 camera;

out vec2 UV;

void main()
{
    UV = aUV;
    gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);
}

    #shader fragment
    #version 330 core

uniform sampler2D fillTexture;

in vec2 UV;

out vec4 FragColor;

void main()
{
    FragColor = texture(fillTexture, UV);
}
