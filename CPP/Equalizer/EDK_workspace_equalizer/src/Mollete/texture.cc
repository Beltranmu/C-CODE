#include "../../include/Mollete/texture.h"

Mollete::Texture::Texture() {
  texture_id = -1;
}

Mollete::Texture::~Texture() {

}

void Mollete::Texture::set_data(const Format f, const EDK3::Type t, const void * data, unsigned int mipmap_LOD){
  
  GLenum gl_format = GL_RGBA;
  GLenum gl_internal_format = GL_RGBA;
  GLenum gl_type = GL_FLOAT;
  bind(0);

  switch (f){
    case F_R: gl_format = GL_R; break;
    case F_RG: gl_format = GL_RG; break;
    case F_RGB: gl_format = GL_RGB; break;
    case F_BGR: gl_format = GL_BGR; break;
    case F_RGBA: gl_format = GL_RGBA; break;
    case F_DEPTH: gl_format = GL_DEPTH; break;
    case F_DEPTH32: gl_format = GL_DEPTH; break;
  }

  switch (format_) {
  case F_R: gl_internal_format = GL_R; break;
  case F_RG: gl_internal_format = GL_RG; break;
  case F_RGB: gl_internal_format = GL_RGB; break;
  case F_BGR: gl_internal_format = GL_BGR; break;
  case F_RGBA: gl_internal_format = GL_RGBA; break;
  case F_DEPTH: gl_internal_format = GL_DEPTH; break;
  case F_DEPTH32: gl_internal_format = GL_DEPTH; break;
  }


  switch (t){

    case EDK3::T_INT: gl_format = GL_INT; break;
    case EDK3::T_UINT: gl_format = GL_UNSIGNED_INT; break;
    case EDK3::T_BYTE: gl_format = GL_BYTE; break;
    case EDK3::T_UBYTE: gl_format = GL_UNSIGNED_BYTE; break;
    case EDK3::T_SHORT: gl_format = GL_SHORT; break;
    case EDK3::T_USHORT: gl_format = GL_UNSIGNED_SHORT; break;
  }


  switch (type_){
  case T_1D:
    glTexImage1D(GL_TEXTURE_1D, mipmap_LOD,
      gl_internal_format,
      width_, 0,
      gl_format, gl_type, data);
    break;
  case T_2D:
    glTexImage2D(GL_TEXTURE_2D, mipmap_LOD,
                 gl_internal_format,
                 width_, height_, 0,
                 gl_format, gl_type, data);

    break;
  case T_3D:
    glTexImage3D(GL_TEXTURE_3D, mipmap_LOD,
      gl_internal_format,
      width_, height_, depth_, 0,
      gl_format, gl_type, data);
    break;
  case T_CUBEMAP:
    for (unsigned int i = 0; i < 6; ++i) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
      gl_internal_format, 
      format_, width_, height_, 0, gl_format, 
      GL_UNSIGNED_BYTE, data);
    }
    break;
  }



}

void Mollete::Texture::bind(unsigned int textUnit) const{

  GLenum gl_target = GL_TEXTURE_2D;
  glActiveTexture(GL_TEXTURE0 + textUnit);

  switch (type_) {
    case EDK3::Texture::T_1D: gl_target = GL_TEXTURE_1D; break;
    case EDK3::Texture::T_3D: gl_target = GL_TEXTURE_3D; break;
    case EDK3::Texture::T_CUBEMAP: gl_target = GL_TEXTURE_CUBE_MAP; break;
  }
  glBindTexture(gl_target, texture_id);
}

unsigned int Mollete::Texture::internal_id() const{
  return texture_id;
}

void Mollete::Texture::set_min_filter(EDK3::Texture::Filter f) {
  set_min_filter(f);
  GLuint gl_filter = GL_NEAREST;
  switch (f)
  {
  case EDK3::Texture::F_LINEAR: gl_filter = GL_LINEAR;
    break;
  case EDK3::Texture::F_NEAREST_MIPMAP_NEAREST: gl_filter = GL_NEAREST_MIPMAP_NEAREST;
    break;
  case EDK3::Texture::F_LINEAR_MIPMAP_NEAREST: gl_filter = GL_LINEAR_MIPMAP_NEAREST;
    break;
  case EDK3::Texture::F_NEAREST_MIPMAP_LINEAR: gl_filter = GL_NEAREST_MIPMAP_LINEAR;
    break;
  case EDK3::Texture::F_LINEAR_MIPMAP_LINEAR: gl_filter = GL_LINEAR_MIPMAP_LINEAR;
    break;
  }
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter);
}

void Mollete::Texture::set_mag_filter(EDK3::Texture::Filter f){
  set_mag_filter(f);
  GLuint gl_filter = GL_NEAREST;
  if (f == EDK3::Texture::F_LINEAR) {
    gl_filter = GL_LINEAR;
  }

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter);
}

void Mollete::Texture::set_wrap_s(EDK3::Texture::Wrap c) {
  set_wrap_s(c);
  GLuint gl_wrap = GL_REPEAT;
  switch (c)
  {
  case EDK3::Texture::W_MIRRORED_REPEAT: gl_wrap = GL_MIRRORED_REPEAT;
    break;
  case EDK3::Texture::W_CLAMP_TO_EDGE: gl_wrap = GL_CLAMP_TO_EDGE;
    break;
  }
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap);
}

void Mollete::Texture::set_wrap_t(EDK3::Texture::Wrap c) {
  set_wrap_t(c);
  GLuint gl_wrap = GL_REPEAT;
  switch (c)
  {
  case EDK3::Texture::W_MIRRORED_REPEAT: gl_wrap = GL_MIRRORED_REPEAT;
    break;
  case EDK3::Texture::W_CLAMP_TO_EDGE: gl_wrap = GL_CLAMP_TO_EDGE;
    break;
  }
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap);
}

void Mollete::Texture::set_wrap_r(EDK3::Texture::Wrap c){
  set_wrap_r(c);
  GLuint gl_wrap = GL_REPEAT;
  switch (c)
  {
  case EDK3::Texture::W_MIRRORED_REPEAT: gl_wrap = GL_MIRRORED_REPEAT;
    break;
  case EDK3::Texture::W_CLAMP_TO_EDGE: gl_wrap = GL_CLAMP_TO_EDGE;
    break;
  }
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, gl_wrap);
}

void Mollete::Texture::generateMipmaps() const {
  GLenum gl_target = GL_TEXTURE_2D;

  switch (type_){
    case EDK3::Texture::T_1D: gl_target = GL_TEXTURE_1D; break;
    case EDK3::Texture::T_3D: gl_target = GL_TEXTURE_3D; break;
    case EDK3::Texture::T_CUBEMAP: gl_target = GL_TEXTURE_CUBE_MAP; break;
  }
  glGenerateMipmap(gl_target);
}


