#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inVertex;
out vec3 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main() 
{
   // Texture Coords, in relation with the loaded models vertex
   // I am inversing the img x axis to correct the image layout. 
   // This fixes the problem of having to inverse the projection in the camera cpp file
    texCoords = inVertex;
    texCoords.x = -inVertex.x;
    texCoords.y = inVertex.y;
    
    vec4 pos = projectionMatrix * viewMatrix * vec4(inVertex , 1.0);
    gl_Position = pos.xyww; 

    
}