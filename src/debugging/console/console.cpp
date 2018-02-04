#include "../../base.h"

using namespace debug;

console_t::console_t( string const & bar_name ) :
   current_group_("")
{
   TwInit(TW_OPENGL, NULL);

   bar_ = TwNewBar(bar_name.c_str());
}

void console_t::current_group( string group_name )
{
   current_group_ = group_name;
}

void console_t::draw_bar() const
{
   TwDraw();
}

TwBar * console_t::bar() const
{
   return bar_;
}

//////////////////////////////////////
// add any items functions
//////////////////////////////////////

string combine( string const & min_value, string const & step, string const & max_value
               ,string const & group_name )
{
   string str = "";

   if (group_name != "")
      str += "group=" + group_name;

   if (step != "")
      str = "step=" + step + " " + str;

   if (max_value != "")
      str = "max=" + max_value + " " + str;

   if (min_value != "")
      str = "min=" + min_value + " " + str;

   return str;
}

void console_t::add_parameter( bool * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_BOOLCPP, data, combine(min_value, step, max_value
                                                                          ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_BOOLCPP, data, combine(min_value, step, max_value
                                                                          ,current_group_).c_str());
      break;
   }
}

void console_t::add_parameter( char * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_CHAR, data, combine(min_value, step, max_value
                                                                       ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_CHAR, data, combine(min_value, step, max_value
                                                                       ,current_group_).c_str());
      break;
   }
}

void console_t::add_parameter( int * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_INT32, data, combine(min_value, step, max_value
                                                                        ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_INT32, data, combine(min_value, step, max_value
                                                                        ,current_group_).c_str());
      break;
   }
}

void console_t::add_parameter( unsigned int * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_UINT32, data, combine(min_value, step, max_value
                                                                         ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_UINT32, data, combine(min_value, step, max_value
                                                                         ,current_group_).c_str());
      break;
   }
}

void console_t::add_parameter( size_t * data, string const & param_name, string const & min_value
        ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
      case PT_read_write:
         TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_UINT32, data, combine(min_value, step, max_value
                 ,current_group_).c_str());
           break;
      case PT_read_only:
         TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_UINT32, data, combine(min_value, step, max_value
                 ,current_group_).c_str());
           break;
   }
}

void console_t::add_parameter( float * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_FLOAT, data, combine(min_value, step, max_value
                                                                        ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_FLOAT, data, combine(min_value, step, max_value
                                                                        ,current_group_).c_str());
      break;
   }
}

void console_t::add_parameter( string * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_STDSTRING, data, combine(min_value, step, max_value
                                                                            ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_STDSTRING, data, combine(min_value, step, max_value
                                                                            ,current_group_).c_str());
      break;
   }
}

void console_t::add_parameter( quat * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_QUAT4F, data, combine(min_value, step, max_value
                                                                         ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_QUAT4F, data, combine(min_value, step, max_value
                                                                         ,current_group_).c_str());
      break;
   }
}

void console_t::add_parameter( vec3 * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_DIR3F, data, combine(min_value, step, max_value
                                                                        ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_DIR3F, data, combine(min_value, step, max_value
                                                                        ,current_group_).c_str());
      break;
   }
}

void console_t::add_parameter( vec4 * data, string const & param_name, string const & min_value
                              ,string const & step, string const & max_value, param_type_t type )
{
   switch (type)
   {
   case PT_read_write:
      TwAddVarRW(bar_, param_name.c_str(), TW_TYPE_COLOR4F, data, combine(min_value, step, max_value
                                                                          ,current_group_).c_str());
      break;
   case PT_read_only:
      TwAddVarRO(bar_, param_name.c_str(), TW_TYPE_COLOR4F, data, combine(min_value, step, max_value
                                                                          ,current_group_).c_str());
      break;
   }
}
