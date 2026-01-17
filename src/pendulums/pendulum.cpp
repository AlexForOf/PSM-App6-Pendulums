#include "pendulums/pendulum.h"
#include <vector>
#include <cmath>
#include "pendulum.h"

Pendulum::Pendulum(sf::Vector2f sc) : screenCenter(sc)
{
    phaseTrace.setPrimitiveType(sf::PrimitiveType::LineStrip);
    reset(3.14 / 4.0, 0);
}

State Pendulum::derivative(const State &s)
{
    double dTheta = s.omega;
    double dOmega = -(g / L) * std::sin(s.theta) - (damping * s.omega);
    return {dTheta, dOmega};
}

void Pendulum::update(double dt)
{
    State k1 = derivative(state);

    State s2 = {state.theta + k1.theta * dt * 0.5, state.omega + k1.omega * dt * 0.5};
    State k2 = derivative(s2);

    State s3 = {state.theta + k2.theta * dt * 0.5, state.omega + k2.omega * dt * 0.5};
    State k3 = derivative(s3);

    State s4 = {state.theta + k3.theta * dt, state.omega + k3.omega * dt};
    State k4 = derivative(s4);

    state.theta += (dt / 6.0) * (k1.theta + 2 * k2.theta + 2 * k3.theta + k4.theta);
    state.omega += (dt / 6.0) * (k1.omega + 2 * k2.omega + 2 * k3.omega + k4.omega);

    addPointToTrace();
}

void Pendulum::reset(double startTheta, double startOmega)
{
    state = {startTheta, startOmega};
    phaseTrace.clear();
}

void Pendulum::addPointToTrace()
{
    float x = screenCenter.x + (float)(state.theta * scale);
    float y = screenCenter.y - (float)(state.omega * scale / 2.0);

    sf::Color color = (std::abs(state.omega) > 2.0) ? sf::Color::Red : sf::Color::Cyan;

    phaseTrace.append({{x, y}, color});
}

void Pendulum::setDamping(double d)
{
    this->damping = d;
}
void Pendulum::setG(double g)
{
    this->g = g;
}
void Pendulum::setL(double L)
{
    this->L = L;
}
void Pendulum::setPhaseTrace(sf::VertexArray pt)
{
    this->phaseTrace = pt;
}
void Pendulum::setScreenCenter(sf::Vector2f sc)
{
    this->screenCenter = sc;
}
void Pendulum::setState(State state)
{
    this->state = state;
}
void Pendulum::setScale(double scale)
{
    this->scale = scale;
}
State Pendulum::getState() const
{
    return state;
}
double Pendulum::getDamping() const
{
    return damping;
}
double Pendulum::getG() const
{
    return g;
}
double Pendulum::getL() const
{
    return L;
}
sf::VertexArray Pendulum::getPhaseTrace() const
{
    return phaseTrace;
}
sf::Vector2f Pendulum::getScreenCenter() const
{
    return screenCenter;
}
double Pendulum::getScale() const
{
    return scale;
}
