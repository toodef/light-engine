#include "base.h"

using namespace SE;

////////////////////////////////////////
/// engine_t class functions
////////////////////////////////////////

void gl_debug_proc( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const * message,
                    GLvoid const * user_param
)
{
   if (type == GL_DEBUG_TYPE_ERROR_ARB)
   {
      cerr << message << endl;
      exit(1);
   }
}

void reshape_func( int w, int h )
{
   if (w < 10)
      w = 10;

   if (h < 10)
      h = 10;

   glViewport(0, 0, w, h);

   engine.set_width(w);
   engine.set_height(h);

   TwWindowSize(engine.width(), engine.height());

   engine.update_proj();
}

bool key_buf[256] = {0};

void close_func()
{
   TwTerminate();
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

void keyboard_func( unsigned char button, int x, int y )
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

   TwEventKeyboardGLUT(button, x, y);
}

void keyboard_up( unsigned char button, int x, int y )
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

void mouse( int button, int state, int x, int y )
{
   if (button == GLUT_LEFT_BUTTON)
   {
      old_x = x;
      old_y = y;

      is_rot = 1;
   } else
      is_rot = 0;

   is_debug = 0;

   TwEventMouseButtonGLUT(button, state, x, y);
}

#ifndef GLUTCALLBACK
   #define GLUTCALLBACK
#endif

void GLUTCALLBACK tw_mouse_func( int x, int y )
{
   if (!TwEventMouseMotionGLUT(x, y))
   {
      is_debug = 1;
   }
}

void mouse_func( int x, int y )
{
   tw_mouse_func(x, y);

   if (is_rot && is_debug)
   {
      switch (is_standart_cam)
      {
         case true:
            engine.rotate_camera((float) (old_x - x) / 200.f, (float) (old_y - y) / 200.f);
            break;
         case false:
            engine.rotate_fake_cam((float) (old_x - x) / 200.f, (float) (old_y - y) / 200.f);
            break;
      }

      old_x = x;
      old_y = y;
   }

   TwEventMouseMotionGLUT(x, y);
}

void mouse_wheel_func( int whell, int direction, int x, int y )
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

engine_t & engine_t::instance()
{
   static engine_t en;
   return en;
}

void engine_t::init( int argc, char ** argv, string const & window_name )
{
   glutInit(&argc, argv);

   glutInitWindowSize(800, 700);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitContextVersion(4, 2);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutPassiveMotionFunc((GLUTmousemotionfun) TwEventMouseMotionGLUT);
   glutSpecialFunc((GLUTspecialfun) TwEventSpecialGLUT);

   TwGLUTModifiersFunc(glutGetModifiers);

   glutCreateWindow(window_name.c_str());

   if (glewInit() != GLEW_OK)
   {
      cerr << "GLEW init failed!" << endl;
   }

   if (!GL_VERSION_4_2)
   {
      cerr << "OpenGL 4.2 not supported!" << endl;
   }

   glDebugMessageCallbackARB(gl_debug_proc, NULL);
   glClampColorARB(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
   glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
   glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);

   glutReshapeFunc(reshape_func);
   glutDisplayFunc(display_func);
   glutKeyboardFunc(keyboard_func);
   glutKeyboardUpFunc(keyboard_up);
   glutCloseFunc(close_func);
   glutMouseFunc(mouse);
   glutMotionFunc(mouse_func);
   glutMouseWheelFunc(mouse_wheel_func);

   env_.reset(new env_t(window_name, material_manager_, texture_manager_, camera_t(vec3(1, 1, 1), vec3(0, 0, 0)), 800, 700, view_angle_, near_, far_));

   Magick::InitializeMagick("");

   update_proj();
   update_mvp();

   env_->fake_cam_.set_frustum(
         obj_manager_t::create_frustrum(env_->default_effect_, buffer_manager_, env_, vec4(1, 1, 0, 0.5f)));

   env_->fake_cam_.update_frustum(view_angle_, 800.0f / 700.0f, near_, far_);

   env_->fake_cam_.frustum()->set_wireframe(false);

   env_->buffer_manager_ = buffer_manager_;
}

engine_t::engine_t()
{
   material_manager_ = new material_manager_t();
   texture_manager_ = new texture_manager_t();
   buffer_manager_ = new buffer_manager_t();
   view_angle_ = 20.0f;
   near_ = 0.1f;
   far_ = 100.0f;
   draw_frustum_ = false;
   visible_boxes_indexes_ = 0;
   fps_ = 0;
   delta_time_ = 0;
   timer_ = environment::timer_t();
   accum_frames_count_ = 30;
}

void engine_t::start()
{
   try
   {
      glutMainLoop();
   }
   catch (std::exception const & err)
   {
      cerr << err.what() << endl;
      return;
   }

}

engine_t::~engine_t()
{
}

