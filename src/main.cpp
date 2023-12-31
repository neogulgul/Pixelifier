#include <SFML/Graphics.hpp>

#include "headers/Application.hpp"
#include "headers/colors.hpp"

#define __VERSION "1.0.3"
#define __TITLE "Pixelifier"

const std::string saves_directory = "saves";

const unsigned int min_resolution =    1;
const unsigned int max_resolution = 2048;

struct Game : Application
{
	enum class Mode { Normal, Pixelated, Pixelated_Colored };
	Mode mode = Mode::Normal;

	bool debug = false;
	utils::Press debug_press;
	sf::Keyboard::Key debug_key = sf::Keyboard::F1;

	utils::Press normal_mode_press;
	utils::Press pixelated_mode_press;
	utils::Press pixelated_colored_mode_press;
	sf::Keyboard::Key normal_mode_key            = sf::Keyboard::Num1;
	sf::Keyboard::Key pixelated_mode_key         = sf::Keyboard::Num2;
	sf::Keyboard::Key pixelated_colored_mode_key = sf::Keyboard::Num3;

	std::string image_filepath;
	sf::Image image;
	sf::Texture image_texture;
	sf::Sprite image_sprite;

	bool save_pixelated, save_pixelated_colored, save_name;
	sf::Image image_pixelated;
	sf::Image image_pixelated_colored;

	std::string colors_filepath;
	std::string colors_file_content;
	std::vector<sf::Color> colors;

	std::string resolution_string;
	sf::Vector2u resolution;

	bool print_time;

	Game(std::string image_filepath, sf::Image image,
	     std::string colors_file_content, sf::Vector2u resolution,
	     bool save_pixelated, bool save_pixelated_colored,
	     bool save_name, bool print_time);
	Game(std::string window_title, sf::Vector2u window_size, sf::Vector2f view_size, unsigned int fps_target,
	     std::string image_filepath, sf::Image image,
	     std::string colors_filepath, std::string colors_file_content,
	     std::string resolution_string, sf::Vector2u resolution,
	     bool save_pixelated, bool save_pixelated_colored,
	     bool save_name, bool print_time);
	void update();
	void draw();
	void destroy();
	void resize(unsigned int width, unsigned int height);

	void setup();
	void setup_clock_message(std::string message, sf::Clock clock);
	void setup_clock_message(unsigned int index, std::string message, sf::Clock clock);
	void load_colors();
	void create_pixelated_image();
	void create_pixelated_colored_image();
	void save_pixelated_image();
	void save_pixelated_colored_image();
	void change_mode();
	void adjust_sprite_scale();
	std::string mode_to_string();
};

Game::Game(std::string image_filepath, sf::Image image,
	       std::string colors_file_content, sf::Vector2u resolution,
	       bool save_pixelated, bool save_pixelated_colored,
	       bool save_name, bool print_time)
{
	this->image_filepath         = image_filepath;
	this->image                  = image;
	this->colors_file_content    = colors_file_content;
	this->resolution             = resolution;
	this->save_pixelated         = save_pixelated;
	this->save_pixelated_colored = save_pixelated_colored;
	this->save_name              = save_name;
	this->print_time             = print_time;

	if (this->resolution.x > image.getSize().x)
		this->resolution.x = image.getSize().x;
	if (this->resolution.y > image.getSize().y)
		this->resolution.y = image.getSize().y;

	setup();
}

Game::Game(std::string window_title, sf::Vector2u window_size, sf::Vector2f view_size, unsigned int fps_target,
           std::string image_filepath, sf::Image image,
           std::string colors_filepath, std::string colors_file_content,
           std::string resolution_string, sf::Vector2u resolution,
           bool save_pixelated, bool save_pixelated_colored,
           bool save_name, bool print_time)
{
	background = colors::background;
	text.init(&window, "assets/fonts/Poco.ttf", 10, 4, true, colors::text, sf::Vector2f(2, 2));
	screenshot_key = sf::Keyboard::F12;

	this->image_filepath         = image_filepath;
	this->image                  = image;
	this->colors_filepath        = colors_filepath;
	this->colors_file_content    = colors_file_content;
	this->resolution_string      = resolution_string;
	this->resolution             = resolution;
	this->save_pixelated         = save_pixelated;
	this->save_pixelated_colored = save_pixelated_colored;
	this->save_name              = save_name;
	this->print_time             = print_time;

	if (this->resolution.x > image.getSize().x)
		this->resolution.x = image.getSize().x;
	if (this->resolution.y > image.getSize().y)
		this->resolution.y = image.getSize().y;

	setup();

	create_window(window_title, window_size, view_size, fps_target);

	change_mode();
}

