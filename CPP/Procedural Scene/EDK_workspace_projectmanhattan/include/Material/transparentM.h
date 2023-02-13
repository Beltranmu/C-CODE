#pragma once


#include <EDK3/matdiffusetexture.h>
#include <EDK3/materialsettings.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/dev/program.h>
#include <Mollete/texture.h>
#include <light/directionallight.h>
#include <light/pointligh.h>
#include <light/sportlight.h>

class  transparentM : public EDK3::MatDiffuseTexture {
public:
	
  transparentM();


  virtual bool load_shaders_from_filepaths(const char* vertex_shader_path, const char* fragment_shader_path);
  virtual bool load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader);

  

  virtual bool enable(const EDK3::MaterialSettings*) const override;
  
  virtual void setupCamera(const float projecton[16], const float view[16]) const override;
  virtual void setupModel(const float m[16]) const override;

  virtual unsigned int num_attributes_required() const override;
  virtual EDK3::Attribute attribute_at_index(const unsigned int attrib_idx) const override;
  virtual EDK3::Type attribute_type_at_index(const unsigned int attrib_index) const override;


  class SettingstransparentM : public EDK3::MaterialSettings {
  public:
    SettingstransparentM()
    {
      for (int i = 0; i < 4; ++i) {
        color_data[i] = 1.0f;
        
      }
 
    }
    void set_color(const float v[4]) { memcpy(color_data, v, sizeof(color_data)); }
    void set_color(float r, float g, float b, float a = 1.0f) {
      color_data[0] = r; color_data[1] = g; color_data[2] = b; color_data[3] = a;
    }

   



    const float *color() const { return color_data; }


    
    

    void set_texture(Mollete::Texture *tex) { texture_ = tex; }
    Mollete::Texture* texture() { return texture_.get(); }
    const Mollete::Texture* texture() const { return texture_.get(); }


  protected:
    virtual ~SettingstransparentM() {}
  private:
    SettingstransparentM(const SettingstransparentM&);
    SettingstransparentM& operator=(const SettingstransparentM&);
    EDK3::ref_ptr<Mollete::Texture> texture_;

    float color_data[4];
  };
  
protected:
  virtual ~transparentM();
  void set_program(EDK3::ref_ptr<EDK3::dev::Program> program);
  EDK3::dev::Program* get_program();
  const EDK3::dev::Program* get_program() const;
private:
  EDK3::ref_ptr<EDK3::dev::Program> program_;
  transparentM(const transparentM&);
  transparentM& operator=(const transparentM &);

}; /* end of EDK Namespace */
