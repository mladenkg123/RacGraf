# SharkJaws2 🦈

A simple 2D underwater game where you control a scuba diver trying to catch fish while avoiding sharks.

## Description

SharkJaws2 is an OpenGL-based game built using GLUT where players control a scuba diver in an underwater environment. The goal is to catch fish to increase your score while avoiding dangerous sharks. As you progress, the game becomes more challenging with increasing difficulty levels.

## Features

- Smooth 2D graphics with texture-based sprites
- Player-controlled scuba diver
- AI-controlled fish and shark movement
- Score tracking system
- Lives system with visual heart indicators
- Progressive difficulty levels
- Game over screen

## Prerequisites

- Windows operating system
- Visual Studio 2013 or later
- OpenGL
- GLUT library

## Project Structure

The main components of the project include:

- Main game logic and rendering
- Texture loading using stb_image
- Collision detection
- Entity movement systems

## Installation

1. Clone the repository
2. Open the solution file `SharkJaws2.sln` in Visual Studio
3. Ensure you have the required texture files in the correct path:

## How to Play

- Use keyboard controls to move the diver
- Collect fish to increase your score
- Avoid sharks - getting hit will cost you a life
- You have 3 lives to start with
- Score points to advance to higher levels
- Game ends when you run out of lives

## Technical Details

The game uses:

- OpenGL for rendering
- GLUT for window management and input handling
- stb_image library for texture loading
- Custom collision detection system
- Dynamic entity movement patterns

## License

This project uses several components with different licenses:

- stb_image: Public Domain or MIT License (see stb_image.h for details)
- GLUT: MIT License

## Contributing

Feel free to fork the project and submit pull requests. Please ensure you follow the existing code style and include appropriate comments for new features.

## Known Issues

- Hardcoded texture file paths need to be updated for different environments
- Window size is fixed and not adjustable during runtime

## Credits

- Game textures should be placed in the appropriate directory
- Uses the stb_image library by Sean Barrett for texture loading
- Built with OpenGL and GLUT framework
