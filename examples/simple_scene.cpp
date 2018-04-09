#include <engine.h>
#include "gui_common.hpp"

using namespace SE;

void display_func_my() {
   engine.draw_frame();
}

int main( int argc, char ** argv ) {
   examples_gui_t gui(argc, argv);
   
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

   engine.static_scene().push(obj_manager_t::create_sphere(engine.default_effect(), engine.get_buffer_manager(), vec4(1, 0, 0, 1), 1));

   gui.start();

   return 0;
}