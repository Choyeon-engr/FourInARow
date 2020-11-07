#include <cmath>

#include "MovementComponent.hpp"
#include "Actor.hpp"

void MovementComponent::Update(float deltaTime)
{
    mAcceleration = mForce / mMass;
    mForce = CML::Vector2D(0.f, 0.f);
    mVelocity += mAcceleration * deltaTime;
    
    if (!CML::CloseToZero(sqrt(mVelocity.LengthSquared())))
    {
        CML::Vector2D position = mOwner->GetPosition();
        position += mVelocity * deltaTime;
        mOwner->SetPosition(position);
    }
    
    mVelocity = CML::Vector2D(0.f, 0.f);
    
    if (!CML::CloseToZero(mAngularSpeed))
    {
        float rotation = mOwner->GetRotation();
        rotation += mAngularSpeed * deltaTime;
        mOwner->SetRotation(rotation);
    }
}
