#pragma once

#include <filesystem>
#include <fstream>

namespace fs
{
	bool is_dir(std::string path)
	{
		return std::filesystem::is_directory(path);
	}

	bool is_file(std::string path)
	{
		return std::filesystem::is_regular_file(path);
	}

	void rm_dir(std::string path)
	{
		std::filesystem::remove_all(path);
	}

	void rm_file(std::string path)
	{
		std::filesystem::remove(path);
	}

	void mk_dir(std::string path)
	{
		if (is_file(path)) rm_file(path);
		if (!is_dir(path)) std::filesystem::create_directory(path);
	}

	void mk_file(std::string path)
	{
		if (is_dir(path)) rm_dir(path);
		if (!is_file(path))
		{
			std::ofstream file(path);
			file.close();
		}
	}

	void write_to_file(std::string path, std::string text)
	{
		if (!is_file(path)) mk_file(path);
		std::ofstream file(path);
		file << text;
		file.close();
	}

	std::string read_from_file(std::string path)
	{
		std::ifstream file(path);
		std::string content(
			(std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>())
		);
		return content;
	}
}
