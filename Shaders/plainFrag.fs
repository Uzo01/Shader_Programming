#version 410 core
//FRAGMENT SHADER

vec3 getDirLight(vec3 norm, vec3 viewDir);
vec3 getPointLig(vec3 norm, vec3 viewDir);
vec3 getSpotLig(vec3 norm, vec3 viewDir);
vec3 getBlinnPhong(vec3 norm, vec3 viewDir);

out vec4 FragColor;
//set the uniform for lighting direction


in vec3 normal ;
in vec3 posWS;// fragment position in world space 
in vec2 uv;
in mat3 TBN;
in vec2 texCoords;

struct directionLight
{
 vec3 position;
 vec3 colour;
 float Kc;
 float Kl;
 float Ke;
};

struct pointLight
{
 vec3 position;
 vec3 colour;
 float Kc;
 float Kl;
 float Ke;
};

struct spotLight
{
 vec3 position;
 vec3 direction;
 vec3 colour;
 float Kc;
 float Kl;
 float Ke;

 float innerRad;
 float outterRad;
};

uniform vec3 ColLight;
uniform vec3 OBjCol;
uniform vec3 dirLight; // Light Direction
uniform vec3 viePos; // camera position

uniform directionLight directionLig;
uniform pointLight pointLig;
uniform spotLight spotLig;
uniform sampler2D CubeDiffTex; // texture sampler 
uniform sampler2D CubeSpecTex;
uniform sampler2D CubeNormalMap;
uniform sampler2D disMap;
uniform int map;

float ambientFactor = 0.5;
float shine = 180;
float specStrength = 0.9;

void main()
{ 


 vec3 norm = texture (CubeNormalMap, uv).xyz;
 norm = norm*2.0-1.0;
 norm = normalize(TBN*norm);
vec3 viewDir = normalize(viePos - posWS);
vec3 result = vec3(0.0);

//Get lighting from constructors
result = getDirLight(norm, viewDir);
vec3 pointLigRes = getPointLig(norm, viewDir);
//vec3 spotLig = getSpotLig(norm, viewDir);
result = result + pointLigRes;
//result = result + spotLig;

//Rim Lighting
float Sharpness = 0.0;
float Brightness = 0.0;
float Rim = dot(norm, viewDir);
Rim = 1.0 - Rim;
Rim = max(0.0, Rim);


//result = result + Rim;

FragColor = vec4(result, 1.0);
//FragColor = texture(CubeNormalMap, uv);
};
//
//
//
//
//Direction light
vec3 getDirLight(vec3 norm, vec3 viewDir)
{
//
vec3 result = vec3 (0.0);
vec3 diffMapColour = texture(CubeDiffTex, uv).xyz; // sample texture with uv coords
vec3 ambientColour = ColLight * diffMapColour * ambientFactor;
//diffuse lighting
float diffFact = dot (norm, -dirLight);
diffFact = max (diffFact, 0.0);
vec3 diffColour = ColLight * diffMapColour * diffFact;
//specular
float specMapColour = texture(CubeSpecTex, uv).x;
float spec = 0.0;
vec3 HalfwayDir = normalize(-dirLight + viewDir);
spec = dot(norm, HalfwayDir);
spec = max (spec, 0.0);
spec = pow (spec, shine);
vec3 specCol = spec * specStrength *ColLight ;
//Rim
float Rim = dot(norm, viewDir);
Rim = 1.0 - Rim;
Rim = max(0.0, Rim);
ambientColour = ambientColour + Rim;
result =  specCol + ambientColour;

return result;
}


//
//
//
//
//Point light
vec3 getPointLig(vec3 norm, vec3 viewDir)
 {
float dist = length(pointLig.position - posWS);
float attn = 1.5/(pointLig.Kc + (pointLig.Kl*dist) + (pointLig.Ke*(dist*dist)));
vec3 pointLigDir = normalize(pointLig.position - posWS);

vec3 diffMapColour = texture(CubeDiffTex, uv).xyz;
vec3 ambientColour = ColLight * diffMapColour * ambientFactor;
ambientColour = ambientColour * attn;
//diffuse
float diffFact = dot (norm, -dirLight);
diffFact = max (diffFact, 0.0);
vec3 diffColour = ColLight * diffMapColour * diffFact;
diffColour = diffColour * attn;
//specular
float specMapColour = texture(CubeSpecTex, uv).x;
float spec = 0.0;
vec3 HalfwayDir = normalize(-dirLight + viewDir);
spec = dot(norm, HalfwayDir);
spec = max (spec, 0.0);
spec = pow (spec, shine);
vec3 specCol = spec * specStrength *ColLight ;
//Rim
float Rim = dot(norm, viewDir);
Rim = 1.0 - Rim;
Rim = max(0.0, Rim);
ambientColour = ambientColour + Rim;
vec3 pointLigRes=  specCol + ambientColour;
return pointLigRes;
 }
//
//
//
//
//Spot light
vec3 getSpotLig(vec3 norm, vec3 viewDir)
{
float dist = length(spotLig.position - posWS);
float attn = 1.0/(spotLig.Kc + (spotLig.Kl*dist) + (spotLig.Ke*(dist*dist)));
vec3 spotLigDir = normalize(spotLig.position - posWS);
// Diffuse
float diffFact = dot (norm, spotLigDir);
diffFact = max (diffFact, 0.0);
vec3 diffMapColour = texture(CubeDiffTex, uv).xyz;
vec3 diffColour = spotLig.colour * diffMapColour * diffFact;
diffColour = diffColour * attn;
//specular
float specMapColour = texture(CubeSpecTex, uv).x;
float spec = 0.0;
vec3 HalfwayDir = normalize(-dirLight + viewDir);
spec = dot(norm, HalfwayDir);
spec = max (spec, 0.0);
spec = pow (spec, shine);
vec3 specCol = spec * specStrength *ColLight ;
specCol = specCol * attn;
//Spot light formula
float theta = dot (-spotLigDir, normalize(spotLig.direction));
float denom = (spotLig.innerRad - spotLig.outterRad);
float illum = (theta - spotLig.outterRad) / denom;
illum = clamp(illum, 0.0,1.0); //clamp: returns the value of x constrained to the range minVal to maxVal
diffColour = diffColour * illum;
specCol = specCol * illum;
float Rim = dot(norm, viewDir);
Rim = 1.0 - Rim;
Rim = max(0.0, Rim);
diffColour = diffColour + Rim;
 vec3 spotLigRes = diffColour + specCol;
return spotLigRes;
}