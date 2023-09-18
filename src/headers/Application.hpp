#pragma once

#include "fs.hpp"
#include "Text.hpp"
#include "utils.hpp"

struct Application
{
	sf::RenderWindow window;
	sf::View view;

	sf::Vector2f mouse_position;
	sf::Vector2f view_size;
	sf::FloatRect viewport;

	sf::Color background;

	unsigned int fps_target, fps_current;
	float fps_update_delta = 0.1; // time, in seconds, between fps updates
	sf::Clock fps_update_clock;
	sf::Clock fps_display_clock;
	sf::Clock fps_calculate_clock;

	Text text;

	utils::Press screenshot_press;
	sf::Keyboard::Key screenshot_key = sf::Keyboard::Unknown;

	Application() {}

	Application(std::string window_title, sf::Vector2u window_size, sf::Vector2f view_size, unsigned int fps_target)
	{
		this->fps_target = fps_target;
		window.create({window_size.x, window_size.y}, window_title, sf::Style::Default);
		window.setFramerateLimit(fps_target);
		change_view_size(view_size);
	}

	void change_view_size(sf::Vector2f view_size)
	{
		this->view_size = view_size;
		view.setSize(view_size);
		view.setCenter(view_size / 2.f);
	}

	void run()
	{
		while (window.isOpen())
		{
			handle_window_events();

			window.clear(background);

			calculate_fps();

			update_mouse_position();

			update();

			update_viewport();

			window.setView(view);

			draw();

			fps_display_clock.restart();

			window.display();

			utils::handle_press(&screenshot_press, utils::pressing(screenshot_key));
			if (screenshot_press.pressed_this_frame) screenshot();
		}

		destroy();
	}

	void handle_window_events()
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::Resized:
					resize(event.size.width, event.size.height);
					break;
			}
		}
	}

	void calculate_fps()
	{
		if (fps_update_clock.getElapsedTime().asSeconds() >= fps_update_delta)
		{
			fps_update_clock.restart();
			fps_current = std::floor(1.f / (
				fps_calculate_clock.getElapsedTime().asSeconds()
				-
				fps_display_clock.getElapsedTime().asSeconds()
			));
		}
		fps_calculate_clock.restart();
	}

	void update_mouse_position()
	{
		mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}

	void update_viewport()
	{
		viewport = sf::FloatRect(view.getCenter() - view_size / 2.f, view_size);
	}

	sf::Vector2f relative_view_position(sf::Vector2f position)
	{
		return {
			viewport.left + position.x,
			viewport.top  + position.y
		};
	}

	void screenshot()
	{
		sf::Texture screenshot_texture;
		screenshot_texture.create(window.getSize().x, window.getSize().y);
		screenshot_texture.update(window);

		fs::mk_dir("screenshots");
		screenshot_texture.copyToImage().saveToFile("screenshots/" + utils::get_current_time_string() + ".png");
	}

	void draw_pixel(sf::Vector2f position, sf::Color color = sf::Color::White)
	{
		sf::VertexArray pixel(sf::Quads, 4);
		pixel[0].position = { position.x    , position.y + 1 };
		pixel[1].position = { position.x    , position.y     };
		pixel[2].position = { position.x + 1, position.y     };
		pixel[3].position = { position.x + 1, position.y + 1 };
		for (int i = 0; i < 4; i++) pixel[i].color = color;
		window.draw(pixel);
	}

	void draw_line(sf::Vector2f start, sf::Vector2f stop, sf::Color color = sf::Color::White)
	{
		sf::VertexArray line(sf::LineStrip, 2);
		line[0].position = start;
		line[1].position = stop;
		for (int i = 0; i < 2; i++) line[i].color = color;
		window.draw(line);
	}

	void draw_dot(sf::Vector2f position, float radius, sf::Color color = sf::Color::White)
	{
		sf::CircleShape dot(radius, 50);
		dot.setOrigin(radius, radius);
		dot.setPosition(position);
		dot.setFillColor(color);
		window.draw(dot);
	}

	virtual void update() {}

	virtual void draw() {}

	virtual void destroy() {}

	virtual void resize(unsigned int width, unsigned int height) {}
};
