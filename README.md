# A Universe That Remembers You

*A Universe That Remembers You* is an experimental C++ space sandbox built with SFML. The project is centered around a simple idea: the player is given a universe to interact with, but the universe does not simply reset every time something goes wrong. Actions are meant to leave consequences behind, allowing the world to develop a history based on what the player creates, moves, damages, or changes.

The project is currently in active development. Right now, most of the work is focused on building the core interface, learning SFML, organizing the program with object-oriented programming, and creating the interaction systems that the larger game will eventually depend on.

## Project Idea

The player begins with a mostly empty area representing dark space. A toolbar underneath the universe contains celestial objects that can eventually be selected, dragged, and placed into the universe. These include asteroids, Earth, the Sun, the Moon, and several fictional planet designs.

A console on the right side of the screen is being designed to record important events. As the project develops, this console will act as a visible history of the universe. Instead of treating actions as isolated events, the goal is for the program to remember what has happened and allow earlier choices to affect later interactions.

The long-term goal is not to create a scientifically perfect universe simulator. The project is more focused on experimentation, consequence, visual interaction, and building a world that feels persistent.

## Current Development Status

The project is still in an early development stage, but the main application window and the first version of the interface have been created.

Currently implemented or being actively developed:

- Resizable SFML application window
- Main dark-space simulation area
- Separate event/console panel
- Responsive positioning for major UI areas
- Image-based celestial object toolbar
- Asteroid, Earth, Sun, and Moon sprites
- Six selectable planet textures
- Active planet display
- Planet-selection dropdown interface
- Mouse input for opening and selecting from the planet menu
- CMake-based project setup
- Initial source files for universe, rendering, physics, random behavior, and celestial-body systems

The next major milestone is turning the interface from a mostly visual layout into an interactive sandbox.

## Current Interface

The application is divided into three main areas.

### Dark Space

The large black region is the main universe area. This is where celestial bodies will eventually be placed and interact with one another.

The goal is for this area to support actions such as:

- Dragging objects into the universe
- Creating multiple celestial bodies
- Moving existing bodies
- Detecting interactions and collisions
- Applying physical behavior
- Changing objects based on previous events
- Preserving consequences instead of simply resetting objects

### Object Toolbar

The bottom portion of the window contains the objects available to the player. The current toolbar includes:

1. Asteroid
2. Earth
3. Sun
4. Moon
5. Selectable planet

The planet slot is different from the other toolbar objects because it represents several possible planets. Clicking the active planet opens a selector containing the six planet designs stored in `assets/img`.

This toolbar will eventually become the starting point for the game's drag-and-drop system. Selecting an object should create a new instance that can be moved into dark space rather than permanently removing the toolbar icon.

### Event Console

The blue panel on the right side of the application is reserved for the universe's event history.

Eventually, it may display messages such as:

```text
Planet created.
Asteroid entered the universe.
Two celestial bodies collided.
A planet's state changed.
```

The exact event system is still being designed. The important idea is that the console should make the universe's history visible to the player.

## Project Structure

