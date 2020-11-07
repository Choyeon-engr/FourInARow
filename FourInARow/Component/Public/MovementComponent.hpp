#pragma once

#include "Component.hpp"
#include "CML.hpp"

class MovementComponent : public Component
{
public:
    MovementComponent(Actor* owner, int updateOrder) : Component(owner, updateOrder), mForce(0.f, 0.f), mAcceleration(0.f, 0.f), mVelocity(0.f, 0.f), mMass(1.f), mAngularSpeed(0.f) {}
    
    void Update(float deltaTime) override;
    
    void AddForce(const CML::Vector2D& force) { mForce += force; }
    
    const float GetMass() const noexcept    { return mMass; }
    void SetMass(float mass)                { mMass = mass; }
    
    const float GetAngularSpeed() const noexcept    { return mAngularSpeed; }
    void SetAngularSpeed(float angularSpeed)        { mAngularSpeed = angularSpeed; }
    
private:
    CML::Vector2D mForce;
    CML::Vector2D mAcceleration;
    CML::Vector2D mVelocity;
    float mMass;
    float mAngularSpeed;
};