void engine_t::update_proj()
{
   env_->proj_ = perspective(view_angle_, (float) env_->width_ / env_->height_, near_, far_);

   update_mvp();
}

void engine_t::update_mvp()
{
   env_->mvp_ = env_->proj_ * env_->cam_.world_view();

   static bool draw_frustum = false;

   if (draw_frustum != draw_frustum_ && draw_frustum_ == true)
   {
      update_frustum();
      draw_frustum = draw_frustum_;
   }

   if (!draw_frustum_)
   {
      env_->fake_cam_.set_pos(env_->cam_.pos());
      env_->fake_cam_.set_dir(env_->cam_.dir());
      env_->fake_cam_.set_up(env_->cam_.up());
      env_->fake_cam_.set_right(env_->cam_.right());
   }
}

void engine_t::rotate_camera( float heading, float pitch )
{
   glm::quat rotate;

   rotate = glm::rotate(rotate, heading, vec3(0, 1, 0));

   float cos_a = dot(env_->cam_.dir(), vec3(0, 1, 0));

   if (cos_a > -0.9f && cos_a < 0.9f)
      rotate = glm::rotate(rotate, pitch, env_->cam_.right());

   if ((cos_a < -0.9 && pitch > 0.0) || (cos_a > 0.9 && pitch < 0.0))
      rotate = glm::rotate(rotate, pitch, env_->cam_.right());

   env_->cam_.set_orientation(rotate);

   update_mvp();
}

void engine_t::move_camera( const vec3 & way )
{
   env_->cam_.set_pos(env_->cam_.pos() + way);
   env_->cam_.set_look_at(env_->cam_.look_at() + way, 0);

   update_mvp();
}

void engine_t::move_camera( float scale, camera_way_t cam_way )
{
   vec3 v;

   switch (cam_way)
   {
      case CW_front:
         v = env_->cam_.dir() * scale;
         break;

      case CW_back:
         v = -env_->cam_.dir() * scale;
         break;

      case CW_right:
         v = env_->cam_.right() * scale;
         break;

      case CW_left:
         v = -env_->cam_.right() * scale;
         break;

      case CW_up:
         v = env_->cam_.up() * scale;
         break;

      case CW_down:
         v = -env_->cam_.up() * scale;
         break;
   }

   env_->cam_.set_pos(env_->cam_.pos() + v);
   env_->cam_.set_look_at(env_->cam_.look_at() + v, 0);

   update_mvp();
}

void engine_t::move_fake_camera( const vec3 & way )
{
   env_->fake_cam_.set_pos(env_->fake_cam_.pos() + way);
   env_->fake_cam_.set_look_at(env_->fake_cam_.look_at() + way, 0);

   update_frustum();
}

void engine_t::move_fake_camera( float scale, camera_way_t cam_way )
{
   vec3 v;

   switch (cam_way)
   {
      case CW_front:
         v = env_->cam_.dir() * scale;
         break;

      case CW_back:
         v = -env_->cam_.right() * scale;
         break;

      case CW_right:
         v = env_->cam_.right() * scale;
         break;

      case CW_left:
         v = -env_->cam_.right() * scale;
         break;

      case CW_up:
         v = env_->cam_.up() * scale;
         break;

      case CW_down:
         v = -env_->cam_.up() * scale;
         break;
   }

   env_->fake_cam_.set_pos(env_->fake_cam_.pos() + v);
   env_->fake_cam_.set_look_at(env_->fake_cam_.look_at() + v, 0);

   update_frustum();
}

float engine_t::fps()
{
   return fps_;
}

void engine_t::set_on_display( on_display_fnc_t on_display_fnc )
{
   on_display_fnc_ = on_display_fnc;
}

void engine_t::on_display()
{
   on_display_fnc_();
}

effect_t engine_t::default_effect() const
{
   return env_->default_effect_;
}

void engine_t::draw_frame()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glClearColor(0, 0, 0, 1);

   glDepthFunc(GL_LEQUAL);

   glEnable(GL_DEPTH_TEST);

   delta_time_ = timer_.time<float>();
   fps_ = 1.0f / delta_time_;

   timer_.restart();

   scene_.draw(env_);
   static_scene_.draw(env_);

   if (draw_frustum_)
   {
      material_manager_->bind_material(env_->fake_cam_.frustum()->get_material().get());
      buffer_manager_->bind_buffer(env_->fake_cam_.frustum()->get_buffer().get());

      env_->fake_cam_.frustum()->draw(env_);

      buffer_manager_->bind_buffer();
      material_manager_->bind_material();
   }

   static GLuint depth_texture;
   static bool is_first = true;

   if (is_first)
   {
      glGenTextures(1, &depth_texture);

      is_first = false;
   }

   if (double_scene_.size())
   {
      glBindTexture(GL_TEXTURE_2D, depth_texture);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glTexImage2D(GL_TEXTURE_2D, 0, 3, env_->width_, env_->height_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

      glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, env_->width_, env_->height_, 0);

      glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

      double_scene_[0]->draw(env_);

      glBindTexture(GL_TEXTURE_2D, 0);
   }

   TwRefreshBar(env_->debug_.bar());

   glDisable(GL_DEPTH_TEST);
}

