#include <displaylist.h>
#include "glm.hpp"
#include <gtx/transform.hpp>
#include "internals.h"

namespace AWK{

  DisplayList::DisplayList() {
    endWithSwap = false;
    textures[0] = 0;
    textures[1] = 0;
  }

  DisplayList::~DisplayList() {}

  void CreateTextureOnGPU(Engine::GPUState* gpuState, uint32_t idTexture) {

    glCreateTextures(GL_TEXTURE_2D, 1, &gpuState->_textureArray[idTexture].openGLid);

    GLuint textureID = gpuState->_textureArray[idTexture].openGLid;
    int32_t width = gpuState->_textureArray[idTexture].width;
    int32_t height = gpuState->_textureArray[idTexture].height;
    GLint minMagFilter = gpuState->_textureArray[idTexture].minMagFilter;

    switch (gpuState->_textureArray[idTexture].type) {

    case Texture::Type::RGBA32F:
      glTextureStorage2D(textureID, 1, GL_RGBA32F, width, height);
      //if (nullptr != gpuState->_textureArray[idTexture].data.get())
        //glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, gpuState->_textureArray[idTexture].data.get());
      break;


    case Texture::Type::RGBA:
      glTextureStorage2D(textureID, 1, GL_RGBA8, width, height);
      if (nullptr != gpuState->_textureArray[idTexture].data.get())
        glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, gpuState->_textureArray[idTexture].data.get());
      break;

    case Texture::Type::RGB:
      glTextureStorage2D(textureID, 1, GL_RGB8, width, height);
      if (nullptr != gpuState->_textureArray[idTexture].data.get())
        glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, gpuState->_textureArray[idTexture].data.get());
      break;

