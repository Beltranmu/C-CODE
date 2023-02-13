#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2021 Javier Benito & Ricardo Beltrán
// Skybox Implementation
// ----------------------------------------------------------------------------

#include <Material/buildingmaterial.h>
#include <EDK3/matdiffusetexture.h>
#include <EDK3/materialsettings.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/dev/program.h>
#include <Mollete/texture.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

class  Skybox : public EDK3::MatDiffuseTexture {
public:

  Skybox();
  
  virtual bool load_shaders_from_filepaths(const char* vertex_shader_path, const char* fragment_shader_path);
  virtual bool load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader);
  
  virtual bool enable(const EDK3::MaterialSettings*) const override;

  virtual void setupCamera(const float projecton[16], const float view[16]) const override;
  virtual void setupModel(const float m[16]) const override;

  virtual unsigned int num_attributes_required() const override;
  virtual EDK3::Attribute attribute_at_index(const unsigned int attrib_idx) const override;
  virtual EDK3::Type attribute_type_at_index(const unsigned int attrib_index) const override;

  void loadCubeMap(EDK3::ref_ptr<Mollete::Texture> *texture);

  class SettingsSkyboxMat : public EDK3::MaterialSettings {
  public:

    SettingsSkyboxMat(){}
    
    void set_cam_pos(float* r) {
      cam_pos_data = r;
    }
    void set_fog_density(float* f) {
      fog_density_data = f;
    }
    void set_fog_active_data(int* f) {
      fog_active_data = f;
    }
    
    const float *cam_pos() const { return cam_pos_data; }
    const float *fog_density() const { return fog_density_data; }
    const int *fog_active() const { return fog_active_data; }
       
    void set_texture(Mollete::Texture *tex) { texture_ = tex; }
    Mollete::Texture* texture() { return texture_.get(); }
    const Mollete::Texture* texture() const { return texture_.get(); }
    EDK3::ref_ptr<Mollete::Texture> texture_;    

  protected:

    virtual ~SettingsSkyboxMat() {}

  private:

    SettingsSkyboxMat(const SettingsSkyboxMat&);
    SettingsSkyboxMat& operator=(const SettingsSkyboxMat&);

    float *cam_pos_data;
    float *fog_density_data;
    int *fog_active_data;

  };

protected:

  virtual ~Skybox();
  void set_program(EDK3::ref_ptr<EDK3::dev::Program> program);
  EDK3::dev::Program* get_program();
  const EDK3::dev::Program* get_program() const;

private:

  EDK3::ref_ptr<EDK3::dev::Program> program_;
  Skybox(const Skybox&);
  Skybox& operator=(const Skybox &);

  std::vector<std::string> faces = { "skybox/right.jpg",
                                     "skybox/left.jpg",
                                     "skybox/top.jpg",
                                     "skybox/bottom.jpg",
                                     "skybox/front.jpg",
                                     "skybox/back.jpg" };

}; /* end of EDK Namespace */