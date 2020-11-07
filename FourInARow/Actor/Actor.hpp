#pragma once

#include <vector>
#include <cstdint>

#include "CML.hpp"

using namespace std;

class Actor
{
public:
    enum State
    {
        EEnable,
        EDisable
    };
    
    enum Kinds
    {
        ENone,
        EAsteroid,
        EJet
    };
    
    Actor(class Game* game);
    virtual ~Actor();
    
    void Update(float deltaTime);
    void UpdateComponents(float deltaTime);
    virtual void UpdateActor(float deltaTime) {}
    
    void Input(const uint8_t* keyState);
    virtual void ActorInput(const uint8_t* keyState) {}
    
    const State& GetState() const noexcept  { return mState; }
    void SetState(const State& state)       { mState = state; }
    
    const Kinds& GetKinds() const noexcept  { return mKinds; }
    void SetKinds(const Kinds& kinds)       { mKinds = kinds; }
    
    const CML::Vector2D& GetPosition() const noexcept   { return mPosition; }
    void SetPosition(const CML::Vector2D& position)     { mPosition = position; }
    
    const float GetRotation() const noexcept    { return mRotation; }
    void SetRotation(float rotation)            { mRotation = rotation; }
    
    const float GetScale() const noexcept   { return mScale; }
    void SetScale(float scale)              { mScale = scale; }
    
    CML::Vector2D GetForwardVector() const noexcept { return CML::Vector2D(CML::Cos(mRotation), -CML::Sin(mRotation)); }
    
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);
    
    class Game* GetGame() const noexcept { return mGame; }
    
private:
    State mState;
    Kinds mKinds;
    
    CML::Vector2D mPosition;
    float mRotation;
    float mScale;
    
    vector<class Component*> mComponents;
    
    class Game* mGame;
};
