#include <SFML/Graphics.hpp>
#include "SoftBody.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Soft Body Simulation");
    window.setFramerateLimit(60);

    SoftBody softBody(30.f, 20, sf::Vector2f(800.f, 450.f));

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            softBody.handleEvent(event, window);
        }

        float dt = clock.restart().asSeconds();

        softBody.update(dt, window);

        window.clear();
        softBody.draw(window);
        window.display();
    }

    return 0;
}
