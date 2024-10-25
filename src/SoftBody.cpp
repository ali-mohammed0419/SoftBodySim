#include "SoftBody.hpp"
#include <cmath>

SoftBody::SoftBody(float radius, int numPoints, sf::Vector2f center)
    : m_radius(radius), m_numPoints(numPoints), stiffness(1000.0f), damping(0.1f), pressure(5000.0f), m_isDragging(false) {
    float angleStep = 2 * 3.14159f / numPoints;
    for (int i = 0; i < numPoints; ++i) {
        float angle = i * angleStep;
        Point p;
        p.position = center + sf::Vector2f(std::cos(angle) * radius, std::sin(angle) * radius);
        p.velocity = sf::Vector2f(0.f, 0.f);
        p.force = sf::Vector2f(0.f, 0.f);
        p.previousForce = sf::Vector2f(0.f, 0.f);
        p.mass = 1.0f;
        m_points.push_back(p);
    }
}

void SoftBody::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (auto& point : m_points) {
            if (std::hypot(point.position.x - mousePos.x, point.position.y - mousePos.y) < m_radius) {
                m_isDragging = true;
                m_draggedPoint = &point;
                break;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        m_isDragging = false;
        m_draggedPoint = nullptr;
    }
    else if (event.type == sf::Event::MouseMoved && m_isDragging) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        m_draggedPoint->position = mousePos;
    }
}

void SoftBody::update(float dt, const sf::RenderWindow& window) {
    dt = std::min(dt, 0.02f);

    for (auto& point : m_points) {
        if (&point == m_draggedPoint) {
            point.velocity = sf::Vector2f(0.f, 0.f);
            point.force = sf::Vector2f(0.f, 0.f);
            continue;
        }

        point.force += sf::Vector2f(0.f, 980.f * point.mass);
        point.velocity += (point.force / point.mass) * dt;
        point.velocity *= std::max(0.f, 1.0f - damping * dt);
        point.position += point.velocity * dt;

        if (point.position.x < 0.f) {
            point.position.x = 0.f;
            point.velocity.x = -point.velocity.x * damping;
        }
        else if (point.position.x > window.getSize().x) {
            point.position.x = window.getSize().x;
            point.velocity.x = -point.velocity.x * damping;
        }
        if (point.position.y < 0.f) {
            point.position.y = 0.f;
            point.velocity.y = -point.velocity.y * damping;
        }
        else if (point.position.y > window.getSize().y) {
            point.position.y = window.getSize().y;
            point.velocity.y = -point.velocity.y * damping;
        }

        point.force = sf::Vector2f(0.f, 0.f);
    }

    for (size_t i = 0; i < m_points.size(); ++i) {
        size_t next = (i + 1) % m_points.size();
        sf::Vector2f dir = m_points[next].position - m_points[i].position;
        float distance = std::hypot(dir.x, dir.y);

        if (distance > 1e-5) {
            float delta = distance - m_radius;
            sf::Vector2f forceDir = dir / distance;
            sf::Vector2f springForce = forceDir * delta * stiffness;
            m_points[i].force += springForce;
            m_points[next].force -= springForce;
        }
    }

    sf::Vector2f center(0.f, 0.f);
    for (const auto& point : m_points) {
        center += point.position;
    }
    center /= static_cast<float>(m_points.size());

    for (auto& point : m_points) {
        sf::Vector2f dir = point.position - center;
        float distance = std::hypot(dir.x, dir.y);
        if (distance > 1e-5) {
            sf::Vector2f forceDir = dir / distance;
            sf::Vector2f pressureForce = forceDir * pressure;
            point.force += pressureForce;
        }
    }

    for (size_t i = 0; i < m_points.size(); ++i) {
        for (size_t j = i + 1; j < m_points.size(); ++j) {
            sf::Vector2f dir = m_points[j].position - m_points[i].position;
            float distance = std::hypot(dir.x, dir.y);

            if (distance < m_radius * 0.5f && distance > 1e-5) {
                sf::Vector2f forceDir = dir / distance;
                sf::Vector2f repelForce = forceDir * (m_radius * 0.5f - distance) * stiffness;
                m_points[i].force -= repelForce;
                m_points[j].force += repelForce;
            }
        }
    }

    for (auto& point : m_points) {
        point.previousForce = point.previousForce * 0.9f + point.force * 0.1f;
    }
}

void SoftBody::draw(sf::RenderWindow& window) {
    for (size_t i = 0; i < m_points.size(); ++i) {
        size_t next = (i + 1) % m_points.size();
        sf::Vertex line[] = {
            sf::Vertex(m_points[i].position, sf::Color::Red),
            sf::Vertex(m_points[next].position, sf::Color::Red)
        };
        window.draw(line, 2, sf::Lines);
    }

    for (const auto& point : m_points) {
        sf::Vector2f arrowEnd = point.position + point.previousForce * 0.003f;
        sf::Vertex arrow[] = {
            sf::Vertex(point.position, sf::Color::White),
            sf::Vertex(arrowEnd, sf::Color::White)
        };
        window.draw(arrow, 2, sf::Lines);
    }
}
