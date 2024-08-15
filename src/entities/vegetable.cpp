#include <entities/vegetable.h>
#include <chrono>

Vegetable::Vegetable() :
        IEntity(EntityIdentificator::EGGPLANT, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

Vegetable::Vegetable(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable, bool is_traversable) :
        IEntity(_id, _type, surface_type, max_states, is_breakable, is_traversable) {
}

void Vegetable::PrintName() {
        std::cout << "Vegetable." << std::endl;
}

bool Vegetable::Update(uint8_t pressedKeys_) {
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

void Vegetable::Hit(bool hitFromLeft) {
    RemoveFromSpacePartitionObjectsTree();
    isMarkedToDelete = true;
}

void Vegetable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
}

IEntity* Vegetable::Create() {
          return new Vegetable();
}

Vegetable::~Vegetable() {

}

/* Eggplant */

Eggplant::Eggplant() :
        Vegetable(EntityIdentificator::EGGPLANT, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Eggplant::PrintName() {
        std::cout << "Eggplant." << std::endl;
}

void Eggplant::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(EggplantAnimation::EGGPLANT_STICKY);
}

IEntity* Eggplant::Create() {
        return new Eggplant();
}

/* Carrot */

Carrot::Carrot() :
        Vegetable(EntityIdentificator::CARROT, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Carrot::PrintName() {
        std::cout << "Carrot." << std::endl;
}

void Carrot::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CarrotAnimation::CARROT_STICKY);
}

IEntity* Carrot::Create() {
        return new Carrot();
}

/* Cabbage */

Cabbage::Cabbage() :
        Vegetable(EntityIdentificator::CABBAGE, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Cabbage::PrintName() {
        std::cout << "Cabbage." << std::endl;
}

void Cabbage::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CabbageAnimation::CABBAGE_STICKY);
}

IEntity* Cabbage::Create() {
        return new Cabbage();
}

/* Zucchini */

Zucchini::Zucchini() :
        Vegetable(EntityIdentificator::ZUCCHINI, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Zucchini::PrintName() {
        std::cout << "Zucchini." << std::endl;
}

void Zucchini::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(ZucchiniAnimation::ZUCCHINI_STICKY);
}

IEntity* Zucchini::Create() {
        return new Zucchini();
}

/* Corn */

Corn::Corn() :
        Vegetable(EntityIdentificator::CORN, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Corn::PrintName() {
        std::cout << "Corn." << std::endl;
}

void Corn::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CornAnimation::CORN_STICKY);
}

IEntity* Corn::Create() {
        return new Corn();
}

/* Turnip */

Turnip::Turnip() :
        Vegetable(EntityIdentificator::TURNIP, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Turnip::PrintName() {
        std::cout << "Turnip." << std::endl;
}

void Turnip::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(TurnipAnimation::TURNIP_STICKY);
}

IEntity* Turnip::Create() {
        return new Turnip();
}

/* Pumpkin */

Pumpkin::Pumpkin() :
        Vegetable(EntityIdentificator::PUMPKIN, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Pumpkin::PrintName() {
        std::cout << "Pumpkin." << std::endl;
}

void Pumpkin::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(PumpkinAnimation::PUMPKIN_STICKY);
}

IEntity* Pumpkin::Create() {
        return new Pumpkin();
}

/* RomaineLettuce */

RomaineLettuce::RomaineLettuce() :
        Vegetable(EntityIdentificator::ROMAINE_LETTUCE, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void RomaineLettuce::PrintName() {
        std::cout << "RomaineLettuce." << std::endl;
}

void RomaineLettuce::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(RomaineLettuceAnimation::ROMAINE_LETTUCE_STICKY);
}

IEntity* RomaineLettuce::Create() {
        return new RomaineLettuce();
}

/* Potato */

Potato::Potato() :
        Vegetable(EntityIdentificator::POTATO, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Potato::PrintName() {
        std::cout << "Potato." << std::endl;
}

void Potato::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(PotatoAnimation::POTATO_STICKY);
}

IEntity* Potato::Create() {
        return new Potato();
}

/* Mushroom */

Mushroom::Mushroom() :
        Vegetable(EntityIdentificator::MUSHROOM, EntityType::VEGETABLE, SurfaceType::SIMPLE, VegetableStateIdentificator::VEGETABLE_MAX_STATES, false, true) {
}

void Mushroom::PrintName() {
        std::cout << "Mushroom." << std::endl;
}

void Mushroom::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(MushroomAnimation::MUSHROOM_STICKY);
}

IEntity* Mushroom::Create() {
        return new Mushroom();
}