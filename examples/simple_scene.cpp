#include <engine.h>
#include "gui_common.hpp"

using namespace SE;

void display_func_my()
{
   engine.draw_frame();
   //engine.debug_bar().draw_bar();
}

int main( int argc, char ** argv )
{
   engine.init(argc, argv, "Default scene");

   engine.set_on_display(display_func_my);

   engine.static_scene().push(
         obj_manager_t::create_line(engine.default_effect(), SE::engine.get_buffer_manager(), vec4(1.0f, 0.0f, 0.0f, 1.0f),
                                    vec3(0.0f, 0.00003f, 0.0f), vec3(1000.0f, 0.00003f, 0.0f)));

   engine.static_scene().push(
         obj_manager_t::create_line(engine.default_effect(), SE::engine.get_buffer_manager(), vec4(0.0f, 1.0f, 0.0f, 1.0f),
                                    vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1000.0f, 0.0f)));

   engine.static_scene().push(
         obj_manager_t::create_line(engine.default_effect(), SE::engine.get_buffer_manager(), vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                    vec3(0.0f, 0.00003f, 0.0f), vec3(0.0f, 0.00003f, 1000.0f)));

   engine.static_scene().push(
         obj_manager_t::create_plane(engine.default_effect(), SE::engine.get_buffer_manager(), 1.0f, 1.0f, 40));

   //engine.start();

   return 0;
}