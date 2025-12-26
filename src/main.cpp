#include "pendulums/pendulum.h"
#include <iostream>
#include <sstream>

void drawAxes(sf::RenderWindow& window, sf::Vector2f center) {
    sf::Vertex xAxis[] = {
        {{0, center.y}, sf::Color(100, 100, 100)},
        {{800, center.y}, sf::Color(100, 100, 100)}
    };
    sf::Vertex yAxis[] = {
        {{center.x, 0}, sf::Color(100, 100, 100)},
        {{center.x, 600}, sf::Color(100, 100, 100)}
    };
    window.draw(xAxis, 2, sf::PrimitiveType::Lines);
    window.draw(yAxis, 2, sf::PrimitiveType::Lines);
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Pendulum Phase Space");
    window.setFramerateLimit(60);

    sf::Clock clock;

    sf::Vector2f center(400, 300);
    Pendulum pendulum(center);

    pendulum.setDamping(0.0);

    std::cout << "CONTROLS:\n";
    std::cout << "[1] No Damping (Perpetual)\n";
    std::cout << "[2] Add Damping (Spiral)\n";
    std::cout << "[Up/Down] Change Gravity\n";
    std::cout << "[Space] Reset to high angle\n";

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Num1)
                {
                    pendulum.setDamping(0.0);
                    pendulum.reset(2.0, 0);
                    std::cout << "Mode: No Damping\n";
                }
                if (key->code == sf::Keyboard::Key::Num2)
                {
                    pendulum.setDamping(0.5);
                    pendulum.reset(2.0, 0);
                    std::cout << "Mode: Damping On (k=0.5)\n";
                }

                if (key->code == sf::Keyboard::Key::Up)
                {
                    double currentG = pendulum.getG();
                    pendulum.setG(currentG + 5.0);
                    pendulum.reset(pendulum.getState().theta, pendulum.getState().omega); 
                    std::cout << "Gravity: " << pendulum.getG() << "\n";
                }
                if (key->code == sf::Keyboard::Key::Down) 
                {
                    pendulum.setG(std::max(0.0, pendulum.getG() - 5.0));
                    pendulum.reset(pendulum.getState().theta, pendulum.getState().omega);
                    std::cout << "Gravity: " << pendulum.getG() << "\n";
                }

                if (key->code == sf::Keyboard::Key::Space) 
                {
                    pendulum.reset(2.0, 0); 
                }
            }
        }

        float dt = clock.restart().asSeconds();
        for(int i=0; i<10; ++i) { 
            pendulum.update(dt / 10.0); 
        }

        window.clear(sf::Color::Black);

        drawAxes(window, center);
        
        window.draw(pendulum.getPhaseTrace());

        float headX = center.x + (float)(pendulum.getState().theta * pendulum.getScale());
        float headY = center.y - (float)(pendulum.getState().omega * pendulum.getScale() / 2.0);
        
        sf::CircleShape head(5);
        head.setOrigin({5, 5});
        head.setPosition({headX, headY});
        head.setFillColor(sf::Color::White);
        window.draw(head);

        window.display();
    }
    return 0;
}