
 /************************************************
  * Texture Class
  * Developed by Ricardo BM & Javier BA
  * Graphic Engine Development
  * ESAT - 2022
  ***********************************************/

#ifndef __TEXTURE_H__
#define __TEXTURE_H__ 1

#include <stdint.h>
#include "glad/glad.h"
namespace AWK {

  //! Texture struct.
  /*! The representation of a texture for the engine. */
  struct Texture {

    Texture() {

      openGLid = 0;
      width = 0;
      height = 0;
      type = Type::NONE;
      isBeingWritten = false;
      data = nullptr;

    }

    Texture(const Texture& texture) {

      openGLid = texture.openGLid;
      width = texture.width;
      height = texture.height;
      type = texture.type;
      minMagFilter = texture.minMagFilter;
      isBeingWritten = texture.isBeingWritten;


      int n = 4;

      switch (type) {

        case Type::R:
          n = 1;
          break;

        case Type::RG:
          n = 2;
          break;

        case Type::RGB:
          n = 3;
          break;

        case Type::RGBA:
          n = 4;
          break;

        case Type::DEPTH:
          n = 1;
          break;

      }

      data = std::make_unique<unsigned char[]>(width * height * n);
      memcpy(data.get(), texture.data.get(), width * height * n);

    }

    ~Texture() {

      //delete(data);

    }

    enum Type {

      NONE = -1,
      RGBA = 0,
      RGB,
      RG,
      R,
      DEPTH,
      RGBA32F,

    };

    GLuint openGLid;
    uint32_t width, height;
    Type type;
    bool isBeingWritten; /*!< Flag that determines if the texture is able to be read or written. True if enable to be written. */
    std::unique_ptr<unsigned char[]> data;

    GLint minMagFilter;

  };
   
}

#endif // !__TEXTURE_H__