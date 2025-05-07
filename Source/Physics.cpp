/*!*****************************************************************************
\par filename: Physics.cpp
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Physics.h"
#include "Transform.h"
#include "QuickMaths.h"
#include "Entity.h"

namespace FFunctional
{

  void Physics::Read(const rapidjson::Value& value)
  {
    JsonConvert(value["acceleration"], acceleration_);
    JsonConvert(value["velocity"], velocity_);
  }

  const glm::vec3* Physics::GetAcceleration() const
  {
    if (this)
    {
      return &acceleration_;
    }
    return NULL;
  }

  const glm::vec3* Physics::GetVelocity() const
  {
    if (this)
    {
      return &velocity_;
    }
    return NULL;
  }

  float Physics::GetRotationVelocity() const
  {
    if (this)
    {
      return rotationalVelocity_;
    }
    return 0.0f;
  }

  const glm::vec3* Physics::GetOldTranslation() const
  {
    if (this)
    {
      return &oldTranslation_;
    }
    return NULL;
  }

  void Physics::AddForce(const glm::vec3* force)
  {
    acceleration_.x += force->x;
    acceleration_.y += force->y;
  }

  void Physics::SetAcceleration(const glm::vec3* acceleration)
  {
    acceleration_.x = acceleration->x;
    acceleration_.y = acceleration->y;
  }

  void Physics::SetVelocity(const glm::vec3* velocity)
  {
    velocity_.x = velocity->x;
    velocity_.y = velocity->y;
  }

  void Physics::SetRotationalVelocity(float rotationalVelocity)
  {
    rotationalVelocity_ = rotationalVelocity;
  }

  void Physics::Update(float dt)
  {
      Transform& transform = GetParent().ComponentGet<Transform>();
      glm::vec3 translation = *transform.GetTranslation();
      float rotVelocity = transform.GetRotation();
      rotVelocity += rotationalVelocity_ * dt;
      oldTranslation_.x = translation.x;
      oldTranslation_.y = translation.y;
      Vector2DScaleAdd(&velocity_, &acceleration_, &velocity_, dt);
      Vector2DScaleAdd(&translation, &velocity_, &translation, dt);
      transform.SetTranslation(&translation);
      transform.SetRotation(rotVelocity);
  }
}