void Game::update()
{
	if (!window.hasFocus()) return;

	utils::handle_press(&debug_press, utils::pressing(debug_key));
	if (debug_press.pressed_this_frame)
		utils::toggle(debug);

	utils::handle_press(&normal_mode_press, utils::pressing(normal_mode_key));
	if (normal_mode_press.pressed_this_frame)
	{
		mode = Mode::Normal;
		change_mode();
	}
	utils::handle_press(&pixelated_mode_press, utils::pressing(pixelated_mode_key));
	if (pixelated_mode_press.pressed_this_frame)
	{
		mode = Mode::Pixelated;
		change_mode();
	}
	utils::handle_press(&pixelated_colored_mode_press, utils::pressing(pixelated_colored_mode_key));
	if (pixelated_colored_mode_press.pressed_this_frame)
	{
		mode = Mode::Pixelated_Colored;
		change_mode();
	}
}

void Game::draw()
{
	window.draw(image_sprite);

	if (debug)
	{
		text.draw("FPS: " + std::to_string(fps_current), utils::Alignment::Start , utils::Alignment::Start, { text.separation, text.separation });
		text.draw(__TITLE                              , utils::Alignment::Center, utils::Alignment::Start, { view_size.x * 0.5f, text.separation });
		text.draw("Version: " + std::string(__VERSION) , utils::Alignment::End   , utils::Alignment::Start, { view_size.x - text.separation, text.separation });

		std::stringstream ss;
		ss << "Image filepath: " << image_filepath
		   << "\n"
		   << "Colors filepath: " << colors_filepath
		   << "\n"
		   << "Colors:"
		   << "\n"
		   << "Color count: " << colors.size()
		   << "\n"
		   << "Resolution string: " << resolution_string
		   << "\n"
		   << "Pixelated resolution: { x: " << resolution.x << ", y: " << resolution.y << " }"
		   << "\n"
		   << "Source resolution: { x: " << image.getSize().x << ", y: " << image.getSize().y << " }"
		   << "\n"
		   << "Window size: { x: " << window.getSize().x << ", y: " << window.getSize().y << " }"
		   << "\n"
		   << "Mode: " + mode_to_string();

		text.draw(ss.str(), utils::Alignment::Start, utils::Alignment::Start, { text.separation, text.separation + text.get_line_spacing() });

		for (int i = 0; i < colors.size(); i++)
		{
			sf::Color color = colors[i];

			static sf::RectangleShape rect;
			static bool initialized = false;
			if (!initialized)
			{
				initialized = true;
				rect.setSize({text.get_line_spacing(), text.get_line_spacing()});
			}

			rect.setFillColor(color);
			rect.setPosition(64 + i * rect.getSize().x, text.separation + text.get_line_spacing() * 3 - 4);
			window.draw(rect);
		}
	}
}

void Game::destroy()
{
}

void Game::resize(unsigned int width, unsigned int height)
{
	change_view_size({(float)width, (float)height});
	adjust_sprite_scale();
}

void Game::setup()
{
	sf::Clock clock;

	load_colors();
	create_pixelated_image();
	create_pixelated_colored_image();
	if (save_pixelated)
		save_pixelated_image();
	if (save_pixelated_colored)
		save_pixelated_colored_image();

	if (print_time)
		setup_clock_message("(Done) Total setup time", clock);
}

void Game::setup_clock_message(std::string message, sf::Clock clock)
{
	utils::print(message + ": " + std::to_string(clock.getElapsedTime().asSeconds()) + "s");
}

void Game::setup_clock_message(unsigned int index, std::string message, sf::Clock clock)
{
	unsigned int total = 3 + save_pixelated + save_pixelated_colored;
	utils::print("(" + std::to_string(index) + "/" + std::to_string(total) + ") " + message + ": " + std::to_string(clock.getElapsedTime().asSeconds()) + "s");
}

