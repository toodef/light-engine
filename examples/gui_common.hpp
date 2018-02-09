#pragma once

#include <GL/glut.h>
#include <engine.h>

// Gui common tools

void resize_func(int w, int h)
{
   if (w < 10)
      w = 10;

   if (h < 10)
      h = 10;

   glViewport(0, 0, w, h);

   SE::engine.set_width(w);
   SE::engine.set_height(h);

   SE::engine.update_proj();
}

bool key_buf[256] = { 0 };

void close_func()
{
   exit(0);
}

bool is_standart_cam = true;

void keyboard_process()
{
   if (key_buf[27])
      close_func();

   switch (is_standart_cam)
   {
   case true:
      if (key_buf['w'])
         SE::engine.move_camera(0.1f, SE::camera_way_t::CW_front);

      if (key_buf['a'])
         SE::engine.move_camera(0.1f, SE::camera_way_t::CW_left);

      if (key_buf['s'])
         SE::engine.move_camera(0.1f, SE::camera_way_t::CW_back);

      if (key_buf['d'])
         SE::engine.move_camera(0.1f, SE::camera_way_t::CW_right);

      if (key_buf['q'])
         SE::engine.move_camera(0.1f, SE::camera_way_t::CW_down);

      if (key_buf['e'])
         SE::engine.move_camera(0.1f, SE::camera_way_t::CW_up);
      break;

   case false:
      if (key_buf['w'])
         SE::engine.move_fake_camera(0.1f, SE::camera_way_t::CW_front);

      if (key_buf['a'])
         SE::engine.move_fake_camera(0.1f, SE::camera_way_t::CW_left);

      if (key_buf['s'])
         SE::engine.move_fake_camera(0.1f, SE::camera_way_t::CW_back);

      if (key_buf['d'])
         SE::engine.move_fake_camera(0.1f, SE::camera_way_t::CW_right);

      if (key_buf['q'])
         SE::engine.move_fake_camera(0.1f, SE::camera_way_t::CW_down);

      if (key_buf['e'])
         SE::engine.move_fake_camera(0.1f, SE::camera_way_t::CW_up);
      break;
   }
}

void keyboard_func(unsigned char button, int x, int y)
{
   key_buf[button] = 1;

   static bool old_cam = false;

   if (button == 9)
   {
      bool tmp = is_standart_cam;

      if (is_standart_cam)
         SE::engine.enable_fake_cam(true);
      else
         SE::engine.enable_fake_cam(false);

      is_standart_cam = old_cam;
      old_cam = tmp;
   }
}

void keyboard_up(unsigned char button, int x, int y)
{
   if (button == 'f')
   {
      if (SE::engine.frustum_state())
         SE::engine.draw_frustum(false);
      else
         SE::engine.draw_frustum(true);
   }

   key_buf[button] = 0;
}

int old_x, old_y;
bool is_rot = 1;

bool is_debug = 0;

void mouse_func(int x, int y)
{
   if (is_rot && is_debug)
   {
      switch (is_standart_cam)
      {
      case true:
         SE::engine.rotate_camera((float)(old_x - x) / 200.f, (float)(old_y - y) / 200.f);
         break;
      case false:
         SE::engine.rotate_fake_cam((float)(old_x - x) / 200.f, (float)(old_y - y) / 200.f);
         break;
      }

      old_x = x;
      old_y = y;
   }
}

void mouse_wheel_func(int whell, int direction, int x, int y)
{
   float new_va = SE::engine.view_angle() - direction;

   if (new_va > 0 && new_va < 180)
      SE::engine.set_fake_cam_va(new_va);
}

void display_func()
{
   keyboard_process();

   SE::engine.on_display();

   glutSwapBuffers();
   glutPostRedisplay();
}

void reshape_func(int w, int h) {
   SE::engine.set_width(w);
   SE::engine.set_height(h);
   SE::engine.draw_frame();
}

void mouse(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON)
   {
      old_x = x;
      old_y = y;

      is_rot = 1;
   }
   else
      is_rot = 0;

   is_debug = 0;
}

class examples_gui_t {
public:
   examples_gui_t(int argc, char ** argv) :
      is_standart_cam_(true)
   {
      glutInit(&argc, argv);

      glutInitWindowSize(800, 700);
      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
      //glutInitContextVersion(4, 2);
      //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
      //glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

      glutCreateWindow("Sample");

      glutReshapeFunc(reshape_func);
      glutDisplayFunc(display_func);
      glutKeyboardFunc(keyboard_func);
      glutKeyboardUpFunc(keyboard_up);
      //glutCloseFunc(close_func);
      glutMouseFunc(mouse);
      glutMotionFunc(mouse_func);
      //glutMouseWheelFunc(mouse_wheel_func);
   }

   void start()
   {
      try {
         glutMainLoop();
      }
      catch (std::exception const & err) {
         std::cerr << err.what() << endl;
         return;
      }
   }

private:
   bool key_buf[256] = { 0 };
   bool is_standart_cam_;
};