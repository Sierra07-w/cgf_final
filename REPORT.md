# Cozy Room Diorama - Report

Final project for Computer Graphics. Simple English (B1).

## 1. A few words about the project

When we started this project, we wanted to build a small room that feels warm. Not a big city, not a complex game. Just one cozy bedroom with simple low-poly furniture. We wrote it in C++17 with OpenGL 3.3 Core. Every object is made from cubes, spheres, cones and cylinders, but together they make a nice little scene.

The main idea was time. We wanted the viewer to feel the day. The sun moves behind the window, the golden light bands move with it, the sky changes colour, and at night the desk lamp becomes the main light. This is why the project is called a diorama: it is a small world that lives through one full day.

> The most important rule for us was simple English and a clear picture. We did not want to hide the effect behind hard text. You open the program and you see the room.

## 2. What you see when the program starts

After the build, the cozy_room window opens. The camera looks at the room from a comfortable angle. In the top left corner there is a small panel called Cozy Room. In the top right corner there is always an FPS counter.

- The 3D room is in the middle of the window. You can rotate it with the mouse.
- The ImGui panel is in the top left. It can be hidden with F5.
- The FPS counter is in the top right. It is green at 60+ FPS, yellow from 30 to 60, and red below 30.
- The camera mode is written at the top of the panel: Orbit or Free.

At first the sun is low in the sky, so the rays come into the room at a small angle. If you turn on a fast day speed, the whole day passes in a few seconds. This is our time-lapse moment.

## 3. The room and its small details

The room is 6 by 6 meters and 4 meters high. The floor is wooden and the walls are cream. The floor and wall patterns are made in code, so we do not need image files.

We filled the room with things that make it feel lived in:

- A bed with a pink blanket and small white dots.
- A small white cat that sleeps on the bed and breathes slowly.
- A desk and a chair near the right wall.
- A bookshelf with books near the left wall. We moved it inside the room so it does not cross the wall.
- A green plant in a pot, a mug with steam, a desk lamp, a yoga mat and two dumbbells.
- A ball that can roll, rain outside the window, curtains and two pictures on the wall.

The style is low-poly 3D cartoon. We did not try to make real photos. Soft colours and simple shapes make the room cozy.

## 4. How day and night works

Inside the code we keep one number that says where the sun is. Every frame, when the cycle is on, this number becomes a little bigger. From it we get the sun position, the sky colour, the window light and the direction of the rays. This is why everything changes together.

| Time | What happens in the scene |
| --- | --- |
| 6 h | Sunrise. The sun is low, the light is pale yellow. |
| 12 h | Noon. The sun is high, the light is bright and cool. |
| 18 h | Sunset. The sun is low again, the light is warm orange. |
| 0 / 24 h | Night. The sun light is gone, the lamp gives warm light. |

There are two ways to control time. If Day / night is on, the sun moves by itself and Day speed changes how fast it moves. If Day / night is off, the Time slider appears. It goes from 0 to 24 hours. Under it we wrote a hint: 6h sunrise, 12h noon, 18h sunset.

> The Time slider is useful when you want to check one exact moment. For example, set 18 h to see the warm evening rays.

## 5. How the sun rays work

The rays are the most important effect. There are nine wide golden light bands. They start across the whole window in a 3 by 3 grid: three columns and three rows. So the light comes from the whole window, not from one small point. The bands fan out into the room and touch the floor, the bed, the carpet and the furniture.

At first we used closed cones. They looked like bright spots on the floor. Then we changed each ray to an open band with four sides. The top of the band is at the window. The bottom is near the surface inside the room. This open shape shows the whole path of the light.

A special shader mode draws the bands with a flat bright colour. It is not normal Blinn-Phong lighting. It looks more like cartoon light. The colour is brighter near the window and softer at the far end.

| Part of the day | Rays |
| --- | --- |
| Morning | Low angle, pale yellow, slowly grows. |
| Noon | Higher angle, brightest, clear path on the floor. |
| Evening | Low angle, warm orange, softer edges. |
| Night | No rays. They come back in the morning. |

We did not add dust, smoke, fog, sparkles or particles. The user asked us to keep the rays clean. The rain, steam and Z letters are old scene effects, not part of the rays.

![Sun rays](rays_check.png)
The room with the nine golden light bands.

