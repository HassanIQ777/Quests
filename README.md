# Daily Quests 1.3.0

A gamified CLI-based quest and to-do list manager designed to make your daily tasks feel like an adventure.

```text
                ____                  _       
               / __ \                | |      
              | |  | |_   _  ___  ___| |_ ___ 
              | |  | | | | |/ _ \/ __| __/ __|
              | |__| | |_| |  __/\__ \ |_\__ \
               \___\_\\__,_|\___||___/\__|___/
```

## Features

- **Quest Categorization**: Distinguish between **Main Quests** (critical tasks) and **Side Quests** (optional/minor tasks).
- **Progress Tracking**: Set completion percentages and visualize them with color-coded progress bars.
- **Interactive TUI**: Fast, keyboard-driven interface for efficient task management.
- **Persistence**: Automatically saves your quests to a directory of your choice.
- **Logging**: Keeps track of your actions in a log file for history.
- **Clean UI**: Uses ANSI colors and ASCII art for a modern terminal feel.

## Getting Started

### Prerequisites

- A C++20 compatible compiler (e.g., `g++` 10+ or `clang` 12+).
- `make` build utility.

### Building

To build the project in release mode:

```bash
make
```

For a debug build:

```bash
make debug
```

### Running

To run the program, you must provide a home directory in which it will create its directory:

```bash
./quests $HOME
./quests ~/Documents
```
Then it will create a Quests directory on its own and store its data in there.

## Controls

Once the program is running, use the following keys:

| Key | Action |
|-----|--------|
| `A` | **Add** a new quest (Main or Side) |
| `S` | **Set** progress for a specific quest |
| `V` | **View** detailed information/description of a quest |
| `R` | **Remove** all 100% completed quests |
| `D` | **Delete** a specific quest |
| `H` | Show **Help** menu |
| `Q` | **Quit** the application |

## Technical Details

- **Language**: C++20
- **Utilities**: Uses my utility library `libutils` for terminal manipulation, file I/O, and string processing.
- **Data Storage**: Quests are stored as JSON inside of a file named quests.json in the specified home directory.

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

## Author

**HassanIQ777**

---
*Stay productive and complete your quests!*
