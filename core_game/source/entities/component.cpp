#include "component.h"
#include "entity.h"

using namespace entities;

std::weak_ptr<entity> component::get_owner_weak()
{
	return owner->self;
}