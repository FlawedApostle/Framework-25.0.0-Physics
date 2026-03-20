#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;

layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 modelMatrix;

layout (location = 0) out VertexStage {
    vec3 normal;
} vs_out;


void main() {
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    gl_Position = viewMatrix * modelMatrix * vVertex;
    vs_out.normal = mat3(normalMatrix) * vNormal;
    
    gl_Position = vVertex;
    vs_out.normal = vNormal;


}