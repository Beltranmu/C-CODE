#version 330

#define N_DIRECTIONAL_LIGHTS 1
#define N_POINT_LIGHTS 2  
#define N_SPOT_LIGHTS 3

 
struct MatSetting {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
}; 

//Directional Light information
struct DirLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
}; 

//Point Light information
struct PointLight {    
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic; 
};

//Spot Light information
struct SpotLight{
  vec3 position;
  vec3 direction;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float constant;
  float linear;
  float quadratic;
  
  float cutOff;
  float outerCutOff;
};

uniform int fog_activate;
uniform float density ;

uniform sampler2D u_colorTexture;
uniform vec3 camera_pos;
uniform MatSetting matsetting;
uniform DirLight dir_light[N_DIRECTIONAL_LIGHTS];
uniform PointLight point_light[N_POINT_LIGHTS];
uniform SpotLight spot_light[N_SPOT_LIGHTS];

in vec3 normal_r;
in vec3 normal;
in vec3 FragPos;
in vec2 uv;

out vec4 fragColor;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir){

  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), matsetting.shininess);
  
  vec3 ambient  = light.ambient  * vec3(texture(u_colorTexture, uv));
  vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_colorTexture, uv));
  vec3 specular = light.specular * spec * vec3(texture(u_colorTexture, uv));
  
  return (ambient + diffuse + specular);  
}  



vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matsetting.shininess);
    
    float distance    = length(light.position - fragPos);
    float attenuation = 1000.0f / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    
    vec3 ambient  = light.ambient  * vec3(texture(u_colorTexture, uv));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_colorTexture, uv));
    vec3 specular = light.specular * spec * vec3(texture(u_colorTexture, uv));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matsetting.shininess);
    
    float distance    = length(light.position - fragPos);
    float attenuation = 1000.0f / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    
    
    
    vec3 ambient  = light.ambient  * vec3(texture(u_colorTexture, uv));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_colorTexture, uv));
    vec3 specular = light.specular * spec * vec3(texture(u_colorTexture, uv));
   
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
} 

void main(){
  vec3 viewDir_fog = camera_pos - FragPos;
  vec3 viewDir = normalize(camera_pos - FragPos);

  vec3 output_val = vec3(0.0);
  for(int i = 0; i < N_DIRECTIONAL_LIGHTS; i++){
    output_val += CalculateDirLight(dir_light[i], normal, viewDir);
  }
  for(int i = 0; i < N_POINT_LIGHTS; i++){
    output_val += CalculatePointLight(point_light[i], normal, FragPos, viewDir);
  }
  for(int i = 0; i < N_SPOT_LIGHTS; i++){
    output_val += CalculateSpotLight(spot_light[i], normal, FragPos, viewDir);  
  }
 
  fragColor = vec4(output_val, 1.0);
  
  
  if(fog_activate != 0){
  
    vec3 u_fog_color = vec3 (0.3f, 0.3f, 0.3f);
    float distance = length(viewDir_fog);
    float visibility = exp(-pow((distance * 0.0007),1.5f));
    visibility = clamp(visibility,0.0f,1.0f);
    fragColor = mix(vec4(u_fog_color,1.0), fragColor, visibility);
    
     //fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
  }
}  
  

