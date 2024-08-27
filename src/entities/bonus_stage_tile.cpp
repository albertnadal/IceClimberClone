#include <entities/bonus_stage_tile.h>
#include <chrono>

BonusStageTile::BonusStageTile() :
        IEntity(EntityIdentificator::BONUS_STAGE_TILE_A, EntityType::TERRAIN, SurfaceType::SIMPLE, BonusStageTileStateIdentificator::BONUS_STAGE_TILE_MAX_STATES, false, true) {
}

BonusStageTile::BonusStageTile(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable, bool is_traversable) :
        IEntity(_id, _type, surface_type, max_states, is_breakable, is_traversable) {
}

void BonusStageTile::PrintName() const {
        std::cout << "BonusStageTile." << std::endl;
}

bool BonusStageTile::Update(uint8_t pressedKeys_) {
        bool needRedraw = false;

        if(!animationLoaded) {
                return false;
        }

        if(animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
                return false;
        }

        if(chrono::system_clock::now() >= nextSpriteTime) {
                // Load next sprite of the current animation
                LoadNextSprite();
                return true;
        }

        return needRedraw;
}

void BonusStageTile::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
}

IEntity* BonusStageTile::Create() {
          return new BonusStageTile();
}

BonusStageTile::~BonusStageTile() {

}

/* BonusStageTileA */

BonusStageTileA::BonusStageTileA() :
        BonusStageTile(EntityIdentificator::BONUS_STAGE_TILE_A, EntityType::TERRAIN, SurfaceType::SIMPLE, BonusStageTileStateIdentificator::BONUS_STAGE_TILE_MAX_STATES, false, true) {
}

void BonusStageTileA::PrintName() const {
        std::cout << "BonusStageTileA." << std::endl;
}

void BonusStageTileA::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BonusStageTileAAnimation::BONUS_STAGE_TILE_A_STICKY);
}

IEntity* BonusStageTileA::Create() {
        return new BonusStageTileA();
}

/* BonusStageTileB */

BonusStageTileB::BonusStageTileB() :
        BonusStageTile(EntityIdentificator::BONUS_STAGE_TILE_B, EntityType::TERRAIN, SurfaceType::SIMPLE, BonusStageTileStateIdentificator::BONUS_STAGE_TILE_MAX_STATES, false, true) {
}

void BonusStageTileB::PrintName() const {
        std::cout << "BonusStageTileB." << std::endl;
}

void BonusStageTileB::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BonusStageTileBAnimation::BONUS_STAGE_TILE_B_STICKY);
}

IEntity* BonusStageTileB::Create() {
        return new BonusStageTileB();
}

/* BonusStageTileC */

BonusStageTileC::BonusStageTileC() :
        BonusStageTile(EntityIdentificator::BONUS_STAGE_TILE_C, EntityType::TERRAIN, SurfaceType::SIMPLE, BonusStageTileStateIdentificator::BONUS_STAGE_TILE_MAX_STATES, false, true) {
}

void BonusStageTileC::PrintName() const {
        std::cout << "BonusStageTileC." << std::endl;
}

void BonusStageTileC::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BonusStageTileCAnimation::BONUS_STAGE_TILE_C_STICKY);
}

IEntity* BonusStageTileC::Create() {
        return new BonusStageTileC();
}
