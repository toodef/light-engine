#include <filesystem>

#include "../../base.h"

using namespace obj_manager;

////////////////////////////////////////
/// object_managert_t class functions
////////////////////////////////////////

obj_ptr_t obj_manager_t::create_triangle( effect_t const & effect, buffer_manager_ptr_t const & vbo_manager
                                               ,vec4 color ,vec3 pos1, vec3 pos2, vec3 pos3, vec3 pos )
{
   vertices_t new_triangle = vertices_t(3);

   vec3 norm = normalize((pos2 - pos1) * (pos3 - pos1));

   new_triangle[0].vertex_    = pos1;
   new_triangle[0].normal_    = norm;
   new_triangle[0].tex_coord_ = vec3();
   new_triangle[0].color_     = color;

   new_triangle[1].vertex_    = pos2;
   new_triangle[1].normal_    = norm;
   new_triangle[1].tex_coord_ = vec3();
   new_triangle[1].color_     = color;

   new_triangle[2].vertex_    = pos3;
   new_triangle[2].normal_    = norm;
   new_triangle[2].tex_coord_ = vec3();
   new_triangle[2].color_     = color;

   if (pos != vec3())
   {
      new_triangle[0].vertex_ += pos;
      new_triangle[1].vertex_ += pos;
      new_triangle[2].vertex_ += pos;
   }

   render_param_t render_par;

   if (color.w < 1.0f)
      render_par.with_alpha = 1;
   else
      render_par.with_alpha = 0;

   render_par.with_idx_buffer = 0;
   render_par.drawing_style   = DS_triangles;
   render_par.texture_type    = TT_none;

   buffer_ptr_t buffer = vbo_manager->create_buffer(new_triangle);

   buffers_t buffers;

   buffers.push_back(buffer);

   return obj_ptr_t(new object_t(effect, buffers, render_par, pos, color));
}

obj_ptr_t obj_manager_t::create_quad( effect_t const & effect
                                           ,buffer_manager_ptr_t const & vbo_manager
                                           ,render_param_t & render_param, vec4 color
                                           ,vec3 pos1, vec3 pos2, vec3 pos3, vec3 pos4, vec3 pos, float size )
{
   vertices_t new_quad = vertices_t(4);

   vec3 norm = normalize((pos2 - pos1) * (pos3 - pos1));

   new_quad[0].vertex_    = pos1 * size;
   new_quad[0].normal_    = norm;
   new_quad[0].tex_coord_ = vec3(0, 1, 0);
   new_quad[0].color_     = color;

   new_quad[1].vertex_    = pos2 * size;
   new_quad[1].normal_    = norm;
   new_quad[1].tex_coord_ = vec3(1, 1, 0);
   new_quad[1].color_     = color;

   new_quad[2].vertex_    = pos3 * size;
   new_quad[2].normal_    = norm;
   new_quad[2].tex_coord_ = vec3(1, 0, 0);
   new_quad[2].color_     = color;

   new_quad[3].vertex_    = pos4 * size;
   new_quad[3].normal_    = norm;
   new_quad[3].tex_coord_ = vec3(0, 0, 0);
   new_quad[3].color_     = color;

   if (pos != vec3())
   {
      new_quad[0].vertex_ += pos;
      new_quad[1].vertex_ += pos;
      new_quad[2].vertex_ += pos;
      new_quad[3].vertex_ += pos;
   }

   if (color.w < 1.0f)
      render_param.with_alpha = 1;
   else
      render_param.with_alpha = 0;

   buffer_ptr_t buffer = vbo_manager->create_buffer(new_quad);

   buffers_t buffers;

   buffers.push_back(buffer);

   return obj_ptr_t(new object_t(effect, buffers, render_param, pos, color, size));
}

