#ifndef __MATERIAL_CUSTOM_CUBEMAP_H__
#define __MATERIAL_CUSTOM_CUBEMAP_H__ 1

//ESAT
#include <ESAT_extra/imgui.h>

//EDK3
#include <EDK3/ref_ptr.h>
#include <EDK3/material.h>
#include <EDK3/materialsettings.h>
#include <EDK3/dev/shader.h>
#include <EDK3/dev/gpumanager.h>
#include <EDK3/texture.h>

//MATHLIB
//#include <MATHLIB/vector_3.h>

//CUSTOM
#include <Mollete/program.h>

class Material_custom_cubemap : public EDK3::Material {

	public:
		Material_custom_cubemap();
		bool enable(const EDK3::MaterialSettings* matsettings) const override;
		void setupCamera(const float projection[16], const float view[16]) const override;
		void setupModel(const float m[16]) const override;

		unsigned int num_attributes_required() const override;
		EDK3::Attribute attribute_at_index(const unsigned int attrib_idx) const override;
		EDK3::Type attribute_type_at_index(const unsigned int attrib_index) const override;

		bool load_shaders_from_filepaths(const char* vertex_shader_path, const char* fragment_shader_path);
		bool load_shaders_source();
		bool load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader);
		
		class Settings : public EDK3::MaterialSettings {
		public:
			Settings() { for (int i = 0; i < 4; ++i) data_[i] = 1.0f; }
			void set_color(const float v[4]) { memcpy(data_, v, sizeof(data_)); }
			void set_color(float r, float g, float b, float a = 1.0f) {
				data_[0] = r; data_[1] = g; data_[2] = b; data_[3] = a;
			}

			const float* color() const { return data_; }
			void set_texture(EDK3::Texture* tex) { texture_ = tex; }
			EDK3::Texture* texture() { return texture_.get(); }
			const EDK3::Texture* texture() const { return texture_.get(); }
		protected:
			virtual ~Settings() {}
		private:
			Settings(const Settings&);
			Settings& operator=(const Settings&);
			float data_[4];
			EDK3::ref_ptr<EDK3::Texture> texture_;
		};

	protected:
		virtual ~Material_custom_cubemap();
		void set_program(EDK3::ref_ptr<EDK3::dev::Program> program);
		EDK3::dev::Program* get_program();
		const EDK3::dev::Program* get_program() const;

	private:
		EDK3::ref_ptr<EDK3::dev::Program> program_;
		Material_custom_cubemap(const Material_custom_cubemap&);
		Material_custom_cubemap& operator=(const Material_custom_cubemap&);
		const char* fragment_path;
		const char* vertex_path;
		
			
};

#endif
