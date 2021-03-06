#include "bnExplosion.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnField.h"
#include "bnTile.h"

using sf::IntRect;

Explosion::Explosion(Field* _field, Team _team, int _numOfExplosions, double _playbackSpeed) : Artifact(_field)
{
  root = this;
  SetLayer(-1000);
  field = _field;
  team = _team;
  numOfExplosions = _numOfExplosions;
  playbackSpeed = _playbackSpeed;
  count = 0;
  setTexture(LOAD_TEXTURE(MOB_EXPLOSION));
  setScale(2.f, 2.f);
  animationComponent = new AnimationComponent(this);
  animationComponent->Setup("resources/mobs/mob_explosion.animation");
  animationComponent->Reload();

  offsetArea = sf::Vector2f(20.f, 0.f);
  SetOffsetArea(offsetArea);

  AUDIO.Play(AudioType::EXPLODE, AudioPriority::LOW);

  animationComponent->SetAnimation("EXPLODE");
  animationComponent->SetPlaybackSpeed(playbackSpeed);
  animationComponent->OnUpdate(0.0f);

  /*
   * On the 12th frame, increment the explosion count, and turn the first 
   * explosion transpatent.
   * 
   * If there are more explosions expected, spawn a copy on frame 8
   */
  animationComponent->AddCallback(12, [this]() {
    this->root->IncrementExplosionCount();
    this->setColor(sf::Color(0, 0, 0, 0));
  }, Animator::NoCallback, true);

  if (_numOfExplosions > 1) {
    animationComponent->AddCallback(8, [this, _field, _team, _numOfExplosions]() {
      this->GetField()->AddEntity(*new Explosion(*this), *this->GetTile());
    }, Animator::NoCallback, true);
  }

  this->RegisterComponent(animationComponent);
}

Explosion::Explosion(const Explosion & copy) : Artifact(copy.GetField())
{
  root = copy.root;

  count = 0; // uneeded for this copy
  SetLayer(-1000);
  field = copy.GetField();
  team = copy.GetTeam();
  numOfExplosions = copy.numOfExplosions-1;
  playbackSpeed = copy.playbackSpeed;
  setTexture(LOAD_TEXTURE(MOB_EXPLOSION));
  setScale(2.f, 2.f);

  animationComponent = new AnimationComponent(this);
  animationComponent->Setup("resources/mobs/mob_explosion.animation");
  animationComponent->Reload();

  SetOffsetArea(copy.offsetArea);

  AUDIO.Play(AudioType::EXPLODE, AudioPriority::LOW);

  animationComponent->SetAnimation("EXPLODE");
  animationComponent->SetPlaybackSpeed(playbackSpeed);
  animationComponent->OnUpdate(0.0f);

  /**
   * Tell root to increment explosion count on frame 12
   * 
   * Similar to the root constructor, if there are more explosions
   * Spawn a copy on frame 8
   */
  animationComponent->AddCallback(12, [this]() {
    this->Delete(); this->root->IncrementExplosionCount();
  }, Animator::NoCallback, true);

  if (numOfExplosions > 1) {
    animationComponent->AddCallback(8, [this]() {
      this->GetField()->AddEntity(*new Explosion(*this), *this->GetTile());
    }, Animator::NoCallback, true);
  }
  else {
    // Last explosion happens behind entities
    this->SetLayer(1000); // ensure bottom draw
  }

  this->RegisterComponent(animationComponent);
}

void Explosion::OnUpdate(float _elapsed) {
  /*
   * Keep root alive until all explosions are completed, then delete root
   */
  if (this == root) {
    if (count == numOfExplosions) {
      Delete();
      return;
    }
  }

  if(this->numOfExplosions != 1) {
    setPosition((GetTile()->getPosition().x + offset.x), (GetTile()->getPosition().y + offset.y));
  } else {
    setPosition((GetTile()->getPosition().x), (GetTile()->getPosition().y));
  }
}

void Explosion::IncrementExplosionCount() {
  count++;
}

void Explosion::SetOffsetArea(sf::Vector2f area)
{
  if ((int)area.x == 0) area.x = 1;
  if ((int)area.y == 0) area.y = 1;

  this->offsetArea = area;

  int randX = rand() % (int)(area.x+0.5f);
  int randY = rand() % (int)(area.y+0.5f);

  int randNegX = 1;
  int randNegY = 1;

  if (rand() % 10 > 5) randNegX = -1;
  if (rand() % 10 > 5) randNegY = -1;

  randX *= randNegX;
  randY *= -randY;

  offset = sf::Vector2f((float)randX, (float)randY);
}

Explosion::~Explosion()
{
}