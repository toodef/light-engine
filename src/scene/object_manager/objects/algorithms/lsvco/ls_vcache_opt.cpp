/// \file ls_vcache_opt.cpp
/// Copyright (C) 2007 Grisha Spivak - rusted dreams
/// Linear-Speed Vertex Cache Optimisation algorithm implementation
/// http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html

#include "../../../../../base.h"

#include <cassert>
#include <vector>
#include <algorithm>

#include "ls_vcache_opt.h"

namespace ls_vcache_opt
{
   namespace
   {
      template<typename t_index>
      struct vertex_data
      {
         short cache_pos_;
         short score_;
         //short num_tris_;
         //short num_active_tris_;
         std::vector<unsigned> tris_;

         vertex_data()
            : cache_pos_( -1 )
         {
            tris_.reserve( 8 );   ///< should be enough for most vertices
         }
      };

      template<typename t_index>
      struct tri_data
      {
         t_index indices_[3];
         short score_;
         bool added_;
      };

      const unsigned vcache_size = 32;

      short vcache_score_table[vcache_size] = 
      {
         750, 750, 750, 1000, 949, 898, 849, 800, 753, 706, 661, 616, 573, 530, 489, 449, 410, 372, 335, 300, 266, 234, 202, 173, 145, 119, 94, 72, 51, 33, 18, 6, 
      };

      short valence_boost_score_table[16] = 
      {
         2000, 1414, 1155, 1000, 894, 816, 756, 707, 667, 632, 603, 577, 555, 535, 516, 500, 
      };

      template<typename t_index>
      inline short vertex_score( vertex_data<t_index> const & v )
      {
         if( v.tris_.empty() )
            return -1;   //< No tri needs this vertex!

         short score = 0;

         if( v.cache_pos_ >= 0 )
         {
            assert( v.cache_pos_ < vcache_size );
            score = vcache_score_table[v.cache_pos_];
         }

         score += valence_boost_score_table[std::min( v.tris_.size() - 1, 
            sizeof(valence_boost_score_table)/sizeof(valence_boost_score_table[0])-1 )];

         return score;
      }
   }

   template<typename t_index>
   void optimize( t_index * indices, unsigned num_tris, unsigned num_verts )
   {
      assert( num_tris != 0 );
      assert( num_verts != 0 );
      assert( num_verts <= std::numeric_limits<t_index>::max() );

      std::vector<tri_data<t_index>> tris(num_tris);
      std::vector<vertex_data<t_index>> verts(num_verts);

      unsigned best_tri = 0;

      /// initialization
      {
         t_index const * idx = indices;
         tri_data<t_index> * tri = tris.data();
         for( unsigned tri_index = 0; tri_index != num_tris; ++tri, ++tri_index, idx += 3 )
         {
            tri->indices_[0] = idx[0];
            tri->indices_[1] = idx[1];
            tri->indices_[2] = idx[2];
            tri->added_ = false;

            verts[idx[0]].tris_.push_back( tri_index );
            verts[idx[1]].tris_.push_back( tri_index );
            verts[idx[2]].tris_.push_back( tri_index );
         }

         for( vertex_data<t_index> * vert = verts.data(), * end = verts.data() + num_verts; 
            vert != end; vert++ )
            vert->score_ = vertex_score( *vert );

         tri = tris.data();
         short max_score = -1;
         for( unsigned tri_index = 0; tri_index != num_tris; tri++, tri_index++ )
         {
            tri->score_ = verts[tri->indices_[0]].score_
               + verts[tri->indices_[1]].score_
               + verts[tri->indices_[2]].score_;

            if( tri->score_ > max_score )
            {
               max_score = tri->score_;
               best_tri = tri_index;
            }
         }
      }

      /// main body of the algorithm
      //{
      //   unsigned tris_left = num_tris;
      //   boost::circular_buffer<t_index> vcache( vcache_size + 3 );

      //   while( tris_left-- )
      //   {
      //      /// add triangle to output list
      //      {
      //         tri_data<t_index> & tri = tris[best_tri];
      //         indices[0] = tri.indices_[0];
      //         indices[1] = tri.indices_[1];
      //         indices[2] = tri.indices_[2];
      //         indices += 3;
      //         tri.added_ = true;

      //         /// reduce valence of used verts
      //         for( t_index const * idx = tri.indices_; 
      //            idx != tri.indices_ + 3; ++idx )
      //         {
      //            vertex_data<t_index> & vert = verts[*idx];
      //            vert.tris_.erase( 
      //               std::find( vert.tris_.begin(), vert.tris_.end(), best_tri ) );

      //            typename boost::circular_buffer<t_index>::iterator in_cache =
      //               std::find( vcache.begin(), vcache.end(), *idx );
      //            if( in_cache != vcache.end() )
      //               vcache.erase( in_cache );   // TODO try erase if closer to begin
      //         }

      //         vcache.push_back( tri.indices_[0] );
      //         vcache.push_back( tri.indices_[1] );
      //         vcache.push_back( tri.indices_[2] );
      //      }

      //      /// update cache pos and score of verts in cache
      //      {
      //         short cache_pos = vcache.size() - 1;
      //         for( typename boost::circular_buffer<t_index>::iterator i = vcache.begin(),
      //            vcache_end = vcache.end(); 
      //            i != vcache_end; ++i, --cache_pos )
      //         {
      //            vertex_data<t_index> & vert = verts[*i];
      //            vert.cache_pos_ = cache_pos >= vcache_size ? -1 : cache_pos;
      //            vert.score_ = vertex_score( vert );
      //         }
      //      }

      //      /// update tris score and look for best tri
      //      {
      //         short max_score = -1;
      //         for( typename boost::circular_buffer<t_index>::iterator i = vcache.begin(),
      //            vcache_end = vcache.end(); 
      //            i != vcache_end; ++i )
      //         {
      //            vertex_data<t_index> & vert = verts[*i];
      //            for( std::vector<unsigned>::iterator itri = vert.tris_.begin(),
      //               tris_end = vert.tris_.end(); 
      //               itri != tris_end; ++itri )
      //            {
      //               tri_data<t_index> & tri = tris[*itri];
      //               assert( !tri.added_ );

      //               tri.score_ = verts[tri.indices_[0]].score_
      //                  + verts[tri.indices_[1]].score_
      //                  + verts[tri.indices_[2]].score_;

      //               if( tri.score_ > max_score )
      //               {
      //                  max_score = tri.score_;
      //                  best_tri = *itri;
      //               }
      //            }
      //         }

      //         if( max_score == -1 )
      //         {
      //            /// best tri not found
      //            /// cache didn't hold verts with non-added tris
      //            tri_data<t_index> * tri = tris.get();
      //            for( unsigned tri_index = 0; tri_index != num_tris; tri++, tri_index++ )
      //            {
      //               if( !tri->added_ && tri->score_ > max_score )
      //               {
      //                  max_score = tri->score_;
      //                  best_tri = tri_index;
      //               }
      //            }
      //         }
      //      }

      //      if( vcache.size() > vcache_size )
      //         vcache.rresize( vcache_size );
      //   }
      //}
   }

   /// explicit instantiation - only two index types are possible
   template
   void optimize( unsigned int * indices, unsigned num_tris, unsigned num_verts );
   template
   void optimize( unsigned short * indices, unsigned num_tris, unsigned num_verts );
}