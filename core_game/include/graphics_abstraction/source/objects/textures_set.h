#pragma once
#include "../objects/object.h"
#include "../objects/texture.h"

namespace graphics_abstraction
{
	struct textures_set : public object
	{
	protected:
		std::vector<texture*> textures;
	protected:
		virtual void bind(internal::pipeline& pipeline) final
		{
			pipeline.textures_set = this;
		}

	public:
		virtual void set_selection(std::vector<texture*> _textures) final
		{
			textures = _textures;
		}
	};

	struct textures_set_builder : public builder
	{
		std::vector<texture*> textures;
	protected:
		virtual object* build_abs(api*& api) final
		{
			return this->build_impl(api);
		}
	};
}