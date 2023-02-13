#ifndef INCLUDE_MOLLETE_BUFFER_H_
#define INCLUDE_MOLLETE_BUFFER_H_ 1

#include <EDK3/dev/buffer.h>
#include <EDK3/dev/opengl.h>



namespace Mollete {

  class Buffer : public EDK3::dev::Buffer{

  public:
    Buffer();
    virtual ~Buffer();
    
    virtual void init(unsigned int size) override;

    virtual void bind(const EDK3::dev::Buffer::Target t) const override;

    virtual unsigned int size() const override;

    virtual void uploadData(
      const void *data,
      unsigned int size,
      unsigned int offset = 0) override;

    virtual void release() override;

  private:
    GLuint buffer_id;
    unsigned int buffer_length;
    EDK3::dev::Buffer::Target b_type;

  };


}

#endif
