#include <entities/number.h>
#include <chrono>

Number::Number() :
        IEntity(EntityIdentificator::ONE, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

Number::Number(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable, bool is_traversable) :
        IEntity(_id, _type, surface_type, max_states, is_breakable, is_traversable) {
}

void Number::PrintName() {
        std::cout << "Number." << std::endl;
}

bool Number::Update(uint8_t pressedKeys_) {
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

void Number::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
}

IEntity* Number::Create() {
          return new Number();
}

Number::~Number() {

}

/* One */

One::One() :
        Number(EntityIdentificator::ONE, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

void One::PrintName() {
        std::cout << "One." << std::endl;
}

void One::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(OneAnimation::ONE_STICKY);
}

IEntity* One::Create() {
        return new One();
}

/* Two */

Two::Two() :
        Number(EntityIdentificator::TWO, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

void Two::PrintName() {
        std::cout << "Two." << std::endl;
}

void Two::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(TwoAnimation::TWO_STICKY);
}

IEntity* Two::Create() {
        return new Two();
}

/* Three */

Three::Three() :
        Number(EntityIdentificator::THREE, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

void Three::PrintName() {
        std::cout << "Three." << std::endl;
}

void Three::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(ThreeAnimation::THREE_STICKY);
}

IEntity* Three::Create() {
        return new Three();
}

/* Four */

Four::Four() :
        Number(EntityIdentificator::FOUR, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

void Four::PrintName() {
        std::cout << "Four." << std::endl;
}

void Four::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(FourAnimation::FOUR_STICKY);
}

IEntity* Four::Create() {
        return new Four();
}

/* Five */

Five::Five() :
        Number(EntityIdentificator::FIVE, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

void Five::PrintName() {
        std::cout << "Five." << std::endl;
}

void Five::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(FiveAnimation::FIVE_STICKY);
}

IEntity* Five::Create() {
        return new Five();
}

/* Six */

Six::Six() :
        Number(EntityIdentificator::SIX, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

void Six::PrintName() {
        std::cout << "Six." << std::endl;
}

void Six::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SixAnimation::SIX_STICKY);
}

IEntity* Six::Create() {
        return new Six();
}

/* Seven */

Seven::Seven() :
        Number(EntityIdentificator::SEVEN, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

void Seven::PrintName() {
        std::cout << "Seven." << std::endl;
}

void Seven::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SevenAnimation::SEVEN_STICKY);
}

IEntity* Seven::Create() {
        return new Seven();
}

/* Eight */

Eight::Eight() :
        Number(EntityIdentificator::EIGHT, EntityType::NUMBER, SurfaceType::SIMPLE, NumberStateIdentificator::NUMBER_MAX_STATES, false, true) {
}

void Eight::PrintName() {
        std::cout << "Eight." << std::endl;
}

void Eight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(EightAnimation::EIGHT_STICKY);
}

IEntity* Eight::Create() {
        return new Eight();
}