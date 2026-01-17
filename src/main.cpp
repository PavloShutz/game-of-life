#include <SFML/Graphics.hpp>

constexpr std::size_t dim = 32; /* cell size */
constexpr std::size_t WIDTH = 1920;
constexpr std::size_t HEIGHT = 1080;

int main()
{
	auto window = sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "CMake SFML Project");
	window.setFramerateLimit(144);

	sf::Clock clock;
	sf::Time dt, timePerFrame = sf::seconds(1.f / 144.f);

	using Line = std::array<sf::Vertex, 2>;

	std::array<Line, WIDTH / dim> vlines = {};
	std::array<Line, HEIGHT / dim> hlines = {};

	for (std::size_t i = 0; i < vlines.size(); ++i) {
		vlines[i][0] = sf::Vertex{ sf::Vector2f(dim * i, 0.f) };                        /* begin point */
		vlines[i][1] = sf::Vertex{ sf::Vector2f(dim * i, static_cast<float>(HEIGHT)) }; /* end point */
	}

	for (std::size_t i = 0; i < hlines.size(); ++i) {
		hlines[i][0] = sf::Vertex{ sf::Vector2f(0.f, dim * i) };                      /* begin point */
		hlines[i][1] = sf::Vertex{ sf::Vector2f(static_cast<float>(WIDTH), dim * i)}; /* end point */
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
		}

		window.clear();
		
		// Draw grid
		for (const auto& vline : vlines)
			window.draw(vline.data(), vline.size(), sf::PrimitiveType::Lines);
		
		for (const auto& hline : hlines)
			window.draw(hline.data(), hline.size(), sf::PrimitiveType::Lines);

		window.display();
	}
}
