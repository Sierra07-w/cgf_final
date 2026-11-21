# Cozy Room Diorama — Presentation

**Student:** [add your name here]  
**Language:** simple English (B1)

This file is the full text of the presentation. It has 12 slides. Under every slide there are short speaker notes. You can copy the slides to PowerPoint, Google Slides or use `PRESENTATION.html` in the same folder.

---

## Slide 1 — Title

**Cozy Room Diorama**

A small 3D cartoon room that lives through one full day.

C++17 · OpenGL 3.3 Core · GLFW · GLAD · GLM · ImGui · CMake

*Speaker notes:*  
Hello. Today I will show my final computer graphics project. It is a cozy bedroom. The sun moves, the light comes through the window, and the room changes from day to night.

---

## Slide 2 — The idea

- I wanted to make a warm and cozy room.
- The room should live during one day.
- The sun should move behind the window.
- The light should be visible inside the room.
- At night the lamp should become important.

*Speaker notes:*  
The main goal was not only to build a room, but to show time with light. I wanted the light path from the window to the floor to be clear.

---

## Slide 3 — The room

- Room size: 6 × 6 × 4 meters.
- Wooden floor and cream walls.
- Bed, pink blanket and a small white cat.
- Desk, chair, bookshelf and plant.
- Lamp, mug, yoga mat and dumbbells.
- Low-poly 3D cartoon style.

*Speaker notes:*  
All objects are made in code from simple shapes: cubes, spheres, cones and cylinders. The floor and walls use simple patterns made by code, not image files.

---

## Slide 4 — The small animations

- The cat breathes slowly.
- If you click the cat, it jumps and shows `*growl*`.
- Rain falls outside the window.
- Steam goes up from the mug.
- Three letters `Z` fly up and fade out.
- The plant leaf moves a little.

*Speaker notes:*  
These details are small, but they make the room feel alive. The rain, steam and Z letters are old scene effects. They are not part of the sun rays.

---

## Slide 5 — Day and night

- The sun moves from left to right.
- The sky changes from light blue to dark blue.
- **Day speed** changes the speed of the day.
- When the cycle is off, use the **Time** slider.
- 6 h sunrise · 12 h noon · 18 h sunset.

*Speaker notes:*  
There are two modes. With Day / night on, the sun moves by itself. With Day / night off, I can choose any hour from 0 to 24 with the slider.

---

## Slide 6 — Light colours

- Noon: cool and bright light.
- Evening: warm and orange light.
- Night: no sun light.
- The desk lamp gives warm light at night.
- The change follows the sun position.

*Speaker notes:*  
The window light and the ray colour are computed from the same time value. This is why the sky, the sun and the rays always match.

---

## Slide 7 — The sun rays

- Nine wide golden light bands.
- Three columns and three rows across the window.
- The rays fan out into the room.
- They touch the floor, bed, carpet and furniture.
- No dust, smoke, fog or particles.

*Speaker notes:*  
This is the most important effect. The light comes from the whole window, not from one small point. I removed dust and particles by request, so the rays are clean.

---

## Slide 8 — Rays during the day

- Morning: low angle, pale yellow.
- Noon: higher angle, brightest.
- Evening: low angle, warm orange.
- Night: no rays at all.
- Next morning: rays slowly come back.

*Speaker notes:*  
The bands move with the sun. They do not stay in one place. This shows the time of day without any text.

---

## Slide 9 — How the rays are made

- Each band is an open shape with four sides.
- The top is at the window.
- The bottom is near the surface inside the room.
- A closed cone made a bright spot, so I changed it.
- A special shader mode makes a flat cartoon colour.

*Speaker notes:*  
The open shape shows the whole path of the light. The special shader mode makes the bands bright, like cartoon rays, not like real glass.

---

## Slide 10 — Controls

- **F5** — show or hide the panel.
- **Tab** — change camera mode.
- **B** — roll the ball.
- **R** — rain on or off.
- **C** — curtains open or closed.
- **L** — desk lamp on or off.
- **Esc** — exit.

*Speaker notes:*  
All keys are simple. The mouse changes the camera. The FPS counter is always in the top right corner.

---

## Slide 11 — Technical part

- C++17 and OpenGL 3.3 Core.
- `Scene.cpp` builds and updates the scene.
- `DayNight.cpp` computes the sun and colours.
- `Atmosphere.cpp` updates effects and rays.
- `fragment.frag` has the light and the ray mode.
- FPS counter: green, yellow or red.

*Speaker notes:*  
`Scene.cpp` is exactly 500 lines. The code is split into small files so it is easy to change one part without breaking the others.

---

## Slide 12 — Result

- Cozy cartoon room with furniture and a cat.
- Full day and night cycle.
- Time slider and lamp key.
- Wide golden rays from the whole window.
- Detailed report and presentation in the project folder.
- All files are pushed to GitHub.

*Speaker notes:*  
I think the project is finished. The main stretch goal, shadow mapping, is not done yet, but the warm and cold colour change is done. Thank you for watching.

---

## Short conclusion for the teacher

The project shows a small room during one day. The sun moves, the sky changes colour, and nine wide light bands cross the room. At night the rays disappear and the lamp gives warm light. I made all models in code and used AI help for code and text. I checked every change and tested the program.
