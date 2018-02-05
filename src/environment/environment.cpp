#include "../base.h"
#include "shaders.h"

using namespace SE;

////////////////////////////////////////
/// camera functions
////////////////////////////////////////

camera_t::camera_t( vec3 const & pos, vec3 const & look_at, quat const & q, vec3 const & dir
                   ,vec3 const & up, vec3 const & right ) :
     pos_(pos)
   , q_(q)
{
   if (dep_data_)
      dep_data_.reset();
   dep_data_ = std::optional<dep_data_t>({ dir, up, right });

   set_look_at(look_at, 1);

   dep_data_->right = normalize(cross(dep_data_->dir, up));
   dep_data_->up    = normalize(cross(dep_data_->right, dep_data_->dir));
}

vec3 const & camera_t::pos() const
{
   return pos_;
}

vec3 const & camera_t::look_at() const
{
   return look_at_;
}

quat const & camera_t::orientation() const
{
   return q_;
}

void camera_t::set_dir( vec3 const & dir )
{
   dep_data_->dir = dir;
}

void camera_t::set_up( vec3 const & up )
{
   dep_data_->up = up;
}

void camera_t::set_right( vec3 const & right )
{
   dep_data_->right = right;
}

vec3 const & camera_t::dir() const
{
   return dep_data_->dir;
}

vec3 const & camera_t::up() const
{
   return dep_data_->up;
}

vec3 const & camera_t::right() const
{
   return dep_data_->right;
}

mat4 camera_t::world_view() const
{
   return glm::lookAt(pos_, look_at_, dep_data_->up);
}

void camera_t::set_pos( vec3 const & pos )
{
   pos_ = pos;
}

void camera_t::set_look_at( vec3 const & look_at, bool dir )
{
   look_at_ = look_at;

   if (dir)
      dep_data_->dir = normalize(look_at_ - pos_);
}

void camera_t::set_orientation( quat const & q )
{
   dep_data_->dir = q * dep_data_->dir;

   set_look_at(pos_ + dep_data_->dir, 0);

   dep_data_->right = q * dep_data_->right;

   dep_data_->up = q * dep_data_->up;

   q_ = q;
}

////////////////////////////////////////
// fake_camera functions
////////////////////////////////////////

fake_camera_t::fake_camera_t( const camera_t & cam, float view_angle, float asp, float z_near, float z_far ) :
     camera_t      (cam.pos(), cam.look_at(), cam.orientation(), cam.dir(), cam.up(), cam.right())
   , view_angle_   (view_angle)
   , near_         (z_near)
   , far_          (z_far)
   , asp_          (asp)
   , frustum_coord_(vector<vec3>(8))
   , enabled_      (false)
{
}

