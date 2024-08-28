#include <entities/game_over.h>
#include <chrono>

GameOver::GameOver() :
        IEntity(EntityIdentificator::GAME_OVER, EntityType::TERRAIN, SurfaceType::SIMPLE, GameOverStateIdentificator::GAME_OVER_MAX_STATES, false, true) {
}

void GameOver::PrintName() const {
    std::cout << "GameOver." << std::endl;
}

bool GameOver::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;
 
    if (isMoving) {
        PositionAddY(-1.5f);
        if((position.GetInitialCellY() - position.GetCellY()) >= MOUNTAIN_VIEWPORT_HEIGHT_CELLS) {
            isMoving = false;
            gameManager->FinishGame(false);
        }
        needRedraw = true;
    }

    if (!animationLoaded) {
        return false;
    }

    if (animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
        return false;
    }

    if (chrono::system_clock::now() >= nextSpriteTime) {
        // Load next sprite of the current animation
        LoadNextSprite();
        return true;
    }

    return needRedraw;
}

void GameOver::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    LoadAnimationWithId(GameOverAnimation::GAME_OVER_MOVE_UP);
    gameManager->PlaySoundById(SoundIdentificator::GAME_OVER_SOUND);
}

IEntity *GameOver::Create() {
    return new GameOver();
}

GameOver::~GameOver() = default;
