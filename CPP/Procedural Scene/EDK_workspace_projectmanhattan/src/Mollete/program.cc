#include "../../include/Mollete/program.h"
#include "../../include/Mollete/shader.h"

#include <EDK3/dev/gpumanager.h>

/* OpenGL Calls:
-glAttachShader
-glDeleteProgram
-glLinkProgram
-glUseProgram
-glGetUniformLocation
-glCreateProgram
-glGetProgramiv
-glGetProgramInfoLog
-glGetAttribLocation
glUniform(1f,2f,Matrix4f,...)
glBindAttribLocation // optional
*/

Mollete::Program::Program(){
  program_id = -1;
}

Mollete::Program::~Program(){
}

void Mollete::Program::attach(EDK3::dev::Shader *shader){

  if (program_id == -1) {
    //glDeleteProgram(program_id);
    program_id = glCreateProgram();
  }
 
  EDK3::dev::GPUManager::CheckGLError("CreateP");
  glAttachShader(program_id, shader->internal_id());
  EDK3::dev::GPUManager::CheckGLError("Attatch");
}

bool Mollete::Program::link(EDK3::scoped_array<char>* link_log){

  glLinkProgram(program_id);
  int linked;
  glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
  if (nullptr != link_log) {
    //if (linked) {
      //link_log->alloc(1);
    //  link_log->memSet('0');
   // }
    //else {
      int output_msg_length;
      glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &output_msg_length);
      link_log->alloc(output_msg_length);
      glGetProgramInfoLog(program_id, output_msg_length, &output_msg_length, link_log->get());
    //}

  }
  return linked == GL_TRUE;
}

void Mollete::Program::use() const{
  glUseProgram(program_id);
  EDK3::dev::GPUManager::CheckGLError("Use");
}

int Mollete::Program::get_attrib_location(const char * name) const{
  return glGetAttribLocation(program_id, name);
  EDK3::dev::GPUManager::CheckGLError("AtribLocation");
}

int Mollete::Program::get_uniform_position(const char * name) const{
  return glGetUniformLocation(program_id, name);
  EDK3::dev::GPUManager::CheckGLError("UniformLcation");
}


/*
    T_SAMPLER_1D,
    T_SAMPLER_2D,
    T_SAMPLER_3D,*/

void Mollete::Program::set_uniform_value(const int uniform_pos, const EDK3::Type uniform_type, const float *value) const{

  switch (uniform_type){
    case EDK3::T_FLOAT_1: glProgramUniform1f(program_id, uniform_pos, *value); break;
    case EDK3::T_FLOAT_2: glProgramUniform2f(program_id, uniform_pos, *value, *(value+1)); break;
    case EDK3::T_FLOAT_3: glProgramUniform3f(program_id, uniform_pos, *value, *(value+1), *(value + 2)); break;                
    case EDK3::T_FLOAT_4: glProgramUniform4f(program_id, uniform_pos, *value, *(value+1), 
                                    *(value + 2), *(value + 3)); break;

    /*case EDK3::T_DOUBLE_1: glUniform1f(uniform_pos, *value); break;
    case EDK3::T_DOUBLE_2: glUniform2f(uniform_pos, *value, *(value + 1)); break;
    case EDK3::T_DOUBLE_3: glUniform3f(uniform_pos, *value, *(value + 1), *(value + 2)); break;
    case EDK3::T_DOUBLE_4: glUniform4f(uniform_pos, *value, *(value + 1),
                                     *(value + 2), *(value + 3)); break;*/

    case EDK3::T_MAT_2x2: glProgramUniformMatrix2fv(program_id, uniform_pos, 1, GL_FALSE, value); break;
    case EDK3::T_MAT_3x3: glProgramUniformMatrix3fv(program_id, uniform_pos, 1, GL_FALSE, value); break;
    case EDK3::T_MAT_4x4: glProgramUniformMatrix4fv(program_id, uniform_pos, 1, GL_FALSE, value); break;


  default:    break;
  }

  EDK3::dev::GPUManager::CheckGLError("Setuniformvalue");
}

