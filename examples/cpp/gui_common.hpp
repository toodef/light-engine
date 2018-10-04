#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <exception>
#include <iostream>
#include <memory>

#include <light_engine.hpp>
#include <frame/scene/object/builtin_objects.hpp>
#include <utils/user_camera.hpp>

using namespace LE;
using namespace LE::Utils;

std::unique_ptr<light_engine_t> light_engine;
scene_ptr_t scene;
user_camera_ptr_t user_camera;

void gl_debug_proc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const * message, GLvoid const * user_param) {
   if (type == GL_DEBUG_TYPE_ERROR_ARB)
      std::cerr << message << std::endl << user_param << std::endl;
}

void resize_func(int w, int h)
{
   if (w < 10)
      w = 10;

   if (h < 10)
      h = 10;

   light_engine->resize(w, h);
}

bool key_buf[256] = { 0 };

void close_func() {
   light_engine.reset();

   exit(0);
}

bool is_standart_cam = true;

void keyboard_process()
{
   if (key_buf[27])
      close_func();

   user_camera->process_keyboard(key_buf);
}

void keyboard_func(unsigned char button, int x, int y) {
   key_buf[button] = 1;
}

void keyboard_up(unsigned char button, int x, int y) {
   key_buf[button] = 0;
}

int old_x, old_y;
bool is_rot = 1;

void display_func()
{
   keyboard_process();

   light_engine->redraw();

   glutSwapBuffers();
   glutPostRedisplay();
}

void mouse_func(int x, int y) {
   user_camera->process_cursore_movement(x, y);
}

void mouse(int button, int state, int x, int y) {
   if ((button == GLUT_LEFT_BUTTON) || (button == GLUT_RIGHT_BUTTON))
      user_camera->process_mouse_press(x, y, button == GLUT_LEFT_BUTTON, state == GLUT_DOWN);
   else if ((button == 3) || (button == 4))
      user_camera->process_wheel(button == 3 ? 1 : -1);
}

class examples_gui_t {
public:
   examples_gui_t(int argc, char ** argv) {
      glutInit(&argc, argv);

      glutInitWindowSize(800, 700);
      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
      glutInitContextVersion(4, 2);
      glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
      glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

      glutCreateWindow("Basic Sample");

      light_engine = std::make_unique<light_engine_t>();
      frame_ptr_t frame = std::make_shared<frame_t>();
      scene = std::make_shared<scene_t>();
      frame->add_scene(scene);
      light_engine->add_frame(frame);
      frame->set_background_color(glm::vec3(0, 0, 0));

      user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
      scene->get_camera()->look_at(glm::vec3(0, 0, 0));

      for (size_t i = 0; i < 100; ++i)
         scene->add_object(builtin_objects_t::point(glm::vec3((float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5), glm::vec3(0, 1, 0)));

      glDebugMessageCallbackARB(gl_debug_proc, NULL);
      glutReshapeFunc(resize_func);
      glutDisplayFunc(display_func);
      glutKeyboardFunc(keyboard_func);
      glutKeyboardUpFunc(keyboard_up);
      glutMouseFunc(mouse);
      glutMotionFunc(mouse_func);

      const char * vend = (const char *)glGetString(GL_VENDOR);
      const char * render = (const char *)glGetString(GL_RENDERER);
      const char * vers = (const char *)glGetString(GL_VERSION);

      std::cout << std::string(vend) << std::endl << std::string(render) << std::endl << std::string(vers) << std::endl;
   }

   void start()
   {
      try {
         glutMainLoop();
      }
      catch (std::exception const & err) {
         std::cerr << err.what() << std::endl;
         return;
      }
   }
};