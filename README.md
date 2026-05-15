# Coffee Dialer

Coffee Dialer is a small desktop app for tracking coffee brews and dialing in better cups over time.

The mission is simple: help you record what you brewed, how you brewed it, and how it tasted so you can make the best coffee you can.

## Current Features

- Add coffee brew entries
- Track bean name
- Track dose
- Track grind setting
- Track yield
- Track brew time
- Add notes
- View past brews in a table
- Edit existing entries
- Delete entries
- Persist data locally with SQLite
- Runs as a macOS `.app`

## Why This Exists

Dialing in coffee is an iterative process. Small changes in dose, grind, yield, brew time, and technique can make a large difference in taste.

Coffee Dialer gives you a simple place to track those variables so you can compare brews and improve intentionally.

## Data Storage

Coffee Dialer stores its local database in macOS Application Support:

```text
~/Library/Application Support/CoffeeDialer/coffee_dialer.db
```

The database is user data and is not stored inside the app bundle.

## Build

This project uses CMake.

```bash
cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release
```

On macOS, the build produces a `.app` bundle:

```text
cmake-build-release/coffee_dialer.app
```

To install locally:

```bash
cp -R cmake-build-release/coffee_dialer.app /Applications/
```

## Tech Stack

- C++
- Dear ImGui
- GLFW
- OpenGL
- SQLite
- CMake

## Future Iterations
- Refactor: App functions for V1, further iterations need a cleanup
  - Move state into struct
  - Separate functionality into sub folders to clean main
- Add brew method selection:
  - V60
  - Espresso
  - Drip
  - French Press
  - Other manual brew methods
- Add filtering
- Add sorting
- Add search
- Add brew ratings
- Add taste notes / tasting tags
- Add recipe presets
- Add basic brew analytics

## Status

Early local desktop app. Built as a practical tool for tracking coffee dialing for me, not really intended for others, but if you like it yay!
