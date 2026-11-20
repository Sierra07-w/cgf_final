#include "scene/Scene.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#include "scene/Atmosphere.h"
#include "scene/DayNight.h"
#include "scene/Primitives.h"
#include "scene/Room.h"

namespace {

std::shared_ptr<Mesh> toMesh(const MeshData& data) {
    return std::make_shared<Mesh>(data);
}

std::shared_ptr<SceneNode> makeNode(const MeshData& data, const glm::mat4& local,
                                    int pattern = 0, bool useTexture = false,
                                    const glm::vec3& colorMultiplier = glm::vec3(1.0f)) {
    auto node = std::make_shared<SceneNode>(toMesh(data), pattern, useTexture, colorMultiplier);
    node->setLocalMatrix(local);
    return node;
}

glm::mat4 translation(float x, float y, float z) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

glm::mat4 scaled(float x, float y, float z) {
    return glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

glm::mat4 rotationZ(float angle) {
    return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 rotationY(float angle) {
    return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 rotationX(float angle) {
    return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

}

Scene::Scene() {
    root = std::make_shared<SceneNode>();

    root->addChild(makeNode(Room::createFloor(), glm::mat4(1.0f), 1));
    root->addChild(makeNode(Room::createWalls(), glm::mat4(1.0f), 2));
    root->addChild(makeNode(Room::createWindowFrame(), glm::mat4(1.0f), 0));

    curtainLeft = makeNode(createCube({0.8f, 1.9f, 0.06f}, glm::vec3(0.9f, 0.82f, 0.78f)),
                           translation(-1.25f, 2.2f, -2.9f));
    curtainRight = makeNode(createCube({0.8f, 1.9f, 0.06f}, glm::vec3(0.9f, 0.82f, 0.78f)),
                            translation(1.25f, 2.2f, -2.9f));
    root->addChild(curtainLeft); root->addChild(curtainRight);

    sun = makeNode(createSphere(0.55f, 20, 14, glm::vec3(1.0f, 0.9f, 0.55f)),
                   translation(-2.2f, 1.4f, -9.0f), 0, false,
                   glm::vec3(3.0f, 2.5f, 1.4f));
    root->addChild(sun);

    root->addChild(makeNode(createPlane(3.4f, 2.4f, glm::vec3(1.0f)),
                            translation(0.0f, 0.012f, 0.8f),
                            4, true, glm::vec3(0.82f, 0.92f, 1.0f)));

    auto bed = std::make_shared<SceneNode>();
    bed->setLocalMatrix(translation(-1.7f, 0.0f, 0.9f));

    bed->addChild(makeNode(createCube({2.3f, 0.35f, 3.0f}, glm::vec3(0.74f, 0.56f, 0.42f)),
                           translation(0.0f, 0.175f, 0.0f)));
    bed->addChild(makeNode(createCube({2.15f, 0.28f, 2.8f}, glm::vec3(0.98f, 0.96f, 0.92f)),
                           translation(0.0f, 0.49f, 0.0f)));
    bed->addChild(makeNode(createCube({2.2f, 0.12f, 1.7f}, glm::vec3(0.95f, 0.6f, 0.72f)),
                           translation(0.0f, 0.69f, -0.5f), 5));
    bed->addChild(makeNode(createCube({1.3f, 0.18f, 0.55f}, glm::vec3(1.0f, 1.0f, 0.98f)),
                           translation(0.0f, 0.72f, 0.95f)));

    root->addChild(bed);

    sleepingCat = std::make_shared<SceneNode>();
    sleepingCat->setLocalMatrix(translation(0.35f, 0.96f, 0.2f));

    const glm::vec3 catWhite(1.0f, 0.98f, 0.96f);
    const glm::vec3 catCream(1.0f, 0.95f, 0.9f);
    const glm::vec3 catPink(1.0f, 0.72f, 0.76f);
    const glm::vec3 catEye(0.28f, 0.27f, 0.3f);

    sleepingCatBody = makeNode(createSphere(0.3f, 20, 14, catWhite),
                               scaled(1.15f, 0.7f, 1.25f));
    sleepingCat->addChild(sleepingCatBody);

    sleepingCatHead = makeNode(createSphere(0.18f, 18, 14, catCream),
                               translation(0.0f, 0.1f, 0.33f) * scaled(1.05f, 0.9f, 1.0f));
    sleepingCat->addChild(sleepingCatHead);

    for (int x = -1; x <= 1; x += 2) {
        const glm::mat4 outerEar = translation(0.1f * x, 0.16f, -0.01f) * rotationZ(-0.2f * x);
        const glm::mat4 innerEar = translation(0.1f * x, 0.16f, 0.03f) * rotationZ(-0.2f * x);

        auto outer = makeNode(createCone(0.07f, 0.15f, 12, catWhite), outerEar);
        auto inner = makeNode(createCone(0.04f, 0.09f, 12, catPink), innerEar);
        sleepingCatHead->addChild(outer);
        sleepingCatHead->addChild(inner);
        catEars.push_back(outer);
        catEars.push_back(inner);
        catEarBase.push_back(outerEar);
        catEarBase.push_back(innerEar);

        sleepingCatHead->addChild(makeNode(createSphere(0.06f, 12, 10, catWhite),
                                           translation(0.07f * x, -0.03f, 0.16f) *
                                           scaled(1.0f, 0.8f, 0.7f)));
        sleepingCatHead->addChild(makeNode(createSphere(0.028f, 10, 8, catEye),
                                           translation(0.07f * x, 0.045f, 0.172f) *
                                           scaled(1.1f, 0.25f, 0.4f)));
    }

    sleepingCatHead->addChild(makeNode(createSphere(0.03f, 10, 8, catPink),
                                       translation(0.0f, -0.015f, 0.205f)));

    catMouth = makeNode(createSphere(0.04f, 10, 8, glm::vec3(0.3f, 0.2f, 0.22f)),
                        translation(0.0f, -0.052f, 0.2f) * scaled(1.0f, 0.08f, 1.0f));
    sleepingCatHead->addChild(catMouth);

    for (int x = -1; x <= 1; x += 2) {
        sleepingCat->addChild(makeNode(createSphere(0.09f, 12, 10, catWhite),
                                       translation(0.12f * x, -0.16f, 0.3f) * scaled(1.2f, 0.6f, 1.0f)));
    }

    for (int i = 0; i < 5; ++i) {
        const float a = -0.8f + static_cast<float>(i) * 0.4f;
        const auto tailLocal = translation(std::cos(a) * 0.4f, 0.02f, std::sin(a) * 0.4f) *
                               rotationY(1.5708f - a) * rotationZ(1.5708f);
        sleepingCat->addChild(makeNode(createCylinder(0.04f, 0.21f, 10, catWhite), tailLocal));
    }
    sleepingCat->addChild(makeNode(createSphere(0.05f, 10, 8, catCream),
                                   translation(0.22f, 0.02f, 0.34f)));

    const glm::vec3 zColor(0.88f, 0.95f, 1.0f);
    for (int i = 0; i < 3; ++i) {
        auto zParticle = std::make_shared<SceneNode>();
        zParticle->addChild(makeNode(createCube({0.16f, 0.025f, 0.02f}, zColor),
                                     translation(0.0f, 0.07f, 0.0f)));
        zParticle->addChild(makeNode(createCube({0.16f, 0.025f, 0.02f}, zColor),
                                     translation(0.0f, -0.07f, 0.0f)));
        zParticle->addChild(makeNode(createCube({0.025f, 0.19f, 0.02f}, zColor),
                                     rotationZ(0.8f)));

        zParticles.push_back(zParticle);
        zPhases.push_back(static_cast<float>(i) / 3.0f);
        zAlphas.push_back(0.0f);
    }

    for (int i = 0; i < 16; ++i) {
        dustMotes.push_back(makeNode(createSphere(0.024f, 8, 6,
                                                  glm::vec3(1.0f, 0.95f, 0.75f)),
                                     glm::mat4(1.0f)));
        dustState.push_back(glm::vec3(-0.3f + (i % 4) * 0.2f, static_cast<float>(i) * 0.37f, 0.7f + (i % 3) * 0.25f));
        dustAlphas.push_back(0.0f);
    }

    const float beamRadii[] = {0.10f, 0.16f, 0.22f};
    for (float radius : beamRadii) {
        beamShafts.push_back(makeNode(createCylinder(radius, 1.0f, 12, glm::vec3(1.0f, 0.92f, 0.7f)),
                                      glm::mat4(1.0f), 0, false, glm::vec3(1.6f)));
        beamAlphas.push_back(0.0f);
    }

    bed->addChild(sleepingCat);

    auto desk = std::make_shared<SceneNode>();
    desk->setLocalMatrix(translation(1.9f, 0.0f, -1.7f));

    desk->addChild(makeNode(createCube({2.1f, 0.12f, 1.15f}, glm::vec3(0.76f, 0.58f, 0.42f)),
                            translation(0.0f, 0.78f, 0.0f)));

    for (int x = -1; x <= 1; x += 2) {
        for (int z = -1; z <= 1; z += 2) {
            desk->addChild(makeNode(createCube({0.1f, 0.72f, 0.1f},
                                               glm::vec3(0.62f, 0.46f, 0.34f)),
                                    translation(0.9f * x, 0.36f, 0.45f * z)));
        }
    }

    root->addChild(desk);

    auto chair = std::make_shared<SceneNode>();
    chair->setLocalMatrix(translation(1.9f, 0.0f, -0.35f));

    chair->addChild(makeNode(createCube({0.8f, 0.12f, 0.8f}, glm::vec3(0.74f, 0.56f, 0.42f)),
                             translation(0.0f, 0.45f, 0.0f)));
    chair->addChild(makeNode(createCube({0.8f, 0.75f, 0.1f}, glm::vec3(0.74f, 0.56f, 0.42f)),
                             translation(0.0f, 0.85f, -0.35f)));

    for (int x = -1; x <= 1; x += 2) {
        for (int z = -1; z <= 1; z += 2) {
            chair->addChild(makeNode(createCube({0.08f, 0.45f, 0.08f},
                                                glm::vec3(0.62f, 0.46f, 0.34f)),
                                     translation(0.32f * x, 0.225f, 0.32f * z)));
        }
    }

    root->addChild(chair);

    auto bookshelf = std::make_shared<SceneNode>();
    bookshelf->setLocalMatrix(translation(-2.65f, 0.0f, -2.35f));

    bookshelf->addChild(makeNode(createCube({0.12f, 2.4f, 0.6f}, glm::vec3(0.72f, 0.54f, 0.38f)),
                                 translation(-0.6f, 1.2f, 0.0f)));
    bookshelf->addChild(makeNode(createCube({0.12f, 2.4f, 0.6f}, glm::vec3(0.72f, 0.54f, 0.38f)),
                                 translation(0.6f, 1.2f, 0.0f)));

    for (int i = 0; i < 4; ++i) {
        bookshelf->addChild(makeNode(createCube({1.3f, 0.08f, 0.6f},
                                                glm::vec3(0.8f, 0.62f, 0.44f)),
                                     translation(0.0f, 0.1f + i * 0.65f, 0.0f)));
    }

    const glm::vec3 bookColors[] = {
        glm::vec3(0.75f, 0.25f, 0.3f),
        glm::vec3(0.25f, 0.5f, 0.75f),
        glm::vec3(0.85f, 0.7f, 0.25f),
        glm::vec3(0.35f, 0.65f, 0.45f),
        glm::vec3(0.7f, 0.4f, 0.65f),
        glm::vec3(0.9f, 0.55f, 0.3f),
        glm::vec3(0.3f, 0.45f, 0.7f),
        glm::vec3(0.6f, 0.75f, 0.35f)
    };

    for (int i = 0; i < 8; ++i) {
        const int shelf = i / 4;
        const int slot = i % 4;
        bookshelf->addChild(makeNode(createCube({0.14f, 0.42f, 0.4f}, bookColors[i]),
                                     translation(-0.42f + slot * 0.28f,
                                                 0.35f + shelf * 0.65f,
                                                 0.0f)));
    }

    root->addChild(bookshelf);

    auto yogaMat = std::make_shared<SceneNode>();
    yogaMat->setLocalMatrix(translation(0.0f, 0.0f, -2.45f));

    yogaMat->addChild(makeNode(createCube({1.6f, 0.025f, 0.7f},
                                          glm::vec3(0.95f, 0.6f, 0.72f)),
                               translation(0.0f, 0.02f, 0.0f)));
    yogaMat->addChild(makeNode(createCylinder(0.085f, 0.7f, 16,
                                              glm::vec3(0.98f, 0.72f, 0.82f)),
                               translation(-0.78f, 0.09f, 0.0f) * rotationX(1.5708f)));

    for (int i = -1; i <= 1; i += 2) {
        yogaMat->addChild(makeNode(createCube({1.45f, 0.008f, 0.025f},
                                              glm::vec3(1.0f, 0.86f, 0.91f)),
                                   translation(0.05f, 0.038f, i * 0.28f)));
    }

    root->addChild(yogaMat);

    for (int i = 0; i < 2; ++i) {
        const bool left = i == 0;
        const float x = left ? -1.48f : -1.25f;
        const float angle = left ? -0.65f : 0.65f;

        auto dumbbell = std::make_shared<SceneNode>();
        dumbbell->setLocalMatrix(translation(x, 0.25f, -2.45f) * rotationZ(angle));

        dumbbell->addChild(makeNode(createCylinder(0.022f, 0.38f, 10,
                                                   glm::vec3(0.6f, 0.65f, 0.7f)),
                                    glm::mat4(1.0f)));

        for (int end = -1; end <= 1; end += 2) {
            dumbbell->addChild(makeNode(createCube({0.16f, 0.11f, 0.16f},
                                                   glm::vec3(0.25f, 0.5f, 0.6f)),
                                        translation(0.0f, 0.19f * end, 0.0f)));
        }

        root->addChild(dumbbell);
    }

    auto plant = std::make_shared<SceneNode>();
    plant->setLocalMatrix(translation(2.45f, 0.0f, 1.6f));

    plant->addChild(makeNode(createCylinder(0.28f, 0.45f, 20, glm::vec3(0.9f, 0.66f, 0.5f)),
                             translation(0.0f, 0.225f, 0.0f)));
    plant->addChild(makeNode(createCylinder(0.25f, 0.05f, 20, glm::vec3(0.2f, 0.14f, 0.1f)),
                             translation(0.0f, 0.47f, 0.0f)));
    plant->addChild(makeNode(createCube({0.05f, 0.6f, 0.05f}, glm::vec3(0.3f, 0.6f, 0.35f)),
                             translation(0.0f, 0.78f, 0.0f)));

    for (int i = 0; i < 5; ++i) {
        const float angle = static_cast<float>(i) * 1.256637f;
        const auto leafLocal = translation(std::cos(angle) * 0.22f,
                                           1.02f + (i % 2) * 0.12f,
                                           std::sin(angle) * 0.22f) *
                               rotationY(angle) *
                               scaled(1.0f, 0.35f, 0.6f);
        const auto leaf = makeNode(createSphere(0.26f, 12, 8, glm::vec3(0.35f, 0.75f, 0.45f)),
                                   leafLocal);
        if (i == 0) {
            plantLeaf = leaf;
        }
        plant->addChild(leaf);
    }

    root->addChild(plant);

    auto lampBase = makeNode(createCylinder(0.22f, 0.06f, 20, glm::vec3(0.25f, 0.2f, 0.16f)),
                             translation(1.5f, 0.87f, -1.7f));

    auto lampArm = makeNode(createCube({0.06f, 0.9f, 0.06f}, glm::vec3(0.3f, 0.25f, 0.2f)),
                            translation(0.0f, 0.45f, 0.0f) * rotationZ(-0.35f));

    lampShade = makeNode(createCylinder(0.25f, 0.3f, 20, glm::vec3(1.0f, 0.72f, 0.35f)),
                         translation(0.0f, 0.45f, 0.0f) * rotationZ(0.35f));

    lampArm->addChild(lampShade);
    lampBase->addChild(lampArm);
    root->addChild(lampBase);

    auto mug = std::make_shared<SceneNode>();
    mug->setLocalMatrix(translation(1.15f, 0.88f, -1.55f));

    mug->addChild(makeNode(createCylinder(0.12f, 0.2f, 16, glm::vec3(0.92f, 0.95f, 0.98f)),
                           translation(0.0f, 0.1f, 0.0f)));
    mug->addChild(makeNode(createCube({0.08f, 0.12f, 0.06f}, glm::vec3(0.92f, 0.95f, 0.98f)),
                           translation(0.15f, 0.1f, 0.0f)));

    steamRoot = std::make_shared<SceneNode>();
    mug->addChild(steamRoot);

    for (int i = 0; i < 4; ++i) {
        const float offset = static_cast<float>(i) * 0.17f;
        const auto puff = makeNode(createSphere(0.05f, 8, 6, glm::vec3(0.85f, 0.9f, 0.95f)),
                                   translation(0.0f, 1.2f + offset, 0.0f));
        steamRoot->addChild(puff);
        steamPuffs.push_back(puff);
        steamState.push_back(glm::vec3(0.0f, offset, 0.0f));
    }

    root->addChild(mug);

    ball = makeNode(createSphere(0.22f, 20, 14, glm::vec3(0.92f, 0.35f, 0.4f)),
                    translation(0.0f, 0.22f, 2.65f));
    ball->addChild(makeNode(createCylinder(0.225f, 0.05f, 20,
                                           glm::vec3(0.98f, 0.95f, 0.9f)),
                            glm::mat4(1.0f)));
    root->addChild(ball);

    root->addChild(makeNode(createCube({0.95f, 0.65f, 0.02f}, glm::vec3(0.95f, 0.72f, 0.82f)),
                            translation(-2.0f, 2.45f, -2.97f), 3));
    root->addChild(makeNode(createCube({0.8f, 0.8f, 0.02f}, glm::vec3(0.65f, 0.85f, 0.95f)),
                            translation(2.2f, 2.5f, -2.97f), 3));

    rainRoot = std::make_shared<SceneNode>();
    root->addChild(rainRoot);

    for (int i = 0; i < 18; ++i) {
        const float x = -0.82f + static_cast<float>(i % 6) * 0.33f;
        const float y = 1.35f + static_cast<float>(i / 6) * 0.55f;
        const auto drop = makeNode(createCube({0.015f, 0.25f, 0.015f},
                                              glm::vec3(0.65f, 0.8f, 0.95f)),
                                   translation(x, y, -3.12f));
        rainRoot->addChild(drop);
        rainDrops.push_back(drop);
        rainState.push_back(glm::vec3(x, y, 1.4f + static_cast<float>(i % 3) * 0.35f));
    }

    root->updateWorld(glm::mat4(1.0f));
}

void Scene::update(float deltaTime, float elapsedTime, Lighting& lighting,
                   const SceneOptions& options) {
    if (options.ballRolling) {
        ballPhase += deltaTime * options.ballSpeed;
    }
    const float ballX = std::sin(ballPhase) * 1.3f;
    ball->setLocalMatrix(translation(ballX, 0.22f, 2.65f) * rotationZ(-ballX / 0.22f));

    const float reactionDuration = 1.6f;
    if (catReactionTime > 0.0f) {
        catReactionTime = std::max(0.0f, catReactionTime - deltaTime);
    }
    const float reactionElapsed = reactionDuration - catReactionTime;
    const bool reacting = catReactionTime > 0.0f;

    const float calmProgress = reacting
        ? 1.0f - std::min(catReactionTime / 0.5f, 1.0f)
        : 1.0f;
    const float calm = calmProgress * calmProgress * (3.0f - 2.0f * calmProgress);

    const float jumpPhase = std::min(reactionElapsed / 0.55f, 1.0f);
    const float jumpY = reacting ? std::sin(jumpPhase * 3.14159265f) * 0.22f : 0.0f;

    const float twitchEnvelope = reacting
        ? (1.0f - calm) * std::min(reactionElapsed / 0.15f, 1.0f)
        : 0.0f;
    const float shake = twitchEnvelope * 0.018f * std::sin(reactionElapsed * 55.0f);

    sleepingCat->setLocalMatrix(translation(0.35f + shake, 0.96f + jumpY, 0.2f) *
                                rotationZ(shake * 2.0f));

    const float breath = std::sin(elapsedTime * 1.8f);
    const float breathScale = 1.0f + 0.045f * calm * breath;
    sleepingCatBody->setLocalMatrix(scaled(1.15f * breathScale,
                                           0.7f * breathScale,
                                           1.25f * breathScale));
    sleepingCatHead->setLocalMatrix(translation(0.0f, 0.1f + 0.012f * calm * breath, 0.33f) *
                                    scaled(1.05f, 0.9f, 1.0f));

    const float earFlatten = twitchEnvelope * 0.9f;
    for (size_t i = 0; i < catEars.size(); ++i) {
        catEars[i]->setLocalMatrix(catEarBase[i] * rotationX(-earFlatten));
    }

    const float mouthOpen = twitchEnvelope *
                            (0.5f + 0.5f * std::sin(reactionElapsed * 40.0f));
    catMouth->setLocalMatrix(translation(0.0f, -0.052f, 0.2f) *
                             scaled(1.0f, 0.08f + mouthOpen * 0.9f, 1.0f));

    updateZParticles(zParticles, zPhases, zAlphas, elapsedTime, calm);

    glm::vec3 beamDirection(0.0f, -1.0f, 0.0f);
    currentDaylight = updateDayNight(sunPhase, deltaTime, options.dayNightOn, options.dayCycleSpeed, sun, lighting, beamDirection);
    updateCurtains(curtainOpen, options.curtainsClosed, deltaTime, curtainLeft, curtainRight);
    lighting.windowLight.intensity *= 0.35f + 0.65f * curtainOpen;
    updateSunbeams(beamShafts, beamAlphas, beamDirection, currentDaylight, curtainOpen);
    updateDust(dustMotes, dustState, dustAlphas, elapsedTime, beamDirection, currentDaylight, curtainOpen);
    updateRain(rainDrops, rainState, options.rainOn, deltaTime);
    updateSteam(steamPuffs, steamState, elapsedTime);

    const float leafAngle = 0.25f * std::sin(elapsedTime * 0.9f);
    plantLeaf->setLocalMatrix(translation(0.22f, 1.02f, 0.0f) * rotationY(leafAngle) *
                              scaled(1.0f, 0.35f, 0.6f));

    root->updateWorld(glm::mat4(1.0f));

    const glm::vec3 lampPosition = glm::vec3(lampShade->getWorldMatrix()[3]);
    lighting.deskLamp.position = lampPosition + options.lampOffset;

    const float flicker = options.flicker
        ? 0.82f + 0.18f * std::sin(elapsedTime * 17.0f) * std::sin(elapsedTime * 7.0f)
        : 1.0f;

    const float lampBase = 1.3f + 1.1f * (1.0f - currentDaylight);
    lighting.deskLamp.intensity = options.lampOn ? lampBase * flicker : 0.0f;
    lighting.fillLight.intensity =
        (options.lampOn ? 0.7f : 0.35f) * (0.55f + 0.45f * currentDaylight);
}

bool Scene::pickCat(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const {
    const glm::vec3 center = glm::vec3(sleepingCat->getWorldMatrix()[3]) +
                             glm::vec3(0.0f, 0.05f, 0.12f);
    const glm::vec3 radii(0.44f, 0.26f, 0.55f);

    const glm::vec3 origin = (rayOrigin - center) / radii;
    const glm::vec3 direction = rayDirection / radii;

    const float a = glm::dot(direction, direction);
    const float b = 2.0f * glm::dot(origin, direction);
    const float c = glm::dot(origin, origin) - 1.0f;
    const float discriminant = b * b - 4.0f * a * c;

    if (a <= 0.0f || discriminant < 0.0f) {
        return false;
    }

    const float farRoot = (-b + std::sqrt(discriminant)) / (2.0f * a);
    return farRoot > 0.0f;
}

void Scene::pokeCat() {
    catReactionTime = 1.6f;
}

bool Scene::isCatGrowling() const {
    return catReactionTime > 0.0f;
}

void Scene::draw(const Shader& shader, const Texture& texture) const {
    shader.setFloat("uAlpha", 1.0f);
    root->draw(shader, texture);
}

float Scene::getDaylight() const {
    return currentDaylight;
}

void Scene::drawParticles(const Shader& shader, const Texture& texture) const {
    glDepthMask(GL_FALSE);
    drawWithAlpha(shader, texture, beamShafts, beamAlphas);
    drawWithAlpha(shader, texture, dustMotes, dustAlphas);
    drawWithAlpha(shader, texture, zParticles, zAlphas);
    glDepthMask(GL_TRUE);
}
