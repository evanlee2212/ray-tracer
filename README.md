# cs4212 Ray Tracer

A CPU ray tracer supporting perspective cameras, triangle and sphere primitives, and Lambertian, Blinn-Phong, and mirror shading. Output is a PNG image. Scenes can be defined programmatically or loaded from JSON scene files.

Also includes an OpenGL rasterizer for real-time interactive scene viewing.

<img width="253" height="252" alt="image" src="https://github.com/user-attachments/assets/ac4da7ec-aa0f-4f7e-b177-95999ab4eb0d" />
<img width="197" height="199" alt="image" src="https://github.com/user-attachments/assets/248ca5a4-0223-45d4-9ff4-bce7339f85d6" />


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
- [nlohmann/json](https://github.com/nlohmann/json)

### Installing dependencies via vcpkg (Windows)
```bash
vcpkg install libpng boost-program-options glew glm glfw3 nlohmann-json --triplet x64-mingw-static
```

### Installing dependencies via apt (Ubuntu/Debian)
```bash
sudo apt install libpng-dev libboost-program-options-dev libglew-dev libglm-dev libglfw3-dev nlohmann-json3-dev
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

### Programmatic scene (testing.exe)
```bash
./testing.exe [width] [height] [filename]
```

| Argument   | Description            | Default       |
|------------|------------------------|---------------|
| `width`    | Image width in pixels  | `100`         |
| `height`   | Image height in pixels | `100`         |
| `filename` | Output PNG filename    | `Default.png` |

- All arguments are positional and optional.
- If `filename` does not end in `.png`, the extension is appended automatically.
- Width and height must be greater than zero.
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

### JSON scene loader (sceneLoaderTest.exe)
```bash
./sceneLoaderTest.exe <scene_file.json>
```

- Takes a single JSON scene file as input.
- Outputs a PNG to the same directory as the input file, with the `.json` extension replaced by `.png`.
```bash
./sceneLoaderTest.exe scenes/oneSphere.json
# outputs: scenes/oneSphere.png
```

#### JSON Scene Format
```json
{
  "scene": {
    "camera": [{
      "position": "0 0 0",
      "viewDir": "0 0 -1",
      "focalLength": 1.0,
      "imagePlaneWidth": 0.5,
      "_name": "main",
      "_type": "perspective"
    }],
    "light": [{
      "position": "0 10 0",
      "intensity": "1.0 1.0 1.0",
      "_type": "point"
    }],
    "shader": [{
      "diffuse": "0 0 1",
      "_name": "blue",
      "_type": "Lambertian"
    }],
    "shape": [{
      "shader": { "_ref": "blue" },
      "center": "0 0 -5",
      "radius": 1,
      "_name": "sphere1",
      "_type": "sphere"
    }]
  }
}
```

Supported types:
- **Cameras:** `perspective`
- **Lights:** `point`
- **Shaders:** `Lambertian`, `BlinnPhong`, `Mirror`
- **Shapes:** `sphere`, `triangle`, `box`, `mesh`, `instance`

---

### OpenGL Rasterizer (rasterizer.exe)

<img width="500" height="500" alt="image" src="https://github.com/user-attachments/assets/f3c2bf11-4384-4cc3-be88-6291233e8c4f" />


The rasterizer provides a real-time interactive view of your scene using OpenGL.

```bash
./rasterizer.exe
```

Place your shader files in the same directory as the executable before running:
- `vertexShader_PrepForPerFragment.glsl`
- `fragmentShader_Lambertian.glsl`

To load a triangle mesh from a file, place `trillist.dat` (comma-separated vertex positions) in the same directory as the executable.

#### Camera Controls

| Key | Action |
|-----|--------|
| `E` | Move forward |
| `Q` | Move backward |
| `A` | Strafe left |
| `D` | Strafe right |
| `W` | Move up |
| `S` | Move down |
| `←` Left Arrow | Rotate camera left (yaw) |
| `→` Right Arrow | Rotate camera right (yaw) |
| `↑` Up Arrow | Rotate camera up (pitch) |
| `↓` Down Arrow | Rotate camera down (pitch) |
| `T` | Print current FPS to console |
| `Escape` | Close the window |

---

## Default Scene (testing.exe)

The default scene contains:

- A ground plane (green, Lambertian)
- A center sphere (white, mirror)
- A right sphere (blue, Lambertian)
- A left sphere (red, Blinn-Phong)
- A point light at `(5, -15, 0)`
- Camera positioned at `(0, 0, -5)`

---

## Ray Tracer vs. Rasterizer: Visual Differences

The ray tracer and rasterizer produce images of the same scene but differ significantly in how they compute the final image.

### Rendering Approach

The ray tracer simulates light by casting rays from the camera into the scene and computing intersections mathematically. The rasterizer works in the opposite direction — it transforms geometry through a series of matrix operations and projects triangles onto the screen, then shades each pixel using interpolated per-fragment values.

### Shadows

The ray tracer supports **hard shadows** natively. When a ray hits a surface, a shadow ray is cast toward each light source. If that ray is blocked, the point is in shadow. The rasterizer as implemented here has **no shadow support** — all geometry is lit regardless of whether other objects are between it and the light.

### Reflections and Mirror Shading

The ray tracer supports **recursive mirror reflections** by spawning secondary rays. The rasterizer does not support reflections — mirror surfaces cannot be represented without additional techniques like cube maps or screen-space reflections, none of which are implemented here.

### Shading Quality

Both renderers implement Blinn-Phong shading, but the ray tracer computes lighting at exact ray-surface intersection points. The rasterizer uses **per-fragment shading**, interpolating surface normals and light directions across each triangle. The results are visually similar for dense meshes but the rasterizer can show shading artifacts on coarse geometry.

### Geometry Representation

The ray tracer intersects rays against mathematical primitives — spheres are perfectly smooth regardless of complexity. The rasterizer approximates all geometry as **triangle meshes**, so spheres appear faceted unless subdivided finely enough.

### Performance

The rasterizer runs in **real time** (60+ FPS) by exploiting GPU parallelism. The ray tracer is a CPU-based offline renderer — even a simple scene at moderate resolution can take seconds to minutes to render. The tradeoff is visual fidelity: the ray tracer produces physically accurate shadows, reflections, and lighting that the rasterizer cannot match without significant additional complexity.

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
├── graphicsLib/      # Core ray tracer (camera, scene, shaders, primitives)
├── JSONSceneLoader/  # JSON scene parser and loader
├── src/              # Main entry point
├── examples/         # Example programs
├── OpenGL/           # OpenGL rasterizer
├── utests/           # Catch2 unit tests
├── cmake/            # CMake modules
└── CMakeLists.txt
```

