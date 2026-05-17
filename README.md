# TURBO RIDER — README

## Overview

Turbo Rider is a single-player console-based bike racing game built in C++.
You ride a bike down a three-lane road and must dodge oncoming obstacles —
cars, barriers, and rocks — for as long as possible. The longer you survive,
the faster the game gets. Your best score is saved to your profile and shown
on the leaderboard.

---

## How to Run

1. Open `Turbo Rider.cpp` in Visual Studio (or any C++ IDE)
2. Make sure you are compiling for **Windows** (the game uses Windows console APIs)
3. Build and run the project
4. A `users.txt` file will be created automatically in the same folder to save player data

---

## Controls

| Key | Action |
|-----|--------|
| A | Move bike to the left lane |
| D | Move bike to the right lane |
| Q | Quit the current game |

---

## How to Play

1. Launch the game and **Register** a new account or **Login** to an existing one
2. From the user menu, select **Start Game**
3. Your bike starts in the middle lane — dodge all obstacles coming down the road
4. Lose a life each time you collide with an obstacle
5. You start with **3 lives**
6. When all lives are lost, you can spend **550 coins** to buy an extra life and continue
7. If you choose not to continue, your score is saved if it beats your best

---

## Difficulty & Speed

The game automatically gets harder as your score increases — no manual speed control needed.

| Time Survived | Level | Description |
|--------------|-------|-------------|
| 0 – 60 sec | EASY | Relaxed pace, fewer obstacles |
| 60 – 90 sec | MEDIUM | Noticeably faster |
| 90 sec+ | HARD | Fast speed, frequent obstacles |
| 120 sec+ | HARD | Even faster |
| 150 sec+ | HARD | Very fast |
| 180 sec+ | HARD | Maximum intensity |

---

## Obstacles

| Obstacle | Color | Description |
|----------|-------|-------------|
| Car | Red | Oncoming vehicle in your lane |
| Barrier | Cyan | Road barrier blocking the lane |
| Rock | White | Rock formation in the road |

---

## Coins & Rewards

- You start each game with the coins saved in your profile
- Earn **50 coins** every 30 seconds of survival
- Spend **550 coins** at the Game Over screen to buy an extra life and continue

---

## Menus

**Main Menu**
- Login
- Register
- Leaderboard
- Exit

**User Menu** (after login)
- Start Game
- View Profile
- Logout

---

## Leaderboard

- Accessible from the main menu without logging in
- Shows all registered players ranked by their best score in seconds

---

## File Storage

Player data is saved to `users.txt` in the same folder as the executable.
Each player's username, password, coins, best score, and lives are stored and
loaded automatically every time the game starts.

---

## OOP Concepts Demonstrated

| Concept | How It's Used |
|---------|--------------|
| **Abstraction** | `Game` is an abstract base class with a pure virtual `play()` method |
| **Inheritance** | `BikeGame` inherits from `Game` and implements `play()` |
| **Encapsulation** | `Player` and `Obstacle` keep all data private with getters and setters |
| **Polymorphism** | `Game*` pointer calls the correct `play()` at runtime via virtual dispatch |
| **Classes & Objects** | `Player`, `Obstacle`, `BikeGame`, `Game` — each with clear responsibilities |
| **Vectors** | `vector<Player>` stores all registered users; `vector<Obstacle>` manages active obstacles |
| **File I/O** | `loadUsers()` and `saveUsers()` read and write `users.txt` to persist player data |
| **STL** | `string`, `vector`, `fstream`, `iomanip` all used throughout |
