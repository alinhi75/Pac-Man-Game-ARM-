# Pac-Man Game for LandTiger Board

## Overview
This project is a Pac-Man game developed for the LandTiger LPC1768 board. It demonstrates the use of the KEIL software debug environment to simulate and test embedded systems. The game implements core Pac-Man mechanics, including navigation through a maze, scoring, power-ups, and a countdown timer.

## Features
- **Maze Design**: The maze is displayed on the LandTiger's LCD, with 240 standard pills and 6 power pills placed randomly.
- **Joystick Navigation**: Pac-Man's movement is controlled via the joystick, with continuous motion until interrupted by a new input or a wall.
- **Scoring and Lives**:
  - Standard Pill: 10 points
  - Power Pill: 50 points
  - Extra Life: Earned every 1000 points
- **Countdown Timer**: Players have 60 seconds to complete the maze.
- **Pause and Resume**: The game starts in pause mode and can be toggled using the INT0 button.
- **Victory and Game Over Conditions**:
  - Victory: Collect all pills before the timer expires.
  - Game Over: Timer reaches zero.
- **Teleport Points**: Allows Pac-Man to reappear on the opposite side of the maze.

## System Requirements
- **Hardware**: LandTiger LPC1768 Board
- **Software**: KEIL µVision IDE

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/pacman-landtiger.git
   cd pacman-landtiger
   ```
2. Open the project in KEIL µVision.
3. Build the project using the SW_Debug compilation target.
4. Deploy the project to the LandTiger board or run the emulator.

## Gameplay
1. **Start the Game**:
   - Upon starting, the game is in PAUSE mode.
   - Press INT0 to begin.
2. **Navigate Pac-Man**:
   - Use the joystick to move Pac-Man through the maze.
   - Pac-Man eats pills to increase the score.
3. **Win or Lose**:
   - Collect all pills to win.
   - If the timer runs out, the game ends with a "Game Over" screen.
4. **Pause/Resume**:
   - Press INT0 to pause or resume the game.

## Code Structure
- **Global Definitions**: Constants and data structures for game settings.
- **Maze Representation**: A 2D array defines walls, pills, and empty spaces.
- **Main Functions**:
  - `drawMaze`: Displays the maze.
  - `drawPacman`: Updates Pac-Man's position and appearance.
  - `updateDirectionAndMove`: Handles joystick input and Pac-Man's movement.
  - `generatePowerPill`: Adds random power pills to the maze.
  - `updateScore`: Updates the score and handles life milestones.
  - `countdownTimer`: Manages the countdown and game over condition.
  - `togglePause`: Pauses or resumes the game.
- **IRQ Handlers**:
  - `RIT_IRQHandler`: Handles repetitive actions like movement and countdown updates.

## Debugging
The project leverages KEIL’s debugging tools:
- Open the emulator view to test joystick inputs and interrupts.
- Use breakpoints to verify game logic and timing.

## Demonstration
A video demonstration of the project, showcasing the gameplay and code functionality, is available [here](#). It includes:
- Initialization
- Gameplay mechanics
- Debugging session

## Future Enhancements
- Add ghost AI to increase difficulty.
- Introduce levels with varying maze layouts.
- Implement sound effects for a more immersive experience.

## Acknowledgments
- Developed as part of the Architetture dei Sistemi di Elaborazione course.
- Inspired by the original Pac-Man game by Namco.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

---

Feel free to contribute by opening issues or submitting pull requests. Enjoy the game!

