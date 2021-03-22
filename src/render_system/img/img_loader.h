#pragma once
#include <string>
#include <vector>

enum e_img_format
{
	rgb,
	rgba
};

class c_image
{
private:
	uint32_t width, height;
	std::vector<char> data;
	e_img_format format;

public:

	c_image() = default;
	c_image(uint32_t w, uint32_t h, std::vector<char> _data, e_img_format format);
	~c_image();

	[[nodiscard]] e_img_format get_format() const { return format; }
	void get_size(int& w, int& h) const { w = width; h = height; }
	[[nodiscard]] const std::vector<char>& get_data() const { return data; }
	void get_data(std::vector<char>& _data) const { _data = this->data; }

	void resize(int w, int h);
};



namespace img_loader
{
	c_image load_from_file(const std::string& path, bool is_flip = false);
	void load_from_file(c_image& out_img, const std::string& path, bool is_flip = false);
	c_image load_from_memory(unsigned char const* data, int size, bool is_flip = false);
}