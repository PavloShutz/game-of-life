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

int main()
{
	auto window = sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "CMake SFML Project");
	window.setFramerateLimit(144);

	sf::Clock clock;
	sf::Time dt, timePerFrame = sf::seconds(1.f / 144.f);

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
	std::vector<std::pair<sf::Vector2f, bool>> cells(VCELLS * HCELLS);
	for (std::size_t i = 0; i < VCELLS; ++i) {
		for (std::size_t j = 0; j < HCELLS; ++j) {
			cells[j * VCELLS + i] = std::make_pair<sf::Vector2f, bool>({ static_cast<float>(i), static_cast<float>(j) }, false);
		}
	}

	while (window.isOpen())
	{
		dt = clock.restart();
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mousePressed->button == sf::Mouse::Button::Left) {
					const auto [posX, posY] = mousePressed->position;
					const decltype(DIM) x = (posX / DIM);
					const decltype(DIM) y = (posY / DIM);
					cells[y * VCELLS + x].second = !cells[y * VCELLS + x].second;
				}
			}
		}

		window.clear();

		// Draw grid
		for (const auto& vline : vlines)
			window.draw(vline.data(), vline.size(), sf::PrimitiveType::Lines);

		for (const auto& hline : hlines)
			window.draw(hline.data(), hline.size(), sf::PrimitiveType::Lines);

		// Draw "alive" cells
		for (const auto& pos : cells) {
			if (pos.second) {
				cell.setPosition(pos.first * static_cast<float>(DIM));
				window.draw(cell);
			}
		}

		window.display();
	}
}
