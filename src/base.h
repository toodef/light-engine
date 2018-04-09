#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <memory>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/constants.hpp>

#define GLM_GTC_quaternion
#include <glm/gtc/quaternion.hpp>

typedef glm::tquat<float, glm::mediump> tquat_f;

using std::string;
using std::map;
using std::vector;

using std::cerr;
using std::cout;
using std::endl;

namespace environment
{
   struct env_t;

   typedef std::shared_ptr<env_t> env_ptr_t;
}

#include "scene/object_manager/objects/attribs/textures/textures.h"
#include "scene/object_manager/objects/attribs/materials/materials.h"
#include "scene/object_manager/objects/attribs/buffers/buffers.h"
#include "scene/render/render.h"
#include "scene/object_manager/objects/object.h"
#include "scene/object_manager/object_manager.h"
#include "environment/environment.h"
#include "scene/scene.h"
#include "engine.h"
