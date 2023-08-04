#include "ImageUtils.h"

void WebPSave(unsigned char* pData, int w, int h, const std::string& path) 
{
	  unsigned char* pOutSmallWebp = nullptr;
    size_t original_webp_size = WebPEncodeRGB(pData, w, h, w * 3, 100, &pOutSmallWebp);
    // Save the original WebP data to a file
    FILE* webp_file = fopen(path.c_str(), "wb");
    fwrite(pOutSmallWebp, 1, original_webp_size, webp_file);
    fclose(webp_file);
    free(pOutSmallWebp);
}