
#include "minitrace.h"
#include "internals.h"
#include <stdio.h>
#include "px_sched.h"
#include <tiny_obj_loader.h>
#include <gtx/transform.hpp>
#include <geometrybuilder.h>
#include "GLFW/glfw3.h"
#include "lua.hpp"
#include <lua_manager.h>
#include "ecs.h"
#include "imgui.h"
#include <material.h>
#include <camera.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

namespace AWK {
  
  uint32_t Engine::next_mat_id = 0;
  uint32_t Engine::next_texture_id = 0;

  Engine::Engine(){ 

    _data = new Data();
    _data->_logger = std::make_unique<Logger>();
    _data->_window = std::make_unique<Window>();
    _GPUState = new GPUState();

    _entityData = new ECSData();
    _GPUState->currentActivatedShadows = 2;
    _GPUState->currentActivatedLights = 1;

    sceneSize = 30.f;

    _GPUState->atmosphereData.atmosphereDensity = 0.222f;
    _GPUState->atmosphereData.lightWave.x = 0.f;
    _GPUState->atmosphereData.lightWave.y = 0.830f;
    _GPUState->atmosphereData.lightWave.z = 1.f;

    onlyOnce = true;


  }

  Engine::~Engine(){ delete _data; delete _GPUState; }

  int bitReverse(int x) {
    unsigned int NO_OF_BITS = sizeof(x) * 8;
    unsigned int reverse_num = 0;
    int i;
    for (i = 0; i < NO_OF_BITS; i++) {
      if ((x & (1 << i)))
        reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }

    return reverse_num;
  }

  void Engine::init(int argc, char* argv[]){

    _data->_logger->initLogger(argc, argv);
    _GPUState->windowWidth = 1280;
    _GPUState->windowHeight = 720;
    _data->_window->initWindow(_GPUState->windowWidth, _GPUState->windowHeight, "Awakened Window");
    
    LuaLoadShader lls(_GPUState);

    const  int size = AWK::Engine::GPUState::kWaterSize;

    // -- CREATE TEXTURE -----------
    {
      Texture newTexture;

      newTexture.data = std::make_unique<unsigned char[]>(1);
      newTexture.data[0] = 255;
      newTexture.width = 1;
      newTexture.height = 1;

      newTexture.type = Texture::Type::RGBA;

      _GPUState->_textureArray.push_back(newTexture);
      next_texture_id++;

      /*Texture blackTexture;

      blackTexture.data = std::make_unique<unsigned char[]>(size*size);
      blackTexture.data[0] = 0;
      blackTexture.width = size;
      blackTexture.height = size;

      blackTexture.type = Texture::Type::RGBA;

      _GPUState->_textureArray.push_back(blackTexture);
      _GPUState->water.blackTexture = _GPUState->_textureArray.size()-1;
      next_texture_id++;*/

    } // --------- !CREATE TEXTURE --
       
    // --- INIT WATER ---------------
    {
      
      _GPUState->water.meshSize = size;

      _GPUState->waterData.data.x = 0.f;
      _GPUState->waterData.data.y = 0.f;
      _GPUState->waterData.data.z = _GPUState->water.meshSize;
      _GPUState->waterData.data.w = 0.f;    // --> Shall get time

      _GPUState->water.waterKData.data1 = glm::vec4(256.0f,1000.0f,20.0f,0.0f);
      _GPUState->water.waterKData.data2 = glm::vec4(1.0f,0.0f,26.0f,0.0f);

      _GPUState->water.waterKFData.data = glm::vec4(_GPUState->water.waterKData.data1.x, _GPUState->water.waterKData.data1.y, 0.f, 0.0f);

      _GPUState->water.butterflyData.x = 0;
      _GPUState->water.butterflyData.y = 0;
      _GPUState->water.butterflyData.z = 0;

      _GPUState->water.inversionData.x = _GPUState->water.butterflyData.y;

    }

    // --- WATER MESH INDICES ----------
    {
      float scale = 0.5f;
      int offset = 0;      
      int offsetsquare = 0;
      int meshWidth = _GPUState->water.meshSize-1;

      _GPUState->waterData.data.x = 0.5f;
      _GPUState->water.meshWaterData.oceanColor = glm::vec4(0.0f, 105.0f / 255.0f, 148.0f / 255.0f, 1.0f);
      for (int y = 0; y < meshWidth; ++y) {
        for (int x = 0; x < meshWidth; ++x) {

          offset = x + y * meshWidth;
          int index = offset + offsetsquare ;
          int offset1 = offset * 8;
          offset *= 2;

          _GPUState->water.meshIndices[offset1 + 0] = index ;
          _GPUState->water.meshIndices[offset1 + 1] = index + meshWidth + 1;
          _GPUState->water.meshIndices[offset1 + 2] = index + 1;
          _GPUState->water.meshIndices[offset1 + 3] = -1;
          _GPUState->water.meshIndices[offset1 + 4] = index + meshWidth + 1 + 1;
          _GPUState->water.meshIndices[offset1 + 5] = index + 1;
          _GPUState->water.meshIndices[offset1 + 6] = index + meshWidth + 1;
          _GPUState->water.meshIndices[offset1 + 7] = -1;

          /*printf(" I: %d -> 1st triangle: %f,%f,%f, 2nd triangle: %f,%f,%f\n", index - offsetsquare, 
            _GPUState->water.meshWaterData.meshIndicesArray[offset].x,
            _GPUState->water.meshWaterData.meshIndicesArray[offset].y,
            _GPUState->water.meshWaterData.meshIndicesArray[offset].z,
            _GPUState->water.meshWaterData.meshIndicesArray[offset + 1].x,
            _GPUState->water.meshWaterData.meshIndicesArray[offset + 1].y,
            _GPUState->water.meshWaterData.meshIndicesArray[offset + 1].z            
          );*/


        }
        offsetsquare++;
      }
   
      
    }

    // --- WATER DEVELOPMENT --------
    {
      _GPUState->water.renderProgram = _GPUState->_programsArray[AWK::Material::MaterialParameters::MaterialType::MaterialType_Water];
      
      const char* waterCompute = R"(

      #version 460

      layout(local_size_x = 1, local_size_y = 1) in;

      layout(binding = 0, rgba32f) readonly uniform image2D heighmap;

      layout(std140, binding = 5) buffer waterMeshBuffer
      {
        vec4 position[];
      };
      
      layout(std140, binding = 6) uniform waterMeshData
      {
        vec4 data;  // --> x = x, y = y, z = size, w = time
      };
      
      float scale = data.x;
      float size = data.z;
      vec2 initPos = vec2((-size*0.5f +0.5f)* scale, (-size*0.5f+0.5f) * scale);
      
      float time = data.w;
      
      void main() {
      
      
        uint index = uint(gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * size);
        float x = float(gl_GlobalInvocationID.x) * scale;
        float y = float(gl_GlobalInvocationID.y) * scale;
        position[index].x = x + initPos.x;
        position[index].y = clamp((imageLoad(heighmap, ivec2(x, y)).r)/255, -10, 10);
        position[index].z = y + initPos.y;
      
      })";


      const char* waterComputeNormal = R"(

      #version 460
        layout(local_size_x = 1, local_size_y = 1) in;

      layout(std140, binding = 4) buffer waterNormalMeshBuffer{
        vec4 normal[];
      };       
      layout(std140, binding = 5) buffer waterMeshBuffer
      {
        vec4 position[];
      };
      
      layout(std140, binding = 6) uniform waterMeshData
      {
        vec4 data;  // --> x = x, y = y, z = size, w = time
      };
      
      int size = int(data.z);

      void main() {
      
        int index1 = int(gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * (size-1));
        int offsetsquare = (index1 ) / (size-1);
        int index = index1 + offsetsquare;
        vec4 vec02 = position[index + 1] - position[index];
        vec4 vec01 = position[index + size] - position[index];
        vec3 n1 = cross(vec01.xyz, vec02.xyz);
        n1 = normalize(n1);
    
        vec4 vec34 = position[index + 1] - position[index + size + 1];
        vec4 vec35 = position[index + size] - position[index + size + 1];
        vec3 n2 = cross(vec34.xyz, vec35.xyz);
        n2 = normalize(n2);
      
       

        int nIndex = index1 * 2;
        normal[nIndex] = vec4(n1,1.0f); 
        normal[nIndex +1] = vec4(n2,1.0f);
  
      })";

     

      GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader, 1, &waterCompute, nullptr);
      glCompileShader(shader);
      //checkShader(shader);
      _GPUState->water.computeProgram = glCreateProgram();
            glAttachShader(_GPUState->water.computeProgram, shader);
      glLinkProgram(_GPUState->water.computeProgram);
      //checkProgram(compute.program);

      char msg[255];
      glGetShaderInfoLog(shader, 255, NULL, msg);
      printf("Compute Water:\n\n %s\n\n", msg);


      shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader, 1, &waterComputeNormal, nullptr);
      glCompileShader(shader);
      //checkShader(shader);
      _GPUState->water.computeNormalProgram = glCreateProgram();
      glAttachShader(_GPUState->water.computeNormalProgram, shader);
      glLinkProgram(_GPUState->water.computeNormalProgram);
      //checkProgram(compute.program);

      glGetShaderInfoLog(shader, 255, NULL, msg);
      printf("Compute Normal Water:\n\n %s\n\n", msg);

      const char* waterComputeK = R"(

      #version 460
      #define PI 3.14159265359f
      layout(local_size_x = 1, local_size_y = 1) in;

      layout(binding = 0, rgba32f) writeonly uniform image2D tilde_h0k;
      layout(binding = 1, rgba32f) writeonly uniform image2D tilde_h0minusk;

      layout (binding = 0) uniform sampler2D noise_r0; 
      layout (binding = 1) uniform sampler2D noise_i0; 
      layout (binding = 2) uniform sampler2D noise_r1; 
      layout (binding = 3) uniform sampler2D noise_i1; 


      layout(std140, binding = 3) uniform Kdata{
        vec4 data1;
        vec4 data2;
      };

      int N = int(data1.x);
      int L = int(data1.y);
      float A = data1.z;
      vec2 w = data2.xy;
      float windSpeed = data2.z;

      const float g = 9.81f;

      vec4 gaussRND(){

        vec2 texCoord = vec2(gl_GlobalInvocationID.xy)/float(N); 
            
        float noise0 = clamp(texture(noise_r0, texCoord).r, 0.001f, 1);
        float noise1 = clamp(texture(noise_i0, texCoord).r, 0.001f, 1);
        float noise2 = clamp(texture(noise_r1, texCoord).r, 0.001f, 1);
        float noise3 = clamp(texture(noise_i1, texCoord).r, 0.001f, 1);
  
        float u0 = 2.0f * PI * noise0;
        float v0 = sqrt(-4.0f * log(noise1));
        float u1 = 2.0f * PI * noise2;
        float v1 = sqrt(-4.0f * log(noise3));
         
        vec4 rnd = vec4(v0 * cos(u0), v0 * sin(u0), v1 * cos(u1), v1 * sin(u1)); 

        return rnd;

      }
      
      void main() {

        vec2 x = vec2(gl_GlobalInvocationID.xy);//- N/2;
        vec2 k = vec2(2.0f * PI * x.x/L, 2.0f * PI *x.y/L);

        float L_ = (windSpeed * windSpeed)/g;
        float mag = length(k);
        if(mag < 0.0001f){ mag = 0.0001f;}
      
        float mag2 = mag * mag;
        float mag4 = mag2 * mag2;

        vec2 minusK = vec2(-k);

        //sqrt(Ph(k))/sqrt(2)
        float h0k = clamp(sqrt((A/mag4) * pow(dot(normalize(k), normalize(w)), 5.0f) * exp(-(1.0f/(mag2 * L_ * L_))) * exp(-mag2*pow(L/2000.0f,2)))/sqrt(2.0f), -4000, 4000);
        float h0minusk = clamp(sqrt((A/mag4) * pow(dot(normalize(-k), normalize(w)), 5.0f) * exp(-(1.0f/(mag2 * L_ * L_))) * exp(-mag2*pow(L/2000.0f,2)))/sqrt(2.0f), -4000, 4000);
        
        vec4 gauss_random = gaussRND();

        imageStore(tilde_h0k, ivec2(gl_GlobalInvocationID.xy), vec4(gauss_random.xy * h0k, 0, 1));
        imageStore(tilde_h0minusk, ivec2(gl_GlobalInvocationID.xy), vec4(gauss_random.zw * h0minusk, 0, 1));
          
      })";


      shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader, 1, &waterComputeK, nullptr);
      glCompileShader(shader);
      //checkShader(shader);
      _GPUState->water.computeKProgram = glCreateProgram();
      glAttachShader(_GPUState->water.computeKProgram, shader);
      glLinkProgram(_GPUState->water.computeKProgram);
      //checkProgram(compute.program);

      glGetShaderInfoLog(shader, 255, NULL, msg);
      printf("Compute K Water:\n\n %s\n\n", msg);


      // Creation of the textures
      _GPUState->water.noiseTexture[0] = createTexture("../../data/textures/noise/Noise256_0.jpg", GL_NEAREST);
      _GPUState->water.noiseTexture[1] = createTexture("../../data/textures/noise/Noise256_1.jpg", GL_NEAREST);
      _GPUState->water.noiseTexture[2] = createTexture("../../data/textures/noise/Noise256_2.jpg", GL_NEAREST);
      _GPUState->water.noiseTexture[3] = createTexture("../../data/textures/noise/Noise256_3.jpg", GL_NEAREST);
      _GPUState->water.butterflyTexture = createTexture("../../data/textures/butterflyTexture.png", GL_NEAREST);

      _GPUState->water.textureTilde_k0 = createTexture(size, size, Texture::Type::RGBA32F, GL_NEAREST);
      _GPUState->water.textureTilde_kminus0 = createTexture(size, size, Texture::Type::RGBA32F,  GL_NEAREST);
      _GPUState->water.heigmapDisplacementTexture = createTexture(size, size, Texture::Type::RGBA32F, GL_NEAREST);
      _GPUState->water.choppyXTexture = createTexture(size, size, Texture::Type::RGBA32F, GL_NEAREST);
      _GPUState->water.choppyZTexture = createTexture(size, size, Texture::Type::RGBA32F, GL_NEAREST);
      uint32_t x = (uint32_t)(log(size) / log(2));
      _GPUState->water.twiddleIndiceTexture = createTexture(x, size, Texture::Type::RGBA32F, GL_NEAREST);
      _GPUState->water.pingpong1 = createTexture(size, size, Texture::Type::RGBA32F, GL_LINEAR);
      _GPUState->water.DyTexture = createTexture(size, size, Texture::Type::RGBA32F, GL_LINEAR);
      _GPUState->water.DxTexture = createTexture(size, size, Texture::Type::RGBA32F, GL_NEAREST);
      _GPUState->water.DzTexture = createTexture(size, size, Texture::Type::RGBA32F, GL_NEAREST);

      const char* waterFusionKCompute = R"(

      #version 460
      #define PI 3.14159265359f
      layout(local_size_x = 1, local_size_y = 1) in;

      layout(binding = 0, rgba32f) writeonly uniform image2D tilde_hkt_dy;    // Height displacement
      layout(binding = 1, rgba32f) writeonly uniform image2D tilde_hkt_dx;    // Choppy-x displacement
      layout(binding = 2, rgba32f) writeonly uniform image2D tilde_hkt_dz;    // Choppy-z displacement
      layout(binding = 3, rgba32f) readonly uniform image2D tilde_h0k;    
      layout(binding = 4, rgba32f) readonly uniform image2D tilde_h0minusk;  
      
      layout(std140, binding = 3) uniform WaterFusionKData{
        vec4 data; 
      };

      int L = int(data.y);
      float t = data.w;

      struct complex {
        
        float real;
        float im;

      };

      complex mul(complex c0, complex c1) {

        complex c;

        c.real = c0.real * c1.real - c0.im * c1.im;
        c.im = c0.real * c1.im + c0.im * c1.real;
        return c;

      }

      complex add(complex c0, complex c1) {

        complex c;

        c.real = c0.real + c1.real;
        c.im = c0.im + c1.im;

        return c;

      }

      complex conj(complex c) {

        complex c_conj = complex(c.real, -c.im);

        return c_conj;

      }
      
      void main() {
        
        vec2 x = ivec2(gl_GlobalInvocationID.xy);
        vec2 k = vec2(2.0f * PI * x.x/L, 2.0f * PI * x.y/L);

        float magnitude = length(k);
        if(magnitude < 0.0001f) magnitude = 0.0001f;

        float w = sqrt(9.81 * magnitude);

        complex complex_tilde_h0k = complex(imageLoad(tilde_h0k, ivec2(gl_GlobalInvocationID.xy)).r,
                                    imageLoad(tilde_h0k, ivec2(gl_GlobalInvocationID.xy)).g);

        complex complex_tilde_h0minuskconj = complex(imageLoad(tilde_h0minusk, ivec2(gl_GlobalInvocationID.xy)).r,
                                             imageLoad(tilde_h0minusk, ivec2(gl_GlobalInvocationID.xy)).g);

        float cosinus = cos(w*t);
        float sinus = sin(w*t);

        // Euler Formula
        complex exp_iwt = complex(cosinus, sinus);
        complex exp_iwt_inv = complex(cosinus, -sinus);

        // dy
        complex h_k_t_dy = add(mul(complex_tilde_h0k, exp_iwt), (mul(complex_tilde_h0minuskconj, exp_iwt_inv)));

        // dx
        complex dx = complex(0.0f, -k.x/magnitude);
        complex h_k_t_dx = mul(dx, h_k_t_dy);

        // dz
        complex dy = complex(0.0f, -k.y/magnitude);
        complex h_k_t_dz = mul(dy, h_k_t_dy);

        imageStore(tilde_hkt_dy, ivec2(gl_GlobalInvocationID.xy), vec4(h_k_t_dy.real, h_k_t_dy.im, 0, 1.f));
        imageStore(tilde_hkt_dx, ivec2(gl_GlobalInvocationID.xy), vec4(h_k_t_dx.real, h_k_t_dx.im, 0, 1.f));
        imageStore(tilde_hkt_dz, ivec2(gl_GlobalInvocationID.xy), vec4(h_k_t_dz.real, h_k_t_dz.im, 0, 1.f));
          
      })";

      shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader, 1, &waterFusionKCompute, nullptr);
      glCompileShader(shader);
      //checkShader(shader);
      _GPUState->water.computeKFusionProgram = glCreateProgram();
      glAttachShader(_GPUState->water.computeKFusionProgram, shader);
      glLinkProgram(_GPUState->water.computeKFusionProgram);
      //checkProgram(compute.program);

      glGetShaderInfoLog(shader, 255, NULL, msg);
      printf("Compute Fusion K Water:\n\n %s\n\n", msg);
            
      const char* waterTwiddleFactorsCompute = R"(

      #version 460
      #define PI 3.14159265359f

      layout(local_size_x = 1, local_size_y = 16) in;

      layout(binding = 0, rgba32f) writeonly uniform image2D twiddleIndices;

      layout(std140, binding = 7) buffer indices {

        int j[];

      };

      layout(std140, binding = 3) uniform WaterFusionKData{
        vec4 data; 
      };

      struct complex {

        float real;
        float im;

      };

      int N = int(data.x);
      float floatN = float(N);

      void main(void) {

        vec2 x = gl_GlobalInvocationID.xy;
                
        float k = mod(x.y * (floatN/pow(2, x.x+1)), N);

        complex twiddle = complex(cos(2.0f * PI * k/floatN), sin(2.0f * PI * k/floatN));

        int butterflySpan = int(pow(2, x.x));

        int butterflyWing;

        if(mod(x.y, pow(2, (x.x + 1))) < pow(2, x.x))
          butterflyWing = 1;
        else
          butterflyWing = 0;
        
        // First Stage, bit reversed indices
        if(x.x == 0) {
          
          // Top Butterfly Wing
          if(butterflyWing == 1) {
             if(x.y == 0)
              imageStore(twiddleIndices, ivec2(x), vec4(twiddle.real, twiddle.im, 0, 1));
            else
              imageStore(twiddleIndices, ivec2(x), vec4(twiddle.real, twiddle.im, 1, 1));
          }else{
            imageStore(twiddleIndices, ivec2(x), vec4(twiddle.real, twiddle.im, 1, 1));
          }

        // Second to log2(N) stage
        }else{

          // Top Butterfly Wing
          if(butterflyWing == 1) {
            imageStore(twiddleIndices, ivec2(x), vec4(twiddle.real, twiddle.im, x.y, x.y + butterflySpan));
          }else{
            imageStore(twiddleIndices, ivec2(x), vec4(twiddle.real, twiddle.im, x.y - butterflySpan, x.y));
          }

        }

      })";

      shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader, 1, &waterTwiddleFactorsCompute, nullptr);
      glCompileShader(shader);
      //checkShader(shader);
      _GPUState->water.computeTwiddleProgram = glCreateProgram();
      glAttachShader(_GPUState->water.computeTwiddleProgram, shader);
      glLinkProgram(_GPUState->water.computeTwiddleProgram);
      //checkProgram(compute.program);

      glGetShaderInfoLog(shader, 255, NULL, msg);
      printf("Compute Twiddle Factor Water:\n\n %s\n\n", msg);

      const char* waterButterflyCompute = R"(

      #version 460
      #define PI 3.14159265359f

      layout(local_size_x = 1, local_size_y = 1) in;

      layout(binding = 0, rgba32f) readonly uniform image2D twiddleIndices;

      layout(binding = 1, rgba32f) uniform image2D pingpong0;   // Dy Displacement

      layout(binding = 2, rgba32f) uniform image2D pingpong1;   // Black

      layout(std140, binding = 3) uniform WaterPingPongData {
        vec4 butterflyData; 
      };

      int stage = int(butterflyData.x);
      int pingpong = int(butterflyData.y);
      int direction = int(butterflyData.z);

      struct complex {

        float real;
        float im;

      };
      
      complex mul(complex c0, complex c1) {

        complex c;

        c.real = c0.real * c1.real - c0.im * c1.im;
        c.im = c0.real * c1.im + c0.im * c1.real;

        return c;

      }

      complex add(complex c0, complex c1) {

        complex c;

        c.real = c0.real + c1.real;
        c.im = c0.im + c1.im;

        return c;

      }

      void horizontalButterflies() {
        
        complex H;
        
        ivec2 x = ivec2(gl_GlobalInvocationID.xy);

        if(pingpong == 0) {

          vec4 data = imageLoad(twiddleIndices, ivec2(stage, x.x)).rgba;
          vec2 p_ = imageLoad(pingpong0, ivec2(data.z, x.y)).rg;
          vec2 q_ = imageLoad(pingpong0, ivec2(data.w, x.y)).rg;
          vec2 w_ = vec2(data.x, data.y);
          
          complex p = complex(p_.x, p_.y);
          complex q = complex(q_.x, q_.y);
          complex w = complex(w_.x, w_.y);

          // Butterfly Operation
          H = add(p, mul(w, q));

          imageStore(pingpong1, x, vec4(H.real, H.im, 0, 1));

        }else if(pingpong == 1) {

          vec4 data = imageLoad(twiddleIndices, ivec2(stage, x.x)).rgba;
          vec2 p_ = imageLoad(pingpong1, ivec2(data.z, x.y)).rg;
          vec2 q_ = imageLoad(pingpong1, ivec2(data.w, x.y)).rg;
          vec2 w_ = vec2(data.x, data.y);
          
          complex p = complex(p_.x, p_.y);
          complex q = complex(q_.x, q_.y);
          complex w = complex(w_.x, w_.y);

          // Butterfly Operation
          H = add(p, mul(w, q));

          imageStore(pingpong0, x, vec4(H.real, H.im, 0, 1));

        }

      }

      void verticalButterflies() {

        complex H;
        ivec2 x = ivec2(gl_GlobalInvocationID.xy);

        if(pingpong == 0) {

          vec4 data = imageLoad(twiddleIndices, ivec2(stage, x.y)).rgba;
          vec2 p_ = imageLoad(pingpong0, ivec2(x.x, data.z)).rg;
          vec2 q_ = imageLoad(pingpong0, ivec2(x.x, data.w)).rg;
          vec2 w_ = vec2(data.x, data.y);
          
          complex p = complex(p_.x, p_.y);
          complex q = complex(q_.x, q_.y);
          complex w = complex(w_.x, w_.y);

          // Butterfly Operation
          H = add(p, mul(w, q));

          imageStore(pingpong1, x, vec4(H.real, H.im, 0, 1));

        }else if(pingpong == 1) {

          vec4 data = imageLoad(twiddleIndices, ivec2(stage, x.y)).rgba;
          vec2 p_ = imageLoad(pingpong1, ivec2(x.x, data.z)).rg;
          vec2 q_ = imageLoad(pingpong1, ivec2(x.x, data.w)).rg;
          vec2 w_ = vec2(data.x, data.y);

          complex p = complex(p_.x, p_.y);
          complex q = complex(q_.x, q_.y);
          complex w = complex(w_.x, w_.y);

          // Butterfly Operation
          H = add(p, mul(w, q));

          imageStore(pingpong0, x, vec4(H.real, H.im, 0, 1));

        }

      }

      void main() {

        if(direction == 0)
          horizontalButterflies();
        else if(direction == 1)
          verticalButterflies();

      })";

        shader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(shader, 1, &waterButterflyCompute, nullptr);
        glCompileShader(shader);
        //checkShader(shader);
        _GPUState->water.computeButterflyProgram = glCreateProgram();
        glAttachShader(_GPUState->water.computeButterflyProgram, shader);
        glLinkProgram(_GPUState->water.computeButterflyProgram);
        //checkProgram(compute.program);

        glGetShaderInfoLog(shader, 255, NULL, msg);
        printf("Compute Butterfly Water:\n\n %s\n\n", msg);

        const char* waterInversionCompute = R"(

        #version 460
        #define PI 3.14159265359f

        layout(local_size_x = 16, local_size_y = 16) in;

        layout(binding = 0, rgba32f) writeonly uniform image2D displacement;

        layout(binding = 1, rgba32f) readonly uniform image2D pingpong0;

        layout(binding = 2, rgba32f) readonly uniform image2D pingpong1;

        layout(std140, binding = 3) uniform WaterInversionData {
          vec4 data; 
        };

        int N = int(data.x);
        int pingpong = int(data.y);

        void main(void) {

          ivec2 x = ivec2(gl_GlobalInvocationID.xy);

          float perms[] = {100.f, -100.f};
          int tmpValue = int(x.x + x.y);
          int index = int(mod(tmpValue, 2));
          float perm = perms[index];

          if(pingpong == 0) {

            float h = imageLoad(pingpong0, x).r;
            imageStore(displacement, x, vec4(perm * (h/float(N*N)),
                                             perm * (h/float(N*N)),
                                             perm * (h/float(N*N)),
                                             1));

            
        

          }else if(pingpong == 1) {

            float h = imageLoad(pingpong1, x).r;
            imageStore(displacement, x, vec4(perm * (h/float(N*N)),
                                             perm * (h/float(N*N)),
                                             perm * (h/float(N*N)),
                                             1));


          }

      })";

        shader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(shader, 1, &waterInversionCompute, nullptr);
        glCompileShader(shader);
        //checkShader(shader);
        _GPUState->water.computeInversionProgram = glCreateProgram();
        glAttachShader(_GPUState->water.computeInversionProgram, shader);
        glLinkProgram(_GPUState->water.computeInversionProgram);
        //checkProgram(compute.program);

        glGetShaderInfoLog(shader, 255, NULL, msg);
        printf("Compute Inversion Water:\n\n %s\n\n", msg);

    }
    
    // -- INIT BUFFERS ------------
    { 
      //Per Frame Data
      const GLsizeiptr perFrameDataSize = sizeof(GPUState::PerPassData);
      glCreateBuffers(1, &_GPUState->perPassDataBuffer);
      glNamedBufferStorage(_GPUState->perPassDataBuffer, perFrameDataSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 0, _GPUState->perPassDataBuffer, 0, perFrameDataSize);

      glEnable(GL_DEPTH_TEST);

      //Per Model Data
      const GLsizeiptr perModelDataSize = sizeof(Engine::GPUState::PerModelData);
      glCreateBuffers(1, &_GPUState->perModelDataBuffer);
      glNamedBufferStorage(_GPUState->perModelDataBuffer, perModelDataSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 1, _GPUState->perModelDataBuffer, 0, perModelDataSize);

      //Per Light Data
      const GLsizeiptr perLightDataSize = sizeof(Engine::GPUState::PerFrameData);
      glCreateBuffers(1, &_GPUState->perFrameDataBuffer);
      glNamedBufferStorage(_GPUState->perFrameDataBuffer, perLightDataSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 2, _GPUState->perFrameDataBuffer, 0, perLightDataSize);
            
      //Per Post Process Data
      const GLsizeiptr perPostProcessDataSize = sizeof(Engine::GPUState::PerPostProcessData);
      glCreateBuffers(1, &_GPUState->perPostProcessDataBuffer);
      glNamedBufferStorage(_GPUState->perPostProcessDataBuffer, perPostProcessDataSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 3, _GPUState->perPostProcessDataBuffer, 0, perPostProcessDataSize);

      // Scattering Data
      const GLsizeiptr scatteringDataSize = sizeof(Engine::GPUState::ScatteringData);
      glCreateBuffers(1, &_GPUState->scatteringDataBuffer);
      glNamedBufferStorage(_GPUState->scatteringDataBuffer, scatteringDataSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 4, _GPUState->scatteringDataBuffer, 0, scatteringDataSize);

      _GPUState->atmosphereData.density = 0.99f;
      _GPUState->atmosphereData.clarity = 0.2f;
      _GPUState->atmosphereData.pollution = 0.03f;
      _GPUState->atmosphereData.brightness = 10.f;
      _GPUState->atmosphereData.planet_scale = 1.f;
      _GPUState->atmosphereData.atmosphere_scale = 1.f;
      _GPUState->atmosphereData.sun_disk_radius = 0.1f;
      _GPUState->atmosphereData.sun_disk_intensity = 0.5f;
      
      _GPUState->scatteringData.airParameters[0] = _GPUState->atmosphereData.density;
      _GPUState->scatteringData.airParameters[1] = _GPUState->atmosphereData.clarity;
      _GPUState->scatteringData.airParameters[2] = _GPUState->atmosphereData.pollution;
      _GPUState->scatteringData.airParameters[3] = _GPUState->atmosphereData.brightness;
      
      _GPUState->scatteringData.sunAndAtmosphereParameters[0] = _GPUState->atmosphereData.planet_scale;
      _GPUState->scatteringData.sunAndAtmosphereParameters[1] = _GPUState->atmosphereData.atmosphere_scale;
      _GPUState->scatteringData.sunAndAtmosphereParameters[2] = _GPUState->atmosphereData.sun_disk_radius;
      _GPUState->scatteringData.sunAndAtmosphereParameters[3] = _GPUState->atmosphereData.sun_disk_intensity;

      _GPUState->scatteringData.sunWave[0] = 1.f;
      _GPUState->scatteringData.sunWave[1] = 1.f;
      _GPUState->scatteringData.sunWave[2] = 1.f;

      glNamedBufferSubData(_GPUState->scatteringDataBuffer, 0, scatteringDataSize, &_GPUState->scatteringData);

      // Water SSBO
      _GPUState->water.bufferSize = sizeof(glm::vec4) * _GPUState->water.meshSize * _GPUState->water.meshSize;
      glCreateBuffers(1, &_GPUState->water.SSBOPosition);
      glNamedBufferStorage(_GPUState->water.SSBOPosition, _GPUState->water.bufferSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 5, _GPUState->water.SSBOPosition, 0, _GPUState->water.bufferSize);

      _GPUState->water.bufferNormalSize = sizeof(glm::vec4) * (_GPUState->water.meshSize - 1) * (_GPUState->water.meshSize - 1) * 2;
      glCreateBuffers(1, &_GPUState->water.SSBONormal);
      glNamedBufferStorage(_GPUState->water.SSBONormal, _GPUState->water.bufferNormalSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 4, _GPUState->water.SSBONormal, 0, _GPUState->water.bufferNormalSize);

      // Water Data - Mesh Indices
      const GLsizeiptr waterMeshIndicesSize = sizeof(_GPUState->water.meshWaterData);
      glCreateBuffers(1, &_GPUState->water.waterMeshIndicesBuffer);
      glNamedBufferStorage(_GPUState->water.waterMeshIndicesBuffer, waterMeshIndicesSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 6, _GPUState->water.waterMeshIndicesBuffer, 0, waterMeshIndicesSize);

      // Water Data - Mesh Position & Size
      const GLsizeiptr waterMeshSize = sizeof(glm::vec4);
      glCreateBuffers(1, &_GPUState->waterData.waterDataBuffer);
      glNamedBufferStorage(_GPUState->waterData.waterDataBuffer, waterMeshSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 6, _GPUState->waterData.waterDataBuffer, 0, waterMeshSize);

      // Water K Data 
      const GLsizeiptr waterKDtatasize = sizeof(_GPUState->water.waterKData);
      glCreateBuffers(1, &_GPUState->water.dataKBuffer);
      glNamedBufferStorage(_GPUState->water.dataKBuffer, waterKDtatasize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 3, _GPUState->water.dataKBuffer, 0, waterKDtatasize);

      // Water K Fusion
      const GLsizeiptr waterKFsize = sizeof(_GPUState->water.waterKFData);
      glCreateBuffers(1, &_GPUState->water.dataKFBuffer);
      glNamedBufferStorage(_GPUState->water.dataKFBuffer, waterKFsize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 3, _GPUState->water.dataKFBuffer, 0, waterKFsize);

      // Twiddle
      const GLsizeiptr waterSBBOtwiddle = size * sizeof(uint32_t);
      glCreateBuffers(1, &_GPUState->water.SBBOtwiddle);
      glNamedBufferStorage(_GPUState->water.SBBOtwiddle, waterSBBOtwiddle, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 7, _GPUState->water.SBBOtwiddle, 0, waterSBBOtwiddle);

      int bits = (int)(log(size) / log(2));

      for (int i = 0; i < size; i++)
      {
        //printf("I : %d ", i);
        uint32_t x = bitReverse(i);
        //printf("x1 : %d ",x);
        const uint32_t mask = CHAR_BIT * sizeof(x) - 1;
        bits &= mask;
        x = (x << bits) | (x >> (-bits & mask));
        //x = x << bits;
        //printf("x2 : %d \n",x);
        // x = Integer.rotateLeft(x, bits);
        _GPUState->water.bitReversedIndices[i] = x;
      }

      
      const GLsizeiptr waterButterflyDatasize = sizeof(_GPUState->water.butterflyData);
      glCreateBuffers(1, &_GPUState->water.butterflyDataBuffer);
      glNamedBufferStorage(_GPUState->water.butterflyDataBuffer, waterButterflyDatasize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 3, _GPUState->water.butterflyDataBuffer, 0, waterButterflyDatasize);

      const GLsizeiptr waterSBBOIndicesSize = sizeof(_GPUState->water.meshIndices);
      glCreateBuffers(1, &_GPUState->water.SSBOIndices);
      glNamedBufferStorage(_GPUState->water.SSBOIndices, waterSBBOIndicesSize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 7, _GPUState->water.SSBOIndices, 0, waterSBBOIndicesSize);


      // Water Inversion
      const GLsizeiptr waterInversionsize = sizeof(_GPUState->water.inversionData);
      glCreateBuffers(1, &_GPUState->water.inversionDataBuffer);
      glNamedBufferStorage(_GPUState->water.inversionDataBuffer, waterInversionsize, NULL, GL_DYNAMIC_STORAGE_BIT);
      glBindBufferRange(GL_UNIFORM_BUFFER, 3, _GPUState->water.inversionDataBuffer, 0, waterInversionsize);


    } // ----------------!INIT BUFFERS --

    // -- INIT MINITRACE ----------
    {
      _data->minitrace = true;
      mtr_init("../../log/trace.json");
      mtr_register_sigint_handler();

      MTR_META_PROCESS_NAME("minitrace_test");
      MTR_META_THREAD_NAME("main thread");
    } // ------- !INIT MINITRACE --

    // -- INIT FRAMEBUFFER --------
    {
      glCreateFramebuffers(1, &_GPUState->frameBuffer);
    } // ------ !INIT FRAMEBUFFER --

    

    // -- INIT ECS ----------------
    {
      RenderComponent renderEmpty;
      TRSComponent lrsEmpty;
      LocalTransformComponent localTransformEmpty;
      WorldTransformComponent worldTransformEmpty;
      NodeComponent nodeEmpty;
      CameraComponent cameraEmpty;
      ViewProjectionComponent vpEmpty;
      LightComponent lightEmpty;
      FlyCameraMovementComponent flyCameraEmpty;
      SunComponent sunEmpty;

      _entityData->renderComponents.push_back(renderEmpty);
      _entityData->trsComponents.push_back(lrsEmpty);
      _entityData->localTransformComponents.push_back(localTransformEmpty);
      _entityData->worldTransformComponents.push_back(worldTransformEmpty);
      _entityData->nodeComponents.push_back(nodeEmpty);
      _entityData->cameraComponents.push_back(cameraEmpty);
      _entityData->VPComponents.push_back(vpEmpty);
      _entityData->lightComponents.push_back(lightEmpty);
      _entityData->flyCameraComponents.push_back(flyCameraEmpty);
      _entityData->sunComponents.push_back(sunEmpty);

      enableSystem(TRANSFORM_SYSTEM);
      enableSystem(TRANSFORM_TREE_SYSTEM);
      enableSystem(TRANSFORM_TREE_TO_ECS_SYSTEM);
      enableSystem(CAMERA_SYSTEM);
      enableSystem(BOUNDING_BOX_SYSTEM);
      //enableSystem(LIGHT_SYSTEM);

      // -- CREATE DEFAULT OBJECT & MATERIAL ---------------------------------------------
      {
        float vertices[] = {
          -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
          -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
           0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
           0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
        };
        uint32_t indices[] = { 0, 2, 1, 1, 2, 3 };
        _entityData->defaultObj = createGeometry(vertices, indices, sizeof(indices) / 4);

        AWK::Material::MaterialParameters new_mat;
        new_mat.data.textureColor.color.r = 1.0f;
        new_mat.data.textureColor.color.g = 0.0f;
        new_mat.data.textureColor.color.b = 0.0f;
        new_mat.data.textureColor.color.a = 1.0f;
        new_mat.type = AWK::Material::MaterialParameters::MaterialType_RenderShadow;
        _entityData->defaultMaterial = createMaterial(new_mat);

      }// --------------------------------------------!CREATE DEFAULT OBJECT & MATERIAL --

      //Create Root Entity
      {
        auto&& ecsTransaciton = _entityData->transaction();
        root = ecsTransaciton.createEntity(Component::AddComponentType::TRS_AND_TRANSFORM | Component::AddComponentType::ADD_NODE);
        uint64_t mask = Component::AddComponentType::ADD_CAMERA ;
        _entityData->camera = ecsTransaciton.createEntity(mask | Component::AddComponentType::ADD_FLY_CAMERA);
        CameraComponent* camComp = ecsTransaciton.getCameraComponent(_entityData->camera);
        camComp->width = _GPUState->windowWidth;
        camComp->height = _GPUState->windowHeight;
        camComp->aspectRatio = (float)camComp->width / (float)camComp->height;
        camComp->FOV = 1.042f;
        camComp->zFar = 200.0f;
        camComp->zNear = 0.1f;
        TRSComponent* trsComp = ecsTransaciton.getTRSComponent(_entityData->camera);
        trsComp->translation.x = 0.0f;//1.0f;
        trsComp->translation.y = 0.0f;//21.0f;
        trsComp->translation.z = 0.0f;//30.0f;
        //trsComp->rotation.x = 3.5f * 1.57f;
        //trsComp->rotation.y = 0.0f;
        //trsComp->rotation.z = 0.0f;

        FlyCameraMovementComponent* flyComp = ecsTransaciton.getFlyCameraComponent(_entityData->camera);
        flyComp->pitch = 0.0f;
        flyComp->yaw = 0.0f;
        flyComp->forward = { 0.0f, 0.0f, -1.0f };

        mask = mask | Component::AddComponentType::ADD_LIGHT;// | Component::AddComponentType::ADD_RENDER | Component::AddComponentType::ADD_NODE;
        _entityData->lights[0] = ecsTransaciton.createEntity(mask | Component::AddComponentType::ADD_FLY_CAMERA | Component::AddComponentType::ADD_SUN);
        camComp = ecsTransaciton.getCameraComponent(_entityData->lights[0]);
        camComp->width = 2048;
        camComp->height = 2048;
        camComp->aspectRatio = (float)camComp->width / (float)camComp->height;
        camComp->FOV = 1.042f;
        camComp->zFar = 50.f; //27.6f;
        camComp->zNear = 0.1f;
        trsComp = ecsTransaciton.getTRSComponent(_entityData->lights[0]);
        trsComp->translation.x = 0.f; //-16.887f; /*-12.6f;  0.f;*/
        trsComp->translation.y = 11.f;//7.563f;//16.8f; //17.729f;
        trsComp->translation.z = 0.f;
        trsComp->rotation.x = 4.710f;
        trsComp->rotation.y = 0.0f; /*-0.673f; */ //0.0f;
        trsComp->rotation.z = 0.0f;
        LightComponent * lightComp = ecsTransaciton.getLightComponent(_entityData->lights[0]);
        lightComp->direction.x = 0;
        lightComp->direction.y = -1;
        lightComp->direction.z = 0;
        lightComp->r = 1.0f;
        lightComp->g = 1.0f;
        lightComp->b = 1.0f;
        lightComp->intensity = 4000.0f;
        SunComponent * sunComp = ecsTransaciton.getSunComponent(_entityData->lights[0]);
        sunComp->lookPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        //RenderComponent* renderComp = ecsTransaciton.

         _entityData->lights[1] = ecsTransaciton.createEntity(mask);
         camComp = ecsTransaciton.getCameraComponent(_entityData->lights[1]);
         camComp->width = 2048;
         camComp->height = 2048;
         camComp->aspectRatio = (float)camComp->width / (float)camComp->height;
         camComp->FOV = 1.042f;
         camComp->zFar = 100.0f;
         camComp->zNear = 0.1f;
         trsComp = ecsTransaciton.getTRSComponent(_entityData->lights[1]);
         trsComp->translation.x = 0.f; 
         trsComp->translation.y = 11.f;
         trsComp->translation.z = 0.f;
         trsComp->rotation.x = 4.717f;
         trsComp->rotation.y = 0.0f;
         trsComp->rotation.z = 0.0f;
 
 
         _entityData->lights[2] = ecsTransaciton.createEntity(mask);
          camComp = ecsTransaciton.getCameraComponent(_entityData->lights[2]);
         camComp->width = 2048;
         camComp->height = 2048;
         camComp->aspectRatio = (float)camComp->width / (float)camComp->height;
         camComp->FOV = 1.042f;
         camComp->zFar = 100.0f;
         camComp->zNear = 0.1f;
         trsComp = ecsTransaciton.getTRSComponent(_entityData->lights[2]);
         trsComp->translation.x = 0.f;
         trsComp->translation.y = 11.f;
         trsComp->translation.z = 0.f;
         trsComp->rotation.x = 4.717f;
         trsComp->rotation.y = 0.0f;
         trsComp->rotation.z = 0.0f;
 
 
         _entityData->lights[3] = ecsTransaciton.createEntity(mask);
         camComp = ecsTransaciton.getCameraComponent(_entityData->lights[3]);
         camComp->width = 2048;
         camComp->height = 2048;
         camComp->aspectRatio = (float)camComp->width / (float)camComp->height;
         camComp->FOV = 1.042f;
         camComp->zFar = 100.0f;
         camComp->zNear = 0.1f;
         trsComp = ecsTransaciton.getTRSComponent(_entityData->lights[3]);
         trsComp->translation.x = 0.f;
         trsComp->translation.y = 11.f;
         trsComp->translation.z = 0.f;
         trsComp->rotation.x = 4.717f;
         trsComp->rotation.y = 0.0f;
         trsComp->rotation.z = 0.0f;

         _GPUState->maxBias = 0.00941713;//0.000065f;
         _GPUState->minBias = 0.00147991f;//0.0000030f;

      }

      for (unsigned int s = 0; s < _entityData->numberActivatedSystems; ++s) {
        systems[s]->init();
      }
      _data->LuaScene.init(this, _entityData);
      

    } // ----------------!INIT ECS --

      // -- INIT GPUSTATE-------------
    {
      _GPUState->blurProgram = AWK::Material::MaterialParameters::MaterialType::MaterialType_Blur;
      //_GPUState->shadowProgram = AWK::Material::MaterialParameters::MaterialType::MaterialType_RenderShadow;

    }// ------------- !INIT GPUSTATE--
  }

  uint32_t Engine::loadObj(const char* path){

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string err;
  
    tinyobj::LoadObj(&attrib, &shape, &material, &err, path);

    uint32_t size_obj = (uint32_t)shape[0].mesh.indices.size();

    AWK::GeometryBuilder geo;
    AWK::GeometryBuilder::VertexAtrrib new_vertex;

    uint32_t offsetVertex = 3;
    uint32_t UVoffsetVertex = 2;
    bool hasUV = true;
    if(attrib.texcoords.size() == 0){
      hasUV = false;
    }

    for (uint32_t i = 0; i < size_obj; i++) {
      int32_t vertex_index = shape[0].mesh.indices[i].vertex_index * 3;
      int32_t normal_index = shape[0].mesh.indices[i].normal_index * 3;
      int32_t uv_index = shape[0].mesh.indices[i].texcoord_index * 2;
      new_vertex.x = attrib.vertices[vertex_index + 0];
      new_vertex.y = attrib.vertices[vertex_index + 1];
      new_vertex.z = attrib.vertices[vertex_index + 2];
      new_vertex.nx = attrib.normals[normal_index + 0];
      new_vertex.ny = attrib.normals[normal_index + 1];
      new_vertex.nz = attrib.normals[normal_index + 2]; 
      if(hasUV){
        new_vertex.u = attrib.texcoords[uv_index];
        new_vertex.v = attrib.texcoords[uv_index + 1];
      }
      else{
        new_vertex.u = 0.0f;
        new_vertex.v = 0.0f;
      } 
      geo.addVertex(&new_vertex);
    }
    return geo.createGeometry(*this);

  }

  uint32_t Engine::createTexture(const char* path, GLint minMagFilter){

    Texture newTexture;

    int w, h, n;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* tmpImg = stbi_load(path, &w, &h, &n, 0);
    
    newTexture.width = (uint32_t)w;
    newTexture.height = (uint32_t)h;
    newTexture.minMagFilter= minMagFilter;
    
    
    newTexture.type = Texture::Type::RGBA;

    switch(n) {

      case 1:
        newTexture.type = Texture::Type::R;
      break;

      case 2:
        newTexture.type = Texture::Type::RG;
      break;

      case 3:
        newTexture.type = Texture::Type::RGB;
      break;

      case 4:
        newTexture.type = Texture::Type::RGBA;
      break;

    }

    newTexture.data = std::make_unique<unsigned char[]>(w * h * n);
    memcpy(newTexture.data.get(), tmpImg, w * h * n);

    _GPUState->_textureArray.push_back(std::move(newTexture));
    uint32_t newTextureIndex = (uint32_t)_GPUState->_textureArray.size() - 1;
    free(tmpImg);

    return next_texture_id++;
   
  }

  uint32_t Engine::createTexture(uint32_t width, uint32_t height, Texture::Type textureType, GLint minMagFilter) {

    Texture newTexture;
    newTexture.width = width;
    newTexture.height = height;
    newTexture.type = textureType;
    newTexture.minMagFilter = minMagFilter;

    int n = 4;

    switch (textureType) {

      case Texture::Type::R:
        n = 1;
        break;

      case Texture::Type::RG:
        n = 2;
        break;

      case Texture::Type::RGB:
        n = 3;
        break;

      case Texture::Type::RGBA:
        n = 4;
        break;

      case Texture::Type::RGBA32F:
        n = 4;
        break;

      case Texture::Type::DEPTH:
        n = 1;
        break;

    }

    newTexture.data = std::make_unique<unsigned char[]>(width * height * n);
    _GPUState->_textureArray.push_back(newTexture);
    uint32_t newTextureIndex = (uint32_t)_GPUState->_textureArray.size()-1;

    return next_texture_id++;

  }

  void Engine::setTexturesFBO(uint32_t cT, uint32_t dT)
  {
    textureDepth = dT;
    textureColor = cT;
  }

  void Engine::showTextures(uint32_t blitTextureID){
    this->blitTextureID = blitTextureID;
    
  }
    
  uint32_t Engine::createGeometry(float* vertex_attrib, uint32_t* vertex_indices, uint32_t size_geometry) {

    
    AWK::GeometryBuilder geo;
    AWK::GeometryBuilder::VertexAtrrib new_vertex;


    for (unsigned int i = 0; i < size_geometry; i++) {
      int32_t index = vertex_indices[i] * sizeof(AWK::GeometryBuilder::VertexAtrrib)/ sizeof(float);
      new_vertex.x = vertex_attrib[index + 0];
      new_vertex.y = vertex_attrib[index + 1];
      new_vertex.z = vertex_attrib[index + 2];
      new_vertex.nx = vertex_attrib[index + 3];
      new_vertex.ny = vertex_attrib[index + 4];
      new_vertex.nz = vertex_attrib[index + 5];
      new_vertex.u = vertex_attrib[index + 6];
      new_vertex.v = vertex_attrib[index + 7];
      geo.addVertex(&new_vertex);
    }
    return geo.createGeometry(*this);
  }

  uint32_t Engine::createMaterial(AWK::Material::MaterialParameters newMat) {

    _GPUState->_materialArray.push_back(newMat);
    uint32_t next_id = next_mat_id++;
    return next_id;
  }

  void Engine::changeMaterialColor(uint32_t mat, AWK::Material::RGBA color){

    _GPUState->_materialArray[mat].data.textureColor.color = color;

  }

  void Engine::changeMaterialTexture(uint32_t mat, uint32_t texture){
    _GPUState->_materialArray[mat].data.textureColor.texture = texture;
  }

  double Engine::getTime() { return glfwGetTime(); }

  void Engine::enableSystem(SystemType type) {

    switch(type){

      case RENDER_SYSTEM: systems.push_back(std::make_unique<RenderSystem>()); break;

      case TRANSFORM_SYSTEM: systems.push_back(std::make_unique<TransformSystem>()); break;

      case TRANSFORM_TREE_SYSTEM :
        systems.push_back(std::make_unique<TransformTreeSystem>()); break;

      case TRANSFORM_TREE_TO_ECS_SYSTEM:
        systems.push_back(std::make_unique<TransformTreeToECSSystem>()); break;

      case CAMERA_SYSTEM:
        systems.push_back(std::make_unique<CameraSystem>()); break;

      case LIGHT_SYSTEM:
        systems.push_back(std::make_unique<LightSystem>()); break;

      case BOUNDING_BOX_SYSTEM:
        systems.push_back(std::make_unique<BoundingBoxSystem>()); break;
     
      default: printf("Add your system here\n"); break;
    }
    _entityData->numberActivatedSystems = (uint32_t)systems.size();
  }

  void Engine::startRenderLoop(std::function<void()>&& logicFunc) {

    px_sched::Scheduler schd;
    px_sched::Sync s0;

    schd.init();
    schd.run(logicFunc, &s0);
    while(_data->_window.get()->isOpened()){

      schd.waitFor(s0);
      schd.run(logicFunc, &s0);//update
      render();

      MTR_BEGIN("main", "SAWP");
      _data->_window.get()->swap();
      MTR_END("main", "SAWP");

    }

    schd.waitFor(s0);
    End();

  }
  
  void Engine::submit(DisplayList&& list) {

      // Lock the mutex
    std::unique_lock<std::mutex> lock_dplmutex(_data->_displayListMutex);
    
      // Push back display list to the list of them
    _data->_listDisplayList.push_back(std::move(list)); //Problem
    
      // Unlock it
    lock_dplmutex.unlock();
    
      // Condition variable to awake render thread
    _data->_cvDisplayList.notify_one();

  }

  void Engine::renderSubmit(DisplayList&& list, uint32_t colorTex, uint32_t depthTex) {
          
      // Lock the mutex
      std::unique_lock<std::mutex> lock_dplmutex(_data->_displayListMutex);

      list.textures[0] = colorTex;
      list.textures[1] = depthTex;
      _data->_listDisplayList.push_back(std::move(list));

      // Unlock it
      lock_dplmutex.unlock();

      // Condition variable to awake render thread
      _data->_cvDisplayList.notify_one();

    //}

  }

  void UploadScatteringData(Engine::GPUState* _GPUState, Engine* engine) {
    
    auto&& transaction = engine->_entityData->transaction();
    LocalTransformComponent* cameraPosition = transaction.getLocalTransformComponent(engine->_entityData->camera);
    CameraComponent* cameraComponent = transaction.getCameraComponent(engine->_entityData->camera);
    FlyCameraMovementComponent* cameraFlyMovComp = transaction.getFlyCameraComponent(engine->_entityData->camera);

    const GLsizeiptr scatteringDataSize = sizeof(Engine::GPUState::ScatteringData);
    glBindBufferRange(GL_UNIFORM_BUFFER, 4, _GPUState->scatteringDataBuffer, 0, scatteringDataSize);

    _GPUState->scatteringData.eyePosition = {cameraPosition->localMatrix[3].x, cameraPosition->localMatrix[3].y, cameraPosition->localMatrix[3].z, cameraComponent->FOV};
    _GPUState->scatteringData.eyeCenterPosition = {cameraFlyMovComp->forward.x, cameraFlyMovComp->forward.y, cameraFlyMovComp->forward.z, 1.f};
    _GPUState->scatteringData.eyeUpDir = {cameraFlyMovComp->up.x, cameraFlyMovComp->up.y, cameraFlyMovComp->up.z, 1.f};

    _GPUState->scatteringData.airParameters[0] = _GPUState->atmosphereData.density;
    _GPUState->scatteringData.airParameters[1] = _GPUState->atmosphereData.clarity;
    _GPUState->scatteringData.airParameters[2] = _GPUState->atmosphereData.pollution;
    _GPUState->scatteringData.airParameters[3] = _GPUState->atmosphereData.brightness;

    _GPUState->scatteringData.sunAndAtmosphereParameters[0] = _GPUState->atmosphereData.planet_scale;
    _GPUState->scatteringData.sunAndAtmosphereParameters[1] = _GPUState->atmosphereData.atmosphere_scale;
    _GPUState->scatteringData.sunAndAtmosphereParameters[2] = _GPUState->atmosphereData.sun_disk_radius;
    _GPUState->scatteringData.sunAndAtmosphereParameters[3] = _GPUState->atmosphereData.sun_disk_intensity;

    glNamedBufferSubData(_GPUState->scatteringDataBuffer, 0, scatteringDataSize, &_GPUState->scatteringData);

  }

  void Engine::render() {

    MTR_BEGIN("main", "render");
    bool swap_exit = false;

    UploadScatteringData(_GPUState, this);
    //Lock the mutex
    do {

      std::unique_lock<std::mutex> lock_dplmutex(_data->_displayListMutex);
      //List is empty, no task to do stop search for lists
      if (_data->_listDisplayList.empty()) {
        _data->_cvDisplayList.wait(lock_dplmutex);
      }
      else {
        DisplayList tmp_list = std::move(_data->_listDisplayList.front());
        _data->_listDisplayList.pop_front();
        lock_dplmutex.unlock();
        tmp_list.AttachTextures(_GPUState);
        swap_exit = tmp_list.executeOnGPU(_GPUState);
      }

    } while (!_data->_listDisplayList.empty() && !swap_exit);

    blitTexture();
    _data->_window.get()->drawImgui(this);
    _data->_window.get()->moveCamera(this);
    
    MTR_END("main", "render");
  }

  void Engine::blitTexture()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, _GPUState->windowWidth, _GPUState->windowHeight);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(_GPUState->_programsArray[0]);
    glBindTextures(0, 1, &_GPUState->_textureArray[blitTextureID].openGLid);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
  }

  void Engine::update() {

    uint64_t map_mask = 0;
    float time = (float)getTime();
    _data->LuaScene.update(time);
    _GPUState->waterData.data.w = time;

    // Water K Fusion
    _GPUState->water.waterKFData.data.w = time;

    //SUN POSITION
    {

      auto&& ecsTransaciton = _entityData->transaction();
      TRSComponent* trsComp = ecsTransaciton.getTRSComponent(_entityData->lights[0]);

      trsComp->translation.x = _GPUState->scatteringData.u_sunPosition.x * 100.0f;
      trsComp->translation.y = _GPUState->scatteringData.u_sunPosition.y * 100.0f;
      trsComp->translation.z = _GPUState->scatteringData.u_sunPosition.z * 100.0f;

      FlyCameraMovementComponent* flyComp = ecsTransaciton.getFlyCameraComponent(_entityData->lights[0]);
      flyComp->pitch = -_GPUState->atmosphereData.pitch;
      flyComp->yaw = -_GPUState->atmosphereData.yaw;

      LightComponent* lightComp = ecsTransaciton.getLightComponent(_entityData->lights[0]);
      lightComp->r = _GPUState->scatteringData.sunWave[0];
      lightComp->g = _GPUState->scatteringData.sunWave[1];
      lightComp->b = _GPUState->scatteringData.sunWave[2];
      lightComp->intensity = _GPUState->atmosphereData.brightness * 400.0f;
  
      
    }
    for (uint32_t i = 0; i < _entityData->numberActivatedSystems; ++i) {

      systems[i]->begin(this);
      for (auto& x : _entityData->entitiesMap) {
        map_mask = x.first;

        // Check keymap with the key of each system
        uint64_t presentmaskAnd = systems[i]->presentmask & map_mask;
        uint64_t missingmaskAnd = systems[i]->missingmask & map_mask;
        bool compatibleWithSystem = presentmaskAnd == systems[i]->presentmask;
        compatibleWithSystem = compatibleWithSystem && (missingmaskAnd == 0);
        if(compatibleWithSystem)
        {
          for (int j = 0; j < _entityData->entitiesMap[map_mask].size(); ++j) {
            
            systems[i]->execute(this, _entityData->entitiesMap[map_mask][j]->id);
          }
        }
      }
      systems[i]->end(this);
    }

    _data->composerTextureListCounter = 0;
    LuaComposer luaComposer(this);

  }

  void Engine::End() {
    if (_data->minitrace) {
      mtr_flush();
      mtr_shutdown();
    }
    _data->_window->closeWindow();

  }

  uint32_t Engine::requestTexture(uint32_t width, uint32_t height, Texture::Type type) {

    if(!_GPUState->_textureArray.empty() &&
        _data->composerTextureListCounter < composerTextureList.size()) {
    
      std::list<uint32_t>::iterator it = composerTextureList.begin();
      uint32_t cnt = 0;

      bool textureValid = false;

      while (cnt < _data->composerTextureListCounter && it != composerTextureList.end()) {
        
        ++cnt;
        ++it;

      }
    
      if (*it < _GPUState->_textureArray.size()) {

        textureValid = _GPUState->_textureArray[*it].width == width &&
                       _GPUState->_textureArray[*it].height == height &&
                       _GPUState->_textureArray[*it].type == type;
      }
    
      ++_data->composerTextureListCounter;
      if(textureValid) {
        return cnt;
      }else{
        DisplayList dl;
        dl.deleteTexture(_GPUState->_textureArray[*it].openGLid);
        composerTextureList.erase(it);
        submit(std::move(dl));
      }

    }

    uint32_t textureIndex = createTexture(width, height, type, GL_LINEAR);  // Example - Returns: 5 (_GPUState->textureArray)
                                                                 // Corresponds with: index[i] composerTextureList;
    
    composerTextureList.push_back(textureIndex);

    ++_data->composerTextureListCounter;
    return (uint32_t)(composerTextureList.size()-1);
    
  }

  void Engine::printM4(const glm::mat4 m, const char* name){
    printf("%s\n", name);
    printf("%06f | %06f | %06f | %06f\n", m[0].x, m[0].y, m[0].z, m[0].w);
    printf("%06f | %06f | %06f | %06f\n", m[1].x, m[1].y, m[1].z, m[1].w);
    printf("%06f | %06f | %06f | %06f\n", m[2].x, m[2].y, m[2].z, m[2].w);
    printf("%06f | %06f | %06f | %06f\n", m[3].x, m[3].y, m[3].z, m[3].w);
    printf("\n\n");
  }
  
}
