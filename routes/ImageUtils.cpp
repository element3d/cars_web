#include "ImageUtils.h"
#include <random>
#include "webp/decode.h"
#include "../stb_image/stb_image.h"

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

void DecodeAndSave(void * data, int size, std::string fullPath, EImageContentType contentType)
{
      int w, h, c;
    unsigned char* d = nullptr;
    if (contentType == EImageContentType::Webp)
    {
      c = 3;
      d = WebPDecodeRGB((const uint8_t*)data, size, &w, &h);
    }
    else
      d = stbi_load_from_memory((unsigned char*)data, size, &w, &h, &c, 0);

    /*int nw = 400;
    int nh = int(400.f * h / w);
    unsigned char* dd = new unsigned char[nw* nh * c];
    avir :: CImageResizer<> ImageResizer( 8 );
    ImageResizer.resizeImage( d, w, h, 0, dd, nw, nh, c, 0 );*/

    WebPSave(d, w, h, c, fullPath);

    // stbi_write_jpg(fullPath.c_str(), nw, nh, c, dd, 100);
    stbi_image_free(d);
    // delete[] dd;
}

std::string GenRandomImageName(int size)
{
const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

	std::string random_string;

	for (std::size_t i = 0; i < size; ++i)
	{
		random_string += CHARACTERS[distribution(generator)];
	}

	return random_string;
}