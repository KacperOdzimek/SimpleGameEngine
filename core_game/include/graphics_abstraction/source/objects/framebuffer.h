#pragma once
#include <map>
#include <cstdint>

namespace graphics_abstraction
{
	struct framebuffer : public object
	{
	protected:
		std::map<uint8_t, texture*> color_buffers;
		texture* depth_stencil_buffer;

		virtual void bind(internal::pipeline& pipeline) final
		{
			pipeline.framebuffer = this;
		}

		virtual void set_color_buffer_impl(uint8_t number) = 0;
		virtual void set_depth_stencil_buffer_impl() = 0;

	public:
		virtual void clear_color_buffers(float r, float g, float b, float a) = 0;
		virtual void clear_depth_buffer() = 0;
		virtual void clear_stencil_buffer() = 0;

		void set_color_buffer(uint8_t number, texture* color_buffer)
		{
			color_buffers.insert({ number, color_buffer });
			set_color_buffer_impl(number);
		}
		void set_depth_stencil_buffer(texture* _depth_stencil_buffer)
		{
			depth_stencil_buffer = _depth_stencil_buffer;
			set_depth_stencil_buffer_impl();
		}
		virtual texture* get_color_buffer(uint8_t number) final
		{
			if (color_buffers.find(number) == color_buffers.end())
				return nullptr;
			return color_buffers.at(number);
		}
		virtual texture* get_depth_stencil_buffer() final
		{
			return depth_stencil_buffer;
		}
	};

	struct framebuffer_builder : public builder
	{
		std::map<uint8_t, texture*> color_buffers;
		texture* depth_stencil_buffer = nullptr;
	protected:
		virtual object* build_abs(api*& api) final
		{
			return this->build_impl(api);
		}
	};
}