obj_ptr_t obj_manager_t::create_plane( effect_t const & effect
                                            ,buffer_manager_ptr_t const & vbo_manager
                                            ,float size_x, float size_z, int frequency, vec4 color )
{
   vertices_t new_grid;

   int half_frequency = (frequency + 1) / 2;
   float tmp_i;

   vertex_t v;

   for (int i = 0; i <= frequency; ++i)
   {
      tmp_i = (float)i / half_frequency - 1.0f;

      v.vertex_ = vec3(-size_x, 0, size_z * tmp_i);
      v.color_  = color;
      new_grid.push_back(v);

      v.vertex_ = vec3(size_x, 0, size_z * tmp_i);
      v.color_  = color;
      new_grid.push_back(v);

      v.vertex_ = vec3(size_x * tmp_i, 0, -size_z);
      v.color_  = color;
      new_grid.push_back(v);

      v.vertex_ = vec3(size_x * tmp_i, 0, size_z);
      v.color_  = color;
      new_grid.push_back(v);
   }

   render_param_t render_par;

   if (color.w < 1.0f)
      render_par.with_alpha = 1;
   else
      render_par.with_alpha = 0;

   render_par.is_wireframe    = 1;
   render_par.with_idx_buffer = 0;
   render_par.drawing_style   = DS_lines;
   render_par.texture_type    = TT_none;

   buffer_ptr_t buffer = vbo_manager->create_buffer(new_grid);

   buffers_t buffers;

   buffers.push_back(buffer);

   return obj_ptr_t(new object_t(effect, buffers, render_par));
}

obj_ptr_t obj_manager_t::create_sky_box( effect_t const & effect
                                        ,buffer_manager_ptr_t const & buffer_manager, float size )
{
   vertices_t new_sky_box = vertices_t(24);

   vec3 x = vec3(size, 0   , 0   );
   vec3 y = vec3(0   , size, 0   );
   vec3 z = vec3(0   , 0   , size);

   vector<vec3> box;

   box.push_back(-x + y + z);
   box.push_back( x + y + z);
   box.push_back(-x - y + z);
   box.push_back( x - y + z);

   box.push_back(-x + y - z);
   box.push_back( x + y - z);
   box.push_back(-x - y - z);
   box.push_back( x - y - z);

   new_sky_box[0].vertex_  = new_sky_box[0].tex_coord_  = box[1];
   new_sky_box[1].vertex_  = new_sky_box[1].tex_coord_  = box[3];
   new_sky_box[2].vertex_  = new_sky_box[2].tex_coord_  = box[0];
   new_sky_box[3].vertex_  = new_sky_box[3].tex_coord_  = box[2];
   new_sky_box[4].vertex_  = new_sky_box[4].tex_coord_  = box[6];
   new_sky_box[5].vertex_  = new_sky_box[5].tex_coord_  = box[3];
   new_sky_box[6].vertex_  = new_sky_box[6].tex_coord_  = box[7];
   new_sky_box[7].vertex_  = new_sky_box[7].tex_coord_  = box[5];
   new_sky_box[8].vertex_  = new_sky_box[8].tex_coord_  = box[6];
   new_sky_box[9].vertex_  = new_sky_box[9].tex_coord_  = box[4];
   new_sky_box[10].vertex_ = new_sky_box[10].tex_coord_ = box[0];
   new_sky_box[11].vertex_ = new_sky_box[11].tex_coord_ = box[5];
   new_sky_box[12].vertex_ = new_sky_box[12].tex_coord_ = box[1];
   new_sky_box[13].vertex_ = new_sky_box[13].tex_coord_ = box[3];

   for (int i = 0; i < 14; ++i)
      new_sky_box[i].tex_coord_ = normalize(new_sky_box[i].tex_coord_);

   render_param_t render_par;

   render_par.is_wireframe    = false;
   render_par.with_idx_buffer = false;
   render_par.drawing_style   = DS_triangle_strip;
   render_par.texture_type    = TT_cubemap;

   buffer_ptr_t buffer = buffer_manager->create_buffer(new_sky_box);

   buffers_t buffers;

   buffers.push_back(buffer);

   return obj_ptr_t(new object_t(effect, buffers, render_par, vec3(), vec4(), size));
}

