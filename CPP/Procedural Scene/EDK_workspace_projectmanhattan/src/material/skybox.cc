#define _CRT_SECURE_NO_WARNINGS

#include <Material/skybox.h>
#include <EDK3/dev/shader.h>
#include <EDK3/dev/gpumanager.h>
#include "matrix_4.h"
#include <stb-master/stb_image.h>
#include <EDK3/dev/opengl.h>
#include <ESAT/math.h>

Skybox::Skybox()
{
}

bool Skybox::load_shaders_from_filepaths(const char * vertex_shader_path, const char * fragment_shader_path)
{
  EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader;
  EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader;
  loadVertexShaderFile(&vertex_shader, vertex_shader_path);
  loadFragmentShaderFile(&fragment_shader, fragment_shader_path);

  return load_shaders(vertex_shader, fragment_shader);
}

bool Skybox::load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader)
{
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

bool Skybox::enable(const EDK3::MaterialSettings *mat_setting) const
{
  
  SettingsSkyboxMat* matsettings = (SettingsSkyboxMat*)mat_setting;

  if (nullptr != matsettings->texture()) {
    int id_texture_sampler2D = get_program()->get_uniform_position("skybox");
    matsettings->texture()->bind(0);
    get_program()->set_uniform_value(id_texture_sampler2D, EDK3::T_UINT_1, (int*)0);
  }

  //Fog
  /*int id_fog = get_program()->get_uniform_position("fog_activate");
  get_program()->set_uniform_value(id_fog, EDK3::T_INT_1, matsettings->fog_active());

  int id_density = get_program()->get_uniform_position("density");
  get_program()->set_uniform_value(id_density, EDK3::T_FLOAT_1, matsettings->fog_density());*/

  return true;

}

void Skybox::setupCamera(const float projecton[16], const float view[16]) const
{

  const ESAT::Mat4 v_matrix = ESAT::Mat4FromColumns(view);

	int position = get_program()->get_uniform_position("view");
	get_program()->set_uniform_value(position, EDK3::T_MAT_4x4, v_matrix.d);

	const ESAT::Mat4 p_matrix = ESAT::Mat4FromColumns(projecton);

	position = get_program()->get_uniform_position("projection");
	get_program()->set_uniform_value(position, EDK3::T_MAT_4x4, p_matrix.d);

}

void Skybox::setupModel(const float m[16]) const
{

  int model_id = get_program()->get_uniform_position("u_m_matrix");
  get_program()->set_uniform_value(model_id, EDK3::T_MAT_4x4, m);

}

unsigned int Skybox::num_attributes_required() const
{
  return 1;
}

EDK3::Attribute Skybox::attribute_at_index(const unsigned int attrib_idx) const
{
  
  switch (attrib_idx) {
  case 0:
    return EDK3::A_POSITION;
    break;

  }

}

EDK3::Type Skybox::attribute_type_at_index(const unsigned int attrib_index) const
{
  
  switch (attrib_index)
  {
    case 0: return EDK3::T_FLOAT_3; break;
  };

}

void Skybox::loadCubeMap(EDK3::ref_ptr<Mollete::Texture> *texture)
{

  glActiveTexture(GL_TEXTURE0);

  texture->alloc();

  EDK3::ref_ptr<Mollete::Texture>* newTexture = (EDK3::ref_ptr<Mollete::Texture>*)texture;

  texture->get()->init(EDK3::Texture::T_CUBEMAP, EDK3::Texture::F_RGBA, 2048, 2048);

  glBindTexture(GL_TEXTURE_CUBE_MAP, texture->get()->internal_id());

  int width, height, nrComponents;
  for (unsigned int i = 0; i < faces.size(); i++)
  {
    unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    }
    else
    {
      std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

Skybox::~Skybox()
{
  
}

void Skybox::set_program(EDK3::ref_ptr<EDK3::dev::Program> program)
{
  program_ = program;
}

EDK3::dev::Program * Skybox::get_program()
{
  return program_.get();
}

const EDK3::dev::Program * Skybox::get_program() const
{
  return program_.get();
}