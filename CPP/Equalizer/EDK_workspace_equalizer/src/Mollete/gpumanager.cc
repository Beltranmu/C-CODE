#include "../../include/Mollete/gpumanager.h"
#include "../../include/Mollete/shader.h"
#include "../../include/Mollete/buffer.h"
#include "../../include/Mollete/program.h"

#include <EDK3/dev/opengl.h>

Mollete::GPUManager::GPUManager(){
}

Mollete::GPUManager::~GPUManager(){
}

void Mollete::GPUManager::newShader(EDK3::ref_ptr<EDK3::dev::Shader>* output) {
  output->allocT<Mollete::Shader>();
}

void Mollete::GPUManager::newBuffer(EDK3::ref_ptr<EDK3::dev::Buffer>* output) {
  output->allocT<Mollete::Buffer>();
}

void Mollete::GPUManager::newProgram(EDK3::ref_ptr<EDK3::dev::Program>* output) {
  output->allocT<Mollete::Program>();
}

void Mollete::GPUManager::enableVertexAttribute(const EDK3::dev::Buffer * buffer, const unsigned int attribute_index, const EDK3::Type type, const bool normalized, const unsigned int offset, const unsigned int stride){

  
  GLenum gl_type = GL_FLOAT ;
  GLint gl_size = 1;
  void* gl_ptr;

  switch (type) {
    case EDK3::T_FLOAT_2: gl_size = 2; break;
    case EDK3::T_FLOAT_3: gl_size = 3; break;
    case EDK3::T_FLOAT_4: gl_size = 4; break;

    case EDK3::T_INT_1: gl_size = 1; gl_type = GL_INT; break;
    case EDK3::T_INT_2: gl_size = 2; gl_type = GL_INT; break;
    case EDK3::T_INT_3: gl_size = 3; gl_type = GL_INT; break;
    case EDK3::T_INT_4: gl_size = 4; gl_type = GL_INT; break;

    case EDK3::T_UINT_1: gl_size = 1; gl_type = GL_UNSIGNED_INT ; break;
    case EDK3::T_UINT_2: gl_size = 2; gl_type = GL_UNSIGNED_INT ; break;
    case EDK3::T_UINT_3: gl_size = 3; gl_type = GL_UNSIGNED_INT ; break;
    case EDK3::T_UINT_4: gl_size = 4; gl_type = GL_UNSIGNED_INT ; break;

    case EDK3::T_SHORT_1: gl_size = 1; gl_type = GL_SHORT; break;
    case EDK3::T_SHORT_2: gl_size = 2; gl_type = GL_SHORT; break;
    case EDK3::T_SHORT_3: gl_size = 3; gl_type = GL_SHORT; break;
    case EDK3::T_SHORT_4: gl_size = 4; gl_type = GL_SHORT; break;

    case EDK3::T_USHORT_1: gl_size = 1; gl_type = GL_UNSIGNED_SHORT; break;
    case EDK3::T_USHORT_2: gl_size = 2; gl_type = GL_UNSIGNED_SHORT; break;
    case EDK3::T_USHORT_3: gl_size = 3; gl_type = GL_UNSIGNED_SHORT; break;
    case EDK3::T_USHORT_4: gl_size = 4; gl_type = GL_UNSIGNED_SHORT; break;

    case EDK3::T_BYTE_1: gl_size = 1; gl_type = GL_BYTE; break;
    case EDK3::T_BYTE_2: gl_size = 2; gl_type = GL_BYTE; break;
    case EDK3::T_BYTE_3: gl_size = 3; gl_type = GL_BYTE; break;
    case EDK3::T_BYTE_4: gl_size = 4; gl_type = GL_BYTE; break;

    case EDK3::T_UBYTE_1: gl_size = 1; gl_type = GL_UNSIGNED_BYTE; break;
    case EDK3::T_UBYTE_2: gl_size = 2; gl_type = GL_UNSIGNED_BYTE; break;
    case EDK3::T_UBYTE_3: gl_size = 3; gl_type = GL_UNSIGNED_BYTE; break;
    case EDK3::T_UBYTE_4: gl_size = 4; gl_type = GL_UNSIGNED_BYTE; break;

    case EDK3::T_DOUBLE_1: gl_size = 1; gl_type = GL_DOUBLE; break;
    case EDK3::T_DOUBLE_2: gl_size = 2; gl_type = GL_DOUBLE; break;
    case EDK3::T_DOUBLE_3: gl_size = 3; gl_type = GL_DOUBLE; break;
    case EDK3::T_DOUBLE_4: gl_size = 4; gl_type = GL_DOUBLE; break;
  }
  buffer->bind(EDK3::dev::Buffer::kTarget_Vertex_Data);
  glEnableVertexAttribArray(attribute_index);
  glVertexAttribPointer(attribute_index, gl_size, gl_type, 
                       normalized , stride, (GLvoid*)offset);
}

