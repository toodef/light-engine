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

      void push( obj_ptr_t const & new_object );
      void push( objects_t const & scene_part );
      void pop( size_t count = 1 );
      void erase( scene_iter_t const & from, scene_iter_t const & to );
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