////////////////////////////////////////
/// scene functions
////////////////////////////////////////

scene_t & engine_t::scene()
{
   return scene_;
}

scene_t & engine_t::static_scene()
{
   return static_scene_;
}

scene_t & engine_t::double_scene()
{
   return double_scene_;
}

////////////////////////////////////////
/// material's and her friend's functions
////////////////////////////////////////

material_ptr_t engine_t::create_material( const string & vertex_shader, const string & fragment_shader,
                                          const string & geometry_shader )
{
   return material_manager_->create_material(vertex_shader, fragment_shader, geometry_shader);
}

texture_ptr_t engine_t::create_texture( const string & file_name, tex_params_t const & params )
{
   return texture_manager_->create_texture(file_name, params);
}

texture_ptr_t engine_t::create_texture( const string & file_name, GLenum type )
{
   return texture_manager_->create_texture(file_name, type);
}

texture_ptr_t engine_t::create_texture( const vector<string> & files_names, tex_params_t const & params )
{
   return texture_manager_->create_texture(files_names, params);
}

texture_ptr_t engine_t::create_texture( const vector<string> & files_names )
{
   return texture_manager_->create_texture(files_names);
}

buffer_ptr_t engine_t::create_buffer( vertices_t data, size_t size, GLenum usage_type )
{
   return buffer_manager_->create_buffer(data, usage_type);
}

material_manager_ptr_t engine_t::get_material_manager() const
{
   return material_manager_;
}

texture_manager_ptr_t engine_t::get_texture_manager() const
{
   return texture_manager_;
}

buffer_manager_t * engine_t::get_buffer_manager() const
{
   return buffer_manager_.get();
}

void engine_t::set_width( int width )
{
   env_->width_ = width;
}

void engine_t::set_height( int height )
{
   env_->height_ = height;
}

void engine_t::set_cam_pos( vec3 const & pos )
{
   env_->cam_.set_pos(pos);
}

void engine_t::set_look_at( vec3 const & look_at )
{
   env_->cam_.set_look_at(look_at, true);
}

void engine_t::rotate_fake_cam( float heading, float pitch )
{
   glm::quat rotate;

   rotate = glm::rotate(rotate, heading, vec3(0, 1, 0));

   float cos_a = dot(env_->fake_cam_.dir(), vec3(0, 1, 0));

   if (cos_a > -0.9f && cos_a < 0.9f)
      rotate = glm::rotate(rotate, pitch, env_->fake_cam_.right());

   if ((cos_a < -0.9 && pitch > 0.0) || (cos_a > 0.9 && pitch < 0.0))
      rotate = glm::rotate(rotate, pitch, env_->fake_cam_.right());

   env_->fake_cam_.set_orientation(rotate);

   update_frustum();
}

void engine_t::set_fake_cam_va( float new_va )
{
   env_->fake_cam_.set_view_angle(new_va);

   update_frustum();
}

float engine_t::view_angle() const
{
   return env_->fake_cam_.view_angle();
}

void engine_t::enable_fake_cam( bool need_fake_cam )
{
   if (!enable_fake_cam_ && need_fake_cam && !draw_frustum_)
      update_fake_cam();

   enable_fake_cam_ = need_fake_cam;
}

void engine_t::update_fake_cam() const
{
   env_->fake_cam_.set_dir(env_->cam_.dir());
   env_->fake_cam_.set_up(env_->cam_.up());
   env_->fake_cam_.set_right(env_->cam_.right());

   env_->fake_cam_.set_pos(env_->cam_.pos());
   env_->fake_cam_.set_look_at(env_->cam_.look_at(), 0);
   env_->fake_cam_.set_orientation(env_->cam_.orientation());
}

bool engine_t::fake_cam_state() const
{
   return enable_fake_cam_;
}

int engine_t::width() const
{
   return env_->width_;
}

int engine_t::height() const
{
   return env_->height_;
}

camera_t engine_t::cam() const
{
   return env_->cam_;
}

console_t engine_t::debug_bar() const
{
   return env_->debug_;
}

void engine_t::update_frustum()
{
   if (draw_frustum_)
      env_->fake_cam_.update_frustum(env_->fake_cam_.view_angle(), (float) env_->width_ / env_->height_, near_, far_);
}

void engine_t::draw_frustum( bool is_need_draw_frustum )
{
   draw_frustum_ = is_need_draw_frustum;
}

bool engine_t::frustum_state() const
{
   return draw_frustum_;
}
