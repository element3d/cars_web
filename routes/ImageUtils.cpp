#include "ImageUtils.h"

void WebPSave(unsigned char* pData, int w, int h, int c, const std::string& path) 
{
	  unsigned char* pOutSmallWebp = nullptr;
    size_t original_webp_size;
    if (c == 3) original_webp_size = WebPEncodeRGB(pData, w, h, w * c, 100, &pOutSmallWebp);
    else original_webp_size = WebPEncodeRGBA(pData, w, h, w * c, 100, &pOutSmallWebp);
    // Save the original WebP data to a file
    FILE* webp_file = fopen(path.c_str(), "wb");
    fwrite(pOutSmallWebp, 1, original_webp_size, webp_file);
    fclose(webp_file);
    free(pOutSmallWebp);
}