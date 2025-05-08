/*!*****************************************************************************
\par filename: stub.cpp
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  Implementation of Tendril Behavior for the Detective Mode state
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "TendrilBehavior.h"
#include "Sprite.h"
#include "PlayerController.h"
#include "Transform.h"
#include "CameraBehavior.h"

namespace FFunctional
{
  void TendrilBehavior::Init()
  {

  }
  void TendrilBehavior::Read(const rapidjson::Value&)
  {

  }
  void TendrilBehavior::Update(float dt)
  {
    dt;
    Transform& tendrilTrans = GetParent().ComponentGet<Transform>();
    ECS* sceneEcs_ = GetParent().GetParent();
    CameraBehavior& cam = sceneEcs_->ComponentGetFirst<CameraBehavior>();
    Transform& camTrans = cam.GetParent().ComponentGet<Transform>();

    tendrilTrans.SetTranslation(camTrans.GetTranslation());
  }
  void TendrilBehavior::SetActive(float opacity)
  {
    Sprite& tendrilSprite = GetParent().ComponentGet<Sprite>();
    Transform& tendrilTrans = GetParent().ComponentGet<Transform>();

    tendrilSprite.SetOpacity(opacity);
    tendrilTrans.SetRotation(sinf(opacity * 2.0f));
    glm::vec3 scale = glm::vec3(2.8 + opacity/4, 2.0 + opacity/4, 1.0);
    tendrilTrans.SetScale(&scale);
  }

  void TendrilBehavior::Dissipate(float opacity)
  {
    Sprite& tendrilSprite = GetParent().ComponentGet<Sprite>();
    tendrilSprite.SetOpacity(opacity);
  }
}