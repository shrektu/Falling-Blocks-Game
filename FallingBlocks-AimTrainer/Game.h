#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Klasa obs³uguj¹ca rozgrywkê (mini silnik gry)
*/


class Game
{
private:
	// Zmienne
	// Okno
	sf::RenderWindow* window;
	sf::VideoMode video_mode;
	sf::Event ev;
	
	// Pozycja myszki 
	sf::Vector2i mouse_pos_window;
	sf::Vector2f mouse_pos_view;

	// Czcionki
	sf::Font font;

	// Tekst
	sf::Text text;

	// Mechanika gry
	bool game_over;
	unsigned points;
	int health;
	float enemy_spawn_timer;
	float enemy_spawn_timer_max;
	int max_enemies;
	bool mouse_held;

	// Obiekty w grze
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	// Funkcje prywatne
	void init_variables();
	void init_window();
	void init_fonts();
	void init_text();
	void init_enemies();

public:
	// Konstruktor / wirtualny destruktor
	Game();
	virtual ~Game();

	// Funkcje cz³onkowskie
	void spawn_enemy();
	void poll_events();
	void update_mouse_positions();
	void update_text();
	void update_enemies();
	void update();
	void render_text(sf::RenderTarget &target);
	void render_enemies(sf::RenderTarget& target);
	void render();

	const bool window_is_open() const;
	const bool is_game_over() const;
};