obj_ptr_t obj_manager_t::create_line( effect_t const & effect
                                           ,buffer_manager_ptr_t const & vbo_manager, vec4 color
                                           ,vec3 from, vec3 to )
{
   vertices_t new_line = vertices_t(2);

   new_line[0].vertex_ = from;
   new_line[0].color_  = color;

   new_line[1].vertex_ = to;
   new_line[1].color_  = color;

   render_param_t render_par;

   if (color.w < 1.0f)
      render_par.with_alpha = 1;
   else
      render_par.with_alpha = 0;

   render_par.is_wireframe    = 1;
   render_par.with_idx_buffer = 0;
   render_par.drawing_style   = DS_lines;
   render_par.texture_type    = TT_none;

   buffer_ptr_t buffer = vbo_manager->create_buffer(new_line);

   buffers_t buffers;

   buffers.push_back(buffer);

   return obj_ptr_t(new object_t(effect, buffers, render_par, vec3(), color));
}

obj_ptr_t obj_manager_t::create_sphere( effect_t const & effect
                                       ,buffer_manager_ptr_t const & buffer_manager
                                       ,vec4 color, size_t detail_level, vec3 pos
                                       ,float size, texture_type_t texture_type )
{
   vector<size_t> end_index_data({ 0 , 4 , 1 ,
                                   0 , 9 , 4 ,
                                   9 , 5 , 4 ,
                                   4 , 5 , 8 ,
                                   4 , 8 , 1 ,
                                   8 , 10, 1 ,
                                   8 , 3 , 10,
                                   5 , 3 , 8 ,
                                   5 , 2 , 3 ,
                                   2 , 7 , 3 ,
                                   7 , 10, 3 ,
                                   7 , 6 , 10,
                                   7 , 11, 6 ,
                                   11, 0 , 6 ,
                                   0 , 1 , 6 ,
                                   6 , 1 , 10,
                                   9 , 0 , 11,
                                   9 , 11, 2 ,
                                   9 , 2 , 5 ,
                                   7 , 2 , 11 });

   const float x = 0.525731112119133606f;
   const float z = 0.850650808352039932f;

   vertex_t v0 ;
   vertex_t v1 ;
   vertex_t v2 ;
   vertex_t v3 ;
   vertex_t v4 ;
   vertex_t v5 ;
   vertex_t v6 ;
   vertex_t v7 ;
   vertex_t v8 ;
   vertex_t v9 ;
   vertex_t v10;
   vertex_t v11;

   v0.vertex_  = normalize(vec3(-x,  0,  z)) * size;
   v1.vertex_  = normalize(vec3( x,  0,  z)) * size;
   v2.vertex_  = normalize(vec3(-x,  0, -z)) * size;
   v3.vertex_  = normalize(vec3( x,  0, -z)) * size;
   v4.vertex_  = normalize(vec3( 0,  z,  x)) * size;
   v5.vertex_  = normalize(vec3( 0,  z, -x)) * size;
   v6.vertex_  = normalize(vec3( 0, -z,  x)) * size;
   v7.vertex_  = normalize(vec3( 0, -z, -x)) * size;
   v8.vertex_  = normalize(vec3( z,  x,  0)) * size;
   v9.vertex_  = normalize(vec3(-z,  x,  0)) * size;
   v10.vertex_ = normalize(vec3( z, -x,  0)) * size;
   v11.vertex_ = normalize(vec3(-z, -x,  0)) * size;

   vertices_t new_sphere;
   new_sphere.push_back(v0 );
   new_sphere.push_back(v1 );
   new_sphere.push_back(v2 );
   new_sphere.push_back(v3 );
   new_sphere.push_back(v4 );
   new_sphere.push_back(v5 );
   new_sphere.push_back(v6 );
   new_sphere.push_back(v7 );
   new_sphere.push_back(v8 );
   new_sphere.push_back(v9 );
   new_sphere.push_back(v10);
   new_sphere.push_back(v11);

   vector<size_t> index_data;

   for (size_t n = 0; n < detail_level - 1; ++n)
   {
      for (size_t i = 0; i < end_index_data.size() - 2; i += 3)
      {
         size_t i0 = end_index_data[i    ];
         size_t i1 = end_index_data[i + 1];
         size_t i2 = end_index_data[i + 2];

         vertex_t v_m01 = vertex_t(normalize(new_sphere[i0].vertex_ + new_sphere[i1].vertex_) * size, vec3()
                                   ,vec3(), color);
         vertex_t v_m12 = vertex_t(normalize(new_sphere[i1].vertex_ + new_sphere[i2].vertex_) * size, vec3()
                                   ,vec3(), color);
         vertex_t v_m02 = vertex_t(normalize(new_sphere[i0].vertex_ + new_sphere[i2].vertex_) * size, vec3()
                                   ,vec3(), color);

         size_t m01 = new_sphere.size();
         new_sphere.push_back(v_m01);

         size_t m12 = new_sphere.size();
         new_sphere.push_back(v_m12);

         size_t m02 = new_sphere.size();
         new_sphere.push_back(v_m02);

         index_data.push_back(i0 );
         index_data.push_back(m01);
         index_data.push_back(m02);
         index_data.push_back(i1 );
         index_data.push_back(m12);
         index_data.push_back(m01);
         index_data.push_back(i2 );
         index_data.push_back(m02);
         index_data.push_back(m12);
         index_data.push_back(m02);
         index_data.push_back(m01);
         index_data.push_back(m12);
      }

      end_index_data.clear();
      end_index_data = index_data;
      index_data.clear();
   }

   for (size_t i = 0; i < 12; ++i)
      new_sphere[i].color_ = color;

   for (size_t i = 0; i < new_sphere.size(); ++i)
   {
      new_sphere[i].normal_ =  new_sphere[i].tex_coord_ =  normalize(new_sphere[i].vertex_);
      new_sphere[i].vertex_ += pos;
   }

   render_param_t render_par;

   if (color.w < 1.0f)
      render_par.with_alpha = 1;
   else
      render_par.with_alpha = 0;

   render_par.is_wireframe    = 1;
   render_par.with_idx_buffer = 1;
   render_par.drawing_style   = DS_quads;
   render_par.texture_type    = texture_type;

   buffer_ptr_t buffer = buffer_manager->create_buffer(new_sphere);

   buffers_t buffers;

   buffers.push_back(buffer);

   idx_buffers_t idx_buffers;

   idx_buffers.push_back(end_index_data);

   return obj_ptr_t(new object_t(effect, buffers, render_par, pos, color, size, idx_buffers));
}

