#include <memory>
#include <utils/image.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace LE;

image_t::image_t(std::string const& path) {
   data_.reset(stbi_load(path.c_str(), &width_, &height_, &channels_num_, 0));
}

image_t::image_t(std::shared_ptr<unsigned char> const& data, size_t width, size_t height, size_t channels_num):
   data_(data), width_(width), height_(height), channels_num_(channels_num) {}


image_t::~image_t() {
   //stbi_image_free(data_.get());
   //data_.reset(nullptr);
}

const std::shared_ptr<unsigned char> image_t::data() const { return data_; }

size_t image_t::width() const { return width_; }
size_t image_t::height() const { return height_; }
size_t image_t::channels_num() const { return channels_num_; }

image_ptr_t image_t::generate_chess(size_t x_cells_num, size_t y_cells_num) {
   std::shared_ptr<unsigned char> data;
   data.reset(new unsigned char[x_cells_num * y_cells_num * 3]);

   for (int c = 0; c < 3; c++)
      for (int i = 0; i < y_cells_num; i++)
         for (int j = 0; j < x_cells_num; j++)
            data.get()[3 * (i * x_cells_num + j) + c] = 255 * ((i + j + 1) % 2);

   return std::make_shared<image_t>(data, x_cells_num, y_cells_num, 3);
}
