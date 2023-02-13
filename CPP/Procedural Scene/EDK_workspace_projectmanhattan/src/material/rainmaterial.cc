#define _CRT_SECURE_NO_WARNINGS

#include <Material/rainmaterial.h>
#include <EDK3/dev/shader.h>
#include <EDK3/dev/gpumanager.h>
#include "matrix_4.h"


RainMaterial::RainMaterial()
{
}

/*bool RainMaterial::load_shaders_from_filepaths(const char * vertex_shader_path, const char * fragment_shader_path) override{
  
  EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader;
  EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader;
  loadVertexShaderFile(&vertex_shader, vertex_shader_path);
  loadFragmentShaderFile(&fragment_shader, fragment_shader_path);
  
  return load_shaders(vertex_shader, fragment_shader);
}*/

bool RainMaterial::load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader,
                                 EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader){

  // Change for assert
  if (vertex_shader == nullptr || fragment_shader == nullptr) {
    return false;
  }

  initGeometryShader();
  EDK3::ref_ptr<EDK3::dev::Program> new_program;
  EDK3::dev::GPUManager::Instance()->newProgram(&new_program);
  set_program(new_program);

  bool all_correct;
  all_correct = vertex_shader->compile();
  all_correct = fragment_shader->compile() && all_correct;
  GLuint geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometryShaderID, 1, &geometryShaderArray_, NULL);
  glCompileShader(geometryShaderID);
  char text[255];
  glGetShaderInfoLog(geometryShaderID, 255, NULL, text);
  printf("%s\n", text);

  get_program()->attach(vertex_shader.get());
  get_program()->attach(fragment_shader.get());
  glAttachShader(get_program()->internal_id(), geometryShaderID);

  get_program()->link();  

  return true;
}

void RainMaterial::initGeometryShader(){

  geometryShaderArray_ = {
    "#version 330\n"
    "layout(points) in;\n"
    "layout(line_strip, max_vertices = 2) out;\n"
    "uniform vec3 direction;\n"
    "void main(){\n"
    "gl_Position = gl_in[0].gl_Position + vec4(-direction, 0.0f);\n"
    "EmitVertex();\n"
    "gl_Position = gl_in[0].gl_Position + vec4(direction, 0.0f);\n"
    "EmitVertex();\n"
    "EndPrimitive();\n"
    "}"
  };

}

bool RainMaterial::enable(const EDK3::MaterialSettings* mat_setting) const{

/*  SettingsBuildingMat* matsettings = (SettingsBuildingMat*)mat_setting;

  if (nullptr != matsettings->texture()) {
    int id_texture_sampler2D = get_program()->get_uniform_position("u_colorTexture");
    matsettings->texture()->bind(0);
    get_program()->set_uniform_value(id_texture_sampler2D, EDK3::T_UINT_1, (int*)0);
  }
  
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
    sprintf(uniform_name, "point_light[%d].position_c\0", i);
    //printf("name :%s", uniform_name);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->p_lights[i].position_c);

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

    sprintf(uniform_name, "spot_light[%d].position_c\0", i);
    id_light_n = get_program()->get_uniform_position(uniform_name);
    get_program()->set_uniform_value(id_light_n, EDK3::T_FLOAT_3, matsettings->s_lights[i].position_c);

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

  }*/



  get_program()->use();
  return true;
}

void RainMaterial::setupCamera(const float projecton[16], const float view[16]) const{
 
  Matrix4x4 view_m = Matrix4x4(view);
  Matrix4x4 projection_m = Matrix4x4(projecton);
  float* pv_matrix = projection_m.Multiply(view_m).m;
  int pv_matrix_id = get_program()->get_uniform_position("u_vp_matrix");
  get_program()->set_uniform_value(pv_matrix_id, EDK3::T_MAT_4x4, pv_matrix);
}

void RainMaterial::setupModel(const float m[16]) const{

  int model_id = get_program()->get_uniform_position("u_m_matrix");
  get_program()->set_uniform_value(model_id, EDK3::T_MAT_4x4, m);
}

unsigned int RainMaterial::num_attributes_required() const{
  return 1;
}

EDK3::Attribute RainMaterial::attribute_at_index(const unsigned int attrib_idx) const{
  switch (attrib_idx) {
  case 0:
    return EDK3::A_POSITION;
  break;
  /*
  case 1:
    return EDK3::A_UV;
  break;

  case 2:
    return EDK3::A_NORMAL;
    break;*/

  }

}

EDK3::Type RainMaterial::attribute_type_at_index(const unsigned int attrib_index) const{
 
  switch (attrib_index)
  {
  case 0: return EDK3::T_FLOAT_3; break;

   //case 1: return EDK3::T_UINT_2; break;

   //case 2: return EDK3::T_FLOAT_3; break;
  };
}

RainMaterial::~RainMaterial()
{
}