# cs3520-proj

## Instructions

Will Usher's SDL 2.0 Tutorial can be found here: https://www.willusher.io/pages/sdl2/.

This starter code has done Lesson 0.

The Makefile is set up to build all `.cpp` files in the `src` folder and create executables in `bin/main`.  To just build the release executable, run `make exe`.

General structure:
 * src: for source (.cpp) files, everything here will get compiled/linked (src/main.cpp provided)
 * test: for test source code (using CxxText, expected to be in extern/cxxtest-4.4)
 * include: for header (.hpp) files, looked in for #include
 * res: for resources (e.g. images)
 * build: where created intermediate (e.g. object) files go
 * bin: where created executables go
 * doc: where generated documentation (Doxygen) goes

Configurations:
 * rel: for speed
 * deb: for debugging
 * cov: for code coverage (with gcov)
 * prf: for performance analysis (with gprof)


# Synesthesia Simulator

A console-based synesthesia simulator written in C++. It maps musical notes to
colors and shapes in real time, lets you play chords that blend colors and
tones together, and includes a second mode that maps letters to fixed colors,
modeling grapheme-color synesthesia.

## Features

- **Note mode**: press keys `1`-`9` to play a note. Each key maps to a
  different frequency, which is mapped to a color (low frequencies render
  red, high frequencies render blue). Playing a note spawns a burst of
  shapes (a ripple, a circle, and a square) in that color, each with a
  randomized size and position.
- **Chords**: hold multiple number keys down together to play a chord.
  The tones are mixed together into one buffer, and the mapped colors are
  blended using `Color::operator+`, so a chord both sounds and looks like
  a genuine combination of its notes.
- **Persistent trails**: shapes fade and grow over time rather than
  disappearing instantly, so the terminal accumulates a fading wash of
  color as you play.
- **Text mode**: press `t` to switch modes. Typing any letter prints it in
  a fixed color specific to that letter, every time, the way real
  grapheme-color synesthetes report always seeing a given letter in the
  same color. Press `Esc` to return to note mode.
- **Real audio**: notes are synthesized from a fundamental frequency plus
  a few quieter harmonic overtones, giving them a richer, more
  instrument-like sound than a single sine tone.

## Requirements

- A C++20 compiler (tested with g++)
- SDL2 and companion libraries (installed automatically, see below)
- A terminal with at least 256-color ANSI support
- A machine with real audio hardware to actually hear sound (a headless
  SSH remote with no sound card will run everything except audio playback)

## Building and Running

```bash
bash SETUP_ONCE.sh
make exe
./bin/main
```

`SETUP_ONCE.sh` installs the SDL2 development packages via `apt` and
downloads/configures CxxTest 4.4 at `extern/cxxtest-4.4`, patching one
file to compile cleanly under modern g++ warning flags.

### Controls

| Key       | Action                                              |
|-----------|------------------------------------------------------|
| `1`-`9`   | Play a note (hold multiple keys for a chord)          |
| `t`       | Switch to text (grapheme-color) mode                  |
| any letter (in text mode) | Print that letter in its fixed color |
| `Esc`     | Return to note mode from text mode                    |
| `q`       | Quit                                                   |

### Hearing Audio

This project builds and runs correctly on a headless remote development
environment, but no sound will play there since there's no audio
hardware — the program detects this and continues silently rather than
crashing. To actually hear audio, build and run on a machine with real
speakers, such as a local Mac or Windows machine:

```bash
git clone <your-repo-url>
cd <repo-folder>
make exe
./bin/main
```

On macOS, SDL2 must be installed via Homebrew rather than as a
bundle:

```bash
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx
```

## Running Tests

```bash
make tests
./bin/test
```

## Memory Checking

```bash
valgrind ./bin/main
```

## Project Structure

```
include/    header files
src/        source files
test/       CxxTest unit test suites
res/        resources (unused currently)
```

### Key Classes

- **`Color`** (`color.hpp`/`.cpp`) — an RGBA color with overloaded
  arithmetic and comparison operators, used to represent the visual
  output of a note or letter.
- **`Shape`** (`shape.hpp`/`.cpp`) — an abstract base class with
  `Circle`, `Square`, and `Ripple` derived classes, each overriding
  `update()`, `draw()`, and `isExpired()`.
- **`Scene`** (`scene.hpp`/`.cpp`) — owns and drives every active shape
  using a `std::deque<std::unique_ptr<Shape>>`, updated and pruned each
  frame with `std::for_each` and `std::remove_if`.
- **`AudioBuffer`** (`audio_buffer.hpp`/`.cpp`) — owns a raw dynamically
  allocated sample array, movable but not copyable, with free functions
  to fill it with a sine wave, a harmonic tone, or mix two buffers
  together for chords.
- **`mapFrequency<T>`** (`pitch_mapper.hpp`) — a function template that
  maps a frequency in a given range to a `Color`, throwing
  `InvalidFrequencyException` if the frequency is out of range.
- **`colorForChar`** (`grapheme_mapper.hpp`) — maps a letter to a fixed
  color spread evenly around a hue wheel, used by text mode.

## Concepts Demonstrated

This project was built for a C++ mini-project assignment requiring at
least 6 of the following concepts to be implemented and explained. It
demonstrates 7:

| Concept | Description | Where |
|---|---|---|
| D | Class with access specifiers, constructor, destructor | `Color` |
| E | 4+ overloaded operators | `Color` (`+`, `-`, `+=`, `==`, `!=`, `<<`) |
| C | Two exception types thrown/caught | `AudioLoadException`, `InvalidFrequencyException` |
| O | Class and function template | `mapFrequency<T>()` |
| F | Inheritance and polymorphism | `Shape` → `Circle`/`Square`/`Ripple` |
| I | Smart pointer, no manual `new`/`delete` | `Scene`'s `std::deque<std::unique_ptr<Shape>>` |
| K | STL algorithm with a lambda | `std::for_each`/`std::remove_if` in `Scene::update` |


## Known Limitations

- Audio requires real audio hardware; it will not play on a headless
  remote development environment, though the program detects this and
  continues running silently rather than crashing.
- Color rendering requires a terminal with 256-color ANSI support; true
  24-bit color is not used since it is not reliably supported across
  common terminal emulators.