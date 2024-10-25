#ifndef SOFTBODY_HPP
#define SOFTBODY_HPP

#include <SFML/Graphics.hpp>
#include <vector>

struct Point {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f force;
    float mass;
    sf::Vector2f previousForce;
};

class SoftBody {
public:
    SoftBody(float radius, int numPoints, sf::Vector2f center);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(float dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    float m_radius;
    int m_numPoints;
    float stiffness;
    float damping;
    float pressure;
    std::vector<Point> m_points;
    bool m_isDragging;
    Point* m_draggedPoint;
};

#endif // SOFTBODY_HPP
