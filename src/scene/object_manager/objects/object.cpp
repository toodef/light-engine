#include "../../../base.h"

using namespace SE;

////////////////////////////////////////
/// object_t class functions
////////////////////////////////////////

object_t::object_t( effect_t const & effect, buffers_t const & buffers
                   ,render_param_t const & render_param, vec3 pos
                   ,vec4 color, float size
                   ,idx_buffers_t const & idx_buffers ):
     material_          (effect.get<0>())
   , texture_           (effect.get<1>())
   , effect_id_         (effect.get<2>())
   , buffers_           (buffers)
   , render_param_      (render_param)
   , pos_               (pos)
   , size_              (size)
   , color_             (color)
   , f_plane_indexes_   (vector<bool>(6))
   , f_plane_is_checked_(vector<bool>(6))
   , last_failed_idx_   (0)
   , idx_buffers_       (idx_buffers)
{
}

object_t::~object_t()
{
}

material_ptr_t object_t::get_material() const
{
   return material_;
}

texture_ptr_t object_t::get_texture() const
{
   return texture_;
}

buffer_ptr_t object_t::get_buffer( size_t idx ) const
{
   return buffers_[idx];
}

// support function for void object_t::draw( env_ptr_t const & env )
void enable_vertex_attrib()
{
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(3);
   glEnableVertexAttribArray(6);
   glEnableVertexAttribArray(9);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0); // vertex
   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid *)(sizeof(vec3))); // normal
   glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t)
                         ,(GLvoid *)(sizeof(vec3) * 2)); // texture coordinates
   glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t)
                         ,(GLvoid *)(sizeof(vec3) * 3)); // color
}

// support function for void object_t::draw( env_ptr_t const & env )
void disable_vertex_attrib()
{
   glDisableVertexAttribArray(9);
   glDisableVertexAttribArray(6);
   glDisableVertexAttribArray(3);
   glDisableVertexAttribArray(0);
}

