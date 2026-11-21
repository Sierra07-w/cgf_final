#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "gfx/Shader.h"
#include "gfx/Texture.h"
#include "scene/Camera.h"
#include "scene/Lighting.h"
#include "scene/Scene.h"

namespace {

Camera camera;
Lighting lighting;
Material roomMaterial;
SceneOptions sceneOptions;

bool showPanel = false;
bool firstMouse = true;
bool dragging = false;
bool leftWasDown = false;
double clickX = 0.0;
double clickY = 0.0;
float lastX = 640.0f;
float lastY = 360.0f;

void updateCursorMode(GLFWwindow* window) {
    const bool captureMouse = camera.getMode() == Camera::Mode::Free && !showPanel;
    glfwSetInputMode(window, GLFW_CURSOR,
                     captureMouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    firstMouse = true;
}

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (action != GLFW_PRESS) {
        return;
    }

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_TAB) {
        camera.toggleMode();
        updateCursorMode(window);
    }

    if (key == GLFW_KEY_F5) {
        showPanel = !showPanel;
        updateCursorMode(window);
    }

    if (key == GLFW_KEY_B) {
        sceneOptions.ballRolling = !sceneOptions.ballRolling;
    }

    if (key == GLFW_KEY_R) {
        sceneOptions.rainOn = !sceneOptions.rainOn;
    }

    if (key == GLFW_KEY_C) {
        sceneOptions.curtainsClosed = !sceneOptions.curtainsClosed;
    }

    if (key == GLFW_KEY_L) {
        sceneOptions.lampOn = !sceneOptions.lampOn;
    }
}

void mouseButtonCallback(GLFWwindow*, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        dragging = action == GLFW_PRESS;
    }
}

void cursorPositionCallback(GLFWwindow*, double xPosition, double yPosition) {
    if (firstMouse) {
        lastX = static_cast<float>(xPosition);
        lastY = static_cast<float>(yPosition);
        firstMouse = false;
        return;
    }

    const float offsetX = static_cast<float>(xPosition) - lastX;
    const float offsetY = static_cast<float>(yPosition) - lastY;
    lastX = static_cast<float>(xPosition);
    lastY = static_cast<float>(yPosition);

    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    if (camera.getMode() == Camera::Mode::Free || dragging) {
        camera.rotate(offsetX, offsetY);
    }
}

void scrollCallback(GLFWwindow*, double, double yOffset) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    camera.zoom(static_cast<float>(yOffset));
}

void buildUi(GLFWwindow* window, Scene& scene) {
    if (!showPanel) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(330.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Cozy Room", &showPanel);

    ImGui::Text("Camera: %s", camera.getMode() == Camera::Mode::Free ? "Free" : "Orbit");
    ImGui::Text("Tab changes camera, F5 closes this panel");

    ImGui::Separator();
    ImGui::Text("Scene");

    ImGui::Checkbox("Rain", &sceneOptions.rainOn);
    ImGui::Checkbox("Curtains", &sceneOptions.curtainsClosed);
    ImGui::Checkbox("Desk lamp", &sceneOptions.lampOn);
    ImGui::Checkbox("Lamp flicker", &sceneOptions.flicker);
    ImGui::Checkbox("Day / night", &sceneOptions.dayNightOn);
    if (sceneOptions.dayNightOn) {
        sceneOptions.dayTime = scene.getTimeOfDay();
        ImGui::SliderFloat("Day speed", &sceneOptions.dayCycleSpeed, 0.01f, 0.2f);
    } else {
        float hours = sceneOptions.dayTime * 24.0f;
        if (ImGui::SliderFloat("Time", &hours, 0.0f, 24.0f, "%.1f h")) {
            sceneOptions.dayTime = hours / 24.0f;
        }
        ImGui::TextDisabled("6h sunrise, 12h noon, 18h sunset");
    }

    if (ImGui::Button(sceneOptions.ballRolling ? "Stop ball" : "Roll ball")) {
        sceneOptions.ballRolling = !sceneOptions.ballRolling;
    }
    ImGui::SliderFloat("Ball speed", &sceneOptions.ballSpeed, 0.4f, 4.0f);

    ImGui::Separator();
    ImGui::Text("Lighting");

    ImGui::SliderFloat("Window light", &lighting.windowLight.intensity, 0.0f, 2.0f);
    ImGui::ColorEdit3("Ambient", &lighting.ambientColor.x);
    ImGui::SliderFloat3("Lamp offset", &sceneOptions.lampOffset.x, -1.0f, 1.0f);

    ImGui::Separator();
    ImGui::Text("Material");
    ImGui::ColorEdit3("Specular", &roomMaterial.specular.x);
    ImGui::SliderFloat("Shininess", &roomMaterial.shininess, 2.0f, 128.0f);

    ImGui::End();

    if (!showPanel) {
        updateCursorMode(window);
    }
}

}

