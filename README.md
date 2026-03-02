# cs4212 Ray Tracer

A CPU ray tracer supporting perspective cameras, triangle and sphere primitives, and Lambertian, Blinn-Phong, and mirror shading. Output is a PNG image.

---

## Dependencies

The following must be installed before building:

- CMake 3.22+
- A C++20 compiler (GCC via MinGW-w64 on Windows, or GCC/Clang on Linux/macOS)
- [vcpkg](https://github.com/microsoft/vcpkg) with the `x64-mingw-static` triplet (Windows), or your system package manager (Linux/macOS)
- libpng
- Boost (`program_options` component)
- OpenGL
- GLEW
- GLM
- GLFW3

### Installing dependencies via vcpkg (Windows)

```bash
vcpkg install libpng boost-program-options glew glm glfw3 --triplet x64-mingw-static
```

### Installing dependencies via apt (Ubuntu/Debian)

```bash
sudo apt install libpng-dev libboost-program-options-dev libglew-dev libglm-dev libglfw3-dev
```

---

## Building

```bash
# Clone the repository
git clone <repo-url>
cd cs4212StarterCode

# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# If using vcpkg on Windows, add the toolchain file:
# cmake -B build -DCMAKE_BUILD_TYPE=Release \
#   -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build
```

---

## Running

```bash
./testing.exe [width] [height] [filename]
```

### Arguments

| Argument   | Description           | Default       |
|------------|-----------------------|---------------|
| `width`    | Image width in pixels | `100`         |
| `height`   | Image height in pixels| `100`         |
| `filename` | Output PNG filename   | `Default.png` |

- All arguments are positional and optional.
- If `filename` does not end in `.png`, the extension is appended automatically.
- Width and height must be greater than zero.

### Examples

```bash
# Default settings (100x100, outputs Default.png)
./testing.exe

# Custom resolution
./testing.exe 1920 1080

# Custom resolution and filename
./testing.exe 1920 1080 output.png

# Help
./testing.exe --help
```

---

## Default Scene

The default scene contains:

- A ground plane (green, Lambertian)
- A center sphere (white, mirror)
- A right sphere (blue, Lambertian)
- A left sphere (red, Blinn-Phong)
- A point light at `(5, -15, 0)`
- Camera positioned at `(0, 0, -5)`

---

## Running Unit Tests

```bash
cd build
ctest --output-on-failure
```

---

## Optional Static Analysis

```bash
# Enable cppcheck
cmake -B build -DENABLE_CPPCHECK=ON

# Enable clang-tidy (requires clang-tidy-13)
cmake -B build -DENABLE_CLANG_TIDY=ON
```

---

## Project Structure

```
cs4212StarterCode/
├── graphicsLib/  # Core ray tracer (camera, scene, shaders, primitives)
├── src/          # Main entry point
├── examples/     # Example programs
├── OpenGL/       # OpenGL viewer
├── utests/       # Catch2 unit tests
├── cmake/        # CMake modules
└── CMakeLists.txt
```

