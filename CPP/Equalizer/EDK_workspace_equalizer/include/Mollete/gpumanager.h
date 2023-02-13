// ----------------------------------------------------------------------------
// Copyright (C) 2014 Jose L. Hidalgo 
// GPUManager Class.
// ----------------------------------------------------------------------------

#ifndef INCLUDE_MOLLETE_GPUMANAGER_H_
#define INCLUDE_MOLLETE_GPUMANAGER_H_ 1


#include <EDK3/dev/gpumanager.h>

namespace Mollete {

  class GPUManager : public EDK3::dev::GPUManager{

  public:
    GPUManager();
    virtual ~GPUManager();

    virtual void newShader(EDK3::ref_ptr<EDK3::dev::Shader> *output) override;
    virtual void newBuffer(EDK3::ref_ptr<EDK3::dev::Buffer> *output) override;
    virtual void newProgram(EDK3::ref_ptr<EDK3::dev::Program> *output) override;

    virtual void enableVertexAttribute(
      const EDK3::dev::Buffer *buffer,
      const unsigned int attribute_index,
      const EDK3::Type type,
      const bool normalized = false,
      const unsigned int offset = 0,
      const unsigned int stride = 0) override;
 
    virtual void disableVertexAttribute(const unsigned int attrib_index) override;


    virtual void drawElements(const DrawMode mode,
      unsigned int count,
      const EDK3::dev::Buffer *buffer,
      const EDK3::Type element_type = EDK3::T_USHORT, 
      const unsigned int offset = 0) const override;
    

    virtual void enableDepthTest(const CompareFunc f) override;
    virtual void disableDepthTest() override;


    

    virtual void enableBlend(BlendParam source, BlendParam destination, BlendOp operation, const float ConstantColor[4]);
    void disableBlend();


    virtual void enableCullFaces(const FaceType f) override;
    virtual void disableCullFaces() override;

    virtual void enableScissor(
      int x, int y,
      unsigned int width,
      unsigned int height) override;
    virtual void disableScissor() override;

    virtual void changeColorMask(
      const bool red = true,
      const bool green = true,
      const bool blue = true,
      const bool alpha = true) override;
    virtual void changeDepthMask(bool enable = true) override;


  private:

  };

 
}

#endif