void Mollete::GPUManager::disableVertexAttribute(const unsigned int attrib_index){
  glDisableVertexAttribArray(attrib_index);
}



void Mollete::GPUManager::drawElements(const DrawMode mode, 
  unsigned int count, const EDK3::dev::Buffer * buffer, 
  const EDK3::Type element_type, const unsigned int offset) const
{
  GLenum gl_type = GL_UNSIGNED_SHORT;
  GLenum gl_mode = GL_TRIANGLES;
  buffer->bind(EDK3::dev::Buffer::kTarget_Elements);

  switch (element_type){
    case EDK3::T_UBYTE: gl_type = GL_UNSIGNED_BYTE; break;
    case EDK3::T_UINT: gl_type = GL_UNSIGNED_INT; break;
  }

  switch (mode){
   
    case EDK3::dev::GPUManager::kDrawMode_TriangleStrip:
      gl_mode = GL_TRIANGLE_STRIP;
    break;

    case EDK3::dev::GPUManager::kDrawMode_Points:
      gl_mode = GL_POINTS;
    break;

    case EDK3::dev::GPUManager::kDrawMode_Lines:
      gl_mode = GL_LINES;
    break;

    case EDK3::dev::GPUManager::kDrawMode_LineStrip:
      gl_mode = GL_LINE_STRIP;
    break;

    case EDK3::dev::GPUManager::kDrawMode_LineLoop:
      gl_mode = GL_LINE_LOOP;
    break;
  }
    glDrawElements(gl_mode, count, gl_type, (GLvoid*)offset);
}


void Mollete::GPUManager::enableDepthTest(const CompareFunc f) {
  

  glEnable(GL_DEPTH_TEST);
  switch (f) {
  case kCompareFunc_Never:
    glDepthFunc(GL_NEVER);
    break;

  case kCompareFunc_Less:
    glDepthFunc(GL_LESS);
    break;
  case kCompareFunc_LessOrEqual:
    glDepthFunc(GL_LEQUAL);
    break;

  case kCompareFunc_Equal:
    glDepthFunc(GL_EQUAL);
    break;
  case kCompareFunc_NotEqual:
    glDepthFunc(GL_NOTEQUAL);
    break;
  case kCompareFunc_GreaterOrEqual:
    glDepthFunc(GL_GEQUAL);
    break;
  case kCompareFunc_Greater:
    glDepthFunc(GL_GREATER);
    break;
  case kCompareFunc_Always:
    glDepthFunc(GL_ALWAYS);
    break;

  }
}

void Mollete::GPUManager::disableDepthTest() {
  glDisable(GL_DEPTH_TEST);
}

