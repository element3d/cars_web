#pragma once

#include <webp/encode.h>
#include <string>

enum class EImageContentType 
{
  Jpeg,
  Png,
  Webp
};

void WebPSave(unsigned char* pData, int w, int h, int c, const std::string& path);
void DecodeAndSave(void * data, int size, std::string fullPath, EImageContentType contentType);
std::string GenRandomImageName(int size);