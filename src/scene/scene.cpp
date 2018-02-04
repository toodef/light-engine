#include "../base.h"
#include "object_manager/objects/algorithms/lsvco/acmr.h"
#include "object_manager/objects/algorithms/lsvco/ls_vcache_opt.h"

using namespace scene;

scene_t::scene_t()
{

}

scene_t ::~scene_t()
{

}


// push back one object to scene
void scene_t::push( obj_ptr_t const & new_object )
{
   objects_.push_back(new_object);
}

// insert few objects to scene
void scene_t::push( objects_t const & scene_part )
{
   objects_.insert(objects_.begin(), scene_part.begin(), scene_part.end());
}

// pop one object from end of scene
void scene_t::pop( size_t count )
{
   objects_.erase(objects_.end() - min<size_t>(objects_.size(), count), objects_.end());
}

// erase part of scene
void scene_t::erase_scene( scene_iter_t const & from, scene_iter_t const & to )
{
   objects_.erase(from, to);
}

// clear scene
void scene_t::clear()
{
   objects_.clear();
}

size_t scene_t::size() const
{
   return objects_.size();
}

// draw all objects
void scene_t::draw( env_ptr_t const & env )
{
   for (size_t i = 0; i < objects_.size(); ++i)
      objects_[i]->draw(env);
}

obj_ptr_t scene_t::get_object( size_t id )
{
   return objects_[id];
}

obj_ptr_t scene_t::operator [] ( size_t id )
{
   return objects_[id];
}

void scene_t::operator << ( obj_ptr_t const & new_object )
{
   objects_.push_back(new_object);
}