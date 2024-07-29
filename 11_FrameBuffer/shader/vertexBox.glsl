#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aUV;

uniform mat4 _modelMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

out vec2 outUV;
out vec3 outFragPos;

void main() {
    gl_Position = _projMatrix * _viewMatrix * _modelMatrix * vec4(aPos, 1.0);
    outUV = aUV;

    outFragPos = vec3(_modelMatrix * vec4(aPos, 1.0));
}