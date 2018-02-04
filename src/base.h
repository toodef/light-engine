#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include <boost/chrono.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace chrono     = boost::chrono;
namespace accums     = boost::accumulators;
namespace filesystem = boost::filesystem;

typedef chrono::system_clock::time_point::duration duration_t;
typedef accums::accumulator_set<float, accums::stats<accums::tag::rolling_mean>> average_t;

typedef boost::shared_ptr<int>   int_ptr_t  ;
typedef boost::shared_ptr<char>  char_ptr_t ;
typedef boost::shared_ptr<float> float_ptr_t;

using boost::scoped_ptr;
using boost::scoped_array;
using boost::shared_ptr;
using boost::weak_ptr;
using boost::intrusive_ptr;

// intrusive_ptr adapters
namespace intrusive_ptr_details
{
   struct intrusive_counter
   {
      intrusive_counter() : count( 0 ) {}
      intrusive_counter( intrusive_counter const & ) : count( 0 ) {}
      intrusive_counter & operator=( intrusive_counter const & ) { return *this; }
      size_t count;
   };
}

// AF5 bookmark
#define DECLARE_INTRUSIVE_COUNTER(name)                                                                            \
   mutable intrusive_ptr_details::intrusive_counter _ptr_count_;                                                   \
   __inline friend void intrusive_ptr_add_ref( name const* p ) { ++p->_ptr_count_.count; }                    \
   __inline friend void intrusive_ptr_release( name const* p ) { if( --p->_ptr_count_.count == 0 ) delete p; }

#include <GL/glew.h>

#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/constants.hpp>

#define GLM_GTC_quaternion
#include <glm/gtc/quaternion.hpp>

typedef glm::tquat<float, glm::mediump> tquat_f;

#include <AntTweakBar.h>

#include <GraphicsMagick/Magick++/Include.h>
#include <GraphicsMagick/Magick++/Image.h>
#include <GraphicsMagick/Magick++/Pixels.h>
#include <GraphicsMagick/Magick++/STL.h>

using std::string;
using std::map;
using std::vector;
using std::list;

using std::auto_ptr;

using std::cerr;
using std::cout;
using std::endl;

namespace environment
{
   struct env_t;

   typedef shared_ptr<env_t> env_ptr_t;
}

typedef shared_ptr<GLint>  int_ptr_t ;
typedef shared_ptr<GLchar> char_ptr_t;

#include "debugging/console/console.h"
#include "scene/object_manager/objects/attribs/textures/textures.h"
#include "scene/object_manager/objects/attribs/materials/materials.h"
#include "scene/object_manager/objects/attribs/buffers/buffers.h"
#include "scene/object_manager/objects/algorithms/lsvco/acmr.h"
#include "scene/object_manager/objects/algorithms/lsvco/ls_vcache_opt.h"
#include "scene/render/render.h"
#include "scene/object_manager/objects/object.h"
#include "scene/object_manager/object_manager.h"
#include "environment/environment.h"
#include "scene/scene.h"
#include "engine.h"
