#include <SDL2_image/SDL_image.h>

#include "Game.hpp"
#include "Actor.hpp"
#include "SpriteComponent.hpp"

bool Game::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO))
        return false;
    
    if (!IMG_Init(IMG_INIT_PNG))
        return false;
    
    mWindow = SDL_CreateWindow("FourInARow", 0, 0, 1024, 768, 0);
    if (!mWindow)
        return false;
    
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
        return false;
    
    CML::Random::Init();
    
    LoadData();
    
    mTicksCount = SDL_GetTicks();
    
    return true;
}

void Game::Loop()
{
    while (mIsRunning)
    {
        Input();
        Update();
        Output();
    }
}

void Game::Quit()
{
    UnloadData();
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
    if (mIsUpdatingActors)
        mPendingActors.emplace_back(actor);
    else
        mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
    auto iter = find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
        return;
    }
    
    iter = find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddSprite(SpriteComponent* sprite)
{
    int drawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    
    for (; iter != mSprites.end(); ++iter)
    if (drawOrder < (*iter)->GetDrawOrder())
        break;
    
    mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
    auto iter = find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const string& fileName)
{
    SDL_Texture* texture = nullptr;
    const string AbsolutePath = string("/Users/choyeon/Game/FourInARow/FourInARow") + fileName;
    auto iter = mTextures.find(AbsolutePath.c_str());
    
    if (iter != mTextures.end())
        texture = iter->second;
    else
    {
        SDL_Surface* surface = IMG_Load(AbsolutePath.c_str());
        if (!surface)
            return nullptr;
        
        texture = SDL_CreateTextureFromSurface(mRenderer, surface);
        SDL_free(surface);
        if (!texture)
            return nullptr;
        
        mTextures.emplace(AbsolutePath, texture);
    }
    
    return texture;
}

void Game::DrawTexture(SDL_Texture* texture, const CML::Vector2D& position, const CML::Vector2D& size)
{
    SDL_Rect rect;
    rect.w = static_cast<int>(size.X);
    rect.h = static_cast<int>(size.Y);
    rect.x = static_cast<int>(position.X) - rect.w / 2;
    rect.y = static_cast<int>(position.Y) - rect.h / 2;
    
    SDL_RenderCopy(mRenderer, texture, nullptr, &rect);
}

void Game::Input()
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT && !mBoardState.IsTerminal())
                {
                    int column = event.button.x - 64;
                    if (column >= 0)
                    {
                        column /= 128;
                        if (column <= 6)
                        {
                            bool isPlayerMoved = TryPlayerMove(&mBoardState, column);
                            if (isPlayerMoved && !mBoardState.IsTerminal())
                                CPUMove(&mBoardState);
                        }
                    }
                }
                break;
        }
    }
    
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    mIsUpdatingActors = true;
    for (auto actor : mActors)
        actor->Input(keyState);
    mIsUpdatingActors = false;
}

void Game::Update()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
    if (deltaTime > 0.05f)
        deltaTime = 0.05f;
    
    mTicksCount = SDL_GetTicks();
    
    mIsUpdatingActors = true;
    for (auto actor : mActors)
        actor->Update(deltaTime);
    mIsUpdatingActors = false;
    
    for (auto pendingActor : mPendingActors)
        mActors.emplace_back(pendingActor);
    mPendingActors.clear();
    
    vector<Actor*> disabledActors;
    for (auto actor : mActors)
        if (actor->GetState() == Actor::EDisable)
            disabledActors.emplace_back(actor);
    
    for (auto disabledActor : disabledActors)
        delete disabledActor;
}

void Game::Output()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 50, 100, 255);
    SDL_RenderClear(mRenderer);
    
    for (auto sprite : mSprites)
        sprite->Draw(mRenderer);
    
    DrawTexture(GetTexture("/Assets/Board.png"), CML::Vector2D(512.f, 384.f), CML::Vector2D(896.f, 768.f));
    
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            CML::Vector2D position(j * 128.f + 128.f, i * 128.f + 64.f);
            
            if (mBoardState.mBoard[i][j] == BoardState::EYellow)
                DrawTexture(GetTexture("/Assets/YellowPiece.png"), position, CML::Vector2D(128.f, 128.f));
            else if (mBoardState.mBoard[i][j] == BoardState::ERed)
                DrawTexture(GetTexture("/Assets/RedPiece.png"), position, CML::Vector2D(128.f, 128.f));
        }
    }
    
    SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
    
}

void Game::UnloadData()
{
    while (!mActors.empty())
        delete mActors.back();
    
    for (auto texture : mTextures)
        SDL_DestroyTexture(texture.second);
    
    mTextures.clear();
}
