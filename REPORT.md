# Cozy Room Diorama - Report

Final project for Computer Graphics. Simple English (B1).

## 1. Introduction

Cozy Room Diorama is a small 3D cartoon bedroom. The room changes during one full day. The sun moves behind the window. Wide golden light bands come from the window and cross the room. At night the sun light disappears and the desk lamp gives warm light.

The project is written in C++17 with OpenGL 3.3 Core. We use GLFW, GLAD, GLM, Dear ImGui and CMake. All libraries are in the project folder.

## 2. How to run

1. Open the CozyRoomDiorama folder in CLion.
2. Choose MinGW and reload CMake.
3. Build and run the cozy_room target.

The program opens a window with the room, an ImGui panel and an FPS counter in the top right corner.

## 3. The room

The room is 6 x 6 x 4 meters. It has a wooden floor and cream walls. The patterns are made in code, so no image files are needed.

Inside the room there is a bed with a pink blanket, a sleeping white cat, a desk and a chair, a bookshelf, a plant, a desk lamp, a mug with steam, a yoga mat, two dumbbells and a ball. All objects are made from simple low-poly shapes.

## 4. Day and night

The sun moves from left to right. The sky changes from light blue in the day to dark blue at night.

With Day / night on, Day speed changes how fast the day goes. With Day / night off, the Time slider sets the hour from 0 to 24. The panel shows the hint: 6 h sunrise, 12 h noon, 18 h sunset.

The window light follows the sun. It is cool and bright at noon and warm near sunset. At night only the lamp gives light.

## 5. The sun rays

The main effect is nine wide golden light bands. They start across the whole window in a 3 x 3 grid and fan out into the room. They reach the floor, the bed, the carpet and the furniture.

Each band is an open shape with four sides. We do not use a closed cone, because it makes a bright spot on the floor. A special shader mode makes the bands look like flat cartoon light.

In the morning the rays are low and pale. At noon they are higher and brighter. In the evening they are low and orange. At night they disappear and come back in the morning. We did not add dust, smoke, fog or particles.

## 6. Controls

| Key | Action |
| --- | --- |
| Tab | Change camera mode |
| F5 | Show or hide the panel |
| B | Start or stop the ball |
| R | Rain on or off |
| C | Curtains open or closed |
| L | Desk lamp on or off |
| Esc | Exit |

The mouse changes the camera. There are Orbit and Free camera modes.

## 7. Code structure

Scene.cpp builds the room and updates all objects. DayNight.cpp computes the sun, the daylight and the ray colour. Atmosphere.cpp updates rain, steam, Z letters, curtains and light bands. Primitives.cpp creates the meshes. main.cpp opens the window, draws the panel and keeps the FPS counter.

## 8. Conclusion

The project is finished. The room keeps its cartoon style. The day and night cycle works. The wide rays start at the window and move with the sun. The lamp has its own key. Shadow mapping is a stretch goal and is not implemented.
