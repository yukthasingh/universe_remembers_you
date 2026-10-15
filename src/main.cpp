#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "universe.h"
#include "renderer.h"


int main()
{
    // initializing game window
    sf::RenderWindow window(sf::VideoMode({800,600}), "A Universe that Remembers You");

    // setting up font and text that shows up on the window when opened
    const sf::Font font("arial.ttf");
    sf::Text text(font, "Don't..mess..this..up. I will remember what you do to me...forever..", 30);

    // making a box that represents dark space
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
        }

        // clear screen
        window.clear();

        // draw dark space
        window.draw(darkSpace);

        // draw text 
        window.draw(text);

        // display the window 
        window.display();
    }
}