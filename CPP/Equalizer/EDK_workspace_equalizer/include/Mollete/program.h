#ifndef INCLUDE_MOLLETE_PROGRAM_H_
#define INCLUDE_MOLLETE_PROGRAM_H_ 1

#include <EDK3/dev/program.h>
#include <EDK3/dev/opengl.h>


/* OpenGL Calls:
glAttachShader
glDeleteProgram
glLinkProgram
glUseProgram
glGetUniformLocation
glCreateProgram
glGetProgramiv
glGetProgramInfoLog
glGetAttribLocation
glUniform(1f,2f,Matrix4f,...)
glBindAttribLocation // optional
*/

namespace Mollete {

  class Program : public EDK3::dev::Program{

  public:
    Program();
    virtual ~Program();
    
    virtual void attach(EDK3::dev::Shader *shader) override;

    virtual bool link(EDK3::scoped_array<char> *link_log = 0L) override;

    virtual void use() const override;

    virtual  int get_attrib_location(const char *name) const override;

    virtual int  get_uniform_position(const char *name) const override;

    virtual void set_uniform_value(
      const int uniform_pos,
      const EDK3::Type uniform_type,
      const float * value) const override;

    virtual void set_uniform_value(
      const int uniform_pos,
      const EDK3::Type uniform_type,
      const int* value) const override;

    virtual void set_uniform_value(
      const int uniform_pos,
      const EDK3::Type uniform_type,
      const unsigned int* value) const override;

    /** returns the internal program ID*/
    virtual unsigned int internal_id() const ;


    
  private:
    GLuint program_id;
  };


}

#endif
