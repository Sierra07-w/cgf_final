# Cozy Room Diorama — Project Report

**Course:** Computer Graphics  
**Project:** Cozy Room Diorama  
**Student:** [add your name here]  
**Date:** November 2026  

---

## 1. The main idea

Our project is a small 3D cartoon bedroom. The room changes during one full day. The sun moves behind the window. Wide light bands come from the window and cross the room. In the evening the light becomes warm and orange. At night the sun light is gone and the desk lamp gives warm light.

The goal was to make a cozy scene with simple low-poly shapes and a beautiful, easy-to-see sun-ray effect. We wanted the rays to be a real path from the window to the floor, the bed and the carpet. We did not want only a bright floor spot.

---

## 2. Tools and libraries

We used:

- **C++17** as the programming language;
- **OpenGL 3.3 Core** for rendering;
- **GLFW** for the window, keyboard and mouse;
- **GLAD** to load OpenGL functions;
- **GLM** for vectors and matrices;
- **Dear ImGui** for the control panel and the FPS counter;
- **CMake** and **Ninja** to build the project;
- **MinGW-w64** as the compiler on Windows.

All libraries are inside the `external/` folder. A fresh clone builds without installing anything.

---

## 3. How to build and run

### In CLion

1. Open the `CozyRoomDiorama` folder.
2. Choose MinGW in Toolchains, for example `C:/msys64/mingw64`.
3. Click **Reload CMake Project**.
4. Build and run the `cozy_room` target.

### From the command line

```powershell
$env:Path = "C:\msys64\mingw64\bin;$env:Path"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
.\build\bin\cozy_room.exe
```

---

## 4. The room and the furniture

The room is 6 × 6 × 4 meters. The floor is wooden. The walls are cream. The floor and the walls use simple patterns made in code, so we do not need image files.

We built these objects with simple meshes:

- a bed with a pink blanket and white dots;
- a small white cat that sleeps on the bed;
- a desk and a chair near the right wall;
- a bookshelf with books on the left wall;
- a green plant in a pot;
- a desk lamp on the desk;
- a mug with steam;
- a pink yoga mat under the window;
- two small dumbbells near the mat;
- a ball that can roll;
- two pictures on the wall.

All objects stay inside the room. We moved the bookshelf closer to the middle, because its left side was outside the room.

---

## 5. Animation and interaction

The scene is alive:

- the cat breathes slowly;
- if you click the cat, it jumps, moves its ears, opens its mouth and shows `*growl*`;
- rain falls outside the window;
- steam goes up from the mug;
- three letters `Z` fly up and fade out;
- the plant leaf moves a little;
- the ball rolls when you press **B**;
- the curtains open and close with **C**;
- the lamp turns on and off with **L**.

These animations are small and simple, but they make the room feel cozy.

---

## 6. Day and night

The sun moves from left to right behind the window. The sky colour changes with the sun:

- day — light blue;
- night — dark blue.

There are two ways to control the time:

1. **Day / night is on.** The sun moves by itself. **Day speed** changes how fast the day goes. This is our time-lapse effect.
2. **Day / night is off.** The **Time** slider sets the hour from 0 to 24.  
   6 h is sunrise, 12 h is noon, 18 h is sunset. The panel shows this hint under the slider.

The window light also changes. It is cool and bright at noon. Near sunset it becomes warm. At night the sun light disappears and the lamp becomes stronger.

---

## 7. The sun rays

This is the main visual effect of the project.

There are **nine wide golden light bands**. They start at different places across the window: three columns and three rows. So the light comes from the whole window, not from one corner. The bands fan out into the room and touch the floor, the bed, the carpet and the furniture.

Each band is an open volume with four side faces. The top face is at the window. The bottom face is near the surface inside the room. We do not use a closed cone, because a closed cone makes a bright spot on the floor. The open shape shows the whole path of the light.

We use a special shader mode for the bands. Instead of normal Blinn-Phong light, the bands use a bright, flat colour. This makes them look like cartoon light. The colour is brighter near the window and softer at the far end.

