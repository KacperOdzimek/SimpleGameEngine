#pragma once
#include "graphics_abstraction.h"
#include "../common/opengl.h"

namespace graphics_abstraction
{
	namespace implementations
	{
		namespace opengl_3_3_api
		{
			struct shader : public graphics_abstraction::shader
			{
			public:
				internal::GLuint id = 0;
			protected:
				virtual void free()
				{
					internal::glDeleteVertexArrays(1, &id);
				}
			public:
				shader(std::vector<std::string>& code, graphics_abstraction::api*& api)
				{
					int success = 0;
					std::vector<uint8_t> info;
					info.resize(512);

					bool add_geometry_shader = code.size() == 3;

					const char* vertex_shader_source = code[0].c_str();
					const char* geometry_shader_source;
					const char* pixel_shader_source;

					if (add_geometry_shader)
					{
						geometry_shader_source = code[1].c_str();
						pixel_shader_source = code[2].c_str();
					}
					else
					{
						pixel_shader_source = code[1].c_str();
					}

					internal::GLuint vertex_shader = internal::glCreateShader(GL_VERTEX_SHADER);
					internal::glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
					internal::glCompileShader(vertex_shader);

					internal::glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
					if (!success)
					{
						internal::glGetShaderInfoLog(vertex_shader, 512, NULL, (char*)(&info.at(0)));

						api->report_error(
							{
								error_source::implementation,
								(uint16_t)internal::glGetError(),
								info
							}
						);
					}
					
					internal::GLuint fragment_shader = internal::glCreateShader(GL_FRAGMENT_SHADER);
					internal::glShaderSource(fragment_shader, 1, &pixel_shader_source, NULL);
					internal::glCompileShader(fragment_shader);

					internal::glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
					if (!success)
					{
						internal::glGetShaderInfoLog(fragment_shader, 512, NULL, (char*)(&info.at(0)));

						api->report_error(
							{
								error_source::implementation,
								(uint16_t)internal::glGetError(),
								info
							}
						);
					}

					id = internal::glCreateProgram();
					internal::glAttachShader(id, vertex_shader);

					internal::GLuint geometry_shader;
					if (add_geometry_shader)
					{
						geometry_shader = internal::glCreateShader(GL_GEOMETRY_SHADER);
						internal::glShaderSource(geometry_shader, 1, &geometry_shader_source, NULL);
						internal::glCompileShader(geometry_shader);

						internal::glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
						if (!success)
						{
							internal::glGetShaderInfoLog(geometry_shader, 512, NULL, (char*)(&info.at(0)));

							api->report_error(
								{
									error_source::implementation,
									(uint16_t)internal::glGetError(),
									info
								}
							);
						}

						internal::glAttachShader(id, geometry_shader);
					}

					internal::glAttachShader(id, fragment_shader);

					internal::glLinkProgram(id);
					internal::glGetProgramiv(id, GL_LINK_STATUS, &success);
					if (!success)
					{
						internal::glGetProgramInfoLog(id, 512, NULL, (char*)(&info.at(0)));

						api->report_error(
							{
								error_source::implementation,
								(uint16_t)internal::glGetError(),
								info
							}
						);
					}
					internal::glDeleteShader(vertex_shader);
					internal::glDeleteShader(fragment_shader);
					if (add_geometry_shader)
						internal::glDeleteShader(geometry_shader);
				}
			protected:
				virtual void set_uniform_value_impl(std::string uniform_name, data_type uniform_data_type, void* value)
				{
					internal::glUseProgram(id);
					auto loc = internal::glGetUniformLocation(id, uniform_name.c_str());
					switch (uniform_data_type)
					{
					case graphics_abstraction::data_type::Int:
						internal::glUniform1i(loc, *(int*)value);
						break;
					case graphics_abstraction::data_type::Float:
						internal::glUniform1f(loc, *(float*)value);
					case graphics_abstraction::data_type::Bool:
						internal::glUniform1i(loc, *(int*)value);
						break;
					case graphics_abstraction::data_type::vec2:
						internal::glUniform2fv(loc, 1, (float*)(value));
						break;
					case graphics_abstraction::data_type::vec3:
						internal::glUniform3fv(loc, 1, (float*)(value));
						break;
					case graphics_abstraction::data_type::vec4:
						internal::glUniform4fv(loc, 1, (float*)(value));
						break;
					case graphics_abstraction::data_type::mat2x2:
						internal::glUniformMatrix2fv(loc, 1, false, (float*)value);
						break;
					case graphics_abstraction::data_type::mat2x3:
						internal::glUniformMatrix2x3fv(loc, 1, false, (float*)value);
						break;
					case graphics_abstraction::data_type::mat2x4:
						internal::glUniformMatrix2x4fv(loc, 1, false, (float*)value);
						break;
					case graphics_abstraction::data_type::mat3x2:
						internal::glUniformMatrix3x2fv(loc, 1, false, (float*)value);
						break;
					case graphics_abstraction::data_type::mat3x3:
						internal::glUniformMatrix3fv(loc, 1, false, (float*)value);
						break;
					case graphics_abstraction::data_type::mat3x4:
						internal::glUniformMatrix3x4fv(loc, 1, false, (float*)value);
						break;
					case graphics_abstraction::data_type::mat4x2:
						internal::glUniformMatrix4x2fv(loc, 1, false, (float*)value);
						break;
					case graphics_abstraction::data_type::mat4x3:
						internal::glUniformMatrix4x3fv(loc, 1, false, (float*)value);
						break;
					case graphics_abstraction::data_type::mat4x4:
						internal::glUniformMatrix4fv(loc, 1, false, (float*)value);
						break;
					default:
						break;
					}
				}

				virtual void set_uniform_value_impl(std::string uniform_name, data_type uniform_data_type, std::vector<float> values)
				{
					internal::glUseProgram(id);
					auto loc = internal::glGetUniformLocation(id, uniform_name.c_str());
					switch (uniform_data_type)
					{
					case graphics_abstraction::data_type::Int:
					case graphics_abstraction::data_type::Bool:
						//TODO: ERROR
						break;
					default:
						set_uniform_value_impl(uniform_name, uniform_data_type, &values[0]);
						break;
					}
				}

				virtual void set_uniform_value_impl(std::string uniform_name, data_type uniform_data_type, int value)
				{
					internal::glUseProgram(id);
					auto loc = internal::glGetUniformLocation(id, uniform_name.c_str());
					switch (uniform_data_type)
					{
					case graphics_abstraction::data_type::Int:
					case graphics_abstraction::data_type::Bool:
						internal::glUniform1i(loc, value);
						break;
					default:
						break;
					}
				}
			};

			struct shader_builder : public graphics_abstraction::shader_builder
			{
				virtual object* build_impl(graphics_abstraction::api*& api)
				{
					return new shader{code, api};
				}
			};
		}
	}
}