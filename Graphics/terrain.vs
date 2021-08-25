#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;

out vec2 TexCoords;
out vec3 normal;
out vec3 fragPosition;
out vec3 Bitangents;
out vec3 Tangents;
out float fog;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float PHI = 1.61803398874989484820459;  // Φ = Golden Ratio   

float gold_noise(in vec2 xy, float seed){
       return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}


void main()
{

    //--------texture coordinates---//
    // mat3 texTrans;
    // float tx = 0.0, ty = 0.0;
    // texTrans[0] = vec3(1.0, 0.0, 0.0);
    // texTrans[1] = vec3(0.0, 1.0, 1.0);
    // texTrans[2] = vec3(tx,ty,1.0);

    //changing value of angle and size transforms the textures
    mat2 scaleTrans;
    float sx = 1.2, sy = 1.2;
    scaleTrans[0] = vec2(sx,0);
    scaleTrans[1] = vec2(0,sy);

    //apply random rotation
    mat2 rotTrans;
    float angle = 1.2; //constant rotation doesn't help with repetative pattern
    rotTrans[0] = vec2(cos(angle),sin(angle));
    rotTrans[1] = vec2(-sin(angle), cos(angle));
    //---------


    TexCoords = scaleTrans * aTexCoords;
    TexCoords = rotTrans * TexCoords;    
    fragPosition = vec3(model * vec4(aPos,1.0f));
    

    //for fog effect
    const float density = 0.07;
    const float gradient = 1;

    vec4 relativePos = view * vec4(fragPosition,1.0); //position relative to camera for fog
    float dist = length(relativePos.xyz);

    fog = exp(-pow((dist * density), gradient));
    fog = clamp(fog, 0.25,1);


    normal = aNormal;
    Bitangents = aBitangents;
    Tangents = aTangents;

    gl_Position = projection * view * model * vec4(aPos, 1.0);

}