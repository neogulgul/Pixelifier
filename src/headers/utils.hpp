#pragma once

#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

namespace utils
{
	const float PI = std::numbers::pi;

	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		UpLeft,
		UpRight,
		DownLeft,
		DownRight,
		Null = -1
	};

	enum class Alignment { Start, Center, End };

	struct Press
	{
		bool pressed_this_frame = false;
		bool pressed_last_frame = false;
	};

	void toggle(bool &boolean)
	{
		boolean = !boolean;
	}

	std::string bool_to_string(bool boolean)
	{
		return boolean ? "True" : "False";
	}

	int random(int min, int max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device{}());
		return std::uniform_int_distribution<int>(min, max)(rng);
	}

	float deg_to_rad(float deg)
	{
		return deg * (PI / 180);
	}

	float rad_to_deg(float rad)
	{
		return rad * (180 / PI);
	}

	float vector_distance(sf::Vector2f v)
	{
		return std::sqrt(
			std::pow(v.x, 2)
			+
			std::pow(v.y, 2)
		);
	}

	bool pressing(sf::Keyboard::Key key)
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	bool pressing(sf::Mouse::Button button)
	{
		return sf::Mouse::isButtonPressed(button);
	}

	void handle_press(Press* press, bool pressing)
	{
		press->pressed_this_frame = false;

		if (pressing)
		{
			if (!press->pressed_last_frame)
			{
				press->pressed_last_frame = true;
				press->pressed_this_frame = true;
			}
		}
		else
		{
			press->pressed_last_frame = false;
		}
	}

	std::string string_replace_substring(std::string string, std::string substring, std::string replacement = "")
	{
		string.replace(string.find(substring), substring.size(), replacement);
		return string;
	}

	std::vector<std::string> string_split(std::string string, char split_char)
	{
		std::vector<std::string> v;
		std::string split_string;

		for (int i = 0; i < string.size(); i++)
		{
			char c = string[i];
			if (c == split_char)
			{
				v.push_back(split_string);
				split_string = "";
			}
			else
			{
				split_string.push_back(c);
				if (i == string.size() - 1) v.push_back(split_string);
			}
		}

		return v;
	}

	std::string string_reverse(std::string string)
	{
		std::reverse(string.begin(), string.end());
		return string;
	}

	std::string get_current_time_string()
	{
		std::chrono::time_point now = std::chrono::system_clock::now();
		std::stringstream ss;
		ss << now.time_since_epoch();
		ss.str(utils::string_replace_substring(ss.str(), "ns"));
		return ss.str();
	}

	sf::Vector2f align(Alignment horizontal_alignment, Alignment vertical_alignment, sf::Vector2f position, sf::Vector2f size)
	{
		switch (horizontal_alignment)
		{
			case Alignment::Start:
				position.x -= 0;
				break;

			case Alignment::Center:
				position.x -= size.x / 2;
				break;

			case Alignment::End:
				position.x -= size.x;
				break;
		}

		switch (vertical_alignment)
		{
			case Alignment::Start:
				position.y -= 0;
				break;

			case Alignment::Center:
				position.y -= size.y / 2;
				break;

			case Alignment::End:
				position.y -= size.y;
				break;
		}

		return position;
	}

	int char_to_hex_int(char c)
	{
		int i = (int)c;

		// numbers
		if (i >= '0' && i <= '9') { i -= '0'; }
		// uppercase letters
		if (i >= 'A' && i <= 'Z') { i -= 'A'; i += 10; }
		// lowercase letters
		if (i >= 'a' && i <= 'z') { i -= 'a'; i += 10; }

		return i;
	}

	sf::Color hex_color(std::string hex_string)
	{
		int r = -1;
		int g = -1;
		int b = -1;
		int a = -1;

		if (hex_string[0] == '#')
		{
			// alpha not specified
			if (hex_string.size() == 7)
			{
				r = char_to_hex_int(hex_string[1]) * 16 + char_to_hex_int(hex_string[2]);
				g = char_to_hex_int(hex_string[3]) * 16 + char_to_hex_int(hex_string[4]);
				b = char_to_hex_int(hex_string[5]) * 16 + char_to_hex_int(hex_string[6]);
				a = 255;
			}
			// alpha specified
			else if (hex_string.size() == 9)
			{
				r = char_to_hex_int(hex_string[1]) * 16 + char_to_hex_int(hex_string[2]);
				g = char_to_hex_int(hex_string[3]) * 16 + char_to_hex_int(hex_string[4]);
				b = char_to_hex_int(hex_string[5]) * 16 + char_to_hex_int(hex_string[6]);
				a = char_to_hex_int(hex_string[7]) * 16 + char_to_hex_int(hex_string[8]);
			}
		}

		if (r < 0 || r > 255) { r = 0; }
		if (g < 0 || g > 255) { g = 0; }
		if (b < 0 || b > 255) { b = 0; }
		if (a < 0 || a > 255) { a = 0; }

		return sf::Color(r, g, b, a);
	}

	void set_top(sf::FloatRect* rect, float top)
	{
		rect->top = top;
	}

	void set_bottom(sf::FloatRect* rect, float bottom)
	{
		rect->top = bottom - rect->height;
	}

	void set_left(sf::FloatRect* rect, float left)
	{
		rect->left = left;
	}

	void set_right(sf::FloatRect* rect, float right)
	{
		rect->left = right - rect->width;
	}

	float get_top(sf::FloatRect rect)
	{
		return rect.top;
	}

	float get_bottom(sf::FloatRect rect)
	{
		return rect.top + rect.height;
	}

	float get_left(sf::FloatRect rect)
	{
		return rect.left;
	}

	float get_right(sf::FloatRect rect)
	{
		return rect.left + rect.width;
	}

	sf::Vector2f get_center(sf::FloatRect rect)
	{
		return {
			rect.left + rect.width  / 2,
			rect.top  + rect.height / 2
		};
	}

	template <typename T>
	void print(T thing)
	{
		std::cout << thing << "\n";
	}

	template <typename T>
	void print_vector2(sf::Vector2<T> v)
	{
		std::cout << "x: " << v.x << ", y: " << v.y << "\n";
	}

	template <typename T>
	void print_vector3(sf::Vector3<T> v)
	{
		std::cout << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << "\n";
	}
}