void Mollete::GPUManager::enableBlend(BlendParam source,
            BlendParam destination, 
            BlendOp operation, 
            const float ConstantColor[4]){
  glEnable(GL_BLEND);
  GLenum sfactor;
 
  switch (source){
    case EDK3::dev::GPUManager::kBlendParam_Zero:
      sfactor = GL_ZERO;
      break;
    case EDK3::dev::GPUManager::kBlendParam_One:
      sfactor = GL_ONE;
      break;
    case EDK3::dev::GPUManager::kBlendParam_SourceColor:
      sfactor = GL_SRC_COLOR;
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusSourceColor:
      sfactor = GL_ONE_MINUS_SRC_COLOR;
      break;
    case EDK3::dev::GPUManager::kBlendParam_DestinationColor:
      sfactor = GL_DST_COLOR;
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusDestinationColor:
      sfactor = GL_ONE_MINUS_DST_COLOR;
      break;
    case EDK3::dev::GPUManager::kBlendParam_SourceAlpha:
      sfactor = GL_SRC_ALPHA;
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusSourceAlpha:
      sfactor = GL_ONE_MINUS_SRC_ALPHA;
      break;
    case EDK3::dev::GPUManager::kBlendParam_DestinationAlpha:
      sfactor = GL_DST_ALPHA;
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusDestinationAlpha:
      sfactor = GL_ONE_MINUS_DST_ALPHA;
      break;
    case EDK3::dev::GPUManager::kBlendParam_ConstantColor:
      sfactor = GL_CONSTANT_COLOR;
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusConstantColor:
      sfactor = GL_ONE_MINUS_CONSTANT_COLOR;
      break;
    case EDK3::dev::GPUManager::kBlendParam_ConstantAlpha:
      sfactor = GL_CONSTANT_ALPHA;
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusConstantAlpha:
      sfactor = GL_ONE_MINUS_CONSTANT_ALPHA;
      break;
    case EDK3::dev::GPUManager::kBlendParam_SourceAlphaSaturate:
      sfactor = GL_SRC_ALPHA_SATURATE;
      break;
  }

  switch (destination) {
    case EDK3::dev::GPUManager::kBlendParam_Zero:
      glBlendFunc(sfactor, GL_ZERO);
      break;
    case EDK3::dev::GPUManager::kBlendParam_One:
      glBlendFunc(sfactor, GL_ONE);
      break;
    case EDK3::dev::GPUManager::kBlendParam_SourceColor:
      glBlendFunc(sfactor, GL_SRC_COLOR);
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusSourceColor:
      glBlendFunc(sfactor, GL_ONE_MINUS_SRC_COLOR);
      break;
    case EDK3::dev::GPUManager::kBlendParam_DestinationColor:
      glBlendFunc(sfactor, GL_DST_COLOR);
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusDestinationColor:
      glBlendFunc(sfactor, GL_ONE_MINUS_DST_COLOR);
      break;
    case EDK3::dev::GPUManager::kBlendParam_SourceAlpha:
      glBlendFunc(sfactor, GL_SRC_ALPHA);
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusSourceAlpha:
      glBlendFunc(sfactor, GL_ONE_MINUS_SRC_ALPHA);
      break;
    case EDK3::dev::GPUManager::kBlendParam_DestinationAlpha:
      glBlendFunc(sfactor, GL_DST_ALPHA);
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusDestinationAlpha:
      glBlendFunc(sfactor, GL_ONE_MINUS_DST_ALPHA);
      break;
    case EDK3::dev::GPUManager::kBlendParam_ConstantColor:
      glBlendFunc(sfactor, GL_CONSTANT_COLOR);
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusConstantColor:
      glBlendFunc(sfactor, GL_ONE_MINUS_CONSTANT_COLOR);
      break;
    case EDK3::dev::GPUManager::kBlendParam_ConstantAlpha:
      glBlendFunc(sfactor, GL_CONSTANT_ALPHA);
      break;
    case EDK3::dev::GPUManager::kBlendParam_OneMinusConstantAlpha:
      glBlendFunc(sfactor, GL_ONE_MINUS_CONSTANT_ALPHA);
      break;
    case EDK3::dev::GPUManager::kBlendParam_SourceAlphaSaturate:
      glBlendFunc(sfactor, GL_SRC_ALPHA_SATURATE);
      break;
  }
  
  switch (operation){
    case EDK3::dev::GPUManager::kBlendOp_Add:
      glBlendEquation(GL_FUNC_ADD);
      break;
    case EDK3::dev::GPUManager::kBlendOp_Subtract:
      glBlendEquation(GL_FUNC_SUBTRACT);
      break;
    case EDK3::dev::GPUManager::kBlendOp_ReverseSubtract:
      glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
      break;
    case EDK3::dev::GPUManager::kBlendOp_Min:
      glBlendEquation(GL_MIN);
      break;
    case EDK3::dev::GPUManager::kBlendOp_Max:
      glBlendEquation(GL_MAX);
    break;
  }
  glBlendColor(ConstantColor[0], ConstantColor[1],
               ConstantColor[2], ConstantColor[3]);
}

void Mollete::GPUManager::disableBlend(){
  glDisable(GL_BLEND);
}

void Mollete::GPUManager::enableCullFaces(const FaceType f){
  glEnable(GL_CULL_FACE);

  switch (f){
    case kFace_Back: glCullFace(GL_BACK); break;
    case kFace_Front: glCullFace(GL_FRONT); break;
    case kFace_FrontAndBack: glCullFace(GL_FRONT_AND_BACK); break;
  }
}

void Mollete::GPUManager::disableCullFaces(){
  glDisable(GL_CULL_FACE);
}

void Mollete::GPUManager::enableScissor(int x, int y, unsigned int width, unsigned int height){
  glEnable(GL_SCISSOR_TEST);
  glScissor(x,y, width, height);
}

void Mollete::GPUManager::disableScissor(){
  glDisable(GL_SCISSOR_TEST);
}

void Mollete::GPUManager::changeColorMask(const bool red, const bool green, const bool blue, const bool alpha){
  GLboolean gl_red, gl_green, gl_blue, gl_alpha;
  gl_red = red == GL_TRUE;
  gl_green = green == GL_TRUE;
  gl_blue = blue == GL_TRUE;
  gl_alpha = alpha == GL_TRUE;
  glColorMask( gl_red, gl_green, gl_blue, gl_alpha);
}

void Mollete::GPUManager::changeDepthMask(bool enable){
  glDepthMask(enable == GL_TRUE);
}
