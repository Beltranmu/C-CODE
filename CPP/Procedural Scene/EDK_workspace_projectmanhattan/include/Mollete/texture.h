#ifndef INCLUDE_MOLLETE_TEXTURE_H_
#define INCLUDE_MOLLETE_TEXTURE_H_ 1

#include <EDK3/texture.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/dev/opengl.h>

namespace Mollete {

  class Texture : public EDK3::Texture{

  public:
    Texture();
    virtual ~Texture();
    
    // Uploading Data to the texture ------------------

    virtual void set_data(
      const Format f, // DataFormat of *data
      const EDK3::Type t,   // DataType of each element of *data
      const void *data,   // the data itself
      unsigned int mipmap_LOD = 0) override; // The LOD to fill (mipmapping only)

  // use the texture in the given texture unit
    virtual void bind(unsigned int textUnit) const override;
    virtual unsigned int internal_id() const override;

    // Other functions to implement (all of them must be implemented)
    // Also, remember to call the parent function in order to set the value.
    virtual void set_min_filter(Filter f) override;// { min_filter_ = f; }
    virtual void set_mag_filter(Filter f) override;// { mag_filter_ = f; }
    virtual void set_wrap_s(Wrap c) override;// { wrap_s_ = c; }
    virtual void set_wrap_t(Wrap c) override;// { wrap_t_ = c; }
    virtual void set_wrap_r(Wrap c) override;// { wrap_r_ = c; }
    virtual void generateMipmaps()  const override; /* needed to implement if filtering uses mipmapping */ 

    void init(Type t, Format internal_format, unsigned int width, unsigned int height = 1, unsigned int depth = 1) {
      type_ = t;
      format_ = internal_format;
      width_ = width;
      height_ = height;
      depth_ = depth;
    }

    // Loads PNG, JPEG, TGA, BMP, PSD, GIF, HDR, PIC
    static bool Load(const char *filename, EDK3::ref_ptr<Texture> *output_tex);

  protected:
//    Texture() : type_(T_Invalid), format_(F_None), width_(0), height_(0), depth_(0) {}



    /** Referenced requires virtual protected destructor */
//    virtual ~Texture() {}

    Texture(const Texture&);
    Texture& operator=(const Texture&);
  
  private:
    GLuint texture_id;
    Type type_;
    Format format_;
    unsigned int width_;
    unsigned int height_;
    unsigned int depth_;
    Filter min_filter_;
    Filter mag_filter_;
    Wrap  wrap_s_;
    Wrap  wrap_t_;
    Wrap  wrap_r_;

  };


}

#endif