```text
universe_remembers_you/
│
├── .vscode/
│   ├── c_cpp_properties.json
│   └── tasks.json
│
├── assets/
│   ├── audio/
│   └── img/
│       ├── Asteroid.png
│       ├── Earth.png
│       ├── Moon.png
│       ├── Planet1.png
│       ├── Planet2.png
│       ├── Planet3.png
│       ├── Planet4.png
│       ├── Planet5.png
│       ├── Planet6.png
│       └── Sun.png
│
├── build/
├── saves/
│
├── src/
│   ├── celestialBody.cpp
│   ├── celestialBody.h
│   ├── main.cpp
│   ├── physics.cpp
│   ├── physics.h
│   ├── renderer.cpp
│   ├── renderer.h
│   ├── universe.cpp
│   └── universe.h
│
├── .gitignore
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## Source Code Organization

The project is gradually being reorganized so that `main.cpp` does not contain the entire game.

### `main.cpp`

Currently responsible for starting the application, creating the window, loading several UI assets, processing SFML events, updating the responsive layout, and rendering the current interface.

As development continues, some responsibilities currently handled here will move into dedicated classes.

### `celestialBody.h` / `celestialBody.cpp`

Intended to represent the shared behavior and data of objects that exist inside the universe. This is one of the main areas where object-oriented programming will be used.

A celestial body may eventually store information such as:

- Position
- Velocity
- Texture and sprite
- Object type
- State
- Previous damage or changes
- Physical properties
- Interaction history

More specialized objects can later build on this common structure.

### `universe.h` / `universe.cpp`

Intended to manage the universe itself. This may eventually include the collection of active celestial bodies, event history, object creation, persistent changes, and interactions between objects.

### `physics.h` / `physics.cpp`

Reserved for physical behavior and calculations. Physics will be introduced gradually after the basic object and input systems are stable.

Possible responsibilities include movement, velocity, attraction, boundaries, and collision behavior.

### `renderer.h` / `renderer.cpp`

Intended to separate drawing and visual responsibilities from game-state logic. This should help keep `main.cpp` and the universe classes easier to understand as the project becomes larger.

### `random.cpp`

Reserved for behavior that requires random generation or variation. Its exact role is still developing alongside the rest of the project.

## Technologies

The project currently uses:

- **C++** for the main program
- **SFML 3.1** for graphics, windows, sprites, input, and future multimedia features
- **CMake** for configuring and building the project
- **Visual Studio / MSVC toolchain** for compilation on Windows
- **Visual Studio Code** as the primary development environment
- **Git/GitHub** for version control and project tracking

## Building the Project

The project uses CMake. From the project root, configure the build directory with:

```bash
cmake -S . -B build
```

Then build the project with:

```bash
cmake --build build
```

When using a Visual Studio multi-configuration generator, the executable will normally be placed in a configuration folder such as:

```text
build/Debug/
```

For example:

```powershell
.\build\Debug\universe_remembers_you.exe
```

The exact executable name depends on the target defined in `CMakeLists.txt`.

### SFML Runtime Files

When SFML is dynamically linked on Windows, the required SFML DLL files must be available when the executable starts. If Windows reports a missing dependency or the program fails before reaching `main()`, check that the correct Debug or Release SFML DLLs are accessible beside the executable or through another correctly configured runtime path.

## Asset Paths

The program currently loads images using paths such as:

```cpp
assets/img/Asteroid.png
```

and:

```cpp
assets/img/Planet1.png
```

Because these are relative paths, the application's working directory matters. The program should normally be launched with the project root as its working directory so that `assets/img` can be found correctly.

The current image assets are:

```text
Asteroid.png
Earth.png
Moon.png
Planet1.png
Planet2.png
Planet3.png
Planet4.png
Planet5.png
Planet6.png
Sun.png
```

## Input and Interaction

The interaction system is currently under development.

The planet selector is the first major mouse-driven UI component. The intended behavior is:

1. The fifth toolbar slot displays the currently selected planet.
2. Clicking that planet opens the planet selection menu.
3. The six available planet designs appear as smaller options.
4. Clicking one changes the active planet.
5. The selector closes after a choice is made.

The next step is to use similar mouse-event logic for dragging objects from the toolbar into dark space.

## Layout

The layout **does not** resize, so the window is a fixed size. This is due to some unexplainable bugs in the resizing of the dark space that I could not fix.

## Game Loop Structure

The main loop follows the standard real-time application pattern:

```text
while window is open
    process events
    update state
    clear frame
    draw universe
    draw interface
    display frame
