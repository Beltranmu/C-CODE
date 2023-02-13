#include <material/matcustomricardo.h>
#include <EDK3/dev/shader.h>
#include <EDK3/dev/gpumanager.h>
#include "matrix_4.h"

MatCustomRicardo::MatCustomRicardo()
{
}

bool MatCustomRicardo::load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, 
                                    EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader)
{
  // Cambiar por assert
  if (vertex_shader == nullptr || fragment_shader == nullptr) {
    return false;
  }

  EDK3::ref_ptr<EDK3::dev::Program> new_program;
  EDK3::dev::GPUManager::Instance()->newProgram(&new_program);
  set_program(new_program);

  bool all_correct;
  all_correct = vertex_shader->compile();
  all_correct = fragment_shader->compile() && all_correct;

  get_program()->attach(vertex_shader.get());
  get_program()->attach(fragment_shader.get());

  get_program()->link();
  

  return true;
}

bool MatCustomRicardo::enable(const EDK3::MaterialSettings* mat_setting) const{
  MatCustomRicardo::SettingsRicardo* ricardo_settings = (SettingsRicardo*)mat_setting;

  const float* color = ricardo_settings->color();
  const float* custom_data = ricardo_settings->custom_data();

  int id_color1 = get_program()->get_uniform_position("color1");
  float value_color1[3] = {*color ,*(color+1), *(color+2) };
  get_program()->set_uniform_value(id_color1, EDK3::T_FLOAT_3, value_color1);
  get_program()->use();
  return true;
}

void MatCustomRicardo::setupCamera(const float projecton[16], const float view[16]) const{
 
  Matrix4x4 view_m = Matrix4x4(view);
  Matrix4x4 projection_m = Matrix4x4(projecton);
  float* pv_matrix = projection_m.Multiply(view_m).m;
  int pv_matrix_id = get_program()->get_uniform_position("u_vp_matrix");
  get_program()->set_uniform_value(pv_matrix_id, EDK3::T_MAT_4x4, pv_matrix);
}

void MatCustomRicardo::setupModel(const float m[16]) const{

  int model_id = get_program()->get_uniform_position("u_m_matrix");
  get_program()->set_uniform_value(model_id, EDK3::T_MAT_4x4, m);
}

unsigned int MatCustomRicardo::num_attributes_required() const{
  return 2;
}

EDK3::Attribute MatCustomRicardo::attribute_at_index(const unsigned int attrib_idx) const{
  switch (attrib_idx) {
  case 0:
    return EDK3::A_POSITION;
  break;

  case 1:
    return EDK3::A_NORMAL;
  break;

  }
  return EDK3::Attribute();
}

EDK3::Type MatCustomRicardo::attribute_type_at_index(const unsigned int attrib_index) const{
 
  switch (attrib_index)
  {
  case 0:
    return EDK3::T_FLOAT_3;
    break;

   case 1:
    return EDK3::T_FLOAT_3;
    break;

  default:
    ;
  };
}

MatCustomRicardo::~MatCustomRicardo()
{
}
