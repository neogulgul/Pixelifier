#pragma once

#include "utils.hpp"

struct Text
{
	sf::RenderWindow* window;
	sf::Text text;
	sf::Font font;

	float separation;

	sf::Color default_color;
	sf::Vector2f default_scale;

	Text() {}

	void init(sf::RenderWindow* window, std::string path, unsigned int size, float separation, bool pixelated, sf::Color default_color, sf::Vector2f default_scale)
	{
		font.loadFromFile(path);

		if (pixelated) font.setSmooth(false);

		text.setFont(font);
		text.setLineSpacing((size * 0.5 + separation) / 13.f);
		text.setCharacterSize(size);
		text.setOrigin(0, text.getCharacterSize() / 2);

		this->window = window;
		this->separation = separation;
		this->default_color = default_color;
		this->default_scale = default_scale;
	}

	float get_line_spacing(float y_scale = 1.0)
	{
		return font.getLineSpacing(text.getCharacterSize()) * text.getLineSpacing() * default_scale.y * y_scale;
	}

	void draw(std::string string, utils::Alignment horizontal_alignment, utils::Alignment vertical_alignment, sf::Vector2f position, sf::Color color, sf::Vector2f scale = {1, 1})
	{
		text.setString(string);
		text.setScale(default_scale.x * scale.x, default_scale.y * scale.y);
		sf::FloatRect bounds = text.getGlobalBounds();
		position = align(horizontal_alignment, vertical_alignment, position, {bounds.width, bounds.height});

		text.setPosition(position - sf::Vector2f(1, 1));
		text.setFillColor(sf::Color::Black);
		window->draw(text);

		text.setPosition(position);
		text.setFillColor(color);
		window->draw(text);
	}

	void draw(std::string string, utils::Alignment horizontal_alignment, utils::Alignment vertical_alignment, sf::Vector2f position, sf::Vector2f scale)
	{
		draw(string, horizontal_alignment, vertical_alignment, position, default_color, scale);
	}

	void draw(std::string string, utils::Alignment horizontal_alignment, utils::Alignment vertical_alignment, sf::Vector2f position)
	{
		draw(string, horizontal_alignment, vertical_alignment, position, default_color);
	}

	void draw_with_bounds(std::string string, sf::FloatRect bounds, sf::Color color, sf::Vector2f scale = { 1, 1 })
	{
		// note for self: does not handle cases where one line splits in to more than two lines.
		text.setFillColor(color);
		text.setPosition(bounds.left, bounds.top);
		text.setScale(default_scale.x * scale.x, default_scale.y * scale.y);

		std::vector<std::string> split_string = utils::string_split(string, ' ');

		std::string string_with_line_breaks = "";
		std::string current_line = "";

		for (int i = 0; i < split_string.size(); i++)
		{
			std::string s = split_string[i] + ' ';

			text.setString(current_line + s);
			if (text.getGlobalBounds().width > bounds.width)
			{
				string_with_line_breaks += current_line + '\n';
				current_line = s;

				text.setString(current_line);
				if (text.getGlobalBounds().width > bounds.width)
				{
					std::string characters_over_the_line = "";
					while (text.getGlobalBounds().width > bounds.width)
					{
						characters_over_the_line += current_line.back();
						current_line.pop_back();
						text.setString(current_line);
					}
					characters_over_the_line = utils::string_reverse(characters_over_the_line);
					string_with_line_breaks += current_line + '\n';
					current_line = characters_over_the_line;
				}
			}
			else
			{
				current_line += s;
			}
		}
		if (current_line != "") string_with_line_breaks += current_line;

		text.setString(string_with_line_breaks);

		if (text.getGlobalBounds().height > bounds.height)
		{
			split_string = utils::string_split(string_with_line_breaks, '\n');
			int original_total_lines = split_string.size();
			for (int i = 0; i < original_total_lines; i++)
			{
				split_string.erase(split_string.begin());
				std::string new_string = "";
				for (int j = 0; j < split_string.size(); j++)
				{
					new_string += split_string[j];
					if (j < split_string.size() - 1) new_string += '\n';
				}
				text.setString(new_string);
				if (text.getGlobalBounds().height <= bounds.height) break;
			}
		}

		window->draw(text);
	}

	void draw_with_bounds(std::string string, sf::FloatRect bounds, sf::Vector2f scale)
	{
		draw_with_bounds(string, bounds, default_color, scale);
	}

	void draw_with_bounds(std::string string, sf::FloatRect bounds)
	{
		draw_with_bounds(string, bounds, default_color);
	}
};
