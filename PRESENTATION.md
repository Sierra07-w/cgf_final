# Cozy Room Diorama — Presentation

Short English (B1). You can use these slides in PowerPoint or Google Slides.

---

## Slide 1 — Title

**Cozy Room Diorama**

A small 3D cartoon room that lives through a full day.

C++17 · OpenGL 3.3 Core · GLFW · GLAD · GLM · ImGui

---

## Slide 2 — The idea

- We wanted a warm, cozy bedroom.
- The sun moves during the day.
- Light comes through the window.
- We wanted to see the light path inside the room.
- At night the lamp becomes important.

---

## Slide 3 — The room

- Room size: 6 × 6 × 4 meters.
- Wooden floor and cream walls.
- Bed, pink blanket and a sleeping cat.
- Desk, chair, bookshelf and plant.
- Lamp, mug, yoga mat and dumbbells.
- Low-poly 3D cartoon style.

---

## Slide 4 — Day and night

- The sun moves from left to right.
- The sky changes from light blue to dark blue.
- Day speed can be changed in the panel.
- When the cycle is off, we use the Time slider.
- 6 h sunrise · 12 h noon · 18 h sunset.

---

## Slide 5 — The sun rays

- Nine wide golden light bands.
- They start across the whole window.
- They fan out into the room.
- They touch the floor, bed, carpet and furniture.
- No dust, smoke, fog or particles.

---

## Slide 6 — Rays during the day

- Morning: low angle, pale light.
- Noon: higher angle, bright light.
- Evening: low angle, warm orange light.
- Night: no sun rays.
- Morning: rays slowly come back.

---

## Slide 7 — Controls

- **F5** — show or hide the panel.
- **Tab** — change camera mode.
- **B** — roll the ball.
- **R** — rain on or off.
- **C** — curtains open or closed.
- **L** — desk lamp on or off.
- **Esc** — exit.

---

## Slide 8 — Technical part

- C++17 and OpenGL 3.3 Core.
- Simple meshes made in code.
- Blinn-Phong light in the fragment shader.
- `DayNight.cpp` computes sun and colours.
- `Atmosphere.cpp` updates effects and rays.
- FPS counter is top right and always visible.

---

## Slide 9 — What was difficult

- The first rays looked like a spot on the floor.
- We found a missing world matrix update.
- We changed cone meshes to open light bands.
- We added a soft bright place on surfaces.
- Now the path from window to floor is clear.

---

## Slide 10 — Result

- Cozy room with furniture and a cat.
- Full day and night cycle.
- Time slider and lamp key.
- Wide cartoon rays from the whole window.
- Simple English README, report and presentation.
- All files are in GitHub.

---

## Speaker notes (short)

We built a small cozy bedroom in OpenGL. The room has many objects, but the main part is the day and night cycle. The sun moves and the light bands move with it. We made nine wide light bands. They start in the window and go to the floor, the bed and the carpet. At night they disappear. The desk lamp gives warm light. We used AI help for code and text, but we checked everything ourselves.
