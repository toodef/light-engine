#pragma once

#include "../base.h"

namespace scene
{
   using namespace object;

   typedef vector<obj_ptr_t> objects_t;
   typedef objects_t::iterator  scene_iter_t;

   class scene_t
   {
   public:
      scene_t ();
      ~scene_t();

      // push back one object
      void push( obj_ptr_t const & new_object );

      // push back few objects
      void push( objects_t const & scene_part );

      // pop one object from end of scene
      void pop( size_t count = 1 );

      // erase part of scene
      void erase_scene( scene_iter_t const & from, scene_iter_t const & to );

      // clear scene
      void clear();

      size_t size() const;

      // draw all objects
      void draw( env_ptr_t const & env );

      obj_ptr_t get_object( size_t id );

      /*
       * operators
       */

      obj_ptr_t operator [] ( size_t id );
      void      operator << ( obj_ptr_t const & new_object );

   private:

      objects_t objects_;
   };
}
