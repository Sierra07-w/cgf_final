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
#include "scene/Camera.h"
#include "scene/Lighting.h"
#include "scene/Room.h"

namespace {

constexpr float twoPi = 6.2831853f;

Camera camera;
Lighting lighting;
Material roomMaterial;

bool showPanel = false;
bool firstMouse = true;
bool dragging = false;
float lastX = 640.0f;
float lastY = 360.0f;

struct AnimationState {
    bool catBreathing = true;
    bool cupSteam = true;
    bool rain = true;
    float catPhase = 0.0f;
    float steamPhase = 0.0f;
    float rainPhase = 0.0f;
};

AnimationState animations;

void updateCursorMode(GLFWwindow* window) {
    const bool captureMouse = camera.getMode() == Camera::Mode::Free && !showPanel;
    glfwSetInputMode(window, GLFW_CURSOR,
                     captureMouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    firstMouse = true;
}

void updateAnimations(float deltaTime) {
    if (animations.catBreathing) {
        animations.catPhase = std::fmod(animations.catPhase + deltaTime * 1.2f, twoPi);
    }
    if (animations.cupSteam) {
        animations.steamPhase = std::fmod(animations.steamPhase + deltaTime * 0.7f, twoPi);
    }
    if (animations.rain) {
        animations.rainPhase = std::fmod(animations.rainPhase + deltaTime * 3.5f, twoPi);
    }
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

void buildUi(GLFWwindow* window) {
    if (!showPanel) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(330.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Cozy Room", &showPanel);

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Camera: %s", camera.getMode() == Camera::Mode::Free ? "Free" : "Orbit");
    ImGui::Text("Tab changes camera, F5 closes this panel");

    ImGui::Separator();
    ImGui::Text("Animations");

    ImGui::Checkbox("Cat breathing", &animations.catBreathing);
    ImGui::ProgressBar(animations.catPhase / twoPi, ImVec2(0.0f, 0.0f), "");

    ImGui::Checkbox("Cup steam", &animations.cupSteam);
    ImGui::ProgressBar(animations.steamPhase / twoPi, ImVec2(0.0f, 0.0f), "");

    ImGui::Checkbox("Rain", &animations.rain);
    ImGui::ProgressBar(animations.rainPhase / twoPi, ImVec2(0.0f, 0.0f), "");

    ImGui::Separator();
    ImGui::Text("Lighting");

    ImGui::SliderFloat("Desk lamp", &lighting.deskLamp.intensity, 0.0f, 5.0f);
    ImGui::SliderFloat("Window light", &lighting.windowLight.intensity, 0.0f, 2.0f);
    ImGui::SliderFloat("Fill light", &lighting.fillLight.intensity, 0.0f, 2.0f);
    ImGui::ColorEdit3("Ambient", &lighting.ambientColor.x);

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

    std::cout << "Tab: camera mode, F5: control panel, Esc: exit" << std::endl;

    {
        const std::string shaderPath = std::string(SHADER_DIR);
        Shader shader(shaderPath + "/vertex.vert", shaderPath + "/fragment.frag");
        Room room;

        glEnable(GL_DEPTH_TEST);

        double previousTime = glfwGetTime();

        while (glfwWindowShouldClose(window) == GLFW_FALSE) {
            const double currentTime = glfwGetTime();
            const float deltaTime =
                std::min(static_cast<float>(currentTime - previousTime), 0.1f);
            previousTime = currentTime;

            updateAnimations(deltaTime);

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

            buildUi(window);
            ImGui::Render();

            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);

            glClearColor(0.08f, 0.075f, 0.07f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (width > 0 && height > 0 && shader.isLoaded()) {
                const float aspect = static_cast<float>(width) / static_cast<float>(height);

                shader.use();
                shader.setMat4("model", glm::mat4(1.0f));
                shader.setMat4("view", camera.getViewMatrix());
                shader.setMat4("projection", camera.getProjectionMatrix(aspect));
                shader.setVec3("viewPosition", camera.getPosition());

                lighting.apply(shader, roomMaterial);
                room.draw();
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
