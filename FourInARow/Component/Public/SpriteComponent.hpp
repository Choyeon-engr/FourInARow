#pragma once

#include <SDL2/SDL.h>

#include "Component.hpp"

class SpriteComponent : public Component
{
public:
    SpriteComponent(class Actor* owner, int drawOrder);
    ~SpriteComponent();
    
    virtual void Draw(SDL_Renderer* renderer);
    
    virtual void SetTexture(SDL_Texture* texture);
    
    const int GetDrawOrder() const noexcept { return mDrawOrder; }
    const int GetTxtWidth()  const noexcept { return mTxtWidth; }
    const int GetTxtHeight() const noexcept { return mTxtHeight; }
    
protected:
    SDL_Texture* mTexture;
    
    int mDrawOrder;
    int mTxtWidth;
    int mTxtHeight;
};
