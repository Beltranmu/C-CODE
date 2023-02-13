#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2021 Javier Benito & Ricardo Beltrán
// Rain Material Implementation
// ----------------------------------------------------------------------------

#include <Material/buildingmaterial.h>
#include <EDK3/matdiffusetexture.h>
#include <EDK3/materialsettings.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/dev/program.h>
#include <Mollete/texture.h>
#include <light/directionallight.h>
#include <light/pointligh.h>
#include <light/sportlight.h>

class  RainMaterial : public Buildingmaterial {
public:
	
  RainMaterial();

  //virtual bool load_shaders_from_filepaths(const char* vertex_shader_path, const char* fragment_shader_path);
  virtual bool load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader) override;
  virtual bool enable(const EDK3::MaterialSettings* mat_setting) const override;
  virtual void setupCamera(const float projecton[16], const float view[16]) const override;
  virtual void setupModel(const float m[16]) const override;
  virtual unsigned int num_attributes_required() const override;
  virtual EDK3::Attribute attribute_at_index(const unsigned int attrib_idx) const override;
  virtual EDK3::Type attribute_type_at_index(const unsigned int attrib_index) const override;
  void initGeometryShader();
  
  /*class SettingsBuildingMat : public EDK3::MaterialSettings {
  public:
    SettingsBuildingMat()
    {
      for (int i = 0; i < 4; ++i) {
        color_data[i] = 1.0f;
        
      }
      shininess_data = 20.f;
    }
    void set_color(const float v[4]) { memcpy(color_data, v, sizeof(color_data)); }
    void set_color(float r, float g, float b, float a = 1.0f) {
      color_data[0] = r; color_data[1] = g; color_data[2] = b; color_data[3] = a;
    }

    void set_ambient(const float v[3]) { memcpy(ambient_data, v, sizeof(ambient_data)); }
    void set_ambient(float r, float g, float b) {
      ambient_data[0] = r; ambient_data[1] = g; ambient_data[2] = b; 
    }

    void set_diffuse(const float v[3]) { memcpy(diffuse_data, v, sizeof(diffuse_data)); }
    void set_diffuse(float r, float g, float b) {
      diffuse_data[0] = r; diffuse_data[1] = g; diffuse_data[2] = b; 
    }

    void set_specular(const float v[3]) { memcpy(specular_data, v, sizeof(specular_data)); }
    void set_specular(float r, float g, float b) {
      specular_data[0] = r; specular_data[1] = g; specular_data[2] = b; 
    }

    void set_shininess(float r) {
      shininess_data = r;
    }

    void set_cam_pos(float* r) {
      cam_pos_data = r;
    }

    const float *color() const { return color_data; }
    const float *ambient() const { return ambient_data; }
    const float *diffuse() const { return diffuse_data; }
    const float *specular() const { return specular_data; }
    const float shininess() const { return shininess_data; }
    const float *cam_pos() const { return cam_pos_data; }
    

    
    

    void set_texture(Mollete::Texture *tex) { texture_ = tex; }
    Mollete::Texture* texture() { return texture_.get(); }
    const Mollete::Texture* texture() const { return texture_.get(); }

    unsigned int n_d_lights;
    unsigned int n_p_lights;
    unsigned int n_s_lights;
    DirectionalLight* d_lights;
    PointLight* p_lights;
    SpotLight* s_lights;
  protected:
    virtual ~SettingsBuildingMat() {}
  private:
    SettingsBuildingMat(const SettingsBuildingMat&);
    SettingsBuildingMat& operator=(const SettingsBuildingMat&);
    EDK3::ref_ptr<Mollete::Texture> texture_;

    float color_data[4];

    float ambient_data[3];
    float diffuse_data[3];
    float specular_data[3];
    float shininess_data;
    float *cam_pos_data;
  };*/
  
protected:

  virtual ~RainMaterial();

private:

  const char* geometryShaderArray_;

}; /* end of EDK Namespace */
