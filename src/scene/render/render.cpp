#include "../../base.h"

using namespace SE;

render_t::render_t()
{
}

render_t::~render_t()
{
}

effect_t render_t::create_effect( size_t id, material_ptr_t const & material, texture_ptr_t const & texture )
{
   effect_t new_effect = effect_t(material, texture, id);

   return new_effect;
}

void render_t::delete_effect( size_t id )
{
}
