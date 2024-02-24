#pragma once
#include "source/entities/component.h"
#include "glm/glm.hpp"

namespace entities
{
	namespace components
	{
		/*
			camera
			component that adds viewport to the game
		*/
		class camera : virtual public entities::component
		{
		public:
			camera(uint32_t _id, float _ortho_width) : ortho_width(_ortho_width), component(_id) {}
			/*
				viewport x width
			*/
			float ortho_width = 20;
			/*
				determines how many layers above the camera entity layer should be visible
			*/
			int rendered_layers = 1;
			/*
				get_deepest_rendered_layer
				returns the deepest layer that would be rendered
			*/
			int get_deepest_rendered_layer();
			/*
				get_projection
				returns 4d matrix that projects vertices from the world space to the screen space
			*/
			glm::mat4 get_projection();
			/*
				get_view_center_location
				get viewport centre in world space
				used in vertices projection
			*/
			glm::vec2 get_view_center_location();
			virtual void on_attach();
		private:
		};
	}
}