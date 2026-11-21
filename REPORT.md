# Cozy Room Diorama — Report

## 1. The idea

We made a small 3D cartoon room. The room lives through one full day. The sun moves behind the window. Light bands come from the window and cross the room. At night the lamp becomes more important.

The project is written in C++17. We use OpenGL 3.3 Core, GLFW, GLAD, GLM, Dear ImGui and CMake.

## 2. How to run

1. Open the `CozyRoomDiorama` folder in CLion.
2. Choose MinGW, for example `C:\msys64\mingw64`.
3. Reload CMake.
4. Build and run the `cozy_room` target.

You can also use the command line. The commands are in `README.md`. All libraries are in `external/`, so you do not need to install them.

## 3. The room

The room is 6 × 6 × 4 meters. It has a wooden floor and cream walls. The floor and the walls use simple procedural patterns.

We modelled the furniture in code:

- a bed with a pink blanket;
- a small white cat that sleeps on the bed;
- a desk and a chair;
- a bookshelf;
- a plant;
- a desk lamp;
- a mug with steam;
- a pink yoga mat and two small dumbbells.

We kept a low-poly 3D cartoon style. All shapes are simple spheres, cubes, cones and cylinders.

## 4. Day and night

There is a day and night cycle. The sun moves from left to right. The sky changes from light blue to dark blue.

When **Day / night** is on, we can change the speed with **Day speed**. When it is off, the **Time** slider sets the hour from 0 to 24. In the panel we show a small hint: 6 h is sunrise, 12 h is noon, 18 h is sunset.

The window light changes with the time. It is cool in the day and warm near sunset. At night the sun light is gone and the desk lamp gives warm light.

## 5. The sun rays

The most important effect is the sun rays. Nine wide golden light bands start across the whole window. They fan out into the room. They reach the floor, the bed, the carpet and the furniture.

The bands are open volumetric shapes. We made a special mesh without a floor cap, so the rays do not look like a bright spot on the floor. Each band has a soft bright place where it touches a surface.

The rays follow the sun:

- in the morning they are low and pale;
- at noon they are higher and brighter;
- in the evening they are low and orange;
- at night they disappear;
- in the morning they slowly come back.

We did not add dust, smoke, fog, sparkles or particles to the rays.

## 6. Controls

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

## 7. How the code is organised

- `Scene.cpp` builds the room and updates all objects.
- `DayNight.cpp` computes the sun position, the light colour and the ray direction.
- `Atmosphere.cpp` updates the rain, steam, Z letters, curtains and light bands.
- `Primitives.cpp` creates the simple meshes.
- `fragment.frag` has the Blinn-Phong light and a special mode for the light bands.
- `main.cpp` starts the window, draws the ImGui panel and keeps the FPS counter in the top right corner.

The FPS counter is always visible. It is green at 60+ FPS, yellow at 30–60 FPS and red below 30 FPS.

## 8. What we learned

We learned how to build a small OpenGL scene from scratch. We learned how to use a shader, simple meshes and basic light. We also learned how to connect an effect to a value that changes in time.

The hardest part was the sun rays. First, the rays looked like a spot on the floor. Then we found that their world matrix was not updated. We fixed that and changed the mesh to open light bands. Now the path from the window to the floor is visible.

## 9. AI use

We used OpenCode (Big Pickle) as an AI assistant. It helped us to check the rules, write code, fix the build, make the README, this report and the presentation. We checked all changes. We built the project and ran it. We made all design choices ourselves.

The AI did not give us models or textures. All shapes and the checker texture are made in code.

## 10. Conclusion

The project is ready. The room keeps its cartoon style. The day and night cycle works. The wide golden rays start at the window and move with the sun. All files are in GitHub.

Shadow mapping is a stretch goal. We did not implement it. We may add it later if we have time.
