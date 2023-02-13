#include "../../include/Mollete/buffer.h"



/* OpenGL Calls used:
     glBindBuffer
     glBufferData
     glBufferSubData
     glDeleteBuffers
     glGenBuffers
*/
Mollete::Buffer::Buffer(){
  buffer_id = -1;
}

Mollete::Buffer::~Buffer(){
}

void Mollete::Buffer::init(unsigned int size){
  if (buffer_id != -1) {
   glDeleteBuffers(1, &buffer_id);
  }
  glGenBuffers(1, &buffer_id); 
  b_type = kTarget_Vertex_Data;
  bind(b_type);
  glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
  buffer_length = size;
}

void Mollete::Buffer::bind(const EDK3::dev::Buffer::Target t) const{
  
  switch (t){
  case kTarget_Vertex_Data: 
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id); 
    break;
  case kTarget_Elements:   
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
    break;
  }
}

unsigned int Mollete::Buffer::size() const{
  GLint buffer_size = 0;
  bind(b_type);
  switch (b_type) {
  case kTarget_Vertex_Data:
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size); break;
  case kTarget_Elements:
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size); break;
  }
   
  
  return buffer_size;
}

void Mollete::Buffer::uploadData(const void * data, unsigned int size, unsigned int offset){
  bind(kTarget_Elements);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void Mollete::Buffer::release(){
 
  glDeleteBuffers(1, &buffer_id);
  buffer_length = 0;

} 