void object_t::draw( env_ptr_t const & env )
{
   static float scale_val = 715.0f;

   if (material_->is_init_)
      env->material_manager_->bind_material(material_.get());


   // TODO: add support to manual add uniforms to object (if doesn't exist) or disable console output of unused uniform variables
   uniform_ptr_t m_mvp       = material_->uniform("m_mvp"      );
//   uniform_ptr_t inv_view    = material_->uniform("inv_view"   );
//   uniform_ptr_t obj_pos     = material_->uniform("obj_pos"    );
//   uniform_ptr_t size        = material_->uniform("size"       );
//   uniform_ptr_t cam_pos     = material_->uniform("cam_pos"    );
//   uniform_ptr_t cube_map    = material_->uniform("cube_map"   );
//   uniform_ptr_t scale       = material_->uniform("scale"      );
//   uniform_ptr_t m_prj       = material_->uniform("m_prj"      );
//   uniform_ptr_t tex         = material_->uniform("tex"        );
//   uniform_ptr_t ref_in      = material_->uniform("ref_in"     );
//   uniform_ptr_t eye_pos     = material_->uniform("eye_pos"    );
//   uniform_ptr_t light_pos   = material_->uniform("light_pos"  );
//   uniform_ptr_t light_color = material_->uniform("light_color");
//   uniform_ptr_t k_a         = material_->uniform("k_a"        );
//   uniform_ptr_t k_d         = material_->uniform("k_d"        );
//   uniform_ptr_t k_s         = material_->uniform("k_s"        );
//   uniform_ptr_t spec_power  = material_->uniform("spec_power" );
//   uniform_ptr_t f_bl        = material_->uniform("f_bl"       );
//   uniform_ptr_t f_tl        = material_->uniform("f_tl"       );
//   uniform_ptr_t f_br        = material_->uniform("f_br"       );
//   uniform_ptr_t f_tr        = material_->uniform("f_tr"       );
//   uniform_ptr_t near_plane  = material_->uniform("near"       );
//   uniform_ptr_t far_plane   = material_->uniform("far"        );

   m_mvp->set      (&env->mvp_);
//   inv_view->set   (inverse(env->proj_));
//   obj_pos->set    (pos_);
//   size->set       (size_);
//   cam_pos->set    (env->cam_.pos());
//   cube_map->set   (0);
//   scale->set      (200.0f);
//   m_prj->set      (env->proj_);
//   tex->set        (0);
//   ref_in->set     (0.75f);
//   eye_pos->set    (env->cam_.pos());
//   light_pos->set  (vec3(2.0f, 2.0f, 2.0f));
//   light_color->set(vec4(1.0f, 1.0f, 1.0f, 1.0f));
//   k_a->set        (0.0f);
//   k_d->set        (0.1f);
//   k_s->set        (0.9f);
//   spec_power->set (27.0f);
//   near_plane->set (env->fake_cam_.near_plane());
//   far_plane->set  (env->fake_cam_.far_plane());

   float va_radians = (env->fake_cam_.view_angle() / 360.0f) * pi<float>();

   float cos_a2 = cos(va_radians);
   float sin_a2 = 1.0f - cos_a2 * cos_a2;

   float top_near_edge = env->fake_cam_.near_plane() * sin_a2 / cos_a2;

   vec3 top    = env->fake_cam_.near_plane() * env->cam_.dir() + top_near_edge * env->cam_.up();
   vec3 bottom = env->fake_cam_.near_plane() * env->cam_.dir() - top_near_edge * env->cam_.up();

   vec3 right = ((float)env->width_ / env->height_) * top_near_edge * env->cam_.right();

   vec3 v1 = bottom - right;
   vec3 v2 = top    - right;
   vec3 v3 = top    + right;
   vec3 v4 = bottom + right;

//   f_bl->set(v1);
//   f_tl->set(v2);
//   f_br->set(v3);
//   f_tr->set(v4);

   switch(render_param_.texture_type)
   {
   case TT_none:
      break;

   case TT_2D:
      env->texture_manager_->bind_texture(texture_.get());
      break;

   case TT_cubemap:
      env->texture_manager_->bind_texture(texture_.get());

      glEnable(GL_TEXTURE_CUBE_MAP);
      glEnable(GL_TEXTURE_GEN_S   );
      glEnable(GL_TEXTURE_GEN_T   );
      glEnable(GL_TEXTURE_GEN_R   );
      break;
   }

   if (render_param_.with_alpha)
   {
      glEnable   (GL_BLEND);
      glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      if (render_param_.is_wireframe)
      {
         glEnable(GL_CULL_FACE);

         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

         if (render_param_.with_idx_buffer)
         {
            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawElements(render_param_.drawing_style, idx_buffers_[i].size(), GL_UNSIGNED_INT
                              ,&idx_buffers_[i][0]);

               disable_vertex_attrib();
            }
         }
         else
         {
            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawArrays(render_param_.drawing_style, 0, buffers_[i]->size());

               disable_vertex_attrib();
            }
         }
      }
      else
      {
         glEnable(GL_CULL_FACE);

         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

         if (render_param_.with_idx_buffer)
         {
            glCullFace(GL_FRONT);

            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawElements(render_param_.drawing_style, idx_buffers_[i].size(), GL_UNSIGNED_INT
                              ,&idx_buffers_[i][0]);

               disable_vertex_attrib();
            }

            glCullFace(GL_BACK);

            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawElements(render_param_.drawing_style, idx_buffers_[i].size(), GL_UNSIGNED_INT
                              ,&idx_buffers_[i][0]);

               disable_vertex_attrib();
            }
         }
         else
         {
            glCullFace(GL_FRONT);

            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawArrays(render_param_.drawing_style, 0, buffers_[i]->size());

               disable_vertex_attrib();
            }

            glCullFace(GL_BACK);

            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawArrays(render_param_.drawing_style, 0, buffers_[i]->size());

               disable_vertex_attrib();
            }
         }
      }

      glDisable(GL_CULL_FACE);
      glDisable(GL_BLEND    );
   }
   else
   {
      if (render_param_.is_wireframe)
      {
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

         if (render_param_.with_idx_buffer)
         {
            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawElements(render_param_.drawing_style, idx_buffers_[i].size(), GL_UNSIGNED_INT
                              ,&idx_buffers_[i][0]);

               disable_vertex_attrib();
            }
         }
         else
         {
            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawArrays(render_param_.drawing_style, 0, buffers_[i]->size());

               disable_vertex_attrib();
            }
         }
      }
      else
      {
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

         if (render_param_.with_idx_buffer)
         {
            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawElements(render_param_.drawing_style, idx_buffers_[i].size(), GL_UNSIGNED_INT
                              ,&idx_buffers_[i][0]);

               disable_vertex_attrib();
            }
         }
         else
         {
            for (size_t i = 0; i < buffers_.size(); ++i)
            {
               env->buffer_manager_->bind_buffer(buffers_[i].get());

               enable_vertex_attrib();

               glDrawArrays(render_param_.drawing_style, 0, buffers_[i]->size());

               disable_vertex_attrib();
            }
         }
      }
   }

   switch(render_param_.texture_type)
   {
   case TT_none:
      break;

   case TT_2D:
      env->texture_manager_->bind_texture();
      break;

   case TT_cubemap:
      glDisable(GL_TEXTURE_GEN_S   );
      glDisable(GL_TEXTURE_GEN_T   );
      glDisable(GL_TEXTURE_GEN_R   );
      glDisable(GL_TEXTURE_CUBE_MAP);

      env->texture_manager_->bind_texture();
      break;
   }

   env->buffer_manager_->bind_buffer    ();
   env->material_manager_->bind_material();
}

GLenum object_t::get_draw_style() const
{
   return drawing_style_;
}

render_param_t object_t::get_render_param() const
{
   return render_param_;
}

void object_t::set_wireframe( bool is_wireframe )
{
   render_param_.is_wireframe = is_wireframe;
}

void object_t::set_effect( effect_t const & effect )
{
   material_  = effect.get<0>();
   texture_   = effect.get<1>();
   effect_id_ = effect.get<2>();
}

void object_t::set_color( vec4 const & color )
{
   color_ = color;
}

vec4 object_t::color() const
{
   return color_;
}

void object_t::is_draw( bool _is_draw )
{
   need_draw_ = _is_draw;
}

bool object_t::is_draw()
{
   return need_draw_;
}

vec3 object_t::pos() const
{
   return pos_;
}

float object_t::size() const
{
   return size_;
}

void object_t::set_failed_plane_idx( size_t idx )
{
   last_failed_idx_ = idx;
}

size_t object_t::failed_plane_idx() const
{
   return last_failed_idx_;
}
