#include <algorithm>
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
    sf::RenderWindow window(sf::VideoMode({1600, 800}), "A Universe that Remembers You", sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);
    sf::View view(sf::FloatRect({0.f, 0.f}, {1600.f, 800.f}));

    // setting up font and text that shows up on the window when opened
    // uhh gotta fix this font stuff cause it is not working
    // const sf::Font font("arial.ttf");
    // sf::Text text(font, "Don't..mess..this..up. I will remember what you do to me...forever..", 30);

    // making a box that represents dark space
    sf::RectangleShape darkSpace;
    darkSpace.setFillColor(sf::Color::Black);
    const sf::Vector2u windowSize = window.getSize();

    // this keeps the dark space box in the center of the window
    const sf::Vector2f size(
        (0.f, static_cast<float>(windowSize.x) - 50.f),
        (0.f, static_cast<float>(windowSize.y) - 200.f));
    darkSpace.setSize(size);
    darkSpace.setPosition({(static_cast<float>(windowSize.x) - size.x) / 2.f, 0.f});

    // making a game loop that keeps the window open until the user closes it
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {

            // close window at exist
            if (event->is<sf::Event::Closed>())
            {
                window.close();
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