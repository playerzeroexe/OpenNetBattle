#pragma once

#include "../bnBattleSceneState.h"
#include "../../bnAnimation.h"

#include <SFML/Graphics/Sprite.hpp>

class Player;

/*
    \brief This state handles transformations
*/
class CharacterTransformBattleState final : public BattleSceneState {
  enum class state : int {
    fadein = 0,
    animate,
    fadeout
  } currState{ state::fadein };

  using TrackedFormData = std::tuple<Player*, int, bool>;

  int lastSelectedForm{ -1 };
  double backdropTimer{ 0 };
  double backdropLength{ 0 };
  bool isLeavingFormChange{ false };
  Animation shineAnimation;
  sf::Sprite shine;
  std::vector<TrackedFormData> tracking;

  const bool FadeInBackdrop();
  const bool FadeOutBackdrop();
  void UpdateAnimation(double elapsed);

public:
  bool IsFinished();
  void onStart() override;
  void onUpdate(double elapsed) override;

  CharacterTransformBattleState(std::vector<TrackedFormData> tracking);
};