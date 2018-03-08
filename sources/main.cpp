#include <iostream>

#include <SFML/Graphics.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,600),"Javelin_Simulator_v1",sf::Style::Close);
    sf::Clock delta, speed;

    sf::View view(sf::Vector2f(0,0),sf::Vector2f(400,300));

    float velocity = 50, height = 1.8;
    float angle = 0;
    const float g = 9.81;
    float x = 0 , y = -height;
    float t = 20;
    bool start = false;
    int precision = 0;
    float p;

    sf::CircleShape javelin(1);
    javelin.setFillColor(sf::Color(255,255,255));
    sf::RectangleShape abscisse;
    sf::RectangleShape ordonnee;
    abscisse.setFillColor(sf::Color(100,100,100));
    ordonnee.setFillColor(sf::Color(100,100,100));

    sf::VertexArray trajectory(sf::Points);

    ImGui::SFML::Init(window);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseWheelMoved) {
                if(event.mouseWheel.delta >= 1) {
                    view.zoom(0.9f);
                }
                if(event.mouseWheel.delta <= -1) {
                    view.zoom(1.1f);
                }
            }
        }
        ImGui::SFML::Update(delta.restart());

        ImGui::Begin("Setup Window");
        ImGui::Text("Gravity : %.2f N/kg", g);
        ImGui::SliderFloat("Velocity",&velocity,0,100,"%.0f m/s");
        ImGui::SliderFloat("Height", &height, 0, 3, "%.2f m");
        ImGui::SliderAngle("Angle", &angle, 0, 89);
        ImGui::SliderFloat("Speed", &t, 0, 25, "%.0f ms");
        ImGui::Combo("Precision",&precision, " 0\0 1\0 2\0\0");

        if(ImGui::Button("Start Simulation")) {
            if(start == false) start = true;
        }
        if(ImGui::Button("Stop")) {
            start = false;
            x = 0;
            y = -height;
            trajectory.clear();
        }
        ImGui::Separator();
        //Set the precision
        if(precision == 0) {
            p = 1;
            ImGui::Text("Current position : %.0f  %.0f",x,y);
        }        else if(precision == 1) {
            p = 0.1;
            ImGui::Text("Current position : %.1f  %.1f",x,y);
        }
        else if(precision == 2) {
            p = 0.01;
            ImGui::Text("Current position : %.2f  %.2f",x,y);
        }
        ImGui::End();

        //Stop calculation when it fall on the ground
        if(y > 0) start = false;

        if(start) {
            //Speed of the simulation
            if(speed.getElapsedTime().asMilliseconds() >= t) {
                y = -(g/(2*pow(velocity,2)*pow(cos(angle),2)))*pow(x,2)+((sin(angle)/cos(angle)))*x+height;
                y = -y;
                trajectory.append(sf::Vector2f(javelin.getPosition().x + javelin.getRadius(), javelin.getPosition().y + javelin.getRadius()));
                x+=p;
                speed.restart();
            }
        }

        javelin.setPosition(x-javelin.getRadius(),y-javelin.getRadius());

        //Set the size of axes according to the zoom
        abscisse.setPosition(sf::Vector2f(view.getCenter().x - view.getSize().x,-view.getSize().y/600/2));
        abscisse.setSize(sf::Vector2f(view.getSize().x*2,view.getSize().y/600));
        ordonnee.setPosition(sf::Vector2f(-view.getSize().x/800/2,view.getCenter().y-view.getSize().y));
        ordonnee.setSize(sf::Vector2f(view.getSize().x/800, view.getSize().y*2));

        view.setCenter(javelin.getPosition());

        window.clear(sf::Color(35,35,35));
        window.setView(view);
        window.draw(abscisse);
        window.draw(ordonnee);
        window.draw(javelin);
        window.draw(trajectory);
        ImGui::Render();
        window.display();
    }
}