void Game::load_colors()
{
	sf::Clock clock;

	std::vector<std::string> colors_file_content_split = utils::string_split(colors_file_content, '\n');
	for (int i = 0; i < colors_file_content_split.size(); i++)
	{
		std::string line = colors_file_content_split[i];
		bool valid_line = line.size() == 7 && line[0] == '#';
		if (!valid_line) continue;
		sf::Color color = utils::hex_color(line);
		colors.push_back(color);
	}

	if (print_time)
		setup_clock_message(1, "Time to load colors", clock);
}

void Game::create_pixelated_image()
{
	sf::Clock clock;

	image_pixelated.create(resolution.x, resolution.y);
	for (unsigned int x = 0; x < resolution.x; x++)
	{
		for (unsigned int y = 0; y < resolution.y; y++)
		{
			sf::Color pixel = image.getPixel(
				std::ceil((float)x / resolution.x * (image.getSize().x - 1)),
				std::ceil((float)y / resolution.y * (image.getSize().y - 1))
			);
			image_pixelated.setPixel(x, y, pixel);
		}
	}

	if (print_time)
		setup_clock_message(2, "Time to create Pixelated image", clock);
}

void Game::create_pixelated_colored_image()
{
	sf::Clock clock;

	auto color_diff = [](sf::Color c1, sf::Color c2) {
		return std::sqrt(
			std::pow(c1.r - c2.r, 2)
			+
			std::pow(c1.g - c2.g, 2)
			+
			std::pow(c1.b - c2.b, 2)
		);
	};

	image_pixelated_colored.create(resolution.x, resolution.y);
	for (unsigned int x = 0; x < resolution.x; x++)
	{
		for (unsigned int y = 0; y < resolution.y; y++)
		{
			sf::Color pixel = image.getPixel(
				std::ceil((float)x / resolution.x * (image.getSize().x - 1)),
				std::ceil((float)y / resolution.y * (image.getSize().y - 1))
			);

			int closest_color_diff = -1;
			sf::Color closest_color;

			for (sf::Color color : colors)
			{
				int current_color_diff = color_diff(pixel, color);
				if (current_color_diff < closest_color_diff || closest_color_diff == -1)
				{
					closest_color_diff = current_color_diff;
					closest_color = color;
				}
			}

			image_pixelated_colored.setPixel(x, y, closest_color);
		}
	}

	if (print_time)
		setup_clock_message(3, "Time to create Pixelated_Colored image", clock);
}

void Game::save_pixelated_image()
{
	sf::Clock clock;

	fs::mk_dir(saves_directory);

	if (save_name)
	{
		std::vector<std::string> image_filepath_split = utils::string_split(image_filepath, '/');
		std::string image_filename = utils::string_split(image_filepath_split[image_filepath_split.size() - 1], '.')[0];
		image_pixelated.saveToFile(saves_directory + "/" + image_filename + "_pixelated" + ".png");
	}
	else
		image_pixelated.saveToFile(saves_directory + "/" + utils::get_current_time_string() + ".png");

	if (print_time)
		setup_clock_message(4, "Time to save Pixelated image", clock);
}

void Game::save_pixelated_colored_image()
{
	sf::Clock clock;

	fs::mk_dir(saves_directory);

	if (save_name)
	{
		std::vector<std::string> image_filepath_split = utils::string_split(image_filepath, '/');
		std::string image_filename = utils::string_split(image_filepath_split[image_filepath_split.size() - 1], '.')[0];
		image_pixelated_colored.saveToFile(saves_directory + "/" + image_filename + "_pixelated_colored" + ".png");
	}
	else
		image_pixelated_colored.saveToFile(saves_directory + "/" + utils::get_current_time_string() + ".png");

	if (print_time)
		setup_clock_message(4 + save_pixelated, "Time to save Pixelated_Colored image", clock);
}

void Game::change_mode()
{
	switch (mode)
	{
		case Mode::Normal:
			image_texture.loadFromImage(image);
			break;
		case Mode::Pixelated:
			image_texture.loadFromImage(image_pixelated);
			break;
		case Mode::Pixelated_Colored:
			image_texture.loadFromImage(image_pixelated_colored);
			break;
	}

	image_sprite.setTexture(image_texture, true);
	adjust_sprite_scale();
}

