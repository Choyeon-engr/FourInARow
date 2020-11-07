#include "Actor.hpp"
#include "Game.hpp"
#include "Component.hpp"

Actor::Actor(Game* game)
: mState(EEnable), mPosition(CML::Vector2D(0.f, 0.f)), mRotation(0.f), mScale(1.f), mGame(game)
{
    mGame->AddActor(this);
}

Actor::~Actor()
{
    mGame->RemoveActor(this);
    
    while (!mComponents.empty())
        delete mComponents.back();
}

void Actor::Update(float deltaTime)
{
    if (mState == EEnable)
    {
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
    }
}

void Actor::UpdateComponents(float deltaTime)
{
    for (auto component : mComponents)
        component->Update(deltaTime);
}

void Actor::Input(const uint8_t* keyState)
{
    if (mState == EEnable)
    {
        for (auto component : mComponents)
            component->Input(keyState);
        
        ActorInput(keyState);
    }
}

void Actor::AddComponent(Component* component)
{
    int updateOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    
    for (; iter != mComponents.end(); ++iter)
    if (updateOrder < (*iter)->GetUpdateOrder())
        break;
    
    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
    auto iter = find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end())
        mComponents.erase(iter);
}
