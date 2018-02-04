/// \file acmr.h
/// Copyright (C) 2008 Grisha Spivak - rusted dreams

#ifndef acmr_h
#define acmr_h

#include <cassert>
#include <algorithm>
#include <limits>
#include <boost/circular_buffer.hpp>

namespace ls_vcache_opt
{
   // ACMR - Average cache miss ratio calculation, 
   // number of post-transform cache misses per triangle, (0.5,3.0]
   template<typename t_index>
   float acmr( t_index const * indices, unsigned num_tris, unsigned vcache_size )
   {
      boost::circular_buffer<t_index> vcache( vcache_size );
      unsigned misses = 0;
      for( t_index const * idx = indices; idx != indices + num_tris*3; ++idx )
      {
         if( std::find( vcache.rbegin(), vcache.rend(), *idx ) == vcache.rend() )
         {
            ++misses;
            vcache.push_back( *idx );
         }
      }

      return float( misses )/num_tris;
   }
}

#endif //acmr_h