#include <utils/image.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace LE;

image_t::image_t(std::string const& path) {
   data_ = stbi_load(path.c_str(), &width_, &height_, &channels_num_, 0);
}

image_t::~image_t() {
   stbi_image_free(data_);
}

const unsigned char* image_t::data() const { return data_; }

size_t image_t::width() const { return width_; }
size_t image_t::height() const { return height_; }
size_t image_t::channels_num() const { return channels_num_; }