The rays follow the time of day:

| Time | What happens |
|------|--------------|
| Morning | Low angle, pale yellow light |
| Noon | Higher angle, brightest light |
| Evening | Low angle, warm orange light |
| Night | No rays at all |
| Next morning | Rays slowly come back |

We did not add dust, smoke, fog, sparkles or particles. The user asked us not to do this. The rain, steam and `Z` letters are old scene effects, not part of the rays.

---

## 8. Technical structure

The code is split into several files:

- **Scene.cpp** builds the room and updates objects. It is exactly 500 lines.
- **DayNight.cpp** computes the sun position, the daylight value, the ray direction and the ray colour.
- **Atmosphere.cpp** updates rain, steam, Z letters, curtains and light bands.
- **Primitives.cpp** creates cubes, spheres, planes, cylinders, cones and light bands.
- **SceneNode.cpp** keeps local and world matrices for each object.
- **fragment.frag** contains the Blinn-Phong light and the special mode for the light bands.
- **main.cpp** opens the window, draws the ImGui panel, handles keys and keeps the FPS counter.

The FPS counter is always in the top right corner. It is green at 60+ FPS, yellow from 30 to 60 FPS and red below 30 FPS. It also shows the frame time in milliseconds.

---

## 9. Controls

| Key | Action |
|-----|--------|
| Tab | Change camera mode |
| F5 | Show or hide the panel |
| B | Start or stop the ball |
| R | Turn rain on or off |
| C | Close or open the curtains |
| L | Turn the desk lamp on or off |
| Esc | Exit |

The mouse changes the camera. We have Orbit and Free camera modes.

---

## 10. Topic requirements

### Core

- A room with furniture models — **done**.
- Sun light through the window with a time control — **done** with the Time slider and Day speed.
- Desk lamp switched by a key — **done**, key **L**.
- Fairy lights switched by a key — **not done**. We made curtains with key **C** instead, by user request.
- Textured walls and floor — **done** with code patterns.

### Stretch

- Dust particles in the light beam — **not done**, removed by user request.
- Warm and cold colour change by time of day — **done**.
- Shadow mapping for the sun — **not done**. It is a possible future improvement.

### Wow moment

- A time-lapse from morning to night with lamps on — **done** in a simple way. Day speed makes the day fast, and the lamp becomes stronger at night.

---

## 11. Testing

We tested the project in these ways:

- We built the project with CMake and Ninja.
- We ran the program and checked that the window opens.
- We checked that the console has no shader errors.
- We tested keys **B**, **R**, **C**, **L**, **Tab**, **F5** and **Esc**.
- We tested the Time slider with the cycle off.
- We checked that the bookshelf is inside the room.
- We checked that `Scene.cpp` stays at 500 lines.

---

## 12. Problems and fixes

**Problem 1: rays looked like a floor spot.**  
The first ray mesh was a closed cone. The bottom cap looked like a bright spot. We changed it to an open light band and added a soft bright place on the surface.

**Problem 2: rays were drawn in the wrong place.**  
We set the local matrix but forgot to update the world matrix. The rays appeared near the origin. We added `updateWorld()` for each ray.

**Problem 3: the bookshelf was outside the room.**  
Its left edge was at x = -3.31 while the wall is at x = -3.0. We moved it to x = -2.25.

---

## 13. AI use

We used an AI assistant called OpenCode (Big Pickle). It helped us to:

- check the project against the course rules;
- write and fix C++ code;
- fix the fresh-clone build;
- make the day/night cycle and the light bands;
- write the README, this report and the presentation;
- find and fix the world matrix and bookshelf problems.

We checked every change. We built the project and ran it. We made all design choices ourselves. The AI did not give us models or textures. All shapes are made in code.

---

## 14. Conclusion

The project is finished. The room keeps its low-poly cartoon style. The day and night cycle works. The wide golden rays start at the whole window and move with the sun. At night they disappear and come back in the morning. The lamp has its own key. The report and presentation are in the project folder.

Shadow mapping is the main stretch goal we did not do. We can add it later if we have more time.