## 6. Every key and button

This section explains what every control does. The keyboard keys and the checkboxes always show the same state. For example, if you press L, the Desk lamp checkbox becomes checked too.

### 6.1 Keyboard keys

| Key | What it does | What you see |
| --- | --- | --- |
| Tab | Changes the camera mode. | The text in the panel changes between Orbit and Free. In Free mode the mouse moves the camera more freely. |
| F5 | Shows or hides the ImGui panel. | Useful when you want a clean view or a screenshot. |
| B | Starts or stops the ball. | The ball rolls on the floor. The panel button text changes to Stop ball or Roll ball. |
| R | Turns rain on or off. | Rain drops appear or disappear outside the window. |
| C | Opens or closes the curtains. | The two curtain parts slide. With curtains closed, less window light comes in. |
| L | Turns the desk lamp on or off. | A warm light appears on the desk. At night it becomes the main light source. |
| Esc | Closes the program. | The window exits. |

### 6.2 Controls inside the panel

| Control | What it does | When to use it |
| --- | --- | --- |
| Rain | Same as key R. Shows rain outside. | For a calm, rainy mood. |
| Curtains | Same as key C. Moves curtains. | To cover the window and reduce the window light. |
| Desk lamp | Same as key L. Turns the lamp on. | For night scenes or a cozy evening. |
| Lamp flicker | Makes the lamp brightness change a little. | Adds a small living feeling to the lamp. |
| Day / night | Turns the automatic cycle on or off. | On: time-lapse. Off: choose an exact hour. |
| Day speed | Changes how fast the sun moves. | Higher value means a faster day. |
| Time | Appears only when the cycle is off. Sets the hour from 0 to 24. | To check sunrise, noon, sunset or night. |
| Roll ball / Stop ball | Toggles the moving ball. | To add motion to the room. |

> Tip: for the best view, turn the day speed up, watch the rays move, then press L at night. This shows the whole idea of the project.

## 7. The small animations

The room is not static. The cat breathes slowly. If you click the cat, it jumps, moves its ears, opens its mouth and shows a small growl text. Steam goes up from the mug. Three letters Z fly up and fade out near the cat. The plant leaf moves a little in the wind. These details are simple, but they make the scene feel alive.

## 8. How the code is organised

We split the code into several files. It is easier to change one part without breaking the others.

| File | Job |
| --- | --- |
| Scene.cpp | Builds the room and updates all objects. It is exactly 500 lines. |
| DayNight.cpp | Computes the sun position, daylight, ray direction and ray colour. |
| Atmosphere.cpp | Updates rain, steam, Z letters, curtains and light bands. |
| Primitives.cpp | Creates cubes, spheres, planes, cylinders, cones and light bands. |
| fragment.frag | Has Blinn-Phong light and the special mode for the rays. |
| main.cpp | Opens the window, draws the panel, handles keys and keeps the FPS counter. |

## 9. Problems we fixed

The first problem was the ray shape. Closed cones made a bright spot on the floor. We changed them to open light bands and added a soft bright place where the light touches a surface.

The second problem was the ray position. We set a local matrix but forgot to update the world matrix. The rays were drawn in the wrong place. After adding the world update, the bands started at the window again.

The third problem was the bookshelf. Its left side was outside the wall. We moved it to x = -2.25, so now it stands fully inside the room.

## 10. What we learned

We learned how to build a small OpenGL scene from scratch. We learned how meshes, shaders, lights and matrices work together. We also learned that a good effect needs many small fixes. The ray effect looked simple in our head, but it took the most time.

We also learned to keep the user in mind. A slider is not useful if the user does not know what 6 h or 18 h means. This is why we added the short hint under the Time slider.

## 11. AI use

We used OpenCode as an AI assistant. It helped us check the rules, write and fix code, find the world matrix problem and improve the text. We tested every change ourselves by building and running the program. The idea, the design and the final decisions were ours.

## 12. Conclusion

The project is finished. The room keeps its cozy low-poly style. The day and night cycle works. The nine wide golden rays start at the whole window and follow the sun. They disappear at night and come back in the morning. The lamp has its own key. Every button and slider is explained in this report.

Shadow mapping is a stretch goal. We did not implement it. We may add it later if we have more time.
