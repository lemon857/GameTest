#pragma once

unsigned char* load_image_png(const char* fullpath, int* width, int* height, int* channels, bool flip = true);

void clear_image(unsigned char* image_data);