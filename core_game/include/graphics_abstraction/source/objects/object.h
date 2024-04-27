#pragma once
#include "../common/api.h"
#include "../private/pipeline.h"
#include "../data/error.h"

namespace graphics_abstraction
{
	struct object
	{
	friend class api;
	friend struct builder;
	public:
		object() {};		
	protected:
		~object() {};
		api* owner = nullptr;
		virtual void bind(internal::pipeline& pipeline) = 0;
		virtual void free() = 0;
	};

	struct builder
	{
		friend api;
	protected:
		virtual object* build_abs(api*& api) = 0;						//Implement as final in abstraction
																		//Check if arguments are sufficient etc.

		virtual object* build_impl(api*& api) = 0;						//Implement in implementation
																		//Create object

		virtual inline object* build(api* api) final
		{
			auto* obj = build_abs(api);
			if (obj != nullptr)
				obj->owner = api;
			return obj;
		}
	};
}