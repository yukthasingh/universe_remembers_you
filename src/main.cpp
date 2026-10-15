#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <SFML/Graphics.hpp>
// #include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "universe.h"
#include "renderer.h"


int main()
{
    // initializing game window
    sf::RenderWindow window(sf::VideoMode({800,600}), "A Universe that Remembers You");

    // setting up font and text that shows up on the window when opened
    // uhh gotta fix this font stuff cause it is not working
    // const sf::Font font("arial.ttf");
    // sf::Text text(font, "Don't..mess..this..up. I will remember what you do to me...forever..", 30);

    //making a box that represents dark space
    sf::RectangleShape darkSpace;
    darkSpace.setSize(sf::Vector2f(400,400));
    darkSpace.setFillColor(sf::Color::Black);
    darkSpace.setPosition({10,20});

    // making a game loop that keeps the window open until the user closes it
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            
            // close window at exist
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }


            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                
                // get global mouse position 
                sf::Vector2i position = sf::Mouse::getPosition();

                // check if mouse hits any corner of the window so everything can resize according to that
                if (position.x < 0 || position.x > window.getSize().x || position.y < 0 || position.y > window.getSize().y) {
                    // if mouse is outside the window, do nothing
                    continue;
                } else {
                    // if mouse is touching the corner of the window and they click it and resize that window, change the size of everything inside it to for adapt to the new size of the window
                    // get new size of the window
                    sf::Vector2u newSize = window.getSize();
                    // resize the dark space box to fit the new size of the window
                    darkSpace.setSize(sf::Vector2f(newSize.x - 400, newSize.y - 400));

                    std::cout << "Window resized to: " << newSize.x << "x," << newSize.y << "y" << std::endl;
                    std::cout << "Dark space is resized to: " << darkSpace.getSize().x << "x," << darkSpace.getSize().y << "y" << std::endl;
                }

        }

        // // clear screen
        window.clear(sf::Color::White);

        // draw dark space
        window.draw(darkSpace);

        // draw text 
        // window.draw(text);

        // display the window 
        window.display();
    }

    return 0;
}