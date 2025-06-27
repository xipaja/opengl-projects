#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lightColor;
    vec3 lightResult = ambientLight * objectColor;
    
    FragColor = vec4(ambientLight, 1.0);
}