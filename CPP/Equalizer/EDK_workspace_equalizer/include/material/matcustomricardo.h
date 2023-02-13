#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2017 Lucas Gonzalez
// Test Material Implementation
// ----------------------------------------------------------------------------



#include <EDK3/matcustom.h>
#include <EDK3/ref_ptr.h>

class EDK3::dev::Shader;
class EDK3::MaterialSettings;

class  MatCustomRicardo : public EDK3::MatCustom {
public:
	
  MatCustomRicardo();
  
  virtual bool load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, 
                            EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader) override;

  virtual bool enable(const EDK3::MaterialSettings*) const override;
  
  virtual void setupCamera(const float projecton[16], const float view[16]) const override;
  virtual void setupModel(const float m[16]) const override;

  virtual unsigned int num_attributes_required() const override;
  virtual EDK3::Attribute attribute_at_index(const unsigned int attrib_idx) const override;
  virtual EDK3::Type attribute_type_at_index(const unsigned int attrib_index) const override;


  class SettingsRicardo : public EDK3::MaterialSettings {
  public:
    SettingsRicardo()
    {
      for (int i = 0; i < 4; ++i) {
        color_data[i] = 1.0f;
        custom_data_[i] = 01.0f;
      }
    }
    void set_color(const float v[4]) { memcpy(color_data, v, sizeof(color_data)); }
    void set_color(float r, float g, float b, float a = 1.0f) {
      color_data[0] = r; color_data[1] = g; color_data[2] = b; color_data[3] = a;
    }
    const float *color() const { return color_data; }

    void set_custom_data(const float data[4]) { memcpy(custom_data_, data, sizeof(custom_data_)); }
    const float *custom_data() const { return custom_data_; }

  protected:
    virtual ~SettingsRicardo() {}
  private:
    SettingsRicardo(const SettingsRicardo&);
    SettingsRicardo& operator=(const SettingsRicardo&);
    float color_data[4];
    float custom_data_[4];
  };
  
protected:
  virtual ~MatCustomRicardo();
private:


}; /* end of EDK Namespace */