void Game::adjust_sprite_scale()
{
	sf::Vector2f sprite_scale = {
		window.getSize().x / (float)image_texture.getSize().x,
		window.getSize().y / (float)image_texture.getSize().y
	};

	if (sprite_scale.x < sprite_scale.y)
		image_sprite.setScale({sprite_scale.x, sprite_scale.x});
	else
	if (sprite_scale.y < sprite_scale.x)
		image_sprite.setScale({sprite_scale.y, sprite_scale.y});
	else
		image_sprite.setScale(sprite_scale);

	image_sprite.setOrigin(image_texture.getSize().x * 0.5f, image_texture.getSize().y * 0.5f);
	image_sprite.setPosition(view_size * 0.5f);
}

std::string Game::mode_to_string()
{
	switch (mode)
	{
		case Mode::Normal:
			return "Normal";
		case Mode::Pixelated:
			return "Pixelated";
		case Mode::Pixelated_Colored:
			return "Pixelated_Colored";
		default:
			return "ERROR";
	}
}

int main(int argc, char* argv[])
{
	using namespace utils;

	std::string image_filepath = "";
	std::string colors_filepath = "";
	std::string resolution_string = "";
	sf::Vector2u resolution;

	bool valid_image            = false;
	bool valid_colors           = false;
	bool invalid_resolution     = false;
	bool invalid_args           = false;
	bool save_pixelated         = false;
	bool save_pixelated_colored = false;
	bool save_name              = false;
	bool print_time             = false;
	bool headless               = false;

	int last_arg = 0;

	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "-i")
		{
			if (image_filepath != "" || i + 1 == argc) continue;
			image_filepath = argv[i + 1];
			valid_image = fs::is_file(image_filepath);
			if (!valid_image)
				print("The specified image doesn't exist.");
			last_arg = i + 1;
		}
		else
		if (arg == "-c")
		{
			if (colors_filepath != "" || i + 1 == argc) continue;
			colors_filepath = argv[i + 1];
			valid_colors = fs::is_file(colors_filepath);
			if (!valid_colors)
				print("The specified colors doesn't exist.");
			last_arg = i + 1;
		}
		else
		if (arg == "-r")
		{
			if (resolution_string != "" || i + 1 == argc) continue;
			resolution_string = argv[i + 1];

			invalid_resolution = true;

			std::vector<std::string> split_string = utils::string_split(resolution_string, 'x');
			if (split_string.size() == 2)
			{
				resolution = {
					(unsigned)atoi(split_string[0].c_str()),
					(unsigned)atoi(split_string[1].c_str())
				};

				invalid_resolution = (resolution.x > max_resolution || resolution.x < min_resolution
				                      ||
				                      resolution.y > max_resolution || resolution.y < min_resolution);
			}
			if (invalid_resolution)
				print("The specified resolution is not valid.");
			last_arg = i + 1;
		}
		else
		if (arg == "-s")
		{
			save_pixelated = true;
			last_arg = i;
		}
		else
		if (arg == "-S")
		{
			save_pixelated_colored = true;
			last_arg = i;
		}
		else
		if (arg == "-n")
		{
			save_name = true;
			last_arg = i;
		}
		else
		if (arg == "-t")
		{
			print_time = true;
			last_arg = i;
		}
		else
		if (arg == "-headless")
		{
			headless = true;
			last_arg = i;
		}
		else
		if (i != last_arg)
		{
			invalid_args = true;
			print(arg + " is not a valid argument.");
		}
	}

	if (image_filepath == "")
		print("Image not specified.");
	if (colors_filepath == "")
		print("Colors not specified.");

	sf::Image image;
	std::string colors_file_content = fs::read_from_file(colors_filepath);

	if (valid_image && valid_colors && !invalid_resolution && !invalid_args && image.loadFromFile(image_filepath))
	{
		if (resolution_string == "")
			resolution = image.getSize();

		if (headless)
			Game game(image_filepath, image, colors_file_content, resolution, save_pixelated, save_pixelated_colored, save_name, print_time);
		else
		{
			Game game(__TITLE, image.getSize(), {(float)image.getSize().x, (float)image.getSize().y}, 60,
			          image_filepath, image,
			          colors_filepath, colors_file_content,
			          resolution_string, resolution,
			          save_pixelated, save_pixelated_colored,
			          save_name, print_time);
			game.run();
		}
	}

	return 0;
}
