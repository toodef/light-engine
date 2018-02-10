#include "base.h"

using namespace SE;

////////////////////////////////////////
/// engine_t class functions
////////////////////////////////////////

void gl_debug_proc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const * message,
   GLvoid const * user_param)
{
   if (type == GL_DEBUG_TYPE_ERROR_ARB)
   {
      std::cerr << message << endl;
      exit(1);
   }
}

engine_t & engine_t::instance()
{
   static engine_t en;
   return en;
}

void engine_t::init( int argc, char ** argv, string const & window_name )
{
   if (glewInit() != GLEW_OK)
      cerr << "GLEW init failed!" << endl;

   if (!GL_VERSION_4_2)
      cerr << "OpenGL 4.2 not supported!" << endl;

   glDebugMessageCallbackARB(gl_debug_proc, NULL);
   glClampColorARB(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
   glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
   glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);

   env_.reset(new env_t(window_name, material_manager_, texture_manager_, camera_t(vec3(1, 1, 1), vec3(0, 0, 0)), 800, 700, view_angle_, near_, far_));

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
   material_manager_ = std::make_shared<material_manager_t>();
   texture_manager_ = std::make_shared<texture_manager_t>();
   buffer_manager_ = std::make_shared<buffer_manager_t>();
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

texture_ptr_t engine_t::create_texture( tex_params_t const & params )
{
   return texture_manager_->create_texture(params);
}

texture_ptr_t engine_t::create_texture( GLenum type )
{
   return texture_manager_->create_texture(type);
}

texture_ptr_t engine_t::create_texture( )
{
   return texture_manager_->create_texture();
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

buffer_manager_ptr_t engine_t::get_buffer_manager() const
{
   return buffer_manager_;
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
