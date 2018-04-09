#include "../../../base.h"

using namespace SE;

////////////////////////////////////////
/// object_t class functions
////////////////////////////////////////

object_t::object_t( effect_t const & effect, buffers_t const & buffers
                   ,render_param_t const & render_param, vec3 pos
                   ,vec4 color, float size
                   ,idx_buffers_t const & idx_buffers ):
     material_          (effect.material)
   , texture_           (effect.texture)
   , effect_id_         (effect.id)
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
   if (material_->is_init_)
      env->material_manager_->bind_material(material_.get());

   uniform_ptr_t m_mvp = material_->uniform("m_mvp");
   m_mvp->set      (&env->mvp_);

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
   material_  = effect.material;
   texture_   = effect.texture;
   effect_id_ = effect.id;
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
