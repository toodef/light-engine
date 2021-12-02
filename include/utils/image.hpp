#include <memory>
#include <string>


namespace LE {
   class image_t {
   public:
      image_t(std::string const& path);
      ~image_t();

      const unsigned char* data() const;
      size_t width() const;
      size_t height() const;
      size_t channels_num() const;

   private:
      unsigned char* data_ = nullptr;
      int width_ = 0, height_ = 0, channels_num_ = 0;
   };

   typedef std::shared_ptr<image_t> image_ptr_t;
}