#pragma once

// Gui common tools

void reshape_func(int w, int h)
{
   if (w < 10)
      w = 10;

   if (h < 10)
      h = 10;

   glViewport(0, 0, w, h);

   engine.set_width(w);
   engine.set_height(h);

   engine.update_proj();
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
         engine.move_camera(0.1f, CW_front);

      if (key_buf['a'])
         engine.move_camera(0.1f, CW_left);

      if (key_buf['s'])
         engine.move_camera(0.1f, CW_back);

      if (key_buf['d'])
         engine.move_camera(0.1f, CW_right);

      if (key_buf['q'])
         engine.move_camera(0.1f, CW_down);

      if (key_buf['e'])
         engine.move_camera(0.1f, CW_up);
      break;

   case false:
      if (key_buf['w'])
         engine.move_fake_camera(0.1f, CW_front);

      if (key_buf['a'])
         engine.move_fake_camera(0.1f, CW_left);

      if (key_buf['s'])
         engine.move_fake_camera(0.1f, CW_back);

      if (key_buf['d'])
         engine.move_fake_camera(0.1f, CW_right);

      if (key_buf['q'])
         engine.move_fake_camera(0.1f, CW_down);

      if (key_buf['e'])
         engine.move_fake_camera(0.1f, CW_up);
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
         engine.enable_fake_cam(true);
      else
         engine.enable_fake_cam(false);

      is_standart_cam = old_cam;
      old_cam = tmp;
   }
}

void keyboard_up(unsigned char button, int x, int y)
{
   if (button == 'f')
   {
      if (engine.frustum_state())
         engine.draw_frustum(false);
      else
         engine.draw_frustum(true);
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
         engine.rotate_camera((float)(old_x - x) / 200.f, (float)(old_y - y) / 200.f);
         break;
      case false:
         engine.rotate_fake_cam((float)(old_x - x) / 200.f, (float)(old_y - y) / 200.f);
         break;
      }

      old_x = x;
      old_y = y;
   }
}

void mouse_wheel_func(int whell, int direction, int x, int y)
{
   float new_va = engine.view_angle() - direction;

   if (new_va > 0 && new_va < 180)
      engine.set_fake_cam_va(new_va);
}

void display_func()
{
   keyboard_process();

   engine.on_display();

   glutSwapBuffers();
   glutPostRedisplay();
}

class examples_gui_t {
public:
   examples_gui_t(int argc, char ** argv) :
      is_standart_cam_(true)
   {
      glutInit(&argc, argv);

      glutInitWindowSize(800, 700);
      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
      glutInitContextVersion(4, 2);
      glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
      glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

      glutCreateWindow(window_name.c_str());

      glutReshapeFunc(reshape_func);
      glutDisplayFunc(display_func);
      glutKeyboardFunc(keyboard_func);
      glutKeyboardUpFunc(keyboard_up);
      glutCloseFunc(close_func);
      glutMouseFunc(mouse);
      glutMotionFunc(mouse_func);
      glutMouseWheelFunc(mouse_wheel_func);
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

   void keyboard_func(unsigned char button, int x, int y)
   {
      key_buf[button] = 1;

      static bool old_cam = false;

      if (button == 9)
      {
         bool tmp = is_standart_cam_;

         if (is_standart_cam_)
            engine.enable_fake_cam(true);
         else
            engine.enable_fake_cam(false);

         is_standart_cam = old_cam;
         old_cam = tmp;
      }
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

private:
   bool key_buf[256] = { 0 };
   is_standart_cam_;
};