void fake_camera_t::update_frustum( float view_angle, float asp, float z_near, float z_far )
{
   float cos_a2 = cos((view_angle_ / 360.0f) * 3.1415926235f);
   float sin_a2 = sin((view_angle_ / 360.0f) * 3.1415926235f);

   float b = z_near / cos_a2;

   float c = b * sin_a2;

   vec3 p1 = z_near * dep_data_->dir + pos_ + c * dep_data_->up;
   vec3 p2 = z_near * dep_data_->dir + pos_ - c * dep_data_->up;

   vec3 tnp = asp * c * dep_data_->right;

   // near_plane quad
   frustum_coord_[0] = p2 - tnp; // bottom left
   frustum_coord_[1] = p1 - tnp; // top left
   frustum_coord_[2] = p1 + tnp; // top right
   frustum_coord_[3] = p2 + tnp; // bottom right

   z_far = 10.0f;

   b = z_far / cos_a2;

   c = b * sin_a2;

   p1 = z_far * dep_data_->dir + pos_ + c * dep_data_->up;
   p2 = z_far * dep_data_->dir + pos_ - c * dep_data_->up;

   tnp = asp * c * dep_data_->right;

   // far quad
   frustum_coord_[4] = p2 + tnp;
   frustum_coord_[5] = p1 + tnp;
   frustum_coord_[6] = p1 - tnp;
   frustum_coord_[7] = p2 - tnp;

   n_near_   = normalize(cross(frustum_coord_[0] - frustum_coord_[3], frustum_coord_[2] - frustum_coord_[3]));
   n_far_    = normalize(cross(frustum_coord_[5] - frustum_coord_[4], frustum_coord_[7] - frustum_coord_[4]));
   n_left_   = normalize(cross(frustum_coord_[6] - frustum_coord_[7], frustum_coord_[0] - frustum_coord_[7]));
   n_right_  = normalize(cross(frustum_coord_[2] - frustum_coord_[3], frustum_coord_[4] - frustum_coord_[3]));
   n_top_    = normalize(cross(frustum_coord_[2] - frustum_coord_[5], frustum_coord_[6] - frustum_coord_[5]));
   n_bottom_ = normalize(cross(frustum_coord_[4] - frustum_coord_[3], frustum_coord_[0] - frustum_coord_[3]));

   d_near_   = dot(frustum_coord_[1], n_near_  );
   d_far_    = dot(frustum_coord_[4], n_far_   );
   d_left_   = dot(frustum_coord_[1], n_left_  );
   d_right_  = dot(frustum_coord_[4], n_right_ );
   d_top_    = dot(frustum_coord_[1], n_top_   );
   d_bottom_ = dot(frustum_coord_[4], n_bottom_);

   vertices_t new_frustum = vertices_t(24);

   for (int i = 0; i < 24; ++i)
      new_frustum[i].color_  = frustum_->color();

   // near_plane
   new_frustum[0].vertex_ = frustum_coord_[0];
   new_frustum[0].normal_ = n_near_;
   new_frustum[1].vertex_ = frustum_coord_[1];
   new_frustum[1].normal_ = n_near_;
   new_frustum[2].vertex_ = frustum_coord_[2];
   new_frustum[2].normal_ = n_near_;
   new_frustum[3].vertex_ = frustum_coord_[3];
   new_frustum[3].normal_ = n_near_;

   // right
   new_frustum[4].vertex_ = frustum_coord_[3];
   new_frustum[4].normal_ = n_right_;
   new_frustum[5].vertex_ = frustum_coord_[2];
   new_frustum[5].normal_ = n_right_;
   new_frustum[6].vertex_ = frustum_coord_[5];
   new_frustum[6].normal_ = n_right_;
   new_frustum[7].vertex_ = frustum_coord_[4];
   new_frustum[7].normal_ = n_right_;

   // far
   new_frustum[8].vertex_  = frustum_coord_[7];
   new_frustum[8].normal_  = n_far_;
   new_frustum[9].vertex_  = frustum_coord_[4];
   new_frustum[9].normal_  = n_far_;
   new_frustum[10].vertex_ = frustum_coord_[5];
   new_frustum[10].normal_ = n_far_;
   new_frustum[11].vertex_ = frustum_coord_[6];
   new_frustum[11].normal_ = n_far_;

   // left
   new_frustum[12].vertex_ = frustum_coord_[0];
   new_frustum[12].normal_ = n_left_;
   new_frustum[13].vertex_ = frustum_coord_[7];
   new_frustum[13].normal_ = n_left_;
   new_frustum[14].vertex_ = frustum_coord_[6];
   new_frustum[14].normal_ = n_left_;
   new_frustum[15].vertex_ = frustum_coord_[1];
   new_frustum[15].normal_ = n_left_;

   // top
   new_frustum[16].vertex_ = frustum_coord_[5];
   new_frustum[16].normal_ = n_top_;
   new_frustum[17].vertex_ = frustum_coord_[2];
   new_frustum[17].normal_ = n_top_;
   new_frustum[18].vertex_ = frustum_coord_[1];
   new_frustum[18].normal_ = n_top_;
   new_frustum[19].vertex_ = frustum_coord_[6];
   new_frustum[19].normal_ = n_top_;

   // bottom
   new_frustum[20].vertex_ = frustum_coord_[3];
   new_frustum[20].normal_ = n_bottom_;
   new_frustum[21].vertex_ = frustum_coord_[4];
   new_frustum[21].normal_ = n_bottom_;
   new_frustum[22].vertex_ = frustum_coord_[7];
   new_frustum[22].normal_ = n_bottom_;
   new_frustum[23].vertex_ = frustum_coord_[0];
   new_frustum[23].normal_ = n_bottom_;

   frustum_->get_buffer()->data(new_frustum);
}

vec3 fake_camera_t::frustum_vertex( unsigned int id )
{
   return frustum_coord_[id];
}

obj_ptr_t fake_camera_t::frustum() const
{
   return frustum_;
}

void fake_camera_t::set_frustum( obj_ptr_t const & frustum )
{
   frustum_ = frustum;
}

float fake_camera_t::view_angle() const
{
   return view_angle_;
}

float fake_camera_t::near_plane() const
{
   return near_;
}

float fake_camera_t::far_plane() const
{
   return far_;
}

float fake_camera_t::asp() const
{
   return asp_;
}

// frustum planes function
vec3 fake_camera_t::n_near() const
{
   return n_near_;
}

vec3 fake_camera_t::n_far() const
{
   return n_far_;
}

vec3 fake_camera_t::n_left() const
{
   return n_left_;
}

vec3 fake_camera_t::n_right() const
{
   return n_right_;
}

vec3 fake_camera_t::n_top() const
{
   return n_top_;
}

vec3 fake_camera_t::n_bottom() const
{
   return n_bottom_;
}

float fake_camera_t::d_near() const
{
   return d_near_;
}

float fake_camera_t::d_far() const
{
   return d_far_;
}

float fake_camera_t::d_left() const
{
   return d_left_;
}

float fake_camera_t::d_right() const
{
   return d_right_;
}

float fake_camera_t::d_top() const
{
   return d_top_;
}

float fake_camera_t::d_bottom() const
{
   return d_bottom_;
}

void fake_camera_t::set_view_angle( float new_va )
{
   view_angle_ = new_va;
}

////////////////////////////////////////
/// environment class functions
////////////////////////////////////////

env_t::env_t( string const & window_handle, material_manager_ptr_t material_manager, texture_manager_ptr_t texture_manager, camera_t cam, int width, int height, float view_angle, float z_near, float z_far ) :
     cam_             (cam)
   , fake_cam_        (fake_camera_t(cam, view_angle, (float)width / height, z_near, z_far))
   , width_           (width)
   , height_          (height_)
   , proj_            (mat4())
   , view_            (mat4())
   , mvp_             (mat4())
   , material_manager_(material_manager)
   , texture_manager_ (texture_manager)
   , default_effect_  (material_manager_->create_material_from_source(shaders::default_vs, shaders::default_fs), NULL, 0)
{}
