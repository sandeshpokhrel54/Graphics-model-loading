#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
vec3 fogColor =  vec3(0.8314, 0.9098, 0.9333);
uniform int fogbool;

void main()
{
    //if it is day / brighter shade to the texture else give a darker shade to the texture
    vec4 environmentShade = vec4(0.3, 0.3, 0.3,1.0); 
    FragColor = texture(skybox, TexCoords) * environmentShade;

    if(fogbool == 1)
        FragColor = mix(FragColor,vec4(fogColor,1.0),0.75);
}