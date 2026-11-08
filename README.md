# Cozy Room Diorama

Финальный проект по компьютерной графике: C++17, OpenGL 3.3 Core, GLFW 3.4, GLAD, GLM 1.0.1, Dear ImGui 1.93.0, CMake.

## Что реализовано

- Диорама комнаты 6×6×4 м: пол, две стены и оконный проем.
- Два режима камеры: Orbit и Free.
- Освещение по модели Блинна-Фонга: теплая настольная лампа, холодный свет из окна и мягкая подсветка.
- Панель управления Dear ImGui с параметрами света, материала и заглушками анимаций.

## Структура

```
CozyRoomDiorama/
├── CMakeLists.txt
├── include/
│   ├── gfx/Shader.h
│   └── scene/
│       ├── Camera.h
│       ├── Lighting.h
│       └── Room.h
├── src/
│   ├── app/main.cpp
│   ├── gfx/Shader.cpp
│   └── scene/
│       ├── Camera.cpp
│       ├── Lighting.cpp
│       └── Room.cpp
├── shaders/
│   ├── vertex.vert
│   └── fragment.frag
└── external/glad/
```

## Сборка в CLion

1. Откройте папку `CozyRoomDiorama` в CLion.
2. В Toolchains выберите MinGW, например `C:\msys64\mingw64`.
3. Нажмите Reload CMake Project. GLFW, GLM и Dear ImGui скачаются автоматически при первом конфигурировании.
4. Соберите и запустите цель `cozy_room`.

## Сборка из консоли

```powershell
$env:Path = "C:\msys64\mingw64\bin;$env:Path"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
.\build\bin\cozy_room.exe
```

## Управление

| Ввод | Действие |
|------|----------|
| Tab | Переключение Orbit / Free |
| F5 | Открыть или закрыть панель ImGui |
| ЛКМ + мышь | Вращение Orbit-камеры |
| Колесо мыши | Приближение Orbit-камеры |
| W, A, S, D | Движение камеры или цели |
| Q, E | Вниз / вверх |
| Мышь | Обзор в режиме Free |
| Esc | Выход |
