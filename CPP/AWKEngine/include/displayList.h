

/************************************************
 * Display List Class
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/
#ifndef __DISPLAYLIST_H__
#define __DISPLAYLIST_H__ 1

#include <memory>
#include <list>
#include <assert.h>
#include <stdint.h>
#include "glm.hpp"
#include "glad/glad.h"
#include "engine.h"
#include "geometrybuilder.h"

namespace AWK{
  
  class GPUState;
  class Engine;
  //! Display List Class.
  /*! Store the commands that will be executed in the render thread*/
  class DisplayList {

    public:
      DisplayList();
      ~DisplayList();
      DisplayList(DisplayList&&) = default;
      DisplayList(const DisplayList&) = delete;
      uint32_t textures[2];

      void AttachTextures(Engine::GPUState* gpuState);
      //void CreateTextureOnGPU(Engine::GPUState* gpuState, uint32_t idTexture);

      //! Create the clear command
      /*!
        \param r. Red component of the clear color.
        \param g. Green component of the clear color.
        \param b. Blue component of the clear color.
        \param a. Alpha component of the clear color.
        \return the Display list itself.
        \sa drawGeometry(), createGeometry().
      */
      DisplayList& clear(float r, float g, float b, float a);

      DisplayList& deleteTexture(uint32_t texture);

      //! Create the command to draw a geometry. Needs the geometry, its transform and its material.
      /*!
        \param id_geo. ID of the geometry to draw.
        \param mat. Transform of the geometry.
        \param id_mat. ID of the material of the geometry.
        \return the Display list itself.
        \sa clear(), createGeometry().
      */
      DisplayList& drawGeometry(uint32_t id_geo, glm::mat4 mat, uint32_t id_mat);

      //! Create the command to create a new geometry. Needs the vertices and their ID.
      /*!
        \param verticesAttribs. Collection of vertexAtribs for each vertex of the new geometry.
        \param id_vertices. Transform of the geometry.
        \param id_mat. ID of the material of the geometry.
        \return the Display list itself.
        \sa clear(), drawGeometry().
      */
      DisplayList& createGeometry(std::vector<GeometryBuilder::VertexAtrrib> verticesAttribs,
                                  std::vector<uint32_t> id_vertices);
     
      DisplayList& setCamera(glm::mat4 viewMatrix, glm::mat4 ProjectionMatrix);
      DisplayList& displayShadows();
      DisplayList& displayLights();
      DisplayList& blur(uint32_t data[4]);
      DisplayList& drawWater(glm::mat4 viewMatrix, glm::mat4 ProjectionMatrix);

      //! Execute all the commands.
      /*!
        \param gpuState. State of the GPU (material geometries) that the commands will need to be executed.
        \return the value of endWithSwap.
        \sa clear(), drawGeometry(), createGeometry.
      */
      bool executeOnGPU(Engine::GPUState* gpuState);
      
    private:
      //! Struct Command.
      /*!
        Base archetype for the rest of the commands
      */
      struct Command{
        virtual ~Command() {};
        //! Executes the command.
        /*!
        * Pure virtual function that the rest of the commands will need to implement
          \param gpuState. State of the GPU (material geometries) that the command will need to be executed.
        */
        virtual void executeOnGPU(Engine::GPUState* gpuState) = 0;
      };

      //! Struct ClearCommand.
      /*!
        Used to have the information of the attributes and methods needed for the clear command.
      */
      struct ClearCommand : public Command{
        virtual void executeOnGPU(Engine::GPUState* gpuState);
          float r; /*!< Red component of the clear color. */  
          float g; /*!< Green component of the clear color. */  
          float b; /*!< Blue component of the clear color. */  
          float a; /*!< Alpha component of the clear color. */  
      };

      struct DeleteTextureCommand : public Command {

        virtual void executeOnGPU(Engine::GPUState* gpuState);
          uint32_t textureID;

      };

      //! Struct DrawGeometry.
      /*!
        Used to have the information of the attributes and methods needed for the draw geometry command.
      */
      struct DrawGeometry: public Command {
        virtual void executeOnGPU(Engine::GPUState* gpuState);
        uint32_t ID_geo;        /*!< ID of the geometry to draw. */
        glm::mat4 modelMatrix;  /*!< Matrix of the geometry. */
        uint32_t ID_mat;        /*!< ID of the material to draw the geometry with. */
      };

      //! Struct CreateGeometry.
      /*!
        Used to have the information of the attributes and methods needed for the create geometry command.
      */
      struct CreateGeometry : public Command {
        virtual void executeOnGPU(Engine::GPUState* gpuState);
        std::vector<GeometryBuilder::VertexAtrrib> verticesAttribs; /*!< Vertex attributes for each vertex of the new geometry. */
        std::vector<uint32_t> indices_vertices; /*!< index of each vertex. */
      };
            
      struct SetCamera : public Command {

        virtual void executeOnGPU(Engine::GPUState* gpuState);
        glm::mat4 viewMatrix;
        glm::mat4 ProjectionMatrix;

      };

      struct DisplayShadows : public Command {

        virtual void executeOnGPU(Engine::GPUState* gpuState);

      };

      struct DisplayLights : public Command {

        virtual void executeOnGPU(Engine::GPUState* gpuState);

      };

      struct Blur : public Command {

        virtual void executeOnGPU(Engine::GPUState* gpuState);
        uint32_t data_[4];
        uint32_t texture;

      };


      struct DrawWater : public Command {

        virtual void executeOnGPU(Engine::GPUState* gpuState);
        glm::mat4 viewMatrix;
        glm::mat4 ProjectionMatrix;
      };

      bool endWithSwap;  /*!< true if the Display list end with a swap of the buffers. Otherwise false */
      std::list<std::unique_ptr<Command>> _commands; /*!< Vector to store all the commands for the Display List */
  };

}

#endif // !__DISPLAYLIST_H__