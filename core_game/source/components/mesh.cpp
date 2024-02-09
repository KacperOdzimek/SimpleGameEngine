#include "mesh.h"

#include "source/entities/entity.h"

#include "source/common/common.h"
#include "source/rendering/renderer.h"

entities::components::mesh::mesh(
	uint32_t _id, 
	std::weak_ptr<assets::mesh> _mesh, 
	std::weak_ptr<assets::shader> _material,
	std::vector<std::weak_ptr<assets::texture>> _textures	
)
	: component(_id), offset({0, 0}), visible(true)
{
	_config.mesh = _mesh.lock();
	_config.material = _material.lock();
	for (auto& txt : _textures)
		_config.textures.push_back(txt.lock());
}

void entities::components::mesh::on_attach()
{
	common::renderer->register_mesh_component(this);
}

entities::components::mesh::~mesh()
{
	common::renderer->unregister_mesh_component(this);
}

void entities::components::mesh::pass_transformation(rendering::transformations_buffer_iterator& tbi)
{
	if (visible)
	{
		glm::vec2 location = owner->get_location() + offset;

		tbi.put(std::move(location.x));
		tbi.put(std::move(location.y));

		tbi.put(scale.x);
		tbi.put(scale.y);
	}
}