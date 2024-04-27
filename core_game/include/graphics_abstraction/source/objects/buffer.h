#pragma once
#include <cstdint>

namespace graphics_abstraction
{
	enum class buffer_type
	{
		unspecified, vertex, indicies, 
		instanced,   other,
	};

	struct buffer : public object
	{
	protected:
		buffer_type buffer_type = buffer_type::unspecified;
		uint32_t size = 0;

		virtual void bind(internal::pipeline& pipeline) final
		{
			/*
			*	TODO: handle other uses
			*/
			switch (buffer_type)
			{
			case graphics_abstraction::buffer_type::vertex:
				pipeline.vertex_buffer = this;
				break;
			case graphics_abstraction::buffer_type::indicies:
				pipeline.indices_buffer = this;
				break;
			case graphics_abstraction::buffer_type::instanced:
				pipeline.instanced_buffer = this;
				break;
			case graphics_abstraction::buffer_type::other:
				break;
			}
		}
	public:
		//returns buffer size
		virtual inline uint32_t get_size() final
		{
			return size;
		}
		//return buffer type
		virtual inline graphics_abstraction::buffer_type get_buffer_type() final
		{
			return buffer_type;
		}
		/*
			returns pointer to the begin of the buffer
			use it to copy new data into the buffer
			call close_data_stream() afterwards
		*/
		virtual void* open_data_stream() = 0;
		/*
			closes data stream
			call after open_data_stream()
		*/
		virtual void close_data_stream() = 0;
		/*
			removes existing buffer and creates a new blank one of the given size
		*/
		virtual void reallocate(uint32_t new_buffer_size) = 0;
	};

	struct buffer_builder : public builder
	{
		buffer_type buffer_type = buffer_type::unspecified;
		uint32_t size = 0;
	protected:
		virtual object* build_abs(api*& api) final
		{
			if (size == 0 || buffer_type == buffer_type::unspecified)
			{
				api->report_error({
						error_source::abstraction,
						3,
						{}
					});

				return nullptr;
			}

			return this->build_impl(api);
		}
	};
}