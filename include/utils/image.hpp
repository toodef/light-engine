#include <memory>
#include <string>


namespace LE {
   class image_t;
   typedef std::shared_ptr<image_t> image_ptr_t;


   class image_t {
   public:
      image_t(std::string const& path);
      image_t(std::shared_ptr<unsigned char> const & data, size_t width, size_t height, size_t channels_num);
      ~image_t();

      const std::shared_ptr<unsigned char> data() const;
      size_t width() const;
      size_t height() const;
      size_t channels_num() const;

      static image_ptr_t generate_chess(size_t x_cells_num, size_t y_cells_num);

   private:
      std::shared_ptr<unsigned char> data_ = nullptr;
      int width_ = 0, height_ = 0, channels_num_ = 0;
   };
}