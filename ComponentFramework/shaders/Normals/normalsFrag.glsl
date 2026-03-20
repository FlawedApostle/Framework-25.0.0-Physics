#version 330 core

in vec3 fragNormal;
out vec4 FragColor;

void main() {
    vec3 n = normalize(fragNormal);
    FragColor = vec4(n * 0.5 + 0.5, 1.0); 
}