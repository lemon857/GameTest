#include "EngineCore/System/ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "EngineCore/Resources/stb_image.h"

unsigned char* load_image_png(const char* fullpath, int* width, int* height, int* channels, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    return stbi_load(fullpath, width, height, channels, 0);
}

void clear_image(unsigned char* image_data)
{
    stbi_image_free(image_data);
}