void Mollete::Program::set_uniform_value(const int uniform_pos, const EDK3::Type uniform_type, const int *value) const{

  unsigned short *value_short = (unsigned short*)value;
  unsigned char *value_byte = (unsigned char*)value;

  switch (uniform_type)
  {
    case EDK3::T_INT_1: glProgramUniform1i(program_id, uniform_pos, *value); break;
    case EDK3::T_INT_2: glProgramUniform2i(program_id, uniform_pos, *value, *(value + 1)); break;
    case EDK3::T_INT_3: glProgramUniform3i(program_id, uniform_pos, *value, *(value + 1), *(value + 2)); break;
    case EDK3::T_INT_4: glProgramUniform4i(program_id, uniform_pos, *value, *(value + 1),
                                  *(value + 2), *(value + 3)); break;

    /*case EDK3::T_BYTE_1: glUniform1i(uniform_pos, *value_byte); break;
    case EDK3::T_BYTE_2: glUniform2i(uniform_pos, *value_byte, *(value_byte + 1)); break;
    case EDK3::T_BYTE_3: glUniform3i(uniform_pos, *value_byte, *(value_byte + 1), 
                                   *(value_byte + 2)); break;
    case EDK3::T_BYTE_4: glUniform4i(uniform_pos, *value_byte, *(value_byte + 1),
                                   *(value_byte + 2), *(value_byte + 3)); break;

    case EDK3::T_SHORT_1: glUniform1i(uniform_pos, *value_short); break;
    case EDK3::T_SHORT_2: glUniform2i(uniform_pos, *value_short, *(value_short + 1)); break;
    case EDK3::T_SHORT_3: glUniform3i(uniform_pos, *value_short, *(value_short + 1),
                                    *(value_short + 2)); break;
    case EDK3::T_SHORT_4: glUniform4i(uniform_pos, *value_short, *(value_short + 1),
                                    *(value_short + 2), *(value_short + 3)); break;
    default: break;*/
  }
  EDK3::dev::GPUManager::CheckGLError("Setuniformvalue2");
}

void Mollete::Program::set_uniform_value(const int uniform_pos, const EDK3::Type uniform_type, const unsigned int *value) const{

  unsigned short *value_short = (unsigned short*)value;
  unsigned char *value_byte = (unsigned char*)value;

  switch (uniform_type)
  {
    case EDK3::T_UINT_1: glProgramUniform1ui(program_id, uniform_pos, *value); break;
    case EDK3::T_UINT_2: glProgramUniform2ui(program_id, uniform_pos, *value, *(value + 1)); break;
    case EDK3::T_UINT_3: glProgramUniform3ui(program_id, uniform_pos, *value, *(value + 1), *(value + 2)); break;
    case EDK3::T_UINT_4: glProgramUniform4ui(program_id, uniform_pos, *value, *(value + 1),
                                  *(value + 2), *(value + 3)); break;


    /*case EDK3::T_UBYTE_1: glUniform1ui(uniform_pos, *value_byte); break;
    case EDK3::T_UBYTE_2: glUniform2ui(uniform_pos, *value_byte, *(value_byte + 1)); break;
    case EDK3::T_UBYTE_3: glUniform3ui(uniform_pos, *value_byte, *(value_byte + 1), 
                                     *(value_byte + 2)); break;
    case EDK3::T_UBYTE_4: glUniform4ui(uniform_pos, *value_byte, *(value_byte + 1),
                                     *(value_byte + 2), *(value_byte + 3)); break;

    case EDK3::T_USHORT_1: glUniform1ui(uniform_pos, *value_short); break;
    case EDK3::T_USHORT_2: glUniform2ui(uniform_pos, *value_short, *(value_short + 1)); break;
    case EDK3::T_USHORT_3: glUniform3ui(uniform_pos, *value_short, *(value_short + 1), 
                                      *(value_short + 2)); break;
    case EDK3::T_USHORT_4: glUniform4ui(uniform_pos, *value_short, *(value_short + 1),
                                      *(value_short + 2),  *(value_short + 3)); break;
  default:
    break;*/
  }
  EDK3::dev::GPUManager::CheckGLError("Setuniformvalue3");
}

unsigned int Mollete::Program::internal_id() const{
  return program_id;
}



