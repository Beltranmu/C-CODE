#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2017 Lucas Gonzalez
// Test Material Implementation
// ----------------------------------------------------------------------------

#include <EDK3/material.h>
#include <EDK3/materialsettings.h>
#include <EDK3/ref_ptr.h>
#include <EDK3/dev/program.h>

//class EDK3::dev::Shader;


class  Materia2Color : public EDK3::Material {
public:
	
  Materia2Color();


  virtual bool load_shaders_from_filepaths(const char* vertex_shader_path, const char* fragment_shader_path);
  virtual bool load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader);

  

  virtual bool enable(const EDK3::MaterialSettings*) const override;
  
  virtual void setupCamera(const float projecton[16], const float view[16]) const override;
  virtual void setupModel(const float m[16]) const override;

  virtual unsigned int num_attributes_required() const override;
  virtual EDK3::Attribute attribute_at_index(const unsigned int attrib_idx) const override;
  virtual EDK3::Type attribute_type_at_index(const unsigned int attrib_index) const override;


  class SettingsMat2Color : public EDK3::MaterialSettings {
  public:
    SettingsMat2Color()
    {
      for (int i = 0; i < 4; ++i) {
        color_data[i] = 1.0f;
        custom_data_[i] = 01.0f;
      }
    }
    void set_color(const float v[4]) { memcpy(color_data, v, sizeof(color_data)); }
    void set_color2(const float v[4]) { memcpy(color_data2, v, sizeof(color_data)); }
    void set_song_data(float *data) { song_data = data; }
    void set_color(float r, float g, float b, float a = 1.0f) {
      color_data[0] = r; color_data[1] = g; color_data[2] = b; color_data[3] = a;
    }
    void set_color2(float r, float g, float b, float a = 1.0f) {
      color_data2[0] = r; color_data2[1] = g; color_data2[2] = b; color_data2[3] = a;
    }
    const float *color() const { return color_data; }
    const float *color2() const { return color_data2; }
    const float* get_song_data()const { return song_data; };

    void set_custom_data(const float data[4]) { memcpy(custom_data_, data, sizeof(custom_data_)); }
    const float *custom_data() const { return custom_data_; }

  protected:
    virtual ~SettingsMat2Color() {}
  private:
    SettingsMat2Color(const SettingsMat2Color&);
    SettingsMat2Color& operator=(const SettingsMat2Color&);
    float color_data[4];
    float color_data2[4];
    float custom_data_[4];
    float *song_data;
  };
  
protected:
  virtual ~Materia2Color();
  void set_program(EDK3::ref_ptr<EDK3::dev::Program> program);
  EDK3::dev::Program* get_program();
  const EDK3::dev::Program* get_program() const;
private:
  EDK3::ref_ptr<EDK3::dev::Program> program_;
  Materia2Color(const Materia2Color&);
  Materia2Color& operator=(const Materia2Color &);

}; /* end of EDK Namespace */
