#include "TestGame.h"

#include <array>
#include <functional>
#include <memory>

#include <glm/trigonometric.hpp>

#include <android_game_engine/Box.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/Quadcopter.h>

namespace age {

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)
    (JNIEnv *env, jobject gameActivity, int windowWidth, int windowHeight, jobject j_asset_manager) {
    GameEngineJNI::init(env, windowWidth, windowHeight, j_asset_manager);
    GameEngineJNI::onCreate(std::make_unique<TestGame>(env, gameActivity));
}

JNI_METHOD_DEFINITION(void, onRollThrustInputJNI)(JNIEnv *env, jobject gameActivity, float roll, float thrust) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onRollThrustInput(roll, thrust);
}

JNI_METHOD_DEFINITION(void, onYawPitchInputJNI)(JNIEnv *env, jobject gameActivity, float yaw, float pitch) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onYawPitchInput(yaw, pitch);
}

TestGame::TestGame(JNIEnv *env, jobject javaActivityObject) : Game(env, javaActivityObject) {}

void TestGame::onCreate() {
    Game::onCreate();
    this->enablePhysicsDebugDrawer(true);

    this->getCam()->setPosition({-10.0f, 5.0f, 7.0f});
    this->getCam()->setLookAtPoint({2.0f, 0.0f, 1.0f});

    {
        auto obj1 = std::make_shared<GameObject>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds");
        obj1->setLabel("obj1");
        obj1->setPosition({4.0f, 3.0f, 5.0f});
        obj1->setScale(glm::vec3(10.0f));
        obj1->setMass(1.0f);
        this->addToWorldList(obj1);
    }

    {
        const auto scale = 100.0f;
        std::shared_ptr<Box> floor(new Box({Texture2D("images/wood.png")},
                                           {Texture2D(glm::vec3(1.0f))},
                                           glm::vec2(scale)));
        floor->setLabel("Floor");
        floor->setScale(glm::vec3{scale, scale, 0.2f});
        floor->setPosition(glm::vec3(0.0f));
        floor->setSpecularExponent(32.0f);
        floor->setFriction(1.0f);
        this->addToWorldList(floor);
    }

    {
        using namespace std::placeholders;

        // Create UAV
        Quadcopter::Parameters params;
        params.mass = 1.0f;

        params.maxRoll = glm::radians(35.0f);
        params.maxPitch = glm::radians(35.0f);

        params.maxRollRate = glm::radians(360.0f);
        params.maxPitchRate = glm::radians(360.0f);
        params.maxYawRate = glm::radians(120.0f);
        params.maxThrust = 15.0f;

        params.controlRates2MotorRotationSpeed = 150.0f;

        params.angle_kp = 2.5f;
        params.angle_ki = 0.0f;
        params.angle_kd = 0.8f;

        params.angleRate_kp = 2.0f;
        params.angleRate_ki = 0.0f;
        params.angleRate_kd = 0.0f;

        params.motorRotationSpeed2Thrust = 2.0E-3f;

        this->uav = std::make_shared<Quadcopter>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds", params);
        this->uav->setLabel("UAV");
        this->uav->setScale({0.363f, 0.363f, 0.053f});
        this->uav->setPosition({0.0f, 0.0f, 3.0f});
        this->uav->setMode(Quadcopter::Mode::ANGLE);
        this->uav->setDamping(0.25f, 0.05f);

        // Connect UAV to joystick controls
//        this->getCam()->setChaseObject(uav.get(), {-5.0f, 0.0f, 1.0f});
        this->addToWorldList(this->uav);
    }
}

void TestGame::onRollThrustInput(float roll, float thrust) {
    this->uav->onRollThrustInput({roll, thrust});
}

void TestGame::onYawPitchInput(float yaw, float pitch) {
    this->uav->onYawPitchInput({yaw, pitch});
}

void TestGame::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                   const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
    gameObject->applyCentralForce(touchDirection * 400.0f);
    this->uav->setOrientation(glm::mat3(1.0f));
    this->uav->setPosition({0.0f, 0.0f, 0.5f});
}

} // namespace age
