#include "Game.h"


// Definiowanie funkcji prywatnych

void Game::init_variables()
{
	this->window = nullptr;

	// Inicjalizacja licznika punktów & timerów spawnu wrogów
	this->game_over = false;
	this->points = 0;
	this->health = 10;
	this->enemy_spawn_timer_max = 20.f;
	this->enemy_spawn_timer = this->enemy_spawn_timer_max;
	this->max_enemies = 4;
	this->mouse_held = false;
}

void Game::init_window()
{
	this->video_mode.height = 600;
	this->video_mode.width = 800;

	this->window = new sf::RenderWindow(this->video_mode, "Falling Blocks", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

void Game::init_fonts()
{
	if (this->font.loadFromFile("Fonts/PixelFont.ttf"))
	{
		std::cerr << "ERROR::GAME::INIT_FONTS() Failed to load the font!\n";
	}
}

void Game::init_text()
{
	this->text.setFont(this->font);
	this->text.setCharacterSize(16);
	this->text.setFillColor(sf::Color::White);
	this->text.setString("NULL");
}

void Game::init_enemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setFillColor(sf::Color::Cyan);

}


// Konstruktor / destruktor

Game::Game()
{
	this->init_variables();
	this->init_window();
	this->init_fonts();
	this->init_text();
	this->init_enemies();
}

Game::~Game()
{
	delete this->window;
}



// Defniniowanie funkcji cz³onkowskich

void Game::spawn_enemy()
{
	/*
		Spawnowanie wrogów i ustawienie losowego koloru i pozycji w oknie
	*/

	this->enemy.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)), 0.f);

	// Losowanie rozmiaru wroga
	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(15.f, 15.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;

	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;

	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;

	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;

	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;

	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}

	// Spawn wroga
	this->enemies.push_back(this->enemy);


}

void Game::poll_events()
{
	// Obs³uga okna
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::update_mouse_positions()
{
	/*
		Funkcja aktualizuj¹ca pozycje myszki wzglêdem okna (Vector2i)
	*/

	this->mouse_pos_window = sf::Mouse::getPosition(*this->window);
	this->mouse_pos_view = this->window->mapPixelToCoords(this->mouse_pos_window);
}

void Game::update_text()
{
	std::stringstream ss;
	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n";

	this->text.setString(ss.str());
}

void Game::update_enemies()
{
	// Aktualizacja timera spawnowania wrogów
	if (this->enemies.size() < this->max_enemies)
	{
		if (this->enemy_spawn_timer >= this->enemy_spawn_timer_max)
		{
			// Spawn wroga i reset timera
			this->spawn_enemy();
			this->enemy_spawn_timer = 0.f;
		}
			
		else
			this->enemy_spawn_timer += 1.f;
	}

	// Przemieszczenie i aktualizacja wrogów
	for (int i{}; i < this->enemies.size(); ++i)
	{
		bool deleted = false;

		this->enemies[i].move(0.f, 4.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health--;
		}
			
	}

	// Sprawdzenie czy wróg zosta³ klikniêty
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouse_held == false)
		{
			this->mouse_held = true;
			bool deleted = false;

			for (int i{}; i < this->enemies.size() && deleted == false; ++i)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mouse_pos_view))
				{


					// Dodaj punkt
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points += 10;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 7;
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 3;
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						this->points += 1;


					// Usuniêcie wroga
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
		
		
	}
	else
	{
		this->mouse_held = false;
	}
	
}

void Game::update()
{
	this->poll_events();

	if (this->game_over == false)
	{
		this->update_mouse_positions();
		this->update_text();
		this->update_enemies();
	}

	if (this->health <= 0)
		this->game_over = true;

}

void Game::render_text(sf::RenderTarget& target)
{
	target.draw(this->text);
}

void Game::render_enemies(sf::RenderTarget& target)
{
	// Renderowanie wrogów
	for (auto& enemy : this->enemies)
	{
		target.draw(enemy);
	}
}

void Game::render()
{
	/*
		Renderowanie okna gry
		- wyczyszczenie okna
		- Renderowanie obiektów gry
		- wyœwietlenie klatki w oknie
	*/

	this->window->clear();

	// Rysowanie elementów gry w oknie
	this->render_enemies(*this->window);

	this->render_text(*this->window);

	// Wyœwietlenie okna
	this->window->display();
}

const bool Game::window_is_open() const
{
	return this->window->isOpen();
}

const bool Game::is_game_over() const
{
	return this->game_over;
}


