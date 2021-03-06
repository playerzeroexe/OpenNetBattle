#include "bnRoll.h"
#include "bnExplodeState.h"
#include "bnField.h"
#include "bnBuster.h"
#include "bnRecoverChipAction.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"
#include "bnLogger.h"
#include "bnBusterChipAction.h"

const std::string RESOURCE_PATH = "resources/navis/roll/roll.animation";

Roll::Roll() : Player()
{
  name = "Roll";
  chargeEffect.setPosition(0, -30.0f);
  chargeEffect.SetFullyChargedColor(sf::Color::Yellow);

  SetLayer(0);
  team = Team::RED;
  this->SetElement(Element::PLUS);

  animationComponent->Setup(RESOURCE_PATH);
  animationComponent->Reload();

  setTexture(*TEXTURES.GetTexture(TextureType::NAVI_ROLL_ATLAS));

  this->SetHealth(1500);

  SetFloatShoe(true);
}

const float Roll::GetHeight() const
{
  return 140.0f;
}

ChipAction * Roll::ExecuteSpecialAction()
{
  return new RecoverChipAction(this, 20);
}

ChipAction* Roll::ExecuteBusterAction()
{
  return new BusterChipAction(this, false, 1);
}

ChipAction* Roll::ExecuteChargedBusterAction()
{
  return new BusterChipAction(this, true, 20);
}