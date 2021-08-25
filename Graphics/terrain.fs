#version 330 core
#define POINT_LIGHTS 7
out vec4 FragColor;
in vec2 TexCoords;
in vec3 normal;
in vec3 fragPosition;
in float fog;

uniform int fogbool;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform vec3 sunPos;
uniform vec3 viewerPos;
uniform vec3 sunColor;

uniform vec3 pLightColor;
uniform vec3 pLights[POINT_LIGHTS];

vec3 PointLightCalc(vec3 plightpos, vec3 fragPosition, vec3 viewDirection, vec3 normal);
vec3 fogColor =  vec3(0.8314, 0.9098, 0.9333);


void main()
{    


    //normal mapping
    // vec3 rgb_normal = texture_normal1 * 0.5 + 0.5;
    vec3 normFromMap = 0.8 *  texture(texture_normal1, TexCoords).rgb;  //the scalar in the front controls the strength of the map
    normFromMap = normalize(normFromMap*2.0 - 1.0);

    //ambient lighting
    float ambientStrength = 0.05;
    // vec4 lightColor = vec4(sunColor, 1.0f); //closer to zero means darker value pass this as uniform of sun's color
    vec3 ambient = ambientStrength * sunColor;


    //diffuse lighting
    // vec3 norm = normalize(normal);
    vec3 norm =  normFromMap; //for normal mapping 
    vec3 lightDirection = normalize(sunPos - fragPosition);
    float diffuseStrength = max(dot(norm,lightDirection),0.0);
    vec3 diffuse = diffuseStrength * sunColor;


    //specular -update this to use specular maps
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(viewerPos - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection),0.0),8);
    vec3 specular = specularStrength * spec * sunColor;


    //overall directional light effect
    vec4 tex = texture(texture_diffuse1, TexCoords) * vec4(ambient+diffuse+specular,1.0f) /*+ texture(texture_specular1,TexCoords) * vec4(specular,1.0f)*/;

    vec3 pointLightEffect = vec3(0.0,0.0,0.0);
    for(int i=0; i<POINT_LIGHTS;i++)
        pointLightEffect+= PointLightCalc(pLights[i], fragPosition, viewDirection, normFromMap);

    FragColor =  tex+ vec4(pointLightEffect,1.0);
    
    if(fogbool ==1)
        FragColor = mix(vec4(fogColor,1.0),FragColor,fog);

}



vec3 PointLightCalc(vec3 plightpos, vec3 fragPosition, vec3 viewDirection, vec3 normal)
{
    vec3 lightDirection = normalize(plightpos- fragPosition);
    //diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    //specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection),0.0),16);

    //attenuate light with distance
    float dist = length(plightpos - fragPosition);
    const float consFac = 1.0, LinearFac = 0.22, quadFac= 0.20;
    float attenuation = 1.0/(consFac +dist*LinearFac + dist*dist*quadFac);

    vec3 ambient = pLightColor * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = pLightColor *diff* vec3(texture(texture_diffuse1, TexCoords));
    // vec3 specular = pLightColor * spec* vec3(texture(texture_specular1, TexCoords));
    vec3 specular = pLightColor * spec * vec3(texture(texture_diffuse1, TexCoords));


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 pointLightEffect = ambient + diffuse + specular; 
    return pointLightEffect;
}