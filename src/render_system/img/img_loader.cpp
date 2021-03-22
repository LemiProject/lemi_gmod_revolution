#include "img_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

c_image::c_image(const uint32_t w, const uint32_t h, std::vector<char> _data, const e_img_format format) : width(w), height(h), data(
	                                                                                                           std::move(_data)), format(format)
{
}

c_image::~c_image()
{
}

void c_image::resize(const int w, const int h)
{
	stbir_resize_uint8(reinterpret_cast<unsigned char*>(data.data()), width, height, 0, reinterpret_cast<unsigned char*>(&data[0]), w, h, 0, format == rgb ? 3 : 4);
	width = w;
	height = h;
}

c_image img_loader::load_from_file(const std::string& path, bool is_flip)
{
	stbi_set_flip_vertically_on_load(is_flip);
	int width, height, channels_num;

	const auto data = std::shared_ptr<stbi_uc>(stbi_load(path.c_str(), &width, &height, &channels_num, 0));
	if (data == nullptr)
		throw std::runtime_error("Cannot load image " + path);

	e_img_format format;

	if (channels_num == 3)
		format = rgb;
	else if (channels_num == 4)
		format = rgba;
	else
		throw std::runtime_error("Invalid channels num: " + std::to_string(channels_num)
			+ "\tin file " + path);

	const auto size = width * height * channels_num;
	return c_image(width, height, { data.get(), data.get() + size }, format);
}

void img_loader::load_from_file(c_image& out_img, const std::string& path, bool is_flip)
{
	stbi_set_flip_vertically_on_load(is_flip);
	int width, height, channels_num;

	const auto data = std::shared_ptr<stbi_uc>(stbi_load(path.c_str(), &width, &height, &channels_num, 0));
	if (data == nullptr)
		throw std::runtime_error("Cannot load image " + path);

	e_img_format format;

	if (channels_num == 3)
		format = rgb;
	else if (channels_num == 4)
		format = rgba;
	else
		throw std::runtime_error("Invalid channels num: " + std::to_string(channels_num)
			+ "\tin file " + path);

	const auto size = width * height * channels_num;
	out_img = c_image(width, height, { data.get(), data.get() + size }, format);
}

c_image img_loader::load_from_memory(unsigned char const* data, int size, bool is_flip)
{
	stbi_set_flip_vertically_on_load(is_flip);
	int width, height, channels_num;

	const auto tx = std::shared_ptr<stbi_uc>(
		stbi_load_from_memory(data, size, &width, &height, &channels_num, 0));
	if (tx == nullptr)
		throw std::runtime_error("Cannot load image ");

	e_img_format format;

	if (channels_num == 3)
		format = rgb;
	else if (channels_num == 4)
		format = rgba;
	else
		throw std::runtime_error("Invalid channels num: " + std::to_string(channels_num)
			+ "\tin file ");

	const auto size = width * height * channels_num;
	return c_image(width, height, { tx.get(), tx.get() + size }, format);
}
