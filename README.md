# SpaceJet Game
## Description
"SpaceJet" is a simple 2D arcade-style game made using SFML (Simple and Fast Multimedia Library). In this game, you control a spaceship and must avoid obstacles that fall from the top of the screen. The objective is to survive as long as possible without colliding with obstacles. The game includes a life counter, and when the spaceship collides with obstacles, it loses a life. After losing all lives, the game ends.
## How to Run
Follow the steps below to compile and run the project on your local machine.

1. Clone the repository:

2. Open a terminal and navigate to the folder where the project is located.

3. Run CMake to generate build files:

- Use the following command to generate the build configuration using CMake:
```
cmake -B build
```
4. Build the project:

- After the CMake configuration is complete, use the following command to build the project:

```
cmake --build build
```
5. Run the executable:

- Once the project is built, you can run the executable from the build directory:

```
build/bin/Debug/main.exe
```
## Installation SFML
To run this game, you need to have SFML installed on your system. SFML is a multimedia library used for handling graphics, events, and other game components.

Download SFML from https://github.com/SFML/cmake-sfml-project?tab=readme-ov-file. Follow the installation guide provided there.

## Game Controls
- W or Up Arrow: Move the spaceship up
- S or Down Arrow: Move the spaceship down
- A or Left Arrow: Move the spaceship left
- D or Right Arrow: Move the spaceship right.

The spaceship has a speed limit for movement, and the screen boundary prevents the spaceship from moving out of bounds.















