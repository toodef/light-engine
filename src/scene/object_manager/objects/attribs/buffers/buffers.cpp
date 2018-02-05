#include "../../../../../base.h"

using namespace SE;

////////////////////////////////////////
/// buffer functions
////////////////////////////////////////

buffer_t::buffer_t( buffer_manager_ptr_t const & manager ) :
     manager_      (manager)
   , id_           (0)
{
   glGenBuffers(1, &id_);

   if (!id_)
      cerr << "Can't generate array buffer!" << endl;
}

buffer_t::~buffer_t()
{
   glDeleteBuffers(1, &id_);
}

void buffer_t::data( vertices_t data, GLenum usage_type )
{
   if (vertices_.size())
      vertices_.clear();

   vertices_ = data;

   glBindBuffer(GL_ARRAY_BUFFER, id_);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * vertices_.size(), &vertices_[0], usage_type);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void * buffer_t::map( GLenum access_type )
{
   void * data;

   if ((data = glMapBuffer(GL_ARRAY_BUFFER, access_type)) == 0)
      cerr << "Can't get parameters location in VRAM!" << endl;

   return data;
}

void buffer_t::unmap()
{
   if (!glUnmapBuffer(GL_ARRAY_BUFFER))
      cerr << "Can't unmap buffer!" << endl;
}

vertices_t buffer_t::vertices() const
{
   return vertices_;
}

GLuint buffer_t::id() const
{
   return id_;
}

size_t buffer_t::size() const
{
   return vertices_.size();
}

////////////////////////////////////////
/// vertex buffer manager functions
////////////////////////////////////////

buffer_manager_t::buffer_manager_t() :
   bound_buffer_(NULL)
{
}

buffer_manager_t::~buffer_manager_t()
{
   bound_buffer_ = NULL;
}

buffer_ptr_t buffer_manager_t::create_buffer( vertices_t data, GLenum usage_type )
{
   buffer_ptr_t new_buffer = std::make_shared<buffer_t>(shared_from_this());

   new_buffer->data(data, usage_type);

   return new_buffer;
}

void buffer_manager_t::bind_buffer( buffer_t * buffer, GLenum buffer_target )
{
   if (buffer)
      switch (buffer_target)
      {
      case GL_ARRAY_BUFFER:
         glBindBuffer(GL_ARRAY_BUFFER, buffer->id());
         break;

      case GL_TRANSFORM_FEEDBACK_BUFFER:
         glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer->id());
         break;
      }
   else
      glBindBuffer(buffer_target, 0);
}

buffer_t * buffer_manager_t::bound_buffer() const
{
   return bound_buffer_;
}

void buffer_manager_t::set_tf_varyings( material_ptr_t const & material, buffer_ptr_t const & tf_buffer
                                       ,const char ** names, GLsizei count )
{
   glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tf_buffer->id());

   glTransformFeedbackVaryings(material->gl_id(), count, names, GL_INTERLEAVED_ATTRIBS);

   glLinkProgram(material->gl_id());

   GLint linked;
   glGetObjectParameterivARB(material->gl_id(), GL_LINK_STATUS, &linked);

   if (!linked)
   {
      char ch[1000];
      glGetProgramInfoLog(material->gl_id(), 1000, NULL, ch);
   }

   glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
}

void * buffer_manager_t::calculate_tf_data( const buffer_ptr_t & bounding_box_buffer
                                           ,const buffer_ptr_t & tf_buffer )
{
   GLuint query, result;

   glEnable(GL_RASTERIZER_DISCARD);

   glGenQueries(1, &query);
   glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

   glBeginTransformFeedback(GL_POINTS);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(3);
   glEnableVertexAttribArray(6);
   glEnableVertexAttribArray(9);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0); // vertex
   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid *)(sizeof(vec3))); // normal
   glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t)
                         ,(GLvoid *)(sizeof(vec3) * 2)); // texture coordinates
   glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t)
                         ,(GLvoid *)(sizeof(vec3) * 3)); // color

   glDrawArrays(GL_POINTS, 0, bounding_box_buffer->size() * sizeof(vertex_t));

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(3);
   glDisableVertexAttribArray(6);
   glDisableVertexAttribArray(9);

   glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
   glGetQueryObjectuiv(query, GL_QUERY_RESULT, &result);

   glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER             , 0);

   glEndTransformFeedback();

   glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tf_buffer->id());

   void * data = glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_WRITE_ONLY);

   glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);

   glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glDisable(GL_RASTERIZER_DISCARD);

   return data;
}