int main() {
    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Cozy Room Diorama", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        std::cerr << "Failed to load OpenGL" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 8.0f;
    ImGui::GetStyle().FrameRounding = 4.0f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    std::cout << "Tab: camera mode, F5: panel, R: rain, C: curtains, B: ball, Esc: exit"
              << std::endl;

    {
        const std::string shaderPath = std::string(SHADER_DIR);
        Shader shader(shaderPath + "/vertex.vert", shaderPath + "/fragment.frag");
        Texture texture;
        Scene scene;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        double previousTime = glfwGetTime();
        double elapsedTime = 0.0;

        while (glfwWindowShouldClose(window) == GLFW_FALSE) {
            const double currentTime = glfwGetTime();
            const float deltaTime =
                std::min(static_cast<float>(currentTime - previousTime), 0.1f);
            previousTime = currentTime;
            elapsedTime += deltaTime;

            scene.update(deltaTime, static_cast<float>(elapsedTime), lighting, sceneOptions);

            const int leftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            if (leftState == GLFW_PRESS && !leftWasDown) {
                leftWasDown = true;
                glfwGetCursorPos(window, &clickX, &clickY);
            } else if (leftState == GLFW_RELEASE && leftWasDown) {
                leftWasDown = false;

                double releaseX = 0.0;
                double releaseY = 0.0;
                glfwGetCursorPos(window, &releaseX, &releaseY);
                const double dx = releaseX - clickX;
                const double dy = releaseY - clickY;

                int windowWidth = 0;
                int windowHeight = 0;
                glfwGetWindowSize(window, &windowWidth, &windowHeight);

                if (dx * dx + dy * dy < 36.0 && windowWidth > 0 && windowHeight > 0 &&
                    !ImGui::GetIO().WantCaptureMouse) {
                    const float ndcX =
                        2.0f * static_cast<float>(releaseX) / windowWidth - 1.0f;
                    const float ndcY =
                        1.0f - 2.0f * static_cast<float>(releaseY) / windowHeight;

                    int framebufferWidth = 0;
                    int framebufferHeight = 0;
                    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

                    if (framebufferWidth > 0 && framebufferHeight > 0) {
                        glm::vec3 rayOrigin;
                        glm::vec3 rayDirection;
                        camera.getRay(ndcX, ndcY,
                                      static_cast<float>(framebufferWidth) / framebufferHeight,
                                      rayOrigin, rayDirection);

                        if (scene.pickCat(rayOrigin, rayDirection)) {
                            scene.pokeCat();
                            std::cout << "*growl*" << std::endl;
                        }
                    }
                }
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (!ImGui::GetIO().WantCaptureKeyboard) {
                camera.move(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS,
                            glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS,
                            glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS,
                            glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS,
                            glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS,
                            glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS,
                            deltaTime);
            }

            buildUi(window, scene);

            const ImGuiIO& fpsIo = ImGui::GetIO();
            const float fps = fpsIo.Framerate;
            const ImVec4 fpsColor = fps >= 60.0f ? ImVec4(0.55f, 0.95f, 0.65f, 1.0f)
                                     : fps >= 30.0f ? ImVec4(1.0f, 0.82f, 0.35f, 1.0f)
                                                    : ImVec4(1.0f, 0.45f, 0.45f, 1.0f);

            ImGui::SetNextWindowPos(ImVec2(fpsIo.DisplaySize.x - 16.0f, 16.0f),
                                    ImGuiCond_Always, ImVec2(1.0f, 0.0f));
            ImGui::SetNextWindowBgAlpha(0.55f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 7.0f));
            ImGui::Begin("##fps", nullptr,
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                         ImGuiWindowFlags_NoSavedSettings |
                         ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                         ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::TextUnformatted("FPS");
            ImGui::SameLine();
            ImGui::TextColored(fpsColor, "%.1f", fps);
            ImGui::TextDisabled("%.2f ms", fps > 0.0f ? 1000.0f / fps : 0.0f);
            ImGui::End();
            ImGui::PopStyleVar();

            if (scene.isCatGrowling()) {
                ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, 90.0f),
                                        ImGuiCond_Always, ImVec2(0.5f, 0.0f));
                ImGui::SetNextWindowBgAlpha(0.0f);
                ImGui::Begin("##growl", nullptr,
                             ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
                ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.82f, 1.0f), "*growl*");
                ImGui::End();
            }

            ImGui::Render();

            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);

            const float daylight = scene.getDaylight();
            glClearColor(0.04f + 0.58f * daylight,
                         0.05f + 0.68f * daylight,
                         0.10f + 0.78f * daylight, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (width > 0 && height > 0 && shader.isLoaded()) {
                const float aspect = static_cast<float>(width) / static_cast<float>(height);

                shader.use();
                shader.setMat4("view", camera.getViewMatrix());
                shader.setMat4("projection", camera.getProjectionMatrix(aspect));
                shader.setVec3("viewPosition", camera.getPosition());

                lighting.apply(shader, roomMaterial);
                scene.draw(shader, texture);
                scene.drawParticles(shader, texture);
            }

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
