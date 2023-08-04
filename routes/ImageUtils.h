#pragma once

#include <webp/encode.h>
#include <string>

enum class EImageContentType 
{
  Jpeg,
  Png,
  Webp
};

void WebPSave(unsigned char* pData, int w, int h, const std::string& path);