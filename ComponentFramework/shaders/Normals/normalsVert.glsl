#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragNormal;

void main() {
    //fragNormal = mat3(modelMatrix) * normal; // transform to world
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    fragNormal = normalize(normalMatrix * normal); // rotate to the correct orientation
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}