#version 330
layout (location = 0) in vec2 vert;
layout (location = 1) in vec2 _uv;
out vec2 uv;
void main()
{
    uv = _uv;
    gl_Position = vec4(vert.x / 720.0 - 1.0, vert.y / 405.0 - 1.0, 0.0, 1.0);
}
