
#include <Material/material_custom_cubemap.h>
#include <Mollete/gpumanager.h>

#include <EDK3/dev/buffer.h>
#include <ESAT/time.h>
#include <ESAT/math.h>
#include <ESAT/input.h>

Material_custom_cubemap::Material_custom_cubemap() {

}
Material_custom_cubemap::~Material_custom_cubemap() {

}

void Material_custom_cubemap::set_program(EDK3::ref_ptr<EDK3::dev::Program> program)
{
	program_ = program;
}

EDK3::dev::Program* Material_custom_cubemap::get_program()
{
	return program_.get();
}

const EDK3::dev::Program* Material_custom_cubemap::get_program() const
{
	return program_.get();
}

bool Material_custom_cubemap::load_shaders_source()
{

	EDK3::ref_ptr<EDK3::dev::Shader> VertexShader;
	EDK3::ref_ptr<EDK3::dev::Shader> FragmentShader;

	EDK3::dev::GPUManager::Instance()->newShader(&VertexShader);
	EDK3::dev::GPUManager::Instance()->newShader(&FragmentShader);

	const char* fragment =
		"#version 330\n"
		"out vec4 fragColor;\n"
		"in vec3 TexCoords;\n"
		"uniform samplerCube u_colorTexture;\n"
		"void main()\n"
		"{\n"
		"fragColor = texture(u_colorTexture, TexCoords);\n"
		"}\n\0";

	const char* vertex =
		"#version 330\n"
		"layout(location = 0) in vec3 a_position;\n"
		"uniform mat4 u_v_matrix;\n"
		"uniform mat4 u_p_matrix;\n"
		"out vec3 TexCoords;\n"
		"void main()\n"
		"{\n"
		"TexCoords = a_position;\n"
		"vec4 pos = u_p_matrix * mat4(mat3(u_v_matrix)) * vec4(a_position, 1.0);\n"
		"gl_Position = pos.xyww;\n"
		"}\n\0";

	VertexShader->loadSource(EDK3::dev::Shader::kType_Vertex, vertex, strlen(vertex) + 1);
	FragmentShader->loadSource(EDK3::dev::Shader::kType_Fragment, fragment, strlen(fragment) + 1);

	return load_shaders(VertexShader, FragmentShader);
}

bool Material_custom_cubemap::load_shaders_from_filepaths(const char* vertex_shader_path, const char* fragment_shader_path)
{
	EDK3::ref_ptr<EDK3::dev::Shader> VertexShader;
	EDK3::ref_ptr<EDK3::dev::Shader> FragmentShader;


	vertex_path = vertex_shader_path;
	fragment_path = fragment_shader_path;

	loadVertexShaderFile(&VertexShader, vertex_shader_path);
	loadFragmentShaderFile(&FragmentShader, fragment_shader_path);

	return load_shaders(VertexShader, FragmentShader);
}

bool Material_custom_cubemap::load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader) {

	vertex_shader->compile();

	/*if (vertex_shader->is_compiled() == false) {
		return false;
	}*/

	fragment_shader->compile();

	/*if (fragment_shader->is_compiled() == false) {
		return false;
	}*/

	EDK3::dev::GPUManager::Instance()->newProgram(&program_);

	set_program(program_);

	get_program()->attach(vertex_shader.get());
	get_program()->attach(fragment_shader.get());
	get_program()->link();

	return true;

}

bool Material_custom_cubemap::enable(const EDK3::MaterialSettings* matsettings) const{

	get_program()->use(); 
	Material_custom_cubemap::Settings* material_ = (Settings*)matsettings;

	int position = get_program()->get_uniform_position("u_colorTexture");
	material_->texture()->bind(0);
	const int value = 0;
	//get_program()->set_uniform_value(position, EDK3::T_INT_1, &value);
	return true;
}

void Material_custom_cubemap::setupModel(const float m[16]) const
{
	int position = get_program()->get_uniform_position("u_m_matrix");
	get_program()->set_uniform_value(position,EDK3::T_MAT_4x4, m);
}

void Material_custom_cubemap::setupCamera(const float projection[16], const float view[16]) const
{

	const ESAT::Mat4 vp_matrix = ESAT::Mat4Multiply(ESAT::Mat4FromColumns(projection), ESAT::Mat4FromColumns(view));

	int position = get_program()->get_uniform_position("u_vp_matrix");
	get_program()->set_uniform_value(position, EDK3::T_MAT_4x4, vp_matrix.d);

	const ESAT::Mat4 v_matrix = ESAT::Mat4FromColumns(view);

	position = get_program()->get_uniform_position("u_v_matrix");
	get_program()->set_uniform_value(position, EDK3::T_MAT_4x4, v_matrix.d);

	const ESAT::Mat4 p_matrix = ESAT::Mat4FromColumns(projection);

	position = get_program()->get_uniform_position("u_p_matrix");
	get_program()->set_uniform_value(position, EDK3::T_MAT_4x4, p_matrix.d);

}

unsigned int Material_custom_cubemap::num_attributes_required() const {
	return 1;
}

EDK3::Attribute Material_custom_cubemap::attribute_at_index(const unsigned int attrib_idx) const {
	switch (attrib_idx) {
		case 0:return EDK3::A_POSITION;
	}

}

EDK3::Type Material_custom_cubemap::attribute_type_at_index(const unsigned int attrib_index) const {
	switch (attrib_index) {
		case 0:return EDK3::T_FLOAT_3;
	}
}
