/*!*****************************************************************************
\par filename: Lighter.cpp
\par  authors: Alijah Rosner
\par     date: 11/23/2022

\brief
    This file contains the behavior component for the lighter, which
    lights the fire.
    Copyright © 2022 DigiPen (USA) Corporation.

*******************************************************************************/
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "Lighter.h"
#include "AttendantA.h"
#include "PlayerController.h"

namespace FFunctional
{
    /* Reads in private variables */
    void LighterBehavior::Read(const rapidjson::Value& value)
    {
        pickup_ = value["pickup"].GetBool();
    }

    void LighterBehavior::Update(float dt)
    {
        GetParent();
        dt;
    }

    bool LighterBehavior::GetLighterBehaviorPickup() const
    {
        return pickup_;
    }

    void LighterBehavior::SetLighterBehaviorPickup(bool flag)
    {
        pickup_ = flag;
    }


    void LighterBehavior::EnterCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        ECS* ecs = parent_.GetParent();

        if (ecs->GetActive() && other.GetParent().Has<PlayerController>())
        {
            PlayerController* player = &other.GetParent().ComponentGet<PlayerController>();
            if (pickup_ == false)
            {
                pickup_ = true;
                player->AddToInventory("Lighter", parent_);
                parent_.Destroy();
            }
        }
    }
}