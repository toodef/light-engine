/// \file ls_vcache_opt.h
/// Copyright (C) 2007 Grisha Spivak - rusted dreams
/// Linear-Speed Vertex Cache Optimisation algorithm implementation
/// http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html

#ifndef ls_vcache_opt_h
#define ls_vcache_opt_h

#include <cassert>
#include <limits>
#include <vector>

namespace ls_vcache_opt
{
   template<typename t_index>
   void optimize( t_index * indices, unsigned num_tris, unsigned num_verts );

   namespace detail
   {
      template<typename t_index, typename t_vertices_range>
      t_index reorder_vertices( t_index * indices, unsigned num_tris, 
         t_vertices_range & new_verts, t_vertices_range & vertices, unsigned num_verts )
      {
         boost::scoped_array<bool> added_verts( new bool[num_verts]() ); // value-initialize to false
         boost::scoped_array<t_index> remap_inides( new t_index[num_verts] );

         t_index vertout = 0;
         for( t_index * idx = indices; idx != indices + num_tris*3; ++idx )
         {
            if( !added_verts[*idx] )
            {
               remap_inides[*idx] = vertout;
               new_verts[vertout++] = vertices[*idx];
               added_verts[*idx] = true;
            }
            *idx = remap_inides[*idx];
         }

         vertices.swap( new_verts );

         return vertout;
      }
   }

   // reorder vertices for more linear access while rendering
   template<typename t_index, typename t_vertex>
   t_index reorder_vertices( t_index * indices, unsigned num_tris, 
      std::unique_ptr<t_vertex> & vertices, unsigned num_verts )
   {
      assert( num_verts <= std::numeric_limits<t_index>::max() );
      boost::scoped_array<t_vertex> new_verts( new t_vertex[num_verts] );
      return detail::reorder_vertices( indices, num_tris, new_verts, vertices, num_verts );
   }

   // reorder vertices for more linear access while rendering
   template<typename t_index, typename t_vertex>
   t_index reorder_vertices( t_index * indices, unsigned num_tris, 
      std::vector<t_vertex> & vertices )
   {
      assert( vertices.size() <= std::numeric_limits<t_index>::max() );
      std::vector<t_vertex> new_verts( vertices.size() );
      return detail::reorder_vertices( indices, num_tris, new_verts, vertices, vertices.size() );
   }
}

#endif //ls_vcache_opt_h