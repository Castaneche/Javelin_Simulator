#include "Javelin.h"
#include <iostream>

Javelin::Javelin(std::string name, sf::Color color, float velocity, float angle, float height)
: sf::Drawable(), _name(name), _color(color), _velocity(velocity), _angle(angle), _height(height), _trajectory(sf::LinesStrip)
{
    //ctor
}
Javelin::Javelin(){}
Javelin::~Javelin()
{
    //dtor
}
float Javelin::calculate(float x)
{
    //Calculate the y position
    float y = -(GRAVITY/(2*pow(_velocity,2)*pow(cos(_angle),2)))*pow(x,2)+((sin(_angle)/cos(_angle)))*x+_height;
    // y = -y because the axes are inverted on a computer
    _trajectory.append(sf::Vertex(sf::Vector2f(x,-y),_color));
    _position = sf::Vector2f(x,-y);
}
std::string Javelin::getName() {
    return _name;
}
sf::Vector2f Javelin::getPos() {
    return _position;
}
std::string Javelin::getInfo() {
    std::ostringstream oss;
    oss << "Velocity : " << _velocity << " m/s\nAngle : " << floor(_angle*180/3.14) << " deg\nHeight : " << _height << " m";
    return oss.str();
}
void Javelin::reset() {
    calculate(0);
    _trajectory.clear();
}