obj_ptr_t obj_manager_t::create_frustrum( effect_t const & effect
                                         ,buffer_manager_ptr_t const & buffer_manager
                                         ,env_ptr_t const & env, vec4 color, float size )
{
   vertices_t new_frustum = vertices_t(24);

   buffer_ptr_t buffer = buffer_manager->create_buffer(new_frustum);

   render_param_t render_par;

   render_par.with_alpha      = 1;
   render_par.is_wireframe    = 0;
   render_par.with_idx_buffer = 0;
   render_par.drawing_style   = DS_quads;
   render_par.texture_type    = TT_none;

   buffers_t buffers;

   buffers.push_back(buffer);

   return obj_ptr_t(new object_t(effect, buffers, render_par, vec3(), color, size));
}

obj_ptr_t obj_manager_t::create_box( effect_t const & effect
                                    ,buffer_manager_ptr_t const & buffer_manager
                                    ,vec4 color, vec3 pos, float size, texture_type_t texture_type )
{
   vertices_t new_box = vertices_t(14);

   vec3 x = vec3(size, 0   , 0   );
   vec3 y = vec3(0   , size, 0   );
   vec3 z = vec3(0   , 0   , size);

   vector<vec3> box;

   box.push_back(pos - x + y + z);
   box.push_back(pos + x + y + z);
   box.push_back(pos - x - y + z);
   box.push_back(pos + x - y + z);

   box.push_back(pos - x + y - z);
   box.push_back(pos + x + y - z);
   box.push_back(pos - x - y - z);
   box.push_back(pos + x - y - z);

   new_box[0].vertex_  = new_box[0].tex_coord_  = box[1];
   new_box[0].normal_  = vec3(0, 0, 1);
   new_box[1].vertex_  = new_box[1].tex_coord_  = box[3];
   new_box[1].normal_  = vec3(0, 0, 1);
   new_box[2].vertex_  = new_box[2].tex_coord_  = box[0];
   new_box[2].normal_  = vec3(0, 0, 1);
   new_box[3].vertex_  = new_box[3].tex_coord_  = box[2];
   new_box[3].normal_  = vec3(0, 0, 1);
   new_box[4].vertex_  = new_box[4].tex_coord_  = box[6];
   new_box[4].normal_  = vec3(1, 0, 0);
   new_box[5].vertex_  = new_box[5].tex_coord_  = box[3];
   new_box[5].normal_  = vec3(1, 0, 0);
   new_box[6].vertex_  = new_box[6].tex_coord_  = box[7];
   new_box[6].normal_  = vec3(1, 0, 0);
   new_box[7].vertex_  = new_box[7].tex_coord_  = box[5];
   new_box[7].normal_  = vec3(1, 0, 0);
   new_box[8].vertex_  = new_box[8].tex_coord_  = box[6];
   new_box[8].normal_  = vec3(0, 0, -1);
   new_box[9].vertex_  = new_box[9].tex_coord_  = box[4];
   new_box[9].normal_  = vec3(0, 0, -1);
   new_box[10].vertex_ = new_box[10].tex_coord_ = box[0];
   new_box[10].normal_ = vec3(0, 0, -1);
   new_box[11].vertex_ = new_box[11].tex_coord_ = box[5];
   new_box[11].normal_ = vec3(0, 0, -1);
   new_box[12].vertex_ = new_box[12].tex_coord_ = box[1];
   new_box[12].normal_ = vec3(-1, 0, 0);
   new_box[13].vertex_ = new_box[13].tex_coord_ = box[3];
   new_box[13].normal_ = vec3(-1, 0, 0);

   for (int i = 0; i < 14; ++i)
   {
      new_box[i].color_     = color;
      new_box[i].tex_coord_ = normalize(new_box[i].tex_coord_ - pos);
   }

   render_param_t render_par;

   if (color.w < 1.0f)
      render_par.with_alpha = 1;
   else
      render_par.with_alpha = 0;

   render_par.is_wireframe    = 0;
   render_par.with_idx_buffer = 0;
   render_par.drawing_style   = DS_triangle_strip;
   render_par.texture_type    = texture_type;

   buffer_ptr_t buffer = buffer_manager->create_buffer(new_box);

   buffers_t buffers;

   buffers.push_back(buffer);

   return obj_ptr_t(new object_t(effect, buffers, render_par, pos, color, size));
}

