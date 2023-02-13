#include "../../include/Mollete/shader.h"


/*OpenGL Calls used :
glCompileShader
glDeleteShader
glShaderSource
glGetShaderiv
glGetShaderInfoLog
glCreateShader
*/

Mollete::Shader::Shader(){
  shader_id = -1;
}

Mollete::Shader::~Shader(){
}


// 1ºEDK type to opgl type
// 2º
void Mollete::Shader::loadSource(const Type shader_type, 
                                 const char * source,
                                 const unsigned int source_size){
  
  if (shader_id != -1) {
    glDeleteShader(shader_id);
  }

  switch (shader_type){
    case kType_Vertex:   shader_id = glCreateShader(GL_VERTEX_SHADER); break;
    case kType_Fragment: shader_id = glCreateShader(GL_FRAGMENT_SHADER); break;
    case kType_Geometry: shader_id = glCreateShader(GL_GEOMETRY_SHADER); break;
  }
 
  GLint size = source_size;
  glShaderSource(shader_id, 1, &source, &size);

}

bool Mollete::Shader::compile(EDK3::scoped_array<char>* output_log){
  glCompileShader(shader_id);

  //int compiled = ;
  //glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (nullptr != output_log ) {
    //if (!compiled) {
      //Get the output mensagge lenght
      int output_length;
      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &output_length);
      //alloc this mensage
      output_log->alloc(output_length);
      // Set the mensage
      glGetShaderInfoLog(shader_id, output_length, NULL, output_log->get());
  
  }
  return is_compiled();
}

bool Mollete::Shader::is_compiled() const{
  int compiled;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
  return compiled == GL_TRUE;
}

const EDK3::dev::Shader::Type Mollete::Shader::type() const{

  int type;
  glGetShaderiv(shader_id, GL_SHADER_TYPE, &type);
  switch (type){
  case  GL_VERTEX_SHADER: return kType_Vertex; break;
  case GL_GEOMETRY_SHADER: return kType_Geometry; break;
  case GL_FRAGMENT_SHADER: return kType_Fragment; break;
  default:                 return kType_Invalid; break;
  }
}

unsigned int Mollete::Shader::internal_id() const{
  return shader_id;
}
