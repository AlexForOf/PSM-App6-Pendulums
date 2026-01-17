#include "pendulums/pendulum.h"
#include <iostream>
#include <sstream>

void drawAxes(sf::RenderWindow &window, sf::Vector2f center, sf::Vector2f viewSize)
{
    sf::Vertex xAxis[] = {
        {{center.x - viewSize.x, center.y}, sf::Color(100, 100, 100)},
        {{center.x + viewSize.x, center.y}, sf::Color(100, 100, 100)}};
    sf::Vertex yAxis[] = {
        {{center.x, center.y - viewSize.y}, sf::Color(100, 100, 100)},
        {{center.x, center.y + viewSize.y}, sf::Color(100, 100, 100)}};
    window.draw(xAxis, 2, sf::PrimitiveType::Lines);
    window.draw(yAxis, 2, sf::PrimitiveType::Lines);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Pendulum Phase Space");
    window.setFramerateLimit(60);

    sf::Clock clock;
    sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    Pendulum pendulum(center);

    pendulum.setDamping(0.0);

    std::cout << "CONTROLS:\n";
    std::cout << "[1] No Damping (Perpetual)\n";
    std::cout << "[2] Add Damping (Spiral)\n";
    std::cout << "[Up/Down] Change Gravity\n";
    std::cout << "[Left mouse] Place pendulum at new position\n";
    std::cout << "[Space] Reset to high angle\n";

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto resizeEvent = event->getIf<sf::Event::Resized>())
            {
                sf::FloatRect visibleArea({{0, 0}, {(float)resizeEvent->size.x, (float)resizeEvent->size.y}});
                window.setView(sf::View(visibleArea));

                center = {resizeEvent->size.x / 2.0f, resizeEvent->size.y / 2.0f};
                pendulum.setScreenCenter(center);

                pendulum.reset(pendulum.getState().theta, pendulum.getState().omega);
            }

            if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
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

            if (const auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    double scale = pendulum.getScale();
                    double clickTheta = (worldPos.x - center.x) / scale;
                    double clickOmega = (center.y - worldPos.y) / (scale / 2.0);

                    pendulum.reset(clickTheta, clickOmega);
                }
            }
        }

        float dt = clock.restart().asSeconds();
        for (int i = 0; i < 10; ++i)
        {
            pendulum.update(dt / 10.0);
        }

        window.clear(sf::Color::Black);

        drawAxes(window, sf::Vector2f(center.x, center.y), window.getView().getSize());

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