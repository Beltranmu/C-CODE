#include <material/materialcube.h>
#include <EDK3/dev/shader.h>
#include <EDK3/dev/gpumanager.h>
#include "matrix_4.h"

MaterialCube::MaterialCube()
{
}

bool MaterialCube::load_shaders_from_filepaths(const char * vertex_shader_path, const char * fragment_shader_path){
  
  EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader;
  EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader;
  loadVertexShaderFile(&vertex_shader, vertex_shader_path);
  loadFragmentShaderFile(&fragment_shader, fragment_shader_path);
  
  return load_shaders(vertex_shader, fragment_shader);
}

bool MaterialCube::load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader,
                                 EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader){

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

bool MaterialCube::enable(const EDK3::MaterialSettings* mat_setting) const{
  SettingsMatCube* matsettings = (SettingsMatCube*)mat_setting;

  const float* color = matsettings->color();
  const float* color2 = matsettings->color2();
  const float* input = matsettings->input();
  const int n = matsettings->n();

  int id_color1 = get_program()->get_uniform_position("color1");
  int id_color2 = get_program()->get_uniform_position("color2");
  int id_input = get_program()->get_uniform_position("input");
  int id_n = get_program()->get_uniform_position("n");

  get_program()->set_uniform_value(id_color1, EDK3::T_FLOAT_4, color);
  get_program()->set_uniform_value(id_color2, EDK3::T_FLOAT_4, color2);
  get_program()->set_uniform_value(id_input, EDK3::T_FLOAT_1, input);
   get_program()->set_uniform_value(id_n, EDK3::T_INT_1, &n);
 
  get_program()->use();
  return true;
}

void MaterialCube::setupCamera(const float projecton[16], const float view[16]) const{
 
  Matrix4x4 view_m = Matrix4x4(view);
  Matrix4x4 projection_m = Matrix4x4(projecton);
  float* pv_matrix = projection_m.Multiply(view_m).m;
  int pv_matrix_id = get_program()->get_uniform_position("u_vp_matrix");
  get_program()->set_uniform_value(pv_matrix_id, EDK3::T_MAT_4x4, pv_matrix);
}

void MaterialCube::setupModel(const float m[16]) const{

  int model_id = get_program()->get_uniform_position("u_m_matrix");
  get_program()->set_uniform_value(model_id, EDK3::T_MAT_4x4, m);
}

unsigned int MaterialCube::num_attributes_required() const{
  return 2;
}

EDK3::Attribute MaterialCube::attribute_at_index(const unsigned int attrib_idx) const{
  switch (attrib_idx) {
  case 0:
    return EDK3::A_POSITION;
  break;

  case 1:
    return EDK3::A_NORMAL;
  break;

  }

}

EDK3::Type MaterialCube::attribute_type_at_index(const unsigned int attrib_index) const{
 
  switch (attrib_index)
  {
  case 0:
    return EDK3::T_FLOAT_3;
    break;

   case 1:
    return EDK3::T_FLOAT_3;
    break;
  };
}

MaterialCube::~MaterialCube()
{
}

void MaterialCube::set_program(EDK3::ref_ptr<EDK3::dev::Program> program){
  program_ = program;
}

EDK3::dev::Program * MaterialCube::get_program()
{
  return program_.get();
}

const EDK3::dev::Program * MaterialCube::get_program() const
{
  return program_.get();
}
