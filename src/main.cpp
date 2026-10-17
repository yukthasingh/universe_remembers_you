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
    sf::RenderWindow window(sf::VideoMode({1600, 800}),
                            "A Universe that Remembers You",
                            sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize,
                            sf::State::Windowed);

    sf::View view(sf::FloatRect({0.f, 0.f}, {1600.f, 800.f}));
    window.setView(view);

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
        [&asteroidSprite, &earthSprite, &sunSprite, &moonSprite, &darkSpace](sf::Vector2u windowSize)
    {
        constexpr float bottomMargin = 25.f;

        const float windowHeight =
            static_cast<float>(windowSize.y);

        // Use the EXACT left and right edges of the black box
        const float areaLeft = darkSpace.getPosition().x;
        const float areaWidth = darkSpace.getSize().x;

        // Five equal sections:
        // asteroid | earth | sun | moon | planet
        const float slotWidth = areaWidth / 5.f;

        // Center each sprite inside its section
        const auto centerInSlot =
            [&](sf::Sprite &sprite, int slot)
        {
            const auto bounds = sprite.getGlobalBounds();

            const float slotLeft =
                areaLeft + slotWidth * static_cast<float>(slot);

            const float x =
                slotLeft + (slotWidth - bounds.size.x) / 2.f;

            const float y =
                windowHeight - bounds.size.y - bottomMargin;

            sprite.setPosition({x, y});
        };

        centerInSlot(asteroidSprite, 0);
        centerInSlot(earthSprite, 1);
        centerInSlot(sunSprite, 2);
        centerInSlot(moonSprite, 3);
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
        optionSprite.setScale({1.f, 1.f}); // Scale down the dropdown options

        dropdownPlanets.push_back(optionSprite);
    }

    const auto updatePlanetLayout =
        [&activePlanet, &dropdownPlanets, &darkSpace](sf::Vector2u windowSize)
    {

        constexpr float bottomMargin = 25.f;

        const float windowHeight =
            static_cast<float>(windowSize.y);

        const float areaLeft = darkSpace.getPosition().x;
        const float areaWidth = darkSpace.getSize().x;
        const float slotWidth = areaWidth / 5.f;

        const auto planetBounds = activePlanet.getGlobalBounds();

        const float slotLeft =
            areaLeft + slotWidth * 4.f;

        const float planetX =
            slotLeft + (slotWidth - planetBounds.size.x) / 2.f;

        const float planetY =
            windowHeight - planetBounds.size.y - bottomMargin;

        activePlanet.setPosition({planetX, planetY});

        // Put the six choices in a compact 2 x 3 grid above the selected planet.
        constexpr float optionGap = 8.f;
        const float optionWidth = dropdownPlanets[0].getGlobalBounds().size.x;
        const float optionHeight = dropdownPlanets[0].getGlobalBounds().size.y;

        for (std::size_t i = 0; i < dropdownPlanets.size(); ++i)
        {
            const float column = static_cast<float>(i % 2);
            const float row = static_cast<float>(i / 2);

            dropdownPlanets[i].setPosition({planetX + column * (optionWidth + optionGap),
                                            planetY - (3.f - row) * (optionHeight + optionGap)});
        }
    };

    updatePlanetLayout(window.getSize());

    // making a game loop that keeps the window open until the user closes it
    while (window.isOpen())
    {
        // INPUT / EVENTS
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *resizedEvent = event->getIf<sf::Event::Resized>())
            {
                const sf::Vector2f newSize(
                    static_cast<float>(resizedEvent->size.x),
                    static_cast<float>(resizedEvent->size.y));

                // Keep world coordinates matched to the resized window.
                view = sf::View(sf::FloatRect({0.f, 0.f}, newSize));
                window.setView(view);

                updateLayout(resizedEvent->size);
                updateSpriteLayout(resizedEvent->size);
                updatePlanetLayout(resizedEvent->size);
            }

            if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f mousePosition =
                        window.mapPixelToCoords(mousePressed->position);

                    // Clicking the selected planet opens/closes the dropdown.
                    if (activePlanet.getGlobalBounds().contains(mousePosition))
                    {
                        isDropdownOpen = !isDropdownOpen;
                    }
                    else if (isDropdownOpen)
                    {
                        bool selectedPlanet = false;

                        for (std::size_t i = 0; i < dropdownPlanets.size(); ++i)
                        {
                            if (dropdownPlanets[i].getGlobalBounds().contains(mousePosition))
                            {
                                activePlanetIndex = static_cast<int>(i);
                                activePlanet.setTexture(planetTextures[i], true);
                                activePlanet.setScale({2.f, 2.f});

                                updatePlanetLayout(window.getSize());
                                isDropdownOpen = false;
                                selectedPlanet = true;
                                break;
                            }
                        }

                        // Clicking somewhere else closes the dropdown.
                        if (!selectedPlanet)
                        {
                            isDropdownOpen = false;
                        }
                    }
                }
            }
        }

        // RENDERING
        window.clear(sf::Color::White);

        window.draw(darkSpace);
        window.draw(consoleBox);

        window.draw(asteroidSprite);
        window.draw(earthSprite);
        window.draw(sunSprite);
        window.draw(moonSprite);
        window.draw(activePlanet);

        if (isDropdownOpen)
        {
            for (const auto &planet : dropdownPlanets)
            {
                window.draw(planet);
            }
        }

        window.display();
    }

    return 0;
}