obj_ptr_t obj_manager_t::create_point( effect_t const & effect
                                      ,buffer_manager_ptr_t const & buffer_manager
                                      ,render_param_t const & render_param
                                      ,vec4 color, vec3 pos, float size )
{
   vertex_t new_point;

   new_point.vertex_    = pos;
   new_point.color_     = color;
   new_point.normal_    = vec3(0, 0, 0);
   new_point.tex_coord_ = vec3();

   vertices_t res = vertices_t(1);

   res[0] = new_point;

   buffer_ptr_t buffer = buffer_manager->create_buffer(res);

   buffers_t buffers;

   buffers.push_back(buffer);

   return obj_ptr_t(new object_t(effect, buffers, render_param, pos, color, size));
}

// load model from *.obj file
obj_ptr_t obj_manager_t::load_model( effect_t const & effect
                                    ,buffer_manager_ptr_t const & buffer_manager
                                    ,render_param_t const & render_param, const string & file_name
                                    ,vec4 color, vec3 pos, float size )
{
   // TODO add load obj file realisation

   //buffers_t     new_obj;
   //idx_buffers_t index_buffers;

   //cout << "Load """ << file_name << """ model";

   //std::experimental::filesystem::path     file(file_name);
   //std::ifstream in_file(file, std::ios::binary);

   //size_t file_size = std::experimental::filesystem::file_size(file);

   //std::vector<GLchar> text(file_size + 1);

   //in_file.read(text.data(), file_size);
   //text[file_size] = 0;

   //Assimp::Importer import;

   //char postprocess_params = 0;

   //if (render_param.drawing_style == DS_triangles)
   //   postprocess_params |= aiProcess_Triangulate | aiProcess_GenSmoothNormals;

   //if (effect.get<1>() && (render_param.texture_type == TT_none))
   //   postprocess_params |= aiProcess_FlipUVs;

   //const aiScene * obj = import.ReadFileFromMemory(text.get(), file_size, postprocess_params);

   //if (obj)
   //{
   //   vertex_t v;

   //   v.color_ = color;

   //   for (size_t i = 0; i < obj->mNumMeshes; ++i)
   //   {
   //      vertices_t mesh;

   //      for (size_t j = 0; j < obj->mMeshes[i]->mNumVertices; ++j)
   //      {
   //         v.vertex_ = vec3(obj->mMeshes[i]->mVertices[j].x, obj->mMeshes[i]->mVertices[j].y
   //            ,obj->mMeshes[i]->mVertices[j].z) * size + pos;

   //         if (obj->mMeshes[i]->HasNormals())
   //            v.normal_ = vec3(obj->mMeshes[i]->mNormals[j].x, obj->mMeshes[i]->mNormals[j].y
   //            ,obj->mMeshes[i]->mNormals[j].z);

   //         if (obj->mMeshes[i]->HasTextureCoords(0))
   //            v.tex_coord_ = vec3(obj->mMeshes[i]->mTextureCoords[0][j].x, obj->mMeshes[i]->mTextureCoords[0][j].y
   //            ,obj->mMeshes[i]->mTextureCoords[0][j].z);

   //         mesh.push_back(v);
   //      }

   //      buffer_ptr_t buffer = buffer_manager->create_buffer(mesh);

   //      new_obj.push_back(buffer);

   //      if (render_param.with_idx_buffer)
   //      {
   //         idx_buffer_t index_buffer;

   //         for (size_t j = 0; j < obj->mMeshes[i]->mNumFaces; ++j)
   //         {
   //            index_buffer.push_back(obj->mMeshes[i]->mFaces[j].mIndices[0]);
   //            index_buffer.push_back(obj->mMeshes[i]->mFaces[j].mIndices[1]);
   //            index_buffer.push_back(obj->mMeshes[i]->mFaces[j].mIndices[2]);
   //         }

   //         index_buffers.push_back(index_buffer);
   //      }
   //   }

   //   cout << " -complete" << endl;

   //   if (render_param.with_idx_buffer)
   //      return obj_ptr_t(new object_t(effect, new_obj, render_param, pos, color, size, index_buffers));

   //   return obj_ptr_t(new object_t(effect, new_obj, render_param, pos, color, size));
   //}
   //else
   //{
   //   cout << " -error!" << endl;

   //   return obj_ptr_t();
   //}

   return obj_ptr_t();
}

