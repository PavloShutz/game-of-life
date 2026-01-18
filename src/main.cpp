#include <SFML/Graphics.hpp>

#include <iostream>

#include <array>
#include <vector>
#include <utility>

constexpr std::size_t DIM = 32; /* cell size */
constexpr std::size_t WIDTH = 1920;
constexpr std::size_t HEIGHT = 1080;
constexpr std::size_t VCELLS = WIDTH / DIM + 1; /* number of vertical cells in a grid */
constexpr std::size_t HCELLS = HEIGHT / DIM + 1; /* number of horizontal cells */

enum class State {
	Editing,
	Simulating
};

int main() {
	auto window = sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Game of Life");
	window.setFramerateLimit(60);

	sf::Clock clock;
	sf::Time dt, timePerFrame = sf::seconds(1.f / 60.f);

	using Line = std::array<sf::Vertex, 2>;

	std::array<Line, VCELLS> vlines = {};
	std::array<Line, HCELLS> hlines = {};

	for (std::size_t i = 0; i < vlines.size(); ++i) {
		vlines[i][0] = sf::Vertex{ sf::Vector2f(DIM * i, 0.f) };                        /* begin point */
		vlines[i][1] = sf::Vertex{ sf::Vector2f(DIM * i, static_cast<float>(HEIGHT)) }; /* end point */
	}

	for (std::size_t i = 0; i < hlines.size(); ++i) {
		hlines[i][0] = sf::Vertex{ sf::Vector2f(0.f, DIM * i) };                       /* begin point */
		hlines[i][1] = sf::Vertex{ sf::Vector2f(static_cast<float>(WIDTH), DIM * i) }; /* end point */
	}

	sf::RectangleShape cell({ static_cast<float>(DIM), static_cast<float>(DIM) });
	std::vector<std::pair<sf::Vector2f, bool>> current(VCELLS * HCELLS);
	std::vector<std::pair<sf::Vector2f, bool>> successor(VCELLS * HCELLS);
	for (std::size_t i = 0; i < VCELLS; ++i) {
		for (std::size_t j = 0; j < HCELLS; ++j) {
			current[j * VCELLS + i] = std::make_pair<sf::Vector2f, bool>({ static_cast<float>(i), static_cast<float>(j) }, false);
			successor[j * VCELLS + i] = std::make_pair<sf::Vector2f, bool>({ static_cast<float>(i), static_cast<float>(j) }, false);
		}
	}

	State state = State::Editing;

	while (window.isOpen()) {
		dt = clock.restart();

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mousePressed->button == sf::Mouse::Button::Left) {
					const auto [posX, posY] = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					const decltype(DIM) x = (posX / DIM);
					const decltype(DIM) y = (posY / DIM);
					current[y * VCELLS + x].second = !current[y * VCELLS + x].second;
				}
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				switch (keyPressed->scancode) {
				case sf::Keyboard::Scancode::R:
					state = State::Simulating;
					break;
				case sf::Keyboard::Scancode::S:
					state = State::Editing;
					break;
				case sf::Keyboard::Scancode::C:
					for (auto& curr : current)
						curr.second = false;
					break;
				default:
					break;
				}
			}
		}

		// update
		if (state == State::Simulating) {
			for (std::size_t i = 0; i < VCELLS; ++i) {
				for (std::size_t j = 0; j < HCELLS; ++j) {
					if (i > 0 && j > 0 && i < VCELLS - 1 && j < HCELLS - 1) {
						const int neighbours =
							static_cast<int>(current[(j - 1) * VCELLS + (i - 1)].second)
							+ static_cast<int>(current[j * VCELLS + (i - 1)].second)
							+ static_cast<int>(current[(j + 1) * VCELLS + (i - 1)].second)
							+ static_cast<int>(current[(j - 1) * VCELLS + i].second)
							+ static_cast<int>(current[(j + 1) * VCELLS + i].second)
							+ static_cast<int>(current[(j - 1) * VCELLS + (i + 1)].second)
							+ static_cast<int>(current[j * VCELLS + (i + 1)].second)
							+ static_cast<int>(current[(j + 1) * VCELLS + (i + 1)].second);

						const bool isAlive = current[j * VCELLS + i].second;
						if (neighbours == 3 || (neighbours == 2 && isAlive))
							successor[j * VCELLS + i].second = true;
						else
							successor[j * VCELLS + i].second = false;
					}
				}
			}
			current = successor;
			sf::sleep(sf::seconds(0.1f));
		}

		// render
		window.clear();

		// Draw grid
		for (const auto& vline : vlines)
			window.draw(vline.data(), vline.size(), sf::PrimitiveType::Lines);

		for (const auto& hline : hlines)
			window.draw(hline.data(), hline.size(), sf::PrimitiveType::Lines);


		// Draw "alive" cells
		for (const auto& data : current) {
			if (data.second) {
				cell.setPosition(data.first * static_cast<float>(DIM));
				window.draw(cell);
			}
		}

		window.display();
	}
}
