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
    sf::RenderWindow window(sf::VideoMode({1600, 800}), "A Universe that Remembers You", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Windowed);
    sf::View view(sf::FloatRect({0.f, 0.f}, {1600.f, 800.f}));

    // setting up font and text that shows up on the window when opened
    // uhh gotta fix this font stuff cause it is not working
    // const sf::Font font("arial.ttf");
    // sf::Text text(font, "Don't..mess..this..up. I will remember what you do to me...forever..", 30);

    // making a box that represents dark space
    sf::RectangleShape darkSpace;
    darkSpace.setFillColor(sf::Color::Black);

    // making a rectangle box that represents a console of events that happen in the universe, this will be used to display text of events that happen in the universe
    sf::RectangleShape consoleBox;
    consoleBox.setFillColor(sf::Color(7, 22, 99));

    const auto updateLayout = [&darkSpace, &consoleBox](sf::Vector2u windowSize)
    {
        const float windowWidth = static_cast<float>(windowSize.x);
        const float windowHeight = static_cast<float>(windowSize.y);
        constexpr float horizontalMargin = 50.f;
        constexpr float topMargin = 25.f;
        constexpr float panelExtension = 50.f;
        const float imageSpace = windowHeight / 4.f;

        // Reserve the bottom for images while extending the black area toward the chat.
        const float darkLeft = horizontalMargin;
        const float darkRight = windowWidth * 4.f / 8.f + panelExtension;
        darkSpace.setPosition({darkLeft, topMargin});
        darkSpace.setSize({std::max(0.f, darkRight - darkLeft + 250.f),
                           std::max(0.f, windowHeight - imageSpace - topMargin)});

        // Extend the chat toward the black area while keeping its outer and bottom margins.
        const float chatLeft = windowWidth * 6.f / 8.f - panelExtension;
        const float chatRight = windowWidth - horizontalMargin;
        consoleBox.setPosition({chatLeft, topMargin});
        consoleBox.setSize({std::max(0.f, chatRight - chatLeft),
                            std::max(0.f, windowHeight - topMargin - horizontalMargin)});
    };

    updateLayout(window.getSize());

    // initializing base images as texture, then making them into sprites and setting their positions

    // asteroid
    sf::Texture asteroidTexture;
    if (!asteroidTexture.loadFromFile("assets/img/Asteroid.png"))
    {
        std::cerr << "Failed to load asteroid texture" << std::endl;
        return -1;
    }

    sf::Sprite asteroidSprite(asteroidTexture);
    asteroidSprite.setScale({2.0f, 2.0f}); // scale up the asteroid sprite

    // earth
    sf::Texture earthTexture;
    if (!earthTexture.loadFromFile("assets/img/Earth.png"))
    {
        std::cerr << "Failed to load earth texture" << std::endl;
        return -1;
    }

    sf::Sprite earthSprite(earthTexture);
    earthSprite.setScale({2.0f, 2.0f}); // scale up the earth sprite

    // sun
    sf::Texture sunTexture;
    if (!sunTexture.loadFromFile("assets/img/Sun.png"))
    {
        std::cerr << "Failed to load sun texture" << std::endl;
        return -1;
    }

    sf::Sprite sunSprite(sunTexture);
    sunSprite.setScale({2.0f, 2.0f}); // scale up the sun sprite

    // moon
    sf::Texture moonTexture;
    if (!moonTexture.loadFromFile("assets/img/Moon.png"))
    {
        std::cerr << "Failed to load moon texture" << std::endl;
        return -1;
    }

    sf::Sprite moonSprite(moonTexture);
    moonSprite.setScale({2.0f, 2.0f}); // scale up the moon sprite

    // updating sprite layouts so basically they are under the black box and that they are spaced out evenly
    const auto updateSpriteLayout =
        [&asteroidSprite, &earthSprite, &sunSprite, &moonSprite](sf::Vector2u windowSize)
    {
        constexpr float horizontalMargin = 50.f;
        constexpr float bottomMargin = 25.f;

        const float windowWidth =
            static_cast<float>(windowSize.x);
        const float windowHeight =
            static_cast<float>(windowSize.y);

        // Use the left 5/8 of the window for the sprites.
        const float imageAreaLeft = horizontalMargin;
        const float imageAreaRight = windowWidth * 5.f / 8.f;
        const float imageAreaWidth =
            imageAreaRight - imageAreaLeft;

        // Divide the available width into four sprite slots.
        const float slotWidth = imageAreaWidth / 5.f;

        // Place each sprite near the bottom of the window.
        const float asteroidHeight =
            asteroidSprite.getGlobalBounds().size.y;
        const float earthHeight =
            earthSprite.getGlobalBounds().size.y;
        const float sunHeight =
            sunSprite.getGlobalBounds().size.y;
        const float moonHeight =
            moonSprite.getGlobalBounds().size.y;

        asteroidSprite.setPosition({imageAreaLeft,
                                    windowHeight - asteroidHeight - bottomMargin});

        earthSprite.setPosition({imageAreaLeft + slotWidth,
                                 windowHeight - earthHeight - bottomMargin});

        sunSprite.setPosition({imageAreaLeft + slotWidth * 2.f,
                               windowHeight - sunHeight - bottomMargin});

        moonSprite.setPosition({imageAreaLeft + slotWidth * 3.f,
                                windowHeight - moonHeight - bottomMargin});
    };

    updateSpriteLayout(window.getSize());

    // here we will be position the planet sprite, where clicking on one sprite will get you a drop down of multiple planets that the user can choose from
    std::vector<sf::Texture> planetTextures;
    planetTextures.reserve(6); // Reserve space for 6 textures

    for (int i = 0; i < 6; ++i)
    {
        std::string filename =
            "assets/img/Planet" + std::to_string(i + 1) + ".png";

        sf::Texture texture;

        if (!texture.loadFromFile(filename))
        {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            return -1;
        }

        planetTextures.push_back(std::move(texture));
    }

    // UI states
    int activePlanetIndex = 0;   // Index of the currently active planet
    bool isDropdownOpen = false; // Whether the dropdown is open or not

    sf::Sprite activePlanet(planetTextures[activePlanetIndex]);
    activePlanet.setScale({2.0f, 2.0f}); // Scale up the active planet sprite

    std::vector<sf::Sprite> dropdownPlanets;

    for (int i = 0; i < 6; ++i)
    {
        sf::Sprite optionSprite(planetTextures[i]);
        optionSprite.setScale({0.5f, 0.5f}); // Scale down the dropdown options

        dropdownPlanets.push_back(optionSprite);
    }

    const auto updatePlanetLayout =
        [&activePlanet, &dropdownPlanets](sf::Vector2u windowSize)
    {
        constexpr float horizontalMargin = 50.f;
        constexpr float bottomMargin = 25.f;

        const float windowWidth =
            static_cast<float>(windowSize.x);

        const float windowHeight =
            static_cast<float>(windowSize.y);

        // Same area used by asteroid, earth, sun, and moon
        const float imageAreaLeft = horizontalMargin;
        const float imageAreaRight = windowWidth * 5.f / 8.f;
        const float imageAreaWidth =
            imageAreaRight - imageAreaLeft;

        // Five total slots
        const float slotWidth = imageAreaWidth / 5.f;

        const auto planetBounds =
            activePlanet.getGlobalBounds();

        // Fifth slot
        const float planetX =
            imageAreaLeft + slotWidth * 4.f;

        const float planetY =
            windowHeight - planetBounds.size.y - bottomMargin;

        activePlanet.setPosition({planetX,
                                  planetY});

        // Dropdown options will appear ABOVE the selected planet
        for (std::size_t i = 0; i < dropdownPlanets.size(); ++i)
        {
            dropdownPlanets[i].setPosition({planetX,
                                            planetY - 45.f * static_cast<float>(i + 1)});
        }
    };

    updatePlanetLayout(window.getSize());

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

            if (const auto *resizedEvent =
                    event->getIf<sf::Event::Resized>())
            {
                const sf::Vector2f size(
                    static_cast<float>(resizedEvent->size.x),
                    static_cast<float>(resizedEvent->size.y));

                view.setSize(size);
                window.setView(view);

                updateLayout(resizedEvent->size);
                updateSpriteLayout(resizedEvent->size);
                updatePlanetLayout(resizedEvent->size);
            }

            // // clear screen
            window.clear(sf::Color::White);

            // draw dark space
            window.draw(darkSpace);

            // draw console box
            window.draw(consoleBox);

            // drawing all sprites
            window.draw(asteroidSprite);
            window.draw(earthSprite);
            window.draw(sunSprite);
            window.draw(moonSprite);
            window.draw(activePlanet);

            // handle mouse click events for the planet dropdown
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (isDropdownOpen)
                {
                    for (const auto &planet : dropdownPlanets)
                    {
                        window.draw(planet);
                    }
                }
            }

            // draw text
            // window.draw(text);

            // display the window
            window.display();
        }

        return 0;
    }
}