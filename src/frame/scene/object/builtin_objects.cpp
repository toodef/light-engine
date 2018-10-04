#include <memory>

#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

object_ptr_t builtin_objects_t::point(glm::vec3 const & position, glm::vec3 const & color) {
   buffer_ptr_t buffer = std::make_shared<buffer_t>(std::vector<glm::vec3>{position}, color);
   return std::make_shared<object_t>(buffer);
}
