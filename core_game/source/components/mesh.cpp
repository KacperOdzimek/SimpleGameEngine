#include "mesh.h"

#include "source/entities/entity.h"

#include "source/common/common.h"
#include "source/rendering/renderer.h"

entities::components::mesh::mesh(uint32_t _id) : component(_id)
{
}

void entities::components::mesh::on_attach()
{
	common::renderer->register_mesh_component(this);
}

void entities::components::mesh::mark_pipeline_dirty()
{
	common::renderer->mark_pipeline_dirty(this->get_render_config());
}