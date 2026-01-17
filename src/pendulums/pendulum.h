#pragma once

#include <SFML/Graphics.hpp>

struct State
{
    double theta;
    double omega;
};

class Pendulum
{
public:
    Pendulum(sf::Vector2f center);
    void reset(double startTheta, double startOmega);
    void update(double dt);

    void setDamping(double d);
    void setG(double g);
    void setL(double L);
    void setPhaseTrace(sf::VertexArray vertex);
    void setScreenCenter(sf::Vector2f sc);
    void setState(State state);
    void setScale(double scale);
    State getState() const;
    double getDamping() const;
    double getG() const;
    double getL() const;
    double getScale() const;
    sf::VertexArray getPhaseTrace() const;
    sf::Vector2f getScreenCenter() const;

private:
    double g = 9.81;
    double L = 1.0;
    double damping = 0.0;
    State state;
    sf::VertexArray phaseTrace;
    sf::Vector2f screenCenter;
    float scale = 100.0f;

    void addPointToTrace();
    State derivative(const State &s);
};
