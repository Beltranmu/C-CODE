#ifndef INCLUDE_MOLLETE_SHADER_H_
#define INCLUDE_MOLLETE_SHADER_H_ 1

#include <EDK3/dev/shader.h>
#include <EDK3/dev/opengl.h>

namespace Mollete {

  class Shader : public EDK3::dev::Shader{

  public:
    Shader();
    virtual ~Shader();
    
    // Loads the source of the Shader
    void loadSource(
      const Type shader_type,
      const char *source,
      const unsigned int source_size) override;
      
    // Compiles the shader. Optionally it can fill the output log of the compilation
    // Returns true if the shader was successfully compiled
    bool compile(EDK3::scoped_array<char> *output_log = NULL) override;
    // Checks if the shader is compiled
    
    bool is_compiled() const override;

    // Returns the type of shader
    const EDK3::dev::Shader::Type type() const override;

    unsigned int internal_id() const override;


    
  private:
    GLuint shader_id;
  };


}

#endif
