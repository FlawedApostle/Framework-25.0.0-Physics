#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in; /// bringing in triangles

layout (line_strip, max_vertices = 6) out;  // 2 triangles per quad  - 3 vert each triangle - 2 triangle = 2 * 3 = 6 

layout(location = 0) uniform mat4 projectionMatrix;
layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 modelMatrix;

layout (location = 0) in VertexStage {
    vec3 normal;
} vs_in[];



void main() {
    float length  = 1.0;
    for(int index = 0; index < 3; index++){
        gl_Position = projectionMatrix  * viewMatrix * gl_in[index].gl_Position;                                                     // * modelMatrix
        EmitVertex();

        gl_Position = projectionMatrix  * viewMatrix  * (gl_in[index].gl_Position + (vec4(vs_in[index].normal * length, 0.0)));        // * modelMatrix
        EmitVertex();

        EndPrimitive();
    }
}