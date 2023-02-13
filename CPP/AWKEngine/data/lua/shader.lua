-- Load Shaders

-- Vertex
comunVertex = [[
#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;

layout (std140, binding = 0) uniform PerPassData {
    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;
};
  
layout (std140, binding = 1) uniform PerModelData{
    uniform mat4 modelmatrix;
    uniform mat4 normalMat;
    uniform vec4 color;
};

out vec4 _color;
out vec4 pointPos;
out vec3 normalInterp;
out vec3 vertPos;
out vec2 uv;

]]

-- Fragment
comunFragment = [[
#version 460 core


in vec4 _color;
in vec4 pointPos;
in mat4 viewPLigh;
in vec3 normalInterp;
in vec3 vertPos;
in vec2 uv;

const vec3 lightPos = vec3(0.0, 8.0, 0.0);
const vec3 lightColor = vec3(1.0, 0.0, 0.0);
const float lightPower = 10.0;

const vec3 diffuseColor = vec3(0.0f, 1.0, 0.0);//????
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float shininess = 16.0;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

#define kMaxLightShadow 4
#define kMaxLights 16

layout (std140, binding = 2) uniform PerFrameData {  

    uniform mat4 MVL[kMaxLightShadow];
    uniform mat4 MPL[kMaxLightShadow];
    // 0 -> MaxShadowMapLight;
    // 1 -> MaxActiveLights 
    // 2 -> ShadowType
    uniform vec4 data;

    uniform vec4 positionAndType[kMaxLights];
    uniform vec4 directionAndCutoff[kMaxLights];
    uniform vec4 colorAndIntensity[kMaxLights];
    uniform vec4 lightProperties[kMaxLights];

};

layout (binding = 1) uniform sampler2D ShadowTex1; 
layout (binding = 2) uniform sampler2D ShadowTex2; 
layout (binding = 3) uniform sampler2D ShadowTex3; 
layout (binding = 4) uniform sampler2D ShadowTex4; 

layout (location = 0) out vec4 out_FragColor;

float maxShadowMapLight = data[0];
float maxLighs = data[1];
float shadowFactor = 0.0f;
int shadowType = int(data[2]);


vec4 doSampling(int i, vec2 uv){

  switch(i){
    case 0:
      return texture(ShadowTex1, uv);

    case 1:
      return texture(ShadowTex2, uv);

    case 2:
      return texture(ShadowTex3, uv);

    case 3:
      return texture(ShadowTex4, uv);
  } 

}

vec2 getTexureSize(int i){

  switch(i){
    case 0:
      return 1.f/textureSize(ShadowTex1, 0);

    case 1:
      return 1.f/textureSize(ShadowTex2, 0);

    case 2:
      return 1.f/textureSize(ShadowTex3, 0);

    case 3:
      return 1.f/textureSize(ShadowTex4, 0);
  } 

}


float findBlocker(vec2 uv, int shadowMapID, float receiver, float searchWidth, float numSamples, float bias){

    vec2 textureOffset = getTexureSize(shadowMapID);
    vec2 currentUV = uv - ((numSamples*0.5f) * textureOffset);

    float stepSize = 2 * searchWidth/numSamples;
    float blockerSum = 0.f;
    float blockerCount = 0.f;

    for(int i = 0; i < numSamples; ++i){

        for(int j = 0; j < numSamples; ++j){

            vec2 newUV = currentUV + vec2(textureOffset.x * j, textureOffset.y * i);

            bool insideTexture = 0 <= newUV.x;
            insideTexture = insideTexture && newUV.x <= 1;
            insideTexture = insideTexture && newUV.y <= 1;
            insideTexture = insideTexture && 0 <= newUV.y;

            if(true /*insideTexture*/) {

                float shadowMapDepth = doSampling(shadowMapID, newUV).x;

                if(shadowMapDepth > receiver + bias) {

                    ++blockerCount;
                    blockerSum += shadowMapDepth;

                }

            }
        }
    }

    float avg = 999.f;

    if(blockerCount != 0)
        avg = blockerSum/blockerCount;

    return avg;
}

float estimatePenumbra(float receiver, float blocker, float light) {

    return ((receiver - blocker) * light) / blocker;

}

void PCSS() {
  
  float numSamples = 8;
  
  float SF = 1.0f / maxShadowMapLight;

  for(int i = 0; i < maxShadowMapLight; ++i ){

    float averageCounter = 0.f;
    float averageFactor = 0.f;

    vec4 newP = MPL[i] * MVL[i] * pointPos;
    //float distance = newP.z;
    newP /= newP.w;
    
    bool insideTexture = -1 < newP.x;
    insideTexture = insideTexture && newP.x < 1;
    insideTexture = insideTexture && newP.y < 1;
    insideTexture = insideTexture && -1 < newP.y;

    vec3 normal = normalize(normalInterp);
    vec3 lightDir = positionAndType[i].xyz - vertPos;
    lightDir = normalize(lightDir);
    float minbias = lightProperties[i].y; 
    float maxbias = lightProperties[i].z; 
    float bias = max(maxbias * (1.f - dot(normal, lightDir)), minbias);  // 0.0001885, 1775 etc.f
    //bias = 0.000205f;

    if(insideTexture){
    
        newP.xyz = newP.xyz * 0.5f + vec3(0.5f);

        vec2 texelOffset = getTexureSize(i);

        float wLight = lightProperties[i].x;

        float searchSamples = 6.f;

        float blocker = findBlocker(newP.xy, i, newP.z, wLight/newP.z, searchSamples, bias);
        float wPenumbra = estimatePenumbra(newP.z, blocker, wLight);

        float stepSize = 2 * wPenumbra / numSamples;

        int range = 4;

        for(int v = -range; v <= range; ++v) {
            for(int u = -range; u <= range; ++u) {

                vec2 currentOffset = vec2(newP.x + (wPenumbra * texelOffset.x * u), newP.y + (wPenumbra * texelOffset.y * v));

                bool neighbourInsideTexture = 0 <= currentOffset.x;
                neighbourInsideTexture = neighbourInsideTexture && (currentOffset.x <= 1);
                neighbourInsideTexture = neighbourInsideTexture && (currentOffset.y <= 1);
                neighbourInsideTexture = neighbourInsideTexture && (0 <= currentOffset.y);
                
                if(neighbourInsideTexture) {

                    ++averageCounter;

                    vec4 neighbourDepth = doSampling(i, currentOffset.xy);

                    if(newP.z <= neighbourDepth.x + bias)
                        ++averageFactor;

                }
          
            }

        }
                 
        averageFactor /= averageCounter;

        //if(blocker > 998)
        //    averageFactor = 1.0f;
        
        vec4 tmpP = doSampling(i, newP.xy);
        shadowFactor += averageFactor * SF;

    }else{

        shadowFactor += SF;

    }

  }

}

void PCF() {

  float SF = 1.0f / maxShadowMapLight;

  for(int i = 0; i < maxShadowMapLight; ++i ){

    vec4 newP = MPL[i] * MVL[i] * pointPos;

    newP /= newP.w;
    
    bool insideTexture = -1 < newP.x;
    insideTexture = insideTexture && newP.x < 1;
    insideTexture = insideTexture && newP.y < 1;
    insideTexture = insideTexture && -1 < newP.y;

    float averageCounter = 0.f;
    float averageFactor = 0.f;

    vec3 normal = normalize(normalInterp);
    vec3 lightDir = positionAndType[i].xyz - vertPos;
    lightDir = normalize(lightDir);
    float minbias = lightProperties[i].y; 
    float maxbias = lightProperties[i].z; 
    float bias = max(minbias * (1.f - dot(normal, lightDir)), maxbias);  // 0.0001885, 1775 etc.f
    //bias = 0.000205f;

    if(insideTexture){
    
        newP.xyz = newP.xyz * 0.5f + vec3(0.5f);

        vec2 texelOffset = getTexureSize(i);

        float searchSamples = 6.f;

        int range = 8;

        for(int v = -range; v <= range; ++v) {

            for(int u = -range; u <= range; ++u) {

                vec2 currentOffset = vec2(newP.x + (texelOffset.x * u ), newP.y + (texelOffset.y * v));

                bool neighbourInsideTexture = 0 < currentOffset.x;
                neighbourInsideTexture = neighbourInsideTexture && (currentOffset.x < 1);
                neighbourInsideTexture = neighbourInsideTexture && (currentOffset.y < 1);
                neighbourInsideTexture = neighbourInsideTexture && (0 < currentOffset.y);
                
                if(neighbourInsideTexture) {
                    ++averageCounter;

                    vec4 neighbourDepth = doSampling(i, currentOffset.xy);

                    if(newP.z <= neighbourDepth.x + bias)
                        ++averageFactor;

                }
          
            }

        }
                 
        averageFactor /= averageCounter;
        
        
        shadowFactor += averageFactor * SF;

    }else{

        shadowFactor += SF;

    }

  }

}


void HardShadow(){

  for(int i = 0; i< maxShadowMapLight; ++i ){

    vec4 newP = MPL[i] * MVL[i] * pointPos;

    newP /= newP.w;
  
    bool outTexture = -1 < newP.x;
    outTexture = outTexture && newP.x < 1;
    outTexture = outTexture && newP.y < 1;
    outTexture = outTexture && -1 < newP.y;

    vec3 normal = normalize(normalInterp);
    vec3 lightDir = positionAndType[i].xyz - vertPos;
    lightDir = normalize(lightDir);
    float minbias = lightProperties[i].y; 
    float maxbias = lightProperties[i].z; 
    float bias = max(minbias * (1.f - dot(normal, lightDir)), maxbias);  // 0.0001885, 1775 etc.f
    bias = maxbias;

    if(outTexture){ 

        newP.xyz = newP.xyz *0.5f + vec3(0.5f);
        
        vec4 tmpP = doSampling(i, newP.xy);

        if(newP.z > tmpP.x+bias){

            //shadowFactor -= 1.0f/maxShadowMapLight;

        }else{

            shadowFactor += 1.0f/maxShadowMapLight;

        }

    }else{

        shadowFactor += 1.0f/maxShadowMapLight;

    }

  }

}

void CalculateShadow() {

    switch(shadowType) {

        case 0:
            HardShadow();
        break;

        case 1:
            PCF();
        break;

        case 2:
            PCSS();
        break;

    }

    if(maxShadowMapLight == 0)
        shadowFactor = 1.f;

}


]]
-- Load Shader Function Call
-- Receives/Gives 3 arguments: Name, Vertex (comunVertex + Main), Fragment (comunFragment + Main)


-- Framebuffer

loadShader("Framebuffer",

[[
#version 460 core

const vec2 position[3] = vec2[3](
                   vec2( -1.0f,  1.0f),
                   vec2( -1.0f, -3.0f),
                   vec2(  3.0f,  1.0f));


     
 const vec2 uv[3] = vec2[3](
                   vec2( 0.0f, 1.0f),
                   vec2( 0.0f, -1.0f),
                   vec2( 2.0f, 1.0f));           
                   
                      
out vec2 uv_;
void main() {
    gl_Position =  vec4(position[gl_VertexID], 0.0f, 1.0f);
    uv_ = uv[gl_VertexID];
}
]]
,
[[
#version 460 core

layout (binding = 0) uniform sampler2D tex0;
layout (location = 0) out vec4 out_FragColor;

in vec2 uv_;

void main(){

  out_FragColor = texture(tex0, uv_);
 
  
}
]])


-- Blur
loadShader("Blur Shader",

[[
#version 460 core

layout (std140, binding = 3) uniform PostProcessData{

    //data[0]->rangeH, 
    //data[1]->rangeV,
    //data[2]->Type// 0->H, 1->, 2-> Both
    uniform vec4 data;
};



const vec2 position[3] = vec2[3](
                   vec2( -1.0f,  1.0f),
                   vec2( -1.0f, -3.0f),
                   vec2(  3.0f,  1.0f));


     
 const vec2 uv[3] = vec2[3](
                   vec2( 0.0f, 1.0f),
                   vec2( 0.0f, -1.0f),
                   vec2( 2.0f, 1.0f));           
                   
                      
out vec2 uv_;
out vec4 data_;
void main() {
    gl_Position =  vec4(position[gl_VertexID], 0.0f, 1.0f);
    uv_ = uv[gl_VertexID];
    data_ = data;
}
]]
,
[[
#version 460 core

layout (binding = 0) uniform sampler2D tex0;
layout (location = 0) out vec4 out_FragColor;

in vec2 uv_;
in vec4 data_;



vec3 Blur(){

    vec2 texelOffset = 1.0f/ textureSize(tex0, 0);
    float weight = 0.6f;
    float decrementOfWeight =0.85f;

    vec3 result = texture(tex0, uv_).rgb *weight;

    //Horizontal
   if(data_[2] == 0 || data_[2] > 1){
        for(int h = 0; h< data_[0]; h++){
            weight*=decrementOfWeight;
            vec2 leftUV = uv_ - vec2(texelOffset.x * (h+1), 0.0f);
            vec2 rightUV = uv_ + vec2(texelOffset.x * (h+1), 0.0f);     
            result += texture(tex0, leftUV).rgb  * weight;
            result += texture(tex0, rightUV).rgb * weight;
        }
    }
    weight = 0.6f;

    //Vertical
    if(data_[2] >= 1){
        for(int v = 0; v < 2; v++){
            vec2 upperUV = uv_ - vec2(0.0f, texelOffset.y * (v+1));
            vec2 lowerUV = uv_ + vec2(0.0f, texelOffset.y * (v+1));
            weight*=decrementOfWeight;
            result += texture(tex0, upperUV ).rgb * weight;
            result += texture(tex0, lowerUV ).rgb * weight;
        }
    }
   
    

    return result;
}

void main(){

 

  out_FragColor = vec4(Blur(), 1.0f);

     

  
}
]])

-- Texture Material
loadShader("Texture Material",
comunVertex..[[

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelmatrix * vec4(Position, 1.f);  
    uv = UV;
    pointPos =  modelmatrix * vec4(Position, 1.f);
    _color = color;

 
    vec4 vertPos4 = pointPos;
    vertPos = vertPos4.xyz / vertPos4.w;
    normalInterp = vec3(normalMat * vec4(Normal, 0.f));
    //normalInterp = Normal;



}
]],
comunFragment..[[
layout (binding = 0) uniform sampler2D tex0;

void main(){
  
  CalculateShadow();


  out_FragColor = shadowFactor * _color * texture(tex0, vec2(uv.x, uv.y));

}
]])

-- Shadow Material
loadShader("Shadow Material",
comunVertex..[[

void main() {



    gl_Position = projectionMatrix * viewMatrix * modelmatrix * vec4(Position, 1.f);  
    uv = UV;
    _color = color;
}
]],
comunFragment..[[
layout (binding = 0) uniform sampler2D tex0;

void main(){
  out_FragColor =   _color;
}
]])

-- Blinn-Phong Shading (Illumination)
loadShader("Blinn-Phong",
comunVertex..[[

void main() {

    gl_Position = projectionMatrix * viewMatrix * modelmatrix * vec4(Position, 1.f);  
    uv = UV;    
    pointPos =  modelmatrix * vec4(Position, 1.f);
    _color = color;
    vec4 vertPos4 = pointPos;
    vertPos = vertPos4.xyz / vertPos4.w;
    normalInterp = vec3(normalMat * vec4(Normal, 0.f));
    //normalInterp = Normal;


}
]],
comunFragment..[[

layout (binding = 0) uniform sampler2D tex0;



float testColor = 0.0f;
void main(){

    CalculateShadow();

    vec3 colorLinear = vec3(0.0f, 0.0f, 0.0f);

    for(int l = 0; l < maxLighs ; ++l){
      int lightType = int(positionAndType[l].w);  
      vec3 normal = normalize(normalInterp);
      vec3 lightDir = positionAndType[l].xyz - vertPos;
      float distance = length(lightDir);
      distance = distance * distance;
      lightDir = normalize(lightDir);

      float lambertian = max(dot(lightDir, normal), 0.f);
      testColor = lambertian;
      float specular = 0.f;

      if(lambertian > 0.f) {

        vec3 viewDir = normalize(-vertPos);

        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, normal), 0.f);
        specular = pow(specAngle, shininess);

      }

      colorLinear += _color.xyz * lambertian * colorAndIntensity[l].xyz * colorAndIntensity[l].w/distance +
                      specColor * specular * colorAndIntensity[l].xyz * colorAndIntensity[l].w/distance;
    }
                     

  vec3 colorGammaCorrected = pow(colorLinear, vec3(1.f/screenGamma));

  out_FragColor = shadowFactor * vec4(colorGammaCorrected,1.0f)* texture(tex0, vec2(uv.x, uv.y));

}

]])



-- Atmospheric Scattering by Wu Hao
loadShader("Atmospheric Scattering by Wu Hao",
comunVertex..[[

out vec3 vPos;

void main(void) {
    
    vPos = Position.xyz;

    vec4 position = viewMatrix * vec4(Position , 0.0f);
    gl_Position = projectionMatrix * vec4(position.xyz * 99.f, 1.0f);
    //gl_Position = projectionMatrix * viewMatrix * modelmatrix * vec4(Position, 1.f);
    //fragmentColor = vec4(Position*0.5 + 0.5, 1.f);

}

]],
comunFragment..[[

layout (std140, binding = 4) uniform ScatteringData {
    
    uniform vec4 eyePosition;       // eyePosition.w == fov
    uniform vec4 u_sunPosition;
    uniform vec4 eyeCenterPosition;
    uniform vec4 eyeUpDir;
    uniform vec4 airParameters;    // x -> density, y -> clarity, z -> pollution, w -> brightness
    uniform vec4 sunAndAtmosphereParameters;   // x -> planet scale, y -> atmosphere scale, z -> sun disk radius, w -> sun disk intensity
    uniform vec4 sunWave;

};

struct Skyprops {

    float density;
    float clarity;
    float pollution;
    float planet_scale;
    float atmosphere_scale;
    float sun_disk_radius;
    float brightness;
    float sun_disk_intensity;

};

vec4 sky_params1;
vec4 sky_params2;
vec4 sky_params3;
vec4 sky_params4;
vec4 sky_params5;
vec4 sky_params6;

vec3 eyePos = eyePosition.xyz;
vec3 u_sunPos = u_sunPosition.xyz;

in vec3 vPos;

Skyprops skyprop;

float sqr(float value) {

    return value * value;

}

void initSkyprop() {

    skyprop.density = airParameters.x;
    skyprop.clarity = airParameters.y;
    skyprop.pollution = airParameters.z;
    skyprop.brightness = airParameters.w;
    skyprop.planet_scale = sunAndAtmosphereParameters.x;
    skyprop.atmosphere_scale = sunAndAtmosphereParameters.y;
    skyprop.sun_disk_radius = sunAndAtmosphereParameters.z;
    skyprop.sun_disk_intensity = sunAndAtmosphereParameters.w;

}

void initSky() {

    vec3 sky_lambda = vec3(680e-9, 550e-9, 450e-9);
    vec3 sky_k = vec3(0.686, 0.678, 0.666);

    float earth_radius = 6.371e6;
    float earth_atmo_thickness = 0.1e6;

    float clarity = 1 + skyprop.clarity;
    float two_pi = 2 * 3.141592f;

    // Compute BetaR
    float factor = 1.86e-31 / (clarity * max(skyprop.density, 0.001f));

    sky_params2.x = factor / pow(sky_lambda.x, 4);
    sky_params2.y = factor / pow(sky_lambda.y, 4);
    sky_params2.z = factor / pow(sky_lambda.z, 4);

    // Compute BetaM
    factor = 1.36e-19 * max(skyprop.pollution, 0.001f);
    sky_params3.x = factor * sky_k.x * sqr(two_pi / sky_lambda.x);
    sky_params3.y = factor * sky_k.y * sqr(two_pi / sky_lambda.y);
    sky_params3.z = factor * sky_k.z * sqr(two_pi / sky_lambda.z);

    // BetaR + BetaM, -(BetaR + BetaM), BetaR / (BetaR + BetaM), BetaM / (BetaR + BetaM)
    sky_params1 = sky_params2 + sky_params3;
    sky_params6 = sky_params2 + sky_params3;
    sky_params6 = sky_params6 * -1.0f;
    sky_params2 = sky_params2/sky_params1;
    sky_params3 = sky_params3/sky_params1;

    // Mie Scattering Phase Constants
    float g = (1 - skyprop.pollution) * 0.2f + 0.75f;
    sky_params1.w =  sqr(1 - g) / (4 * 3.141592f);
    sky_params2.w = -2 * g;
    sky_params3.w = 1 + sqr(g);

    float planet_radius = earth_radius * skyprop.planet_scale;
    float atmo_radius = planet_radius + earth_atmo_thickness * skyprop.atmosphere_scale;
    sky_params4.x = planet_radius;
    sky_params4.y = atmo_radius * atmo_radius;
    sky_params4.z = 0.15f + 0.75f * 0.5f;
    sky_params4.w = atmo_radius * atmo_radius - planet_radius * planet_radius;

    // Sun Disk Cutoff
    sky_params1.y = -(1 - 0.015 * skyprop.sun_disk_radius);
    sky_params1.x = 1 / (1 + sky_params1.y);
    sky_params1.y *= sky_params1.x;

    sky_params5.x = sunWave.x;
    sky_params5.y = sunWave.y;
    sky_params5.z = sunWave.z;
    sky_params5.w = 1.0f;
    sky_params5 = sky_params5 * skyprop.brightness;
    sky_params5.w = skyprop.sun_disk_intensity;

    sky_params6.w = clarity * 3 / (16 * 3.141592f);

}

vec3 get_Pixel_Dir() {

    return normalize(vPos);

}

vec3 calcExtinction(float dist) {

    return exp(dist * sky_params6.xyz);

}

vec3 calcScattering(float cos_theta) {

    float r_phase = (cos_theta * cos_theta) * sky_params6.w + sky_params6.w;
    float m_phase = sky_params1.w * pow(sky_params2.w * cos_theta + sky_params3.w, -1.5f);
    return sky_params2.xyz * r_phase + (sky_params3.xyz * m_phase);

}

float baseOpticalDepth(vec3 ray) {

    float a1 = sky_params4.x * ray.y;
    return sqrt(a1 * a1 + sky_params4.w) - a1;

}

float opticalDepth(vec3 pos, vec3 ray) {

    pos.y += sky_params4.x;
    float a0 = sky_params4.y - dot(pos, pos);
    float a1 = dot(pos, ray);
    return sqrt(a1 * a1 + a0) - a1;

}

vec3 getColor(vec3 view_vec) {

    vec3 sun_vec = normalize(u_sunPos);
    float cos_theta = dot(view_vec, sun_vec);
    float ray_dist = baseOpticalDepth(view_vec);

    vec3 extinction = calcExtinction(ray_dist);
    vec3 light_ray_pos = view_vec * (ray_dist * sky_params4.z);
    float light_ray_dist = opticalDepth(light_ray_pos, sun_vec);
    float light_ray_dist_full = opticalDepth(view_vec * ray_dist, sun_vec);
    vec3 incoming_light = calcExtinction(light_ray_dist);

    vec3 scattering = calcScattering(cos_theta);
    scattering *= 1.0f - extinction;

    vec3 in_scatter = incoming_light * scattering;

    float sun_strength = clamp(cos_theta * sky_params1.x + sky_params1.y, 0.0f, 1.0f);
    sun_strength *= sun_strength;
    vec3 sun_disk = extinction * sun_strength;
    return sky_params5.xyz * (sky_params5.w * sun_disk + in_scatter);

}

void main() {

    initSkyprop();
    initSky();

    vec3 view_vec = get_Pixel_Dir();
    vec3 outcolor = getColor(view_vec);
    out_FragColor = vec4(outcolor, 1.0f);
    //out_FragColor = vec4(normalize(vPos), 1.f);
    
}
]])

-- Water
loadShader("Water",[[

#version 460 core
out vec4 frag_color;
out vec3 position_world;
out vec4 normal_;



layout(std140, binding = 4) buffer waterNormalBuffer
{
  vec4 normal[];
};

layout(std140, binding = 5) buffer waterMeshBuffer
{
  vec4 poss[];
};

layout(std140, binding = 7) buffer waterMeshIndicesBuffer
{
  ivec4 indicess[];
};



layout(std140, binding = 6) uniform waterMeshInfo
{
   //vec4 Vindices[kMaxVIndexNumber];
   mat4 viewMatrix;
   mat4 projectionMatrix;
   vec4 u_oceancolor; 
};


 uint Index2(){
    int c = gl_VertexID%3;
    int i = gl_VertexID/3;
   switch(c){
   case 0 : return int(indicess[i].x); break;
   case 1 : return int(indicess[i].y); break;
   case 2 : return int(indicess[i].z); break;
   }
 }



 /*uint Index(){
    int c = gl_VertexID%3;
    int i = gl_VertexID/3;
   switch(c){
   case 0 : return int(Vindices[i].x); break;
   case 1 : return int(Vindices[i].y); break;
   case 2 : return int(Vindices[i].z); break;
   }
 }*/

void main() {

  uint id = Index2();//indicess[gl_VertexID];//indexTest;
  //uint id2 = Index();
  vec3 v = poss[id].xyz;
  gl_Position = projectionMatrix * viewMatrix * vec4 (v,  1.0);
  normal_ = normal[gl_VertexID/3];
  frag_color = u_oceancolor;
  
}

]],[[

#version 460 core

in vec4 frag_color;
in vec3 position_world;
in vec4 normal_;

layout (location = 0) out vec4 out_FragColor;

layout (std140, binding = 4) uniform ScatteringData {
    
    uniform vec4 eyePosition;       // eyePosition.w == fov
    uniform vec4 u_sunPosition;
    uniform vec4 eyeCenterPosition;
    uniform vec4 eyeUpDir;
    uniform vec4 airParameters;    // x -> density, y -> clarity, z -> pollution, w -> brightness
    uniform vec4 sunAndAtmosphereParameters;   // x -> planet scale, y -> atmosphere scale, z -> sun disk radius, w -> sun disk intensity
    uniform vec4 sunWave;

};


struct Skyprops {

    float density;
    float clarity;
    float pollution;
    float planet_scale;
    float atmosphere_scale;
    float sun_disk_radius;
    float brightness;
    float sun_disk_intensity;

};

vec4 sky_params1;
vec4 sky_params2;
vec4 sky_params3;
vec4 sky_params4;
vec4 sky_params5;
vec4 sky_params6;


vec3 eyePos = eyePosition.xyz;
vec3 u_sunPos = u_sunPosition.xyz;
vec3 normal_world = normal_.xyz;
Skyprops skyprop;


void initSkyprop() {

    skyprop.density = airParameters.x;
    skyprop.clarity = airParameters.y;
    skyprop.pollution = airParameters.z;
    skyprop.brightness = airParameters.w;
    skyprop.planet_scale = sunAndAtmosphereParameters.x;
    skyprop.atmosphere_scale = sunAndAtmosphereParameters.y;
    skyprop.sun_disk_radius = sunAndAtmosphereParameters.z;
    skyprop.sun_disk_intensity = sunAndAtmosphereParameters.w;

}

void initSky() {
    initSkyprop();
    vec3 sky_lambda = vec3(680e-9, 550e-9, 450e-9);
    vec3 sky_k = vec3(0.686, 0.678, 0.666);

    float earth_radius = 6.371e6;
    float earth_atmo_thickness = 0.1e6;

    float clarity = 1 + skyprop.clarity;
    float two_pi = 2 * 3.141592f;

    // Compute BetaR
    float factor = 1.86e-31 / (clarity * max(skyprop.density, 0.001f));

    sky_params2.x = factor / pow(sky_lambda.x, 4);
    sky_params2.y = factor / pow(sky_lambda.y, 4);
    sky_params2.z = factor / pow(sky_lambda.z, 4);

    // Compute BetaM
    factor = 1.36e-19 * max(skyprop.pollution, 0.001f);
    sky_params3.x = factor * sky_k.x * sqrt(two_pi / sky_lambda.x);
    sky_params3.y = factor * sky_k.y * sqrt(two_pi / sky_lambda.y);
    sky_params3.z = factor * sky_k.z * sqrt(two_pi / sky_lambda.z);

    // BetaR + BetaM, -(BetaR + BetaM), BetaR / (BetaR + BetaM), BetaM / (BetaR + BetaM)
    sky_params1 = sky_params2 + sky_params3;
    sky_params6 = sky_params2 + sky_params3;
    sky_params6 = sky_params6 * -1.0f;
    sky_params2 = sky_params2/sky_params1;
    sky_params3 = sky_params3/sky_params1;

    // Mie Scattering Phase Constants
    float g = (1 - skyprop.pollution) * 0.2f + 0.75f;
    sky_params1.w =  sqrt(1 - g) / (4 * 3.141592f);
    sky_params2.w = -2 * g;
    sky_params3.w = 1 + sqrt(g);

    float planet_radius = earth_radius * skyprop.planet_scale;
    float atmo_radius = planet_radius + earth_atmo_thickness * skyprop.atmosphere_scale;
    sky_params4.x = planet_radius;
    sky_params4.y = atmo_radius * atmo_radius;
    sky_params4.z = 0.15f + 0.75f * 0.5f;
    sky_params4.w = atmo_radius * atmo_radius - planet_radius * planet_radius;

    // Sun Disk Cutoff
    sky_params1.y = -(1 - 0.015 * skyprop.sun_disk_radius);
    sky_params1.x = 1 / (1 + sky_params1.y);
    sky_params1.y *= sky_params1.x;

    sky_params5.x = sunWave.x;
    sky_params5.y = sunWave.y;
    sky_params5.z = sunWave.z;
    sky_params5.w = 1.0f;
    sky_params5 = sky_params5 * skyprop.brightness;
    sky_params5.w = skyprop.sun_disk_intensity;

    sky_params6.w = clarity * 3 / (16 * 3.141592f);

}




	vec3 hdr (vec3 color, float exposure) {
      return 1.0 - exp(-color * exposure);
      
    }

vec3 calcExtinction(float dist) {

    return exp(dist * sky_params6.xyz);

}

vec3 calcScattering(float cos_theta) {

    float r_phase = (cos_theta * cos_theta) * sky_params6.w + sky_params6.w;
    float m_phase = sky_params1.w * pow(sky_params2.w * cos_theta + sky_params3.w, -1.5f);
    return sky_params2.xyz * r_phase + (sky_params3.xyz * m_phase);

}

float baseOpticalDepth(vec3 ray) {

    float a1 = sky_params4.x * ray.y;
    return sqrt(a1 * a1 + sky_params4.w) - a1;

}

float opticalDepth(vec3 pos, vec3 ray) {

    pos.y += sky_params4.x;
    float a0 = sky_params4.y - dot(pos, pos);
    float a1 = dot(pos, ray);
    return sqrt(a1 * a1 + a0) - a1;

}

vec3 getSkyColor(vec3 view_vec) {

    vec3 sun_vec = normalize(u_sunPos);
    float cos_theta = dot(view_vec, sun_vec);
    float ray_dist = baseOpticalDepth(view_vec);

    vec3 extinction = calcExtinction(ray_dist);
    vec3 light_ray_pos = view_vec * (ray_dist * sky_params4.z);
    float light_ray_dist = opticalDepth(light_ray_pos, sun_vec);
    float light_ray_dist_full = opticalDepth(view_vec * ray_dist, sun_vec);
    vec3 incoming_light = calcExtinction(light_ray_dist);

    vec3 scattering = calcScattering(cos_theta);
    scattering *= 1.0f - extinction;

    vec3 in_scatter = incoming_light * scattering;

    float sun_strength = clamp(cos_theta * sky_params1.x + sky_params1.y, 0.0f, 1.0f);
    sun_strength *= sun_strength;
    vec3 sun_disk = extinction * sun_strength;
    return sky_params5.xyz * (sky_params5.w * sun_disk + in_scatter);}








  void main(void)
  {
    initSky();
    out_FragColor.w = 1.0;
  	vec3 outcolor=vec3(0.0);

    vec3 sunpos = u_sunPos;
    vec3 incidentdir = normalize(position_world-sunpos);


    vec3 standardnormal = normal_world;

    vec3 reflectiondir = normalize(incidentdir-2.0*standardnormal*dot(incidentdir,standardnormal));

    vec3 eyepos = eyePos;


    vec3 eyeraydir = normalize(eyepos-position_world);          ///from point to eye
    vec3 directionalLightDir = normalize(vec3(0.0,1.0,0.0));
  	float specular1 = pow(max(0.01,dot(eyeraydir,reflectiondir)),5000.0);


     vec3 ocean_color = frag_color.xyz;



     vec3 sky_color = vec3(3.2, 9.6, 12.8)*0.8;


     vec3 view = normalize(eyePos - position_world);

     float incidentAngle=acos(abs(dot(standardnormal, view)));
     float transmittanceAngle = asin(sin(incidentAngle)/1.33);      //1.33 is the water transmittance ratio
     float temp1 = tan(incidentAngle-transmittanceAngle)/tan(incidentAngle+transmittanceAngle);
     float temp2 = sin(incidentAngle-transmittanceAngle)/sin(incidentAngle+transmittanceAngle);

     float fresnel =(temp1*temp1+temp2*temp2)*0.5;
    //gl_FragColor.xyz = vec3(fresnel);
    //return;

    ///use negative eyeraydir for the direction from eye to point
    /// and get the reflection direction
    vec3 eyeReflDir = normalize(-eyeraydir-2.0*standardnormal*dot(-eyeraydir,standardnormal));
    eyeReflDir.y=abs(eyeReflDir.y);
    vec3 sun_color = getSkyColor(normalize(sunpos));
     sky_color = getSkyColor(eyeReflDir)*10.0;


     vec3 sky = fresnel * sky_color;


     float specular2 = clamp(dot(standardnormal, directionalLightDir), 0.0, 1.0);
    specular2 = pow(specular2,1.0);


     vec3 water = (1.0 - fresnel) * ocean_color  * specular2 * sky_color;

     outcolor = 100.0*specular1*sun_color + sky + water;

    out_FragColor = vec4(hdr(outcolor,0.35), 1.0);
   // out_FragColor = vec4(outcolor , 1.0);
    return;
    
  }


]])


