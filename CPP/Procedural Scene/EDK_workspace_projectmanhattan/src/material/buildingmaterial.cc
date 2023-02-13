#define _CRT_SECURE_NO_WARNINGS

#include <Material/buildingmaterial.h>
#include <EDK3/dev/shader.h>
#include <EDK3/dev/gpumanager.h>
#include "matrix_4.h"


Buildingmaterial::Buildingmaterial()
{
}

bool Buildingmaterial::load_shaders_from_filepaths(const char * vertex_shader_path, const char * fragment_shader_path){
  
  EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader;
  EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader;
  loadVertexShaderFile(&vertex_shader, vertex_shader_path);
  loadFragmentShaderFile(&fragment_shader, fragment_shader_path);
  
  return load_shaders(vertex_shader, fragment_shader);
}

bool Buildingmaterial::load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader,
                                 EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader){

  // Change for assert
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
  

  return all_correct;
}

bool Buildingmaterial::enable(const EDK3::MaterialSettings* mat_setting) const{
  SettingsBuildingMat* matsettings = (SettingsBuildingMat*)mat_setting;

  if (nullptr != matsettings->texture()) {
    int id_texture_sampler2D = get_program()->get_uniform_position("u_colorTexture");
    matsettings->texture()->bind(0);
    get_program()->set_uniform_value(id_texture_sampler2D, EDK3::T_UINT_1, (int*)0);
  }
  //Fog
  int id_fog = get_program()->get_uniform_position("fog_activate");
  get_program()->set_uniform_value(id_fog, EDK3::T_INT_1, matsettings->fog_active());
  int id_density= get_program()->get_uniform_position("density");
  get_program()->set_uniform_value(id_density, EDK3::T_FLOAT_1, matsettings->fog_density());

  //Camera position_c
  int id_cam_pos = get_program()->get_uniform_position("camera_pos");
  get_program()->set_uniform_value(id_cam_pos, EDK3::T_FLOAT_3, matsettings->cam_pos());

  //Material settings
  int id_mat_set = get_program()->get_uniform_position("matsetting.ambient");
  get_program()->set_uniform_value(id_mat_set, EDK3::T_FLOAT_3, matsettings->ambient());

  id_mat_set = get_program()->get_uniform_position("matsetting.diffuse");
  get_program()->set_uniform_value(id_mat_set, EDK3::T_FLOAT_3, matsettings->diffuse());

  id_mat_set = get_program()->get_uniform_position("matsetting.specular");
  get_program()->set_uniform_value(id_mat_set, EDK3::T_FLOAT_3, matsettings->specular());

  id_mat_set = get_program()->get_uniform_position("matsetting.shininess");
  float shininess = matsettings->shininess();
  get_program()->set_uniform_value(id_mat_set, EDK3::T_FLOAT_1, &shininess);


  //Lights
  char uniform_name[50] = "\0";
  int id_light_n = -1;

  // Directionals
  for (int i = 0; i < matsettings->n_d_lights; i++) {
    sprintf(uniform_name, "dir_light[%d].direction\0",i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->d_lights[i].direction);

    sprintf(uniform_name, "dir_light[%d].ambient\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->d_lights[i].ambient);

    sprintf(uniform_name, "dir_light[%d].diffuse\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->d_lights[i].diffuse);

    sprintf(uniform_name, "dir_light[%d].specular\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->d_lights[i].specular);
  }

  //Point Lights
  for (int i = 0; i < matsettings->n_p_lights; i++) {
    sprintf(uniform_name, "point_light[%d].position\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->p_lights[i].position);

    sprintf(uniform_name, "point_light[%d].ambient\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->p_lights[i].ambient);

    sprintf(uniform_name, "point_light[%d].diffuse\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->p_lights[i].diffuse);

    sprintf(uniform_name, "point_light[%d].specular\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->p_lights[i].specular);

    sprintf(uniform_name, "point_light[%d].constant\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    float poimt_light_data = matsettings->p_lights[i].constant;
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_1, &poimt_light_data);

    sprintf(uniform_name, "point_light[%d].linear\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    poimt_light_data = matsettings->p_lights[i].linear;
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_1, &poimt_light_data);

    sprintf(uniform_name, "point_light[%d].quadratic\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    poimt_light_data = matsettings->p_lights[i].quadratic;
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_1, &poimt_light_data);


  }

  // Spot Lights
  for (int i = 0; i < matsettings->n_s_lights; i++) {

    sprintf(uniform_name, "spot_light[%d].direction\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->s_lights[i].direction);

    sprintf(uniform_name, "spot_light[%d].position\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->s_lights[i].position);

    sprintf(uniform_name, "spot_light[%d].ambient\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->s_lights[i].ambient);

    sprintf(uniform_name, "spot_light[%d].diffuse\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->s_lights[i].diffuse);

    sprintf(uniform_name, "spot_light[%d].specular\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->s_lights[i].specular);

    sprintf(uniform_name, "spot_light[%d].constant\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    float poimt_light_data = matsettings->s_lights[i].constant;
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_1, &poimt_light_data);


    sprintf(uniform_name, "spot_light[%d].linear\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    poimt_light_data = matsettings->s_lights[i].linear;
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_1, &poimt_light_data);

    sprintf(uniform_name, "spot_light[%d].quadratic\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    poimt_light_data = matsettings->s_lights[i].quadratic;
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_1, &poimt_light_data);

    sprintf(uniform_name, "spot_light[%d].cutOff\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    poimt_light_data = matsettings->s_lights[i].cutOff;
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_1, &poimt_light_data);

    sprintf(uniform_name, "spot_light[%d].outerCutOff\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    poimt_light_data = matsettings->s_lights[i].outerCutOff;
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_1, &poimt_light_data);

  }



  get_program()->use();
  return true;
}

void Buildingmaterial::setupCamera(const float projecton[16], const float view[16]) const{
 
  Matrix4x4 view_m = Matrix4x4(view);
  Matrix4x4 projection_m = Matrix4x4(projecton);
  float* pv_matrix = projection_m.Multiply(view_m).m;
  int pv_matrix_id = get_program()->get_uniform_position("u_vp_matrix");
  get_program()->set_uniform_value(pv_matrix_id, EDK3::T_MAT_4x4, pv_matrix);
}

void Buildingmaterial::setupModel(const float m[16]) const{

  int model_id = get_program()->get_uniform_position("u_m_matrix");
  get_program()->set_uniform_value(model_id, EDK3::T_MAT_4x4, m);
}

unsigned int Buildingmaterial::num_attributes_required() const{
  return 3;
}

EDK3::Attribute Buildingmaterial::attribute_at_index(const unsigned int attrib_idx) const{
  switch (attrib_idx) {
  case 0:
    return EDK3::A_POSITION;
  break;

  case 1:
    return EDK3::A_UV;
  break;

  case 2:
    return EDK3::A_NORMAL;
    break;

  }

}

EDK3::Type Buildingmaterial::attribute_type_at_index(const unsigned int attrib_index) const{
 
  switch (attrib_index)
  {
  case 0: return EDK3::T_FLOAT_3; break;

   case 1: return EDK3::T_UINT_2; break;

   case 2: return EDK3::T_FLOAT_3; break;
  };
}

Buildingmaterial::~Buildingmaterial()
{
}

void Buildingmaterial::set_program(EDK3::ref_ptr<EDK3::dev::Program> program){
  program_ = program;
}

EDK3::dev::Program * Buildingmaterial::get_program()
{
  return program_.get();
}

const EDK3::dev::Program * Buildingmaterial::get_program() const
{
  return program_.get();
}