    case Texture::Type::RG:
      glTextureStorage2D(textureID, 1, GL_RG8, width, height);
      if (nullptr != gpuState->_textureArray[idTexture].data.get())
        glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RG, GL_UNSIGNED_BYTE, gpuState->_textureArray[idTexture].data.get());
      break;

    case Texture::Type::R:
      glTextureStorage2D(textureID, 1, GL_R8, width, height);
      if (nullptr != gpuState->_textureArray[idTexture].data.get())
        glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, gpuState->_textureArray[idTexture].data.get());
      break;

    case Texture::Type::DEPTH:
      glTextureStorage2D(textureID, 1, GL_DEPTH_COMPONENT16, width, height);
      if (nullptr != gpuState->_textureArray[idTexture].data.get())
        glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_DEPTH_COMPONENT16, GL_UNSIGNED_BYTE, gpuState->_textureArray[idTexture].data.get());
      break;

    }

    gpuState->_textureArray[idTexture].isBeingWritten = false;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, minMagFilter);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, minMagFilter);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glGenerateTextureMipmap(textureID);

  }

  void DisplayList::AttachTextures(Engine::GPUState* gpuState) {

    if ((textures[0]) == 0 || (textures[1] == 0)){ 
     return;
    }
    if(gpuState->_textureArray.size() <= textures[0] ||
       gpuState->_textureArray.size() <= textures[1]){
      return;
    }
    
    if((textures[0]) > 0 && (textures[1] > 0)){
      assert((gpuState->_textureArray[textures[0]].width == gpuState->_textureArray[textures[1]].width) &&
        (gpuState->_textureArray[textures[0]].height == gpuState->_textureArray[textures[1]].height), "ERROR! Textures size missmatch");

        if(gpuState->_textureArray[textures[0]].openGLid == 0)
          CreateTextureOnGPU(gpuState, textures[0]);

        if(gpuState->_textureArray[textures[1]].openGLid == 0)
          CreateTextureOnGPU(gpuState, textures[1]);

      uint32_t framebufferWidth = gpuState->_textureArray[textures[0]].width;
      uint32_t framebufferHeight = gpuState->_textureArray[textures[1]].height;

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      glBindFramebuffer(GL_FRAMEBUFFER, gpuState->frameBuffer);

      glViewport(0, 0, framebufferWidth, framebufferHeight);

      glNamedFramebufferTexture(gpuState->frameBuffer, GL_COLOR_ATTACHMENT0, gpuState->_textureArray[textures[0]].openGLid, 0);
      glNamedFramebufferTexture(gpuState->frameBuffer, GL_DEPTH_ATTACHMENT, gpuState->_textureArray[textures[1]].openGLid, 0);

      GLenum status = glCheckNamedFramebufferStatus(gpuState->frameBuffer, GL_FRAMEBUFFER);
      if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
      }
    }
  }
  
  DisplayList& DisplayList::clear(float r, float g, float b, float a)
  {
    //assert(endWithSwap == false);
    auto cmd = std::make_unique<ClearCommand>();
    cmd->r = r;
    cmd->g = g;
    cmd->b = b;
    cmd->a = a;
    _commands.push_back(std::move(cmd));
    return  *this;
  }

  DisplayList& DisplayList::deleteTexture(uint32_t texture) {

    auto cmd = std::make_unique<DeleteTextureCommand>();
    cmd->textureID = texture;
    _commands.push_back(std::move(cmd));
    return  *this;

  }

  DisplayList& DisplayList::drawGeometry(uint32_t id_geo, glm::mat4 mat, uint32_t id_mat)
  {
    auto cmd = std::make_unique<DrawGeometry>();
    cmd->ID_geo = id_geo;
    cmd->modelMatrix = mat;
    cmd->ID_mat = id_mat;

    _commands.push_back(std::move(cmd));

    return *this;
  }

  DisplayList& DisplayList::createGeometry(std::vector<GeometryBuilder::VertexAtrrib> verticesAttribs, std::vector<uint32_t> id_vertices)
  {
    auto cmd = std::make_unique<CreateGeometry>();
    cmd->indices_vertices = id_vertices;
    cmd->verticesAttribs = verticesAttribs;

    _commands.push_back(std::move(cmd));

    return *this;
  }
  
  DisplayList& DisplayList::setCamera(glm::mat4 viewMatrix, glm::mat4 ProjectionMatrix) {

    auto cmd = std::make_unique<SetCamera>();
    cmd->viewMatrix = viewMatrix;
    cmd->ProjectionMatrix = ProjectionMatrix;
    _commands.push_back(std::move(cmd));

    return *this;

  }

  DisplayList& DisplayList::displayShadows()
  {
    auto cmd = std::make_unique<DisplayShadows>();
    _commands.push_back(std::move(cmd));

    return *this;
  }

  DisplayList& DisplayList::blur(uint32_t data[4])
  {
    auto cmd = std::make_unique<Blur>();
    cmd->data_[0] = data[0];
    cmd->data_[1] = data[1];
    cmd->data_[2] = data[2];
    cmd->texture = data[3];
    _commands.push_back(std::move(cmd));
    return *this;
  }

  DisplayList& DisplayList::drawWater(glm::mat4 viewMatrix, glm::mat4 ProjectionMatrix)
  {
    auto cmd = std::make_unique<DrawWater>();
    cmd->viewMatrix = viewMatrix;
    cmd->ProjectionMatrix = ProjectionMatrix;
    _commands.push_back(std::move(cmd));
    return  *this;
  }

  bool DisplayList::executeOnGPU(Engine::GPUState* gpuState)
  {
    for (std::unique_ptr<Command>& i : _commands) {
      i->executeOnGPU(gpuState);
    }
    return endWithSwap;
  }
 
  void DisplayList::ClearCommand::executeOnGPU(Engine::GPUState* gpuState) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void DisplayList::DeleteTextureCommand::executeOnGPU(Engine::GPUState* gpuState) {

    glDeleteTextures(1, &textureID);  

  }

  void DisplayList::CreateGeometry::executeOnGPU(Engine::GPUState* gpuState){

    Engine::GPUState::Geometry new_geomtetry;
    
    AWK::GeometryBuilder::VertexAtrrib* new_vertex = &verticesAttribs[0];
    uint32_t* index_vertices = &indices_vertices[0];

    uint32_t size_vertex_attribs = sizeof(AWK::GeometryBuilder::VertexAtrrib);
    glCreateVertexArrays(1, &new_geomtetry.VAO);
    new_geomtetry.numElements = (uint32_t)indices_vertices.size();
    size_t size_vertex_buffer = verticesAttribs.size() * size_vertex_attribs ;
    size_t  size_element_buffer = indices_vertices.size() * sizeof(uint32_t);
    
    GLuint VBO;
    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, size_vertex_buffer, new_vertex, GL_DYNAMIC_STORAGE_BIT);

    GLuint handleElements;
    glCreateBuffers(1, &handleElements);
    glNamedBufferStorage(handleElements, size_element_buffer, index_vertices, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(new_geomtetry.VAO, handleElements);

    glVertexArrayVertexBuffer(new_geomtetry.VAO, 0, VBO, 0,  size_vertex_attribs);
    glEnableVertexArrayAttrib(new_geomtetry.VAO, 0);    // Layout binding point - Position
    glEnableVertexArrayAttrib(new_geomtetry.VAO, 1);    // Layout binding point - Normal
    glEnableVertexArrayAttrib(new_geomtetry.VAO, 2);    // Layout binding point - UV

    glVertexArrayAttribFormat(new_geomtetry.VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(new_geomtetry.VAO, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glVertexArrayAttribFormat(new_geomtetry.VAO, 2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6);

    glVertexArrayAttribBinding(new_geomtetry.VAO, 0, 0);
    glVertexArrayAttribBinding(new_geomtetry.VAO, 1, 0);
    glVertexArrayAttribBinding(new_geomtetry.VAO, 2, 0);

    gpuState->_geometryArray.push_back(new_geomtetry);
  }

  void DisplayList::DrawGeometry::executeOnGPU(Engine::GPUState* gpuState) {

    //Pass the model matrix
    glm::mat3 upperLeftModelMatrix;
    glm::mat4 normalMatrix;

    upperLeftModelMatrix[0].x = modelMatrix[0].x;
    upperLeftModelMatrix[0].y = modelMatrix[0].y;
    upperLeftModelMatrix[0].z = modelMatrix[0].z;

    upperLeftModelMatrix[1].x = modelMatrix[1].x;
    upperLeftModelMatrix[1].y = modelMatrix[1].y;
    upperLeftModelMatrix[1].z = modelMatrix[1].z;

    upperLeftModelMatrix[2].x = modelMatrix[2].x;
    upperLeftModelMatrix[2].y = modelMatrix[2].y;
    upperLeftModelMatrix[2].z = modelMatrix[2].z;

    upperLeftModelMatrix = glm::transpose(glm::inverse(upperLeftModelMatrix));

    normalMatrix[0].x = upperLeftModelMatrix[0].x;
    normalMatrix[0].y = upperLeftModelMatrix[0].y;
    normalMatrix[0].z = upperLeftModelMatrix[0].z;
    
    normalMatrix[1].x = upperLeftModelMatrix[1].x;
    normalMatrix[1].y = upperLeftModelMatrix[1].y;
    normalMatrix[1].z = upperLeftModelMatrix[1].z;
    
    normalMatrix[2].x = upperLeftModelMatrix[2].x;
    normalMatrix[2].y = upperLeftModelMatrix[2].y;
    normalMatrix[2].z = upperLeftModelMatrix[2].z;

    Engine::GPUState::PerModelData data;
    data.modelMatrix = modelMatrix;
    data.normalMat = normalMatrix;
    data.materialData = gpuState->_materialArray[ID_mat].data;

    glBindVertexArray(gpuState->_geometryArray[ID_geo].VAO);
    GLuint program_id = gpuState->_programsArray[gpuState->_materialArray[ID_mat].type];
    glUseProgram(program_id);

    GLuint idTexture = data.materialData.textureColor.texture;
    bool hasTexture = ((idTexture != 0) && (gpuState->_materialArray[ID_mat].type >= Material::MaterialParameters::MaterialType_TextureColor));

    if (hasTexture) {
      if (gpuState->_textureArray[idTexture].openGLid == 0)
        CreateTextureOnGPU(gpuState, idTexture);
      glBindTextures(0, 1, &gpuState->_textureArray[idTexture].openGLid);

    }
    else {
      if (gpuState->_textureArray[0].openGLid == 0)
        CreateTextureOnGPU(gpuState, 0);
      glBindTextures(0, 1, &gpuState->_textureArray[0].openGLid);

    }
    //ShadowMap
    for (uint32_t i = 0; i < gpuState->kMaxShadows; ++i) {
      glBindTextures(1 + i, 1, 0);
    }
    for (uint32_t i = 0; i < gpuState->currentActivatedShadows; ++i) {
      glBindTextures(1+i, 1, &gpuState->_textureArray[gpuState->_shadowTexture[i]].openGLid);

    }

      GLsizeiptr perModelDataSize = sizeof(Engine::GPUState::PerModelData);
      glBindBufferRange(GL_UNIFORM_BUFFER, 1, gpuState->perModelDataBuffer, 0, perModelDataSize);
      glNamedBufferSubData(gpuState->perModelDataBuffer, 0, perModelDataSize, &data);

      glDrawElements(GL_TRIANGLES, gpuState->_geometryArray[ID_geo].numElements, GL_UNSIGNED_INT, 0);
         
  }

  void DisplayList::SetCamera::executeOnGPU(Engine::GPUState* gpuState) {

    Engine::GPUState::PerPassData data;
    data.vm = viewMatrix;
    data.pm = ProjectionMatrix;
    
    glNamedBufferSubData(gpuState->perPassDataBuffer, 0, sizeof(Engine::GPUState::PerPassData), &data);
       
  } 
  
  void DisplayList::DisplayShadows::executeOnGPU(Engine::GPUState* gpuState) {

    gpuState->frameData.shadows.data[0] = gpuState->currentActivatedShadows;
    gpuState->frameData.shadows.data[1] = gpuState->currentActivatedLights;

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, gpuState->perFrameDataBuffer, 0, sizeof(Engine::GPUState::PerFrameData));
    glNamedBufferSubData(gpuState->perFrameDataBuffer, 0, sizeof(Engine::GPUState::PerFrameData), &gpuState->frameData);

  }
  
  void DisplayList::Blur::executeOnGPU(Engine::GPUState* gpuState) {

    Engine::GPUState::PerPostProcessData data;
    data.data[0] = this->data_[0];
    data.data[1] = this->data_[1];
    data.data[2] = this->data_[2];
    data.data[3] = this->data_[3];

    glBindBufferRange(GL_UNIFORM_BUFFER, 3, gpuState->perModelDataBuffer, 0, sizeof(Engine::GPUState::PerPostProcessData));
    glNamedBufferSubData(gpuState->perModelDataBuffer, 0, sizeof(Engine::GPUState::PerPostProcessData), &data);
    glUseProgram(gpuState->_programsArray[gpuState->blurProgram]);
    glBindTextures(0, 1, &gpuState->_textureArray[texture].openGLid);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

  }


  void DisplayList::DrawWater::executeOnGPU(Engine::GPUState* gpuState) {
      
    //KData
    GLuint idTexture = gpuState->water.noiseTexture[0];
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindTextures(0, 1, &gpuState->_textureArray[idTexture].openGLid);

    idTexture = gpuState->water.noiseTexture[1];
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindTextures(1, 1, &gpuState->_textureArray[idTexture].openGLid);

    idTexture = gpuState->water.noiseTexture[2];
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindTextures(2, 1, &gpuState->_textureArray[idTexture].openGLid);

    idTexture = gpuState->water.noiseTexture[3];
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindTextures(3, 1, &gpuState->_textureArray[idTexture].openGLid);

    const GLsizeiptr waterKDataSize = sizeof(gpuState->water.waterKData);
    glBindBufferRange(GL_UNIFORM_BUFFER, 3, gpuState->water.dataKBuffer, 0, waterKDataSize);
    glNamedBufferSubData(gpuState->water.dataKBuffer, 0, waterKDataSize, &gpuState->water.waterKData);

    idTexture = gpuState->water.textureTilde_k0;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(0, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    idTexture = gpuState->water.textureTilde_kminus0;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(1, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glUseProgram(gpuState->water.computeKProgram);
    glDispatchCompute(gpuState->water.meshSize, gpuState->water.meshSize, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    //KFUSION
    const GLsizeiptr waterKFDataSize = sizeof(gpuState->water.waterKFData);
    glBindBufferRange(GL_UNIFORM_BUFFER, 3, gpuState->water.dataKFBuffer, 0, waterKFDataSize);
    glNamedBufferSubData(gpuState->water.dataKFBuffer, 0, waterKFDataSize, &gpuState->water.waterKFData);

    idTexture = gpuState->water.textureTilde_k0;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(3, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    idTexture = gpuState->water.textureTilde_kminus0;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(4, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    idTexture = gpuState->water.heigmapDisplacementTexture;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(0, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    idTexture = gpuState->water.choppyXTexture;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(1, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    idTexture = gpuState->water.choppyZTexture;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(2, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glUseProgram(gpuState->water.computeKFusionProgram);
    glDispatchCompute(gpuState->water.meshSize, gpuState->water.meshSize, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Water Twiddle
    idTexture = gpuState->water.twiddleIndiceTexture;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(0, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    const GLsizeiptr waterSBBOtwiddle = gpuState->water.meshSize * sizeof(uint32_t);
    glBindBufferRange(GL_UNIFORM_BUFFER, 7, gpuState->water.SBBOtwiddle, 0, waterSBBOtwiddle);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, gpuState->water.SBBOtwiddle);
    glNamedBufferSubData(gpuState->water.SBBOtwiddle, 0, waterSBBOtwiddle, &gpuState->water.bitReversedIndices);

    int log_2_N = (int)(log(gpuState->water.meshSize) / log(2));
    glUseProgram(gpuState->water.computeTwiddleProgram);
    glDispatchCompute(log_2_N, gpuState->water.meshSize / 16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


    // Water Butterfly
    idTexture = gpuState->water.twiddleIndiceTexture;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(0, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    idTexture = gpuState->water.heigmapDisplacementTexture; // PingPong0 Texture
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(1, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    idTexture = gpuState->water.pingpong1;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(2, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    
    int32_t pingpong = 0;
    
    // 1D FFT Horizontal
    for(int y = 0; y < log_2_N; ++y) {

      gpuState->water.butterflyData.x = pingpong;
      gpuState->water.butterflyData.y = 0;
      gpuState->water.butterflyData.z = y;

      const GLsizeiptr waterButterflyDataSize = sizeof(gpuState->water.butterflyData);
      glBindBufferRange(GL_UNIFORM_BUFFER, 3, gpuState->water.butterflyDataBuffer, 0, waterButterflyDataSize);
      glNamedBufferSubData(gpuState->water.butterflyDataBuffer, 0, waterButterflyDataSize, &gpuState->water.butterflyData);
      glUseProgram(gpuState->water.computeButterflyProgram);
      glDispatchCompute(gpuState->water.meshSize , gpuState->water.meshSize, 1);
      glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

      ++pingpong;
      pingpong %= 2;

    }

    // 1D FFT Vertical
    for (int x = 0; x < log_2_N; ++x) {

      gpuState->water.butterflyData.x = pingpong;
      gpuState->water.butterflyData.y = 1;
      gpuState->water.butterflyData.z = x;

      const GLsizeiptr waterButterflyDataSize = sizeof(gpuState->water.butterflyData);
      glBindBufferRange(GL_UNIFORM_BUFFER, 3, gpuState->water.butterflyDataBuffer, 0, waterButterflyDataSize);
      glNamedBufferSubData(gpuState->water.butterflyDataBuffer, 0, waterButterflyDataSize, &gpuState->water.butterflyData);
      glUseProgram(gpuState->water.computeButterflyProgram);
      glDispatchCompute(gpuState->water.meshSize , gpuState->water.meshSize , 1);
      glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

      ++pingpong;
      pingpong %= 2;

    }
    // Water Inversion
    idTexture = gpuState->water.DyTexture;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(0, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    idTexture = gpuState->water.heigmapDisplacementTexture;   // PingPong 0
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(1, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    idTexture = gpuState->water.pingpong1;                    // PingPong 1
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(2, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    gpuState->water.inversionData.x = gpuState->water.meshSize;
    gpuState->water.inversionData.y = pingpong;

    const GLsizeiptr waterInversionDataSize = sizeof(gpuState->water.inversionData);
    glBindBufferRange(GL_UNIFORM_BUFFER, 3, gpuState->water.inversionDataBuffer, 0, waterInversionDataSize);
    glNamedBufferSubData(gpuState->water.inversionDataBuffer, 0, waterInversionDataSize, &gpuState->water.inversionData);
    
    glUseProgram(gpuState->water.computeInversionProgram);
    glDispatchCompute(gpuState->water.meshSize/16, gpuState->water.meshSize/16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // COMPUTE
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, gpuState->water.SSBOPosition);

    const GLsizeiptr waterDataSize = sizeof(gpuState->waterData.data);
    glBindBufferRange(GL_UNIFORM_BUFFER, 6, gpuState->waterData.waterDataBuffer, 0, waterDataSize);
    glNamedBufferSubData(gpuState->waterData.waterDataBuffer, 0, waterDataSize, &gpuState->waterData.data);

    idTexture = gpuState->water.DyTexture;
    if (gpuState->_textureArray[idTexture].openGLid == 0)
      CreateTextureOnGPU(gpuState, idTexture);
    glBindImageTexture(0, gpuState->_textureArray[idTexture].openGLid, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glUseProgram(gpuState->water.computeProgram);
    glDispatchCompute(gpuState->water.meshSize, gpuState->water.meshSize, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    //NORMAL
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, gpuState->water.SSBONormal);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, gpuState->water.SSBOPosition);

    glUseProgram(gpuState->water.computeNormalProgram);
    glDispatchCompute(gpuState->water.meshSize - 1, gpuState->water.meshSize - 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);



    // RENDER    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, gpuState->water.SSBONormal);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, gpuState->water.SSBOPosition);

    gpuState->water.meshWaterData.view = viewMatrix;
    gpuState->water.meshWaterData.proyection = ProjectionMatrix;
    const GLsizeiptr waterMeshIndicesSize = sizeof(gpuState->water.meshWaterData);
    glBindBufferRange(GL_UNIFORM_BUFFER, 6, gpuState->water.waterMeshIndicesBuffer, 0, waterMeshIndicesSize);
    glNamedBufferSubData(gpuState->water.waterMeshIndicesBuffer, 0, waterMeshIndicesSize, &gpuState->water.meshWaterData);

    const GLsizeiptr waterSBBOIndicesSize = sizeof(gpuState->water.meshIndices);
    glBindBufferRange(GL_UNIFORM_BUFFER, 7, gpuState->water.SSBOIndices, 0, waterSBBOIndicesSize);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, gpuState->water.SSBOIndices);
    glNamedBufferSubData(gpuState->water.SSBOIndices, 0, waterSBBOIndicesSize, &gpuState->water.meshIndices);
    

    glUseProgram(gpuState->water.renderProgram);

    glDrawArrays(GL_TRIANGLES, 0, (gpuState->water.meshSize - 1) * (gpuState->water.meshSize-1) * 6);
 


  }

}
