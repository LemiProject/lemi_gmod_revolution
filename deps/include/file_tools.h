#pragma once
#include <filesystem>
#include <string>

#ifndef LAPI
#define LAPI
#endif


namespace file_tools
{
	LAPI void create_file(const std::string& path);
	LAPI bool exist(const std::string& path);
	LAPI void create_directory(const std::string& path);

	//Dont work
	//LAPI std::string& read_file(const std::string& path);
	//Dont work
	LAPI void read_file(std::string& out, const std::string& path);

	LAPI std::string get_hack_directory();
	LAPI std::filesystem::path get_hack_directory_path();
}