#pragma once

#include <memory>
#include <utility>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/PhysicsMotionState.h>

namespace age {

///
/// \brief Wrapper class for a rigid physics body.
///
class PhysicsRigidBody {
public:
    explicit PhysicsRigidBody(std::unique_ptr<btCollisionShape> collisionShape);
    
    void* getNativeBody();
    
    void setOrientation(const glm::mat3 &orientation);
    void setPosition(const glm::vec3 &position);
    
    void setDamping(float linearDamping, float angularDamping);
    void setMass(float mass);
    
    void setScale(const glm::vec3 &scale);
    
    bool isActive() const;
    
    std::pair<glm::mat3, glm::vec3> getTransform() const;
    
    void applyCentralForce(const glm::vec3 &force);
    void applyTorque(const glm::vec3 &torque);
    void applyForce(const glm::vec3 &force, const glm::vec3 &relPos);
    void clearForces();
    
    void setLinearVelocity(const glm::vec3 &velocity);
    void setAngularVelocity(const glm::vec3 &velocity);
    
private:
    std::unique_ptr<PhysicsMotionState> motionState;
    std::unique_ptr<btCollisionShape> collisionShape;
    std::unique_ptr<btRigidBody> body;
};

inline bool PhysicsRigidBody::isActive() const {return this->body->isActive();}

inline void PhysicsRigidBody::applyCentralForce(const glm::vec3 &force) {
    this->body->applyCentralForce({force.x, force.y, force.z});
}

inline void PhysicsRigidBody::applyTorque(const glm::vec3 &torque) {
    this->body->applyTorque({torque.x, torque.y, torque.z});
}

inline void PhysicsRigidBody::applyForce(const glm::vec3 &force, const glm::vec3 &relPos) {
    this->body->applyForce({force.x, force.y, force.z},
                           {relPos.x, relPos.y, relPos.z});
}

inline void PhysicsRigidBody::clearForces() {this->body->clearForces();}

inline void PhysicsRigidBody::setLinearVelocity(const glm::vec3 &velocity) {
    this->body->setLinearVelocity({velocity.x, velocity.y, velocity.z});
}

inline void PhysicsRigidBody::setAngularVelocity(const glm::vec3 &velocity) {
    this->body->setAngularVelocity({velocity.x, velocity.y, velocity.z});
}

} // namespace age