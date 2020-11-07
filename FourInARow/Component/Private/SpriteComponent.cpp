#include "SpriteComponent.hpp"
#include "Game.hpp"
#include "Actor.hpp"
#include "CML.hpp"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
: Component(owner, drawOrder), mTexture(nullptr), mDrawOrder(drawOrder), mTxtWidth(0), mTxtHeight(0)
{
    mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
    mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
    if (mTexture)
    {
        SDL_Rect rect;
        rect.w = static_cast<int>(mTxtWidth * mOwner->GetScale());
        rect.h = static_cast<int>(mTxtHeight * mOwner->GetScale());
        rect.x = static_cast<int>(mOwner->GetPosition().X - rect.w / 2);
        rect.y = static_cast<int>(mOwner->GetPosition().Y - rect.h / 2);
        
        SDL_RenderCopyEx(renderer, mTexture, nullptr, &rect, -CML::ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
    }
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
    mTexture = texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &mTxtWidth, &mTxtHeight);
}
