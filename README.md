# Collect-and-Conquer-2D-Game-OpenGL


## project Overview

This project involves developing a 2D top-view game using OpenGL. The goal is to design and implement a game where the player navigates a scene filled with obstacles, collectables, and power-ups. The player has five lives and the game ends either when all lives are lost or the time runs out.

## Game Features

- **Player**: 
  - Starts in the center of the screen.
  - Controlled via keyboard to move in four directions.
  - Rotates towards the direction of movement.
  - Health bar displays remaining lives, which decrease upon colliding with obstacles or boundaries.

- **Obstacles**: 
  - Positioned around the scene to challenge the player.

- **Collectables**: 
  - At least five collectables are randomly placed in each game run.
  - Collectables disappear upon collection and increase the game score.

- **Power-Ups**: 
  - Two types with distinct functionalities.
  - Randomly appear and remain fixed until collected.
  - Effects last for a few seconds and then deactivate.
  - Do not affect the player's health bar.

- **Display Elements**:
  - Player's health bar, game score, and remaining game time are shown on the screen.

- **Game End**:
  - An end game screen is displayed with a win or loss message based on the game state.

- **Sound**:
  - Background music plays at game start.
  - Different sound effects for collecting items, colliding with obstacles, game win, and game loss.

- **Texture**:
  - Textures applied to all scene elements except small objects where textures might not be visible.

## Requirements

- **Programming Language**: C++ with OpenGL.


## Instructions

1. **Setup**: Clone this repository and set up your development environment with OpenGL.
2. **Implementation**: Follow the requirements and features outlined above to develop the game.
3. **Testing**: Ensure all features work as expected and test for any bugs.
4. **Submission**: Submit your code by the deadline through the provided link.
