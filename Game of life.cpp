#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <ctime>

bool paused = false;

struct GridSlot
{
	sf::RectangleShape* rect;
	bool alive = false;
	int neighbors = 0;

	void draw(sf::RenderWindow* window) 
	{
		if (alive) rect->setFillColor(sf::Color::Black);
		else {
			if(!paused) rect->setFillColor(sf::Color::White);
			else rect->setFillColor(sf::Color(220,220,220));
		}
		window->draw(*rect);
	}
};

const unsigned int W = 800;
const unsigned int H = 800;
const unsigned int gridW = 50;
const unsigned int gridH = 50;

sf::Vector2i getCoord(sf::Vector2i move, sf::Vector2i coord) {
	int newx = coord.x + move.x, newy = coord.y + move.y;
	if (newx < 0) newx = gridW - 1;
	if (newx >= gridW) newx = 0;
	if (newy < 0) newy = gridH - 1;
	if (newy >= gridH) newy = 0;
	return sf::Vector2i(newx, newy);
}

int main()
{

	std::srand(unsigned(std::time(0)));

	sf::RenderWindow window(sf::VideoMode(W, H), "The game of life!", sf::Style::Default);
	window.setFramerateLimit(30);

	GridSlot grid[gridW][gridH];
	for (unsigned i = 0; i < gridW; ++i) {
		for (unsigned j = 0; j < gridH; ++j) {
			float x = float(W) / float(gridW) * float(i), y = float(H) / float(gridH) * float(j);
			grid[i][j].rect = new sf::RectangleShape;
			grid[i][j].rect->setPosition(sf::Vector2f(x, y));
			grid[i][j].rect->setSize(sf::Vector2f(float(W) / float(gridW), float(H) / float(gridH)));
			grid[i][j].alive = std::rand() % 2 == 1;
		}
	}

	bool pressedPrev = false;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			std::cout << mousePos.x << " " << mousePos.y << " -> ";
			mousePos.x /= W / gridW;
			mousePos.y /= H / gridH;
			std::cout << mousePos.x << " " << mousePos.y << std::endl;
			grid[mousePos.x][mousePos.y].alive = true;
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			std::cout << mousePos.x << " " << mousePos.y << " -> ";
			mousePos.x /= W / gridW;
			mousePos.y /= H / gridH;
			std::cout << mousePos.x << " " << mousePos.y << std::endl;
			grid[mousePos.x][mousePos.y].alive = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ) {
			if (!pressedPrev) {
				pressedPrev = true;
				paused = !paused;
			}
		}
		else pressedPrev = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
			for (unsigned i = 0; i < gridW; ++i) {
				for (unsigned j = 0; j < gridH; ++j) {
					grid[i][j].alive = false;
				}
			}
		}

		window.clear();
		for (unsigned i = 0; i < gridW; ++i) {
			for (unsigned j = 0; j < gridH; ++j) {
				grid[i][j].draw(&window);
			}
		}
		window.display();

		if (!paused) {

			for (unsigned i = 0; i < gridW; ++i) {
				for (unsigned j = 0; j < gridH; ++j) {
					int sum = 0;
					bool state = grid[i][j].alive;

					for (int di = -1; di <= 1; ++di) {
						for (int dj = -1; dj <= 1; ++dj) {
							if (di == dj && dj == 0) continue;
							sf::Vector2i newpos = getCoord(sf::Vector2i(di, dj), sf::Vector2i(i, j));
							sum += grid[newpos.x][newpos.y].alive;
						}
					}
					grid[i][j].neighbors = sum;
				}
			}

			for (unsigned i = 0; i < gridW; ++i) {
				for (unsigned j = 0; j < gridH; ++j) {
					bool state = grid[i][j].alive;
					int neighbors = grid[i][j].neighbors;
					if (state == 0 && neighbors == 3)  grid[i][j].alive = true;
					if (state == 1 && (neighbors < 2 || neighbors > 3)) grid[i][j].alive = false;
				}
			}

		}
	}

	return 0;
}