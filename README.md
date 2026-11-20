# Cozy Room Diorama — a small cozy bedroom

This is our final project for Computer Graphics.  
We use C++17, OpenGL 3.3 Core, GLFW, GLAD, GLM, Dear ImGui and CMake.

## What we made

- The room is 6 × 6 × 4 m. It has a wooden floor, cream walls and a white window. There is a bed with a pink blanket with white dots. A small white cat sleeps on the bed. We made the cat from spheres, cones and a curved tail. There is also a desk, a chair, a bookshelf, a plant, a lamp and a mug with steam. Under the window there is a pink yoga mat. Two small dumbbells are near it.
- It rains outside the window. Leaves move. Steam goes up from the mug. The lamp can flicker. The cat breathes slowly. Three letters `Z` fly up and fade out.
- There is a day and night cycle. The sun moves behind the window. The light changes too. Use **Day / night** and **Day speed** in the panel. If the cycle is off, you can set the light by hand.
- The FPS counter is always in the top right corner. It is green at 60+ FPS, yellow at 30–60 and red below 30. It also shows the frame time in ms.
- A ball rolls when you press **Roll ball** or **B**. You can change its speed with **Ball speed**. Press **Stop ball** or **B** again to stop it.
- Click the cat. It jumps, moves its ears, opens its mouth and shows `*growl*`. Then it goes back to sleep.
- There are two camera modes: Orbit and Free.
- We use Blinn-Phong light: a warm lamp, cool window light and soft fill light.
- The ImGui panel controls rain, lamp, ball, light, material and lamp position.

## Project structure

```
CozyRoomDiorama/
├── CMakeLists.txt
├── CMakePresets.json
├── include/
│   ├── gfx/
│   │   ├── Mesh.h
│   │   ├── Shader.h
│   │   └── Texture.h
│   └── scene/
│       ├── Camera.h
│       ├── DayNight.h
│       ├── Lighting.h
│       ├── Primitives.h
│       ├── Room.h
│       ├── Scene.h
│       └── SceneNode.h
├── src/
│   ├── app/main.cpp
│   ├── gfx/
│   │   ├── Mesh.cpp
│   │   ├── Shader.cpp
│   │   └── Texture.cpp
│   └── scene/
│       ├── Camera.cpp
│       ├── DayNight.cpp
│       ├── Lighting.cpp
│       ├── Primitives.cpp
│       ├── Room.cpp
│       ├── Scene.cpp
│       └── SceneNode.cpp
├── shaders/
│   ├── vertex.vert
│   └── fragment.frag
└── external/
    ├── glad/
    ├── glfw/
    ├── glm/
    └── imgui/
```

GLFW, GLM, GLAD and Dear ImGui are in `external/`. You do not need to install them.

## Build in CLion

1. Open the `CozyRoomDiorama` folder in CLion.
2. Choose MinGW in Toolchains, for example `C:\msys64\mingw64`.
3. Click **Reload CMake Project**.
4. Build and run the `cozy_room` target.

## Build from the command line

```powershell
$env:Path = "C:\msys64\mingw64\bin;$env:Path"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
.\build\bin\cozy_room.exe
```

## Controls

| Input | Action |
|-------|--------|
| Tab | Change camera mode |
| F5 | Show or hide the panel |
| B | Start or stop the ball |
| Left mouse button + drag | Turn the Orbit camera |
| Mouse wheel | Zoom in or out |
| W, A, S, D | Move the camera |
| Q, E | Move down / up |
| Mouse | Look around in Free mode |
| Esc | Exit |

## AI Use

We used an AI assistant for this project. The tool was OpenCode (Big Pickle).

The AI helped us to:

- check the project against the course rules;
- write the FPS counter, the day/night cycle and the build fix;
- move the day/night code into its own file;
- write this README.

We checked all changes. We built the project and ran it. We tested the controls. We made all design choices ourselves.

The AI did not give us any models or textures. We made all shapes and the checker texture in code.
