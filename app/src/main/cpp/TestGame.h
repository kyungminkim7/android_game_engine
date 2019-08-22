#pragma once

#include <android_game_engine/Game.h>

#include <android_game_engine/GameEngineJNI.h>

namespace age {

extern "C" {
JNI_METHOD_DECLARATION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject gameActivity, int windowWidth, int windowHeight, jobject j_asset_manager);
JNI_METHOD_DECLARATION(void, onRollThrustInputJNI)(JNIEnv *env, jobject gameActivity, float roll, float thrust);
JNI_METHOD_DECLARATION(void, onYawPitchInputJNI)(JNIEnv *env, jobject gameActivity, float yaw, float pitch);
}

class Quadcopter;

class TestGame : public Game {
public:
    TestGame(JNIEnv *env, jobject javaActivityObject);

    void onCreate() override;

    void onRollThrustInput(float roll, float thrust);
    void onYawPitchInput(float yaw, float pitch);

protected:
    void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                             const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) override;
    
private:
    std::shared_ptr<Quadcopter> uav;
};

} // namespace age