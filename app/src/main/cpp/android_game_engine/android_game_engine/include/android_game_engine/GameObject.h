#pragma once

#include <chrono>
#include <vector>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <glm/fwd.hpp>

#include "Mesh.h"
#include "Model.h"
#include "PhysicsRigidBody.h"

namespace age {

class ShaderProgram;

///
/// \brief The GameObject class represents an object in the 3D virtual world.
///
class GameObject {
public:
    using Meshes = std::vector<Mesh>;
    
    GameObject();
    
    ///
    /// \brief GameObject Loads vertex and texture data and creates a model
    ///                   for the game object.
    /// \param modelFilepath Filepath to the model data.
    /// \exception ge::LoadError Failed to load mesh data from model file.
    /// \exception ge::LoadError Failed to load texture image from file.
    ///
    explicit GameObject(const std::string &modelFilepath);
    
    virtual ~GameObject() = default;
    
    ///
    /// \brief onUpdate Updates the game object's state.
    ///
    /// This should be called on every iteration of the game loop.
    /// The base implementation does nothing.
    ///
    /// \param updateDuration Elapsed time since the last frame.
    ///
    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    
    virtual void updateFromPhysics();
    
    virtual void render(ShaderProgram *shader);
    
    void setMesh(std::shared_ptr<Meshes> mesh);
    
    glm::mat4 getModelMatrix() const;
    
    ///
    /// \brief getNormalMatrix Returns the normal matrix.
    ///
    /// The returned normal matrix can be used to correct vertex normal vectors distorted through
    /// non-uniform scaling of the game object's model.
    ///
    /// The recommended use of this function is to use this to set a uniform value in an OpenGL
    /// vertex shader and then multiply by the vertex normal.
    ///
    /// Ex) correctedNormal = normalMatrix * vertexNormal;
    ///
    /// \return The normal matrix of the game object.
    ///
    glm::mat3 getNormalMatrix() const;
    
    glm::mat4 getViewMatrix() const;
    
    void setPosition(const glm::vec3 &position);
    glm::vec3 getPosition() const;
    
    void setOrientation(const glm::mat3 &orientation);
    void setOrientation(const glm::vec3 &orientationX,
                        const glm::vec3 &orientationY,
                        const glm::vec3 &orientationZ);
    glm::mat3 getOrientation() const;
    glm::vec3 getOrientationX() const;
    glm::vec3 getOrientationY() const;
    glm::vec3 getOrientationZ() const;
    
    /// \name Look At
    ///
    /// Points the game object at a desired point or direction.
    ///
    /// If the new direction is linearly dependent with the game object's original normal,
    /// make sure to call GameObject::setNormal...() afterwards to properly set the new orientation.
    ///@{
    void setLookAtPoint(const glm::vec3 &lookAtPoint);
    void setLookAtDirection(const glm::vec3 &lookAtDirection);
    ///@}
    
    glm::vec3 getLookAtDirection() const;
    
    /// \name Normal
    ///
    /// Sets the game object's normal.
    ///
    /// If the new normal direction is linearly dependent with the game object's original
    /// look at direction, make sure to call GameObject::setLookAt...() afterwards to properly
    /// set the new orientation.
    ///@{
    void setNormalDirection(const glm::vec3 &normalDirection);
    ///@>
    
    glm::vec3 getNormalDirection() const;
    
    ///
    /// \brief rotate Rotates the game object about an axis in the world coordinate frame.
    /// \param angle_rad Angle to rotate game object by (rad).
    /// \param axis Axis (in world coordinate frame) to rotate game object about.
    ///
    void rotate(float angle_rad, const glm::vec3 &axis);
    
    ///
    /// \brief translate Translates the game object in the world coordinate frame.
    /// \param translation Amount to translate the game object by in the world coordinate frame.
    ///
    void translate(const glm::vec3 &translation);
    
    ///
    /// \brief translateInLocalFrame Translates the game object in the local coordinate frame.
    /// \param translation Amount to translate the game object by in the local coordinate frame.
    ///
    void translateInLocalFrame(const glm::vec3 &translation);
    
    void setScale(const glm::vec3 &scale);
    
    void setSpecularExponent(float specularExponent);
    
    PhysicsRigidBody* getPhysicsBody();
    
    void setMass(float mass);

protected:
    void setCollisionShape(std::unique_ptr<btCollisionShape> collisionShape);

private:
    Model model;
    
    std::shared_ptr<Meshes> meshes;
    float specularExponent = 64.0f;
    
    std::unique_ptr<PhysicsRigidBody> physicsBody = nullptr;
};

inline glm::mat4 GameObject::getModelMatrix() const {return this->model.getModelMatrix();}
inline glm::mat3 GameObject::getNormalMatrix() const {return this->model.getNormalMatrix();}
inline glm::mat4 GameObject::getViewMatrix() const {return this->model.getViewMatrix();}
inline glm::vec3 GameObject::getPosition() const {return this->model.getPosition();}
inline glm::mat3 GameObject::getOrientation() const {return this->model.getOrientation();}
inline glm::vec3 GameObject::getOrientationX() const {return this->model.getOrientationX();}
inline glm::vec3 GameObject::getOrientationY() const {return this->model.getOrientationY();}
inline glm::vec3 GameObject::getOrientationZ() const {return this->model.getOrientationZ();}
inline glm::vec3 GameObject::getLookAtDirection() const {return this->model.getLookAtDirection();}
inline glm::vec3 GameObject::getNormalDirection() const {return this->model.getNormalDirection();}

} // namespace age