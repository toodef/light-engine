#pragma once

#include "../../base.h"

namespace debug
{
   using namespace glm;

   typedef enum
   {
        PT_read_write
      , PT_read_only
   } param_type_t;

   class console_t
   {
   public:
      console_t( string const & bar_name );

      void current_group( string group_name );

      void draw_bar() const;

      TwBar * bar() const;

      void add_parameter( bool * data         , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
      void add_parameter( char * data         , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
      void add_parameter( int * data          , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
       void add_parameter( unsigned int * data, string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
       void add_parameter( size_t * data      , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
      void add_parameter( float * data        , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
      void add_parameter( string * data       , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
      void add_parameter( quat * data         , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
      void add_parameter( vec3 * data         , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );
      void add_parameter( vec4 * data         , string const & param_name, string const & min_value = ""
                                              , string const & step = "", string const & max_value = ""
                                              , param_type_t type = PT_read_write );

   private:
      TwBar * bar_;

      string current_group_;
   };
}