```

Keeping event processing separate from rendering is important. Rendering should occur every frame, not only when a keyboard, mouse, resize, or window event happens.

The program should also return from `main()` only after the main window loop has ended. Returning from inside the main loop would immediately terminate the application after its first iteration.

## Planned Object-Oriented Design

One of the goals of this project is to learn object-oriented programming by applying it to an actual game rather than building disconnected practice examples.

A possible structure is:

```text
CelestialBody
├── Planet
├── Star
├── Moon
└── Asteroid
```

`CelestialBody` can hold functionality shared by all space objects, while specialized classes can add their own behavior.

The project may eventually use concepts including:

- Classes and objects
- Constructors
- Encapsulation
- Inheritance
- Polymorphism
- Composition
- Vectors of game objects
- References and pointers where appropriate
- Separation of rendering, simulation, and input logic

The goal is not to force every OOP concept into the project. The code should use these ideas when they make the program easier to organize and extend.

## The "Remembers You" System

The central concept of the project is persistence of consequences.

In many sandbox games, destroying or changing something can simply be undone by resetting the level. This project is intended to experiment with a different idea: changes become part of the universe's history.

Eventually, an object could contain state representing what has happened to it. The universe could also maintain an event history recording major player actions and interactions.

A simplified event might eventually look conceptually like:

```cpp
struct UniverseEvent
{
    std::string description;
};
```

The universe could then maintain a collection of events that the console can display.

The final implementation may change significantly as the project develops.

## Planned Features

Near-term goals:

- Finish and polish the planet dropdown
- Add mouse hover/click feedback
- Add click-and-drag interaction
- Drag toolbar objects into dark space
- Create new celestial-body instances when objects are dropped
- Keep placed objects inside the universe boundaries
- Begin moving celestial-body logic out of `main.cpp`
- Display real messages in the event console

Later goals:

- Multiple active celestial bodies
- Object movement
- Collision detection
- Basic physics
- Object state changes after interactions
- Persistent universe history
- Save/load support using the `saves` directory
- Sound effects and/or ambient audio
- More visual feedback
- Improved menus and UI
- More complex relationships between previous and future events

These goals are intentionally flexible. The design will continue changing as the underlying systems become more complete.

## Current Known Issues / Work in Progress

Because the project is still under active development, some systems are incomplete.

- The planet dropdown still needs visual polishing.
- Mouse interaction is still being expanded.
- Drag-and-drop has not yet been fully implemented.
- The event console is currently primarily visual and does not yet contain the complete event-history system.
- Physics systems are still in an early stage.
- Some functionality still lives in `main.cpp` and will be moved into classes as the architecture develops.
- Asset sizing and spacing may need further adjustment at unusual window sizes.

## Development Approach

This project is also a learning project. Features are being developed while learning SFML and strengthening C++ and object-oriented programming skills.

That means development includes more than writing the final lines of code. A significant part of the process involves reading documentation, testing small changes, debugging build problems, understanding SFML's event system, experimenting with coordinates and sprites, and restructuring code when a better design becomes clear.

Rather than attempting to build every system at once, development is being approached in layers:

```text
Window and build system
        ↓
Responsive interface
        ↓
Textures and sprites
        ↓
Mouse interaction
        ↓
Drag and drop
        ↓
Celestial-body objects
        ↓
Universe event system
        ↓
Physics and interactions
        ↓
Persistent consequences
```

This keeps each new feature connected to something that already works.

## Development Log

### Interface and Assets

The first major interface pass established the resizable application window, dark-space area, and right-side console. The layout was changed from fixed positions to calculations based on the current window size so that the interface could respond to resizing.

The first visual assets were then added: an asteroid, Earth, the Sun, the Moon, and six planet designs. These assets form the beginning of the toolbar that will eventually let the player create objects inside the universe.

Development then moved to the planet-selection system. The active planet occupies the final toolbar slot, while the remaining planet textures are used as menu options. This introduced more work with SFML sprites, vectors, texture lifetimes, mouse events, bounds, and responsive positioning.

One debugging issue caused the application to close immediately after launching. The problem came from the structure of the main loop: rendering and the final `return 0` had ended up in the wrong scope. The event loop, rendering loop, and program exit were reorganized so that the application continues running until the window is actually closed.

## Why This Project

The project combines several areas I want to improve at the same time: C++, object-oriented programming, interactive graphics, game architecture, debugging, and visual design.

Instead of learning each concept only through isolated exercises, the project gives those concepts a reason to exist. Vectors can store objects. Classes can represent celestial bodies. Event handling can control the interface. Collision logic can change the universe. File handling can eventually preserve a save. The structure of the program grows alongside the features that require it.

The result is meant to be both a game project and a record of learning how larger C++ programs are designed.

## Contributing / Development Notes

This is currently a personal learning project and is changing frequently. File organization, class responsibilities, UI design, and gameplay systems may be refactored as development continues.

When adding a feature, the general priority is:

1. Make the behavior work clearly.
2. Test it with the existing interface.
3. Keep it compatible with resizing and current controls.
4. Refactor repeated logic when the structure becomes clear.
5. Document important changes.

## License

A `LICENSE` file is included in the repository. Refer to that file for the project's current licensing terms.

---

**Status:** In development  
**Language:** C++  
**Graphics / Window Library:** SFML 3.1  
**Build System:** CMake  
**Current Focus:** UI interaction, planet selection, drag-and-drop, and object-oriented celestial-body architecture
