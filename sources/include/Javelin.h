#ifndef JAVELIN_H
#define JAVELIN_H

#define GRAVITY 9.81

#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

class Javelin : public sf::Drawable
{
    public:
        Javelin();
        Javelin(std::string name, sf::Color color, float velocity, float angle, float height);
        virtual ~Javelin();
        float calculate(float x);
        std::string getName();
        sf::Vector2f getPos();
        std::string getInfo(); //return a string that will be displaying in imgui's window
        void reset();
    private:
        // sf::Drawable function
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.draw(_trajectory,states);
        }

        std::string _name;
        sf::Color _color;

        //Parameters
        float _velocity;
        float _angle;
        float _height;

        sf::Vector2f _position; //just a data to know the current position

        //a lot of points that describe a curve
        sf::VertexArray _trajectory;
};

#endif // JAVELIN_H
