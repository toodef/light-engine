#pragma once

#include <GL/glut.h>
#include <exception>
#include <iostream>

void resize_func(int w, int h)
{
   if (w < 10)
      w = 10;

   if (h < 10)
      h = 10;
}

bool key_buf[256] = { 0 };

void close_func() {
   exit(0);
}

bool is_standart_cam = true;

void keyboard_process()
{
   if (key_buf[27])
      close_func();
}

void keyboard_func(unsigned char button, int x, int y) {
   key_buf[button] = 1;
}

void keyboard_up(unsigned char button, int x, int y) {
   key_buf[button] = 0;
}

int old_x, old_y;
bool is_rot = 1;

void mouse_func(int x, int y)
{
   if (is_rot) {
      old_x = x;
      old_y = y;
   }
}

void display_func()
{
   keyboard_process();

   glutSwapBuffers();
   glutPostRedisplay();
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
}

class examples_gui_t {
public:
   examples_gui_t(int argc, char ** argv) {
      glutInit(&argc, argv);

      glutInitWindowSize(800, 700);
      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

      glutCreateWindow("Basic Sample");

      glutReshapeFunc(resize_func);
      glutDisplayFunc(display_func);
      glutKeyboardFunc(keyboard_func);
      glutKeyboardUpFunc(keyboard_up);
      glutMouseFunc(mouse);
      glutMotionFunc(mouse_func);
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