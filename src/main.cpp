#include <algorithm>
#include <iostream>
#include <optional>
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
    // this is using a windows font for now so the universe log can actually show text before shipping v1
    sf::Font consoleFont;
    bool fontLoaded = consoleFont.openFromFile("C:/Windows/Fonts/arial.ttf");

    if (!fontLoaded)
    {
        std::cerr << "Could not load font for the event console" << std::endl;
    }

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

        const float gridWidth = optionWidth * 2.f + optionGap;
        const float gridLeft = slotLeft + (slotWidth - gridWidth) / 2.f;

        for (std::size_t i = 0; i < dropdownPlanets.size(); ++i)
        {
            const float column = static_cast<float>(i % 2);
            const float row = static_cast<float>(i / 2);

            dropdownPlanets[i].setPosition({gridLeft + column * (optionWidth + optionGap),
                                            planetY - (3.f - row) * (optionHeight + optionGap)});
        }
    };

    updatePlanetLayout(window.getSize());

    // this is the event history that will be displayed in the blue console box
    std::vector<std::string> eventHistory;
    eventHistory.push_back("Universe initialized.");
    eventHistory.push_back("Drag an object into dark space.");

    const auto addEvent =
        [&eventHistory](const std::string &message)
    {
        eventHistory.push_back(message);

        // only keeping the newest events so the console does not get completely filled up
        constexpr std::size_t maxEvents = 12;

        if (eventHistory.size() > maxEvents)
        {
            eventHistory.erase(eventHistory.begin());
        }

        std::cout << message << std::endl;
    };

    // Placed sprites remain in the universe after being dropped.
    std::vector<sf::Sprite> placedSprites;

    const auto centerSpriteOrigin =
        [](sf::Sprite &sprite)
    {
        const auto bounds = sprite.getLocalBounds();

        sprite.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                          bounds.position.y + bounds.size.y / 2.f});
    };

    const auto isInsideDarkSpace =
        [&darkSpace](sf::Vector2f point)
    {
        return darkSpace.getGlobalBounds().contains(point);
    };

    const auto keepSpriteInDarkSpace =
        [&darkSpace](sf::Sprite &sprite, sf::Vector2f position)
    {
        const sf::FloatRect area = darkSpace.getGlobalBounds();
        const sf::Vector2f spriteSize = sprite.getGlobalBounds().size;
        const float halfWidth = spriteSize.x / 2.f;
        const float halfHeight = spriteSize.y / 2.f;

        const float minX = area.position.x + halfWidth;
        const float maxX = area.position.x + area.size.x - halfWidth;
        const float minY = area.position.y + halfHeight;
        const float maxY = area.position.y + area.size.y - halfHeight;

        position.x = minX <= maxX
                         ? std::clamp(position.x, minX, maxX)
                         : area.position.x + area.size.x / 2.f;
        position.y = minY <= maxY
                         ? std::clamp(position.y, minY, maxY)
                         : area.position.y + area.size.y / 2.f;
        sprite.setPosition(position);
    };

    // variables for dragging things from the toolbar into dark space
    std::optional<sf::Sprite> draggedSprite;
    std::string draggedName;
    bool isDragging = false;

    // the planet icon has two jobs, so this helps tell the difference between clicking it and dragging it
    bool pendingPlanetClick = false;
    sf::Vector2f planetPressPosition;

    const auto beginDrag =
        [&draggedSprite, &draggedName, &isDragging, &centerSpriteOrigin](const sf::Sprite &sourceSprite, const std::string &name, sf::Vector2f mousePosition)
    {
        // copy the toolbar sprite instead of moving the real toolbar sprite
        // this means the toolbar never loses an option after the user places something
        draggedSprite.emplace(sourceSprite);
        centerSpriteOrigin(*draggedSprite);
        draggedSprite->setScale({1.25f, 1.25f});
        draggedSprite->setPosition(mousePosition);

        draggedName = name;
        isDragging = true;
    };

    const auto finishDrag =
        [&draggedSprite, &draggedName, &isDragging, &placedSprites,
         &isInsideDarkSpace, &keepSpriteInDarkSpace, &addEvent](sf::Vector2f mousePosition)
    {
        if (!isDragging || !draggedSprite)
        {
            return;
        }

        // only make the object permanent if it was actually dropped into dark space
        if (isInsideDarkSpace(mousePosition))
        {
            keepSpriteInDarkSpace(*draggedSprite, mousePosition);
            placedSprites.push_back(*draggedSprite);
            addEvent(draggedName + " placed in dark space. This change is permanent.");
        }
        else
        {
            addEvent(draggedName + " returned to the toolbar.");
        }

        draggedSprite.reset();
        draggedName.clear();
        isDragging = false;
    };

    // making a game loop that keeps the window open until the user closes it
    while (window.isOpen())
    {
        // INPUT / EVENTS
        while (const std::optional event = window.pollEvent())
        {
            // close window at exit
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
                for (auto &sprite : placedSprites)
                {
                    keepSpriteInDarkSpace(sprite, sprite.getPosition());
                }
            }

            // handle mouse click events for the toolbar and planet dropdown
            if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f mousePosition =
                        window.mapPixelToCoords(mousePressed->position);

                    // if the dropdown is already open, check whether the user picked one of the planets first
                    if (isDropdownOpen)
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

                                addEvent("Planet " + std::to_string(i + 1) + " selected.");
                                break;
                            }
                        }

                        if (selectedPlanet)
                        {
                            continue;
                        }

                        // Clicking somewhere else closes the dropdown.
                        if (!activePlanet.getGlobalBounds().contains(mousePosition))
                        {
                            isDropdownOpen = false;
                        }
                    }

                    // clicking the planet should open the dropdown, but dragging it should create a planet in dark space
                    if (activePlanet.getGlobalBounds().contains(mousePosition))
                    {
                        pendingPlanetClick = true;
                        planetPressPosition = mousePosition;
                        continue;
                    }

                    // every other toolbar object can immediately start dragging when it is clicked
                    if (asteroidSprite.getGlobalBounds().contains(mousePosition))
                    {
                        beginDrag(asteroidSprite, "Asteroid", mousePosition);
                    }
                    else if (earthSprite.getGlobalBounds().contains(mousePosition))
                    {
                        beginDrag(earthSprite, "Earth", mousePosition);
                    }
                    else if (sunSprite.getGlobalBounds().contains(mousePosition))
                    {
                        beginDrag(sunSprite, "Sun", mousePosition);
                    }
                    else if (moonSprite.getGlobalBounds().contains(mousePosition))
                    {
                        beginDrag(moonSprite, "Moon", mousePosition);
                    }
                }
            }

            // keep the dragged sprite attached to the mouse while the mouse moves
            if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                const sf::Vector2f mousePosition =
                    window.mapPixelToCoords(mouseMoved->position);

                // a little movement means the user wanted to drag the planet instead of opening its dropdown
                if (pendingPlanetClick)
                {
                    const float differenceX = mousePosition.x - planetPressPosition.x;
                    const float differenceY = mousePosition.y - planetPressPosition.y;
                    if (differenceX * differenceX + differenceY * differenceY > 49.f)
                    {
                        pendingPlanetClick = false;
                        isDropdownOpen = false;

                        beginDrag(activePlanet,
                                  "Planet " + std::to_string(activePlanetIndex + 1),
                                  mousePosition);
                    }
                }

                if (isDragging && draggedSprite)
                {
                    draggedSprite->setPosition(mousePosition);
                }
            }

            // when the user lets go, either open the planet dropdown or finish placing the object
            if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseReleased->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f mousePosition =
                        window.mapPixelToCoords(mouseReleased->position);

                    if (pendingPlanetClick)
                    {
                        isDropdownOpen = !isDropdownOpen;
                        pendingPlanetClick = false;
                    }
                    else if (isDragging)
                    {
                        finishDrag(mousePosition);
                    }
                }
            }
        }

        // RENDERING
        // clear screen
        window.clear(sf::Color::White);

        // draw dark space
        window.draw(darkSpace);

        // draw everything that has already been permanently placed in the universe
        for (const auto &sprite : placedSprites)
        {
            window.draw(sprite);
        }

        // draw console box
        window.draw(consoleBox);

        // draw the event history inside the console box
        if (fontLoaded)
        {
            const sf::Vector2f consolePosition = consoleBox.getPosition();
            const sf::Vector2f consoleSize = consoleBox.getSize();

            sf::Text title(consoleFont, "UNIVERSE LOG", 20);
            title.setFillColor(sf::Color::White);
            title.setPosition({consolePosition.x + 18.f,
                               consolePosition.y + 16.f});
            window.draw(title);

            const std::size_t visibleEvents =
                std::min<std::size_t>(eventHistory.size(), 10);

            const std::size_t firstEvent =
                eventHistory.size() - visibleEvents;

            float eventY = consolePosition.y + 55.f;

            for (std::size_t i = firstEvent; i < eventHistory.size(); ++i)
            {
                sf::Text eventText(consoleFont, "> " + eventHistory[i], 15);
                eventText.setFillColor(sf::Color(225, 230, 255));
                eventText.setPosition({consolePosition.x + 18.f, eventY});

                // shorten a message if it is too wide to fit in the blue box
                if (eventText.getGlobalBounds().size.x >= consoleSize.x - 36.f)
                {
                    std::istringstream words(eventHistory[i]);

                    std::string word;
                    std::string wrapped;
                    std::string currentLine;

                    while (words >> word)
                    {
                        // see what the line would look like if we added the next word
                        std::string testLine = currentLine;

                        if (!testLine.empty())
                        {
                            testLine += " ";
                        }

                        testLine += word;

                        eventText.setString("> " + testLine);

                        // if the new word makes the line too wide, start a new line
                        if (eventText.getGlobalBounds().size.x >= consoleSize.x - 36.f)
                        {
                            if (!wrapped.empty())
                            {
                                wrapped += "\n ";
                            }

                            wrapped += currentLine;
                            currentLine = word;
                        }
                        else
                        {
                            currentLine + testLine;
                        }
                    }

                    window.draw(eventText);

                    eventY += 28.f;

                    if (eventY > consolePosition.y + consoleSize.y - 25.f)
                    {
                        break;
                    }
                }
            }

            // drawing all sprites in the toolbar
            // these always stay here even after the user drags a copy into dark space
            window.draw(asteroidSprite);
            window.draw(earthSprite);
            window.draw(sunSprite);
            window.draw(moonSprite);
            window.draw(activePlanet);

            // draw the dropdown above everything else when it is open
            if (isDropdownOpen)
            {
                for (const auto &planet : dropdownPlanets)
                {
                    window.draw(planet);
                }
            }

            // draw the object being dragged last so it stays visible under the mouse
            if (isDragging && draggedSprite)
            {
                window.draw(*draggedSprite);
            }

            // display the window
            window.display();
        }

        return 0;
    }
}