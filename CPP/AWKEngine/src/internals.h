/**
 * Internals Files
 * Developed by Javier BA & Ricardo BM
 * Graphic Engine Development
 * ESAT - 2022 */

#ifndef __INTERNALS_H__
#define __INTERNALS_H__ 1
#include "window.h"
#include <mutex>
#include <glad/glad.h>
#include "displayList.h"
#include "fmod.hpp"
#include <logger.h>
#include "sound_manager.h"
#include <engine.h>
#include <vector>
#include <glm.hpp>
#include <material.h>
#include <component.h>
#include <lua_manager.h>

#include <map>
#include <transformtree.h>
#include <texture.h>

class GLFWWindow;
enum FMOD_RESULT;

namespace AWK{

  struct EntityHandler {

    uint32_t components[Component::ComponentType::MAX] = {}; // 0 nothing 
    uint64_t componentMask;
    uint32_t mapID;
    uint32_t id;

  };

  struct Engine::Data {

    LuaCreateScene LuaScene;
    std::unique_ptr<Window> _window;
    std::unique_ptr<Logger> _logger;
    std::mutex _displayListMutex;
    std::condition_variable _cvDisplayList;
    std::list<DisplayList> _listDisplayList;
    bool minitrace;  
    uint32_t composerTextureListCounter;

  };

  struct Engine::GPUState {

    static const uint32_t kMaxShadows = 4;
    static const uint32_t kMaxLigths = 16;
    static const uint32_t kWaterSize = 256;
    static const uint32_t kNwaterIndex = (kWaterSize - 1) * (kWaterSize - 1) * 8;
    uint32_t currentActivatedShadows;
    uint32_t currentActivatedLights;
    uint32_t blurProgram;
    //uint32_t shadowProgram;

    GLuint perPassDataBuffer;
    GLuint perFrameDataBuffer;
    GLuint perModelDataBuffer;
    GLuint perPostProcessDataBuffer;
    GLuint scatteringDataBuffer;
    GLuint materialBuffer;

    GLuint frameBuffer;
    uint32_t colorTexture, depthTexture;
    uint32_t windowWidth, windowHeight;
    
    
    struct MeshWaterData {
     // glm::vec4 meshIndicesArray[(kWaterSize-1) * (kWaterSize - 1) * 2];
      glm::mat4 view;
      glm::mat4 proyection;
      glm::vec4 oceanColor;
    };

    struct WaterData {
      GLuint waterDataBuffer;
      glm::vec4 data;  // --> x = x, y = y, z = size, w = time
    };
    WaterData waterData;


    // int N = int(data1.x);
    // int L = int(data1.y);
    // float A = data1.z;
    // vec2 w = data2.xy;
    // float windSpeed = data2.z;
    struct KData {
      glm::vec4 data1; 
      glm::vec4 data2; 
    };

    // int N = int(data.x);
    // int L = int(data.y);
    // float time = data.w;
    struct waterFusionKData {
      glm::vec4 data;
    };

    struct Water{
      GLuint SSBOPosition;  // Shader Storage Buffer Object
      GLuint SSBONormal;  // Shader Storage Buffer Object
      GLuint SSBOIndices;  // Shader Storage Buffer Object
      GLuint VAO;
      GLuint VBO;
  
      GLuint computeProgram;  
      GLuint computeNormalProgram;  
      GLuint computeKProgram;
      GLuint computeKFusionProgram;
      GLuint computeTwiddleProgram;
      GLuint computeButterflyProgram;
      GLuint computeInversionProgram;
      GLuint renderProgram;
      uint32_t meshSize;
      uint32_t bufferSize;
      uint32_t bufferNormalSize;
      uint32_t numElements;
      uint32_t meshIndices[kNwaterIndex];
      GLuint waterMeshIndicesBuffer;
      MeshWaterData meshWaterData;

      KData waterKData;
      uint32_t noiseTexture[4];
      uint32_t textureTilde_k0;
      uint32_t textureTilde_kminus0;
      GLuint dataKBuffer;

      waterFusionKData waterKFData;
      uint32_t heigmapDisplacementTexture;
      uint32_t choppyXTexture;
      uint32_t choppyZTexture;
      uint32_t pingpong1;
      uint32_t DyTexture;
      uint32_t DxTexture;
      uint32_t DzTexture;
      GLuint dataKFBuffer;

      uint32_t twiddleIndiceTexture;
      uint32_t SBBOtwiddle;
      int32_t bitReversedIndices[kWaterSize];

      GLuint butterflyDataBuffer;
      glm::vec4 butterflyData;
      uint32_t butterflyTexture;

      GLuint inversionDataBuffer;
      glm::vec4 inversionData;

    };
    Water water;

    struct PerPostProcessData {
      float data[4];
    };

    struct PerPassData{
      glm::mat4 vm;
      glm::mat4 pm;
    };

    struct ShadowData {
      glm::mat4 MVL[kMaxShadows];
      glm::mat4 MPL[kMaxShadows];
      float data[4];
    };

    struct LightData {

      glm::vec4 positionAndType[kMaxLigths];
      glm::vec4 directionAndCutoff[kMaxLigths];
      glm::vec4 colorAndIntensity[kMaxLigths];
      glm::vec4 lightProperties[kMaxLigths];

    };

    struct PerFrameData {

      ShadowData shadows;
      LightData lights;

    };

    struct PerModelData {
      glm::mat4 modelMatrix;
      glm::mat4 normalMat;
      AWK::Material::MaterialParameters::Data materialData;
    };

    struct ScatteringData {

      glm::vec4 eyePosition;       // eyePosition.w == fov
      glm::vec4 u_sunPosition;
      glm::vec4 eyeCenterPosition;
      glm::vec4 eyeUpDir;
      glm::vec4 airParameters;    // x -> density, y -> clarity, z -> pollution, w -> brightness
      glm::vec4 sunAndAtmosphereParameters;   // x -> planet scale, y -> atmosphere scale, z -> sun disk radius, w -> sun disk intensity
      glm::vec4 sunWave;    // x -> r, y -> g, z -> b

    };

    struct AtmosphereDataImGUI {

      float density;
      float clarity;
      float pollution;
      float planet_scale;
      float atmosphere_scale;
      float sun_disk_radius;
      float brightness;
      float sun_disk_intensity;

      int32_t sunBrightness;
      float atmosphereDensity;
      glm::vec3 lightWave;
      float yaw, pitch;
      
    };

    struct Geometry {
      GLuint ID;
      GLuint VAO;  
      uint32_t numElements;
    };
    
    std::vector<Geometry> _geometryArray;
    std::vector <AWK::Material::MaterialParameters> _materialArray;
    
    std::vector<Texture> _textureArray; 
    std::vector<GLuint> _programsArray;  
    GLuint _shadowTexture[kMaxShadows];
    GLuint _lightShadowTexture[kMaxShadows];
    PerFrameData frameData;
    ScatteringData scatteringData;
    AtmosphereDataImGUI atmosphereData;
    float minBias;
    float maxBias;
    
  };

  struct Window::Data {

    GLFWwindow* _mainWindow;

    int32_t _width;
    int32_t _height;

    GLuint imguiVao;
    GLuint imguiProgram;
    GLuint imguihandleVBO;
    GLuint imguihandleElements;


  };
    
  const int32_t kNChannels = 32; ///< maximum number of channels 
  /** @struct Data
   *  @brief This structure collects all the data for the sound demo
   */
  struct SoundManager::Data {
    FMOD::System* _soundSystem; ///< fmod sound system
    FMOD::Channel* _channel[kNChannels]; ///< array of channels for the effects
    FMOD::Channel* piano_channel[kNChannels];///< array of channels for the piano
    FMOD::Sound* _sound[kNChannels];///< array of sounds
    FMOD::DSP* dsp;
    FMOD_RESULT _result;///< FMOD variable to collect errors
    int32_t id_sound;   ///< used to know how many sound are loaded

    float pianof[12];   ///< frequency for the piano's notes

    //Crossfading
    bool crossfading_mode;      ///< represents the state of the crossfading true sound 1 activated and 0 stopped otherwise 0 activated and 1 stopped
    bool crossfading_active;    ///< represents the state of the crossfading true if it still in the transition false it doesn't
    int32_t sound_fade_in_id;   ///< id of the sound to play it and then increase their gain 
    int32_t sound_fade_out_id;  ///< id of the sound to decrease their gain and then stop it 
    float crossfade_time;       ///< time for the crossfading effect

    //BRANCHING
    /** @struct Branching
     *  @brief This structure manages the branching-sound mode
     */
    struct Branching{
      int32_t id; ///Node id
      int32_t sound_id;
      enum SoundType {
        kSoundType_Normal = 0, ///< Normal Value
        kSoundType_Transition, ///< Transition Value
        kSoundType_Combat,     ///< Combat Value
      };
      SoundType type;       ///< Defines the type of the sound
      int32_t defaultNext;  ///< Default next node
      int32_t nextList;
    };
    bool changed;                  ///< Determines whether to change or not the current node
    bool end_branch;               ///< True when the sound of the current node has finished
    int32_t default_next_stage;    ///< Default next stage
    int32_t next_stage;            ///< Defines the next stage
    int32_t posible_next_stage;    ///< Defines a possible next stage
    int32_t transition;            ///< Determines whether it has transition or not
  };

}

#endif // !__INTERNALS_H__