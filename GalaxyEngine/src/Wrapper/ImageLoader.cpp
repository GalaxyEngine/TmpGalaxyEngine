#include "pch.h"
#include "Wrapper/ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace GALAXY {
	void Wrapper::ImageLoader::FlipVerticalOnLoad(const bool flagTrueIfShouldFlip)
	{
		stbi_set_flip_vertically_on_load(flagTrueIfShouldFlip);
	}

	void Wrapper::ImageLoader::ImageFree(void* data)
	{
		stbi_image_free(data);
	}

	void Wrapper::ImageLoader::ImageFree(const Image& image)
	{
		stbi_image_free(image.data);
	}

	Wrapper::Image Wrapper::ImageLoader::Load(char const* filename, int req_comp)
	{
		int x, y, comp;
		unsigned char* data = stbi_load(filename, &x, &y, &comp, req_comp);

		Image image;
		image.data = data;
		image.size.x = x;
		image.size.y = y;
		return image;
	}
}