#include <iostream>

#include <SFML/Graphics.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <imgui/wrappers.h>
#include <Javelin.h>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,600),"Javelin Simulator",sf::Style::Close);
    sf::Image icon;
    if(icon.loadFromFile("data/icon.png")) {
        window.setIcon(15,36,icon.getPixelsPtr());
    }

    //Clock
    sf::Clock delta, speed;
    //View
    sf::View view(sf::Vector2f(0,0),sf::Vector2f(400,300));
    //Params
    float velocity = 50, height = 1.8;
    float angle = 0;
    float p; //precision
    int currentJavelin = -1; //No javelin selected = -1
    //ImGui variable
    char* name = new char[20];
    memset(name,0,sizeof name); //reset the name
    float x = 0;
    float t = 20;
    bool play = false;
    int precision = 0;
    bool follow = false;

    //Javelins
    std::vector<Javelin*> javelins;

    //Axis
    sf::RectangleShape abscisse;
    sf::RectangleShape ordonnee;
    abscisse.setFillColor(sf::Color(100,100,100));
    ordonnee.setFillColor(sf::Color(100,100,100));

    //Initialise imgui
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
        //Calculate the numbers of pixel needed to move the view according to the size
        float ratioX = view.getSize().x/800 * .05;
        float ratioY = view.getSize().y/600 * .05;
        //Moving the view with arrows
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            view.move(0,-ratioY);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            view.move(0,ratioY);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            view.move(-ratioX,0);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            view.move(ratioX,0);

        //Update ImGui
        ImGui::SFML::Update(delta.restart());

        //Parameters Window
//======================================================================================
        ImGui::Begin("Parameters");
        ImGui::Text("Gravity : %.2f N/kg", GRAVITY);
        ImGui::SliderFloat("Velocity",&velocity,0,100,"%.0f m/s");
        ImGui::SliderFloat("Height", &height, 0, 3, "%.2f m");
        ImGui::SliderAngle("Angle", &angle, 0, 89);
        ImGui::InputText("Name",name,20);
        if(ImGui::Button("Add",sf::Vector2f(50,25))) {
            javelins.push_back(new Javelin(string(name),sf::Color::White,velocity,angle,height)); // add javelin according to params
            currentJavelin = javelins.size()-1; //Select this new javelin
            memset(name,0,sizeof name); //reset the name
        }
        ImGui::End();
//=======================================================================================
        ImGui::Begin("Player");
        ImGui::SliderFloat("Speed", &t, 0, 25, "%.0f ms");
        ImGui::Combo("Precision",&precision, " 0\0 1\0 2\0\0");
        std::vector<std::string> javelinsNames;
        for(int i = 0; i < javelins.size(); i++) {
            javelinsNames.push_back(javelins[i]->getName());
        }
        for(int i = 0; i < javelins.size(); i++) {
            for(int j = 0; j < javelins.size(); j++) {
                if(javelins[i]->getName().compare(javelins[j]->getName()) == 0) {
                    //If the name already exist => we delete the oldest
                    if(i < j) javelins.erase(javelins.begin() + i);
                    else if(i > j) javelins.erase(javelins.begin() + j);
                }
            }
        }
        ImGui::Combo("Javelin",&currentJavelin,javelinsNames);
        ImGui::Checkbox("View : Follow Javelin", &follow);
        ImGui::Columns(2,"",false); //Create two columns, one for the Play button and one for the reset button
        ImGui::SetColumnOffset(1,60);
        if(ImGui::Button("Play",sf::Vector2f(50,25))) {
            if(!play && javelins.size() != 0) {
                play = true;
                x = 0;
            }
        }
        ImGui::NextColumn(); //Switch to the second column
        if(ImGui::Button("Reset",sf::Vector2f(50,25))) {
            play = false;
            x = 0;
            if(javelins.size() != 0)
                javelins[currentJavelin]->reset();
        }
        ImGui::Columns(1); //Stop column
        ImGui::Separator();
        ImGui::Text("Data : ");
        if(javelins.size() != 0) {
            if(precision == 0) {
                p = 1;
                ImGui::Text("Current position : %.0f  %.0f",x,javelins[currentJavelin]->getPos().y);
            }        else if(precision == 1) {
                p = 0.1;
                ImGui::Text("Current position : %.1f  %.1f",x,javelins[currentJavelin]->getPos().y);
            }
            else if(precision == 2) {
                p = 0.01;
                ImGui::Text("Current position : %.2f  %.2f",x,javelins[currentJavelin]->getPos().y);
            }
        }
        if(javelins.size() != 0)
            ImGui::Text(javelins[currentJavelin]->getInfo().c_str());
        ImGui::End();
//=======================================================================================

        if(javelins.size() != 0)
           if(javelins[currentJavelin]->getPos().y > 0) play = false;

        if(play) {
            if(speed.getElapsedTime().asMilliseconds() >= t) {
                javelins[currentJavelin]->calculate(x);
                x+=p;
                speed.restart();
            }
        }
        if(javelins.size() != 0 && follow)
            view.setCenter(javelins[currentJavelin]->getPos());

        //Set axes position according to the size of the view
        abscisse.setPosition(sf::Vector2f(view.getCenter().x - view.getSize().x,-view.getSize().y/600/2));
        abscisse.setSize(sf::Vector2f(view.getSize().x*2,view.getSize().y/600));
        ordonnee.setPosition(sf::Vector2f(-view.getSize().x/800/2,view.getCenter().y-view.getSize().y));
        ordonnee.setSize(sf::Vector2f(view.getSize().x/800, view.getSize().y*2));

        window.clear(sf::Color(35,35,35));
        window.setView(view);
        window.draw(abscisse);
        window.draw(ordonnee);
        if(javelins.size() != 0)
            window.draw(*javelins[currentJavelin]);
        ImGui::Render();
        window.display();
    }
}
