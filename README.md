2048 Game Implementation

Overview

This project is a C++ implementation of the popular game 2048, featuring a graphical user interface built with wxWidgets. The game includes core gameplay mechanics, a visually styled "Game Over" screen, and high-score persistence.

Features

Classic 2048 Gameplay: Merge tiles by sliding them in four directions.

Graphical Interface: A clean, interactive UI built using wxWidgets.

Game Over Screen: A semi-transparent overlay displaying the final score and restart instructions.

High Score Persistence: Automatically saves and loads the highest score.

How to Play

Start the Game: Launch the executable.

Move Tiles: Use arrow keys to move tiles up, down, left, or right.

Game Over Options:

Press R to restart the game.

Press Q to quit.

Technical Details

Language: C++

Framework: wxWidgets for GUI

File I/O: Standard C++ library for saving and loading high scores.

Setup & Installation

Install wxWidgets: Follow the wxWidgets installation guide.

Build the Project:

Clone the repository.

Use a compatible IDE (e.g., Visual Studio, Code::Blocks) to build and run the project.

File Structure

/2048-Game
├── src                 # Source files
├── assets             # Game resources (if applicable)
├── highscore.txt      # Stores the highest score
├── README.md          # Project documentation
└── main.cpp           # Entry point

Acknowledgments

This project was developed as a personal project to explore GUI design, game logic implementation, and file I/O operations in C++.

License

This project is licensed under the MIT License - see the LICENSE file for details.

Author: LucasFeel free to contribute, report issues, or suggest enhancements!

