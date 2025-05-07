/*!*****************************************************************************
\par filename: Lighter.cpp
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the behavior component for the poison vial, which
    disables the attendant.
    Copyright © 2022 DigiPen (USA) Corporation.

*******************************************************************************/
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "QuickMaths.h"
#include "PoisonVial.h"
#include "AttendantA.h"
#include "PlayerController.h"

namespace FFunctional
{
    /* Reads in private variables */
    void PoisonVialBehavior::Read(const rapidjson::Value& value)
    {
        pickup_ = value["pickup"].GetBool();
        used_ = value["used"].GetBool();
    }

    void PoisonVialBehavior::Update(float dt)
    {
        dt;
    }

    bool PoisonVialBehavior::GetPoisonVialPickup() const
    {
        return pickup_;
    }

    bool PoisonVialBehavior::GetPoisonVialUsed() const
    {
        return used_;
    }

    void PoisonVialBehavior::SetPoisonVialPickup(bool flag)
    {
        pickup_ = flag;
    }

    void PoisonVialBehavior::SetPoisonVialUsed(bool flag)
    {
        used_ = flag;

    }

    void PoisonVialBehavior::EnterCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        ECS* ecs = parent_.GetParent();

        if (ecs->GetActive() && other.GetParent().Has<PlayerController>())
        {
            PlayerController* player = &other.GetParent().ComponentGet<PlayerController>();
            if (GetPoisonVialPickup() == false)
            {
                SetPoisonVialPickup(true);
                player->AddToInventory("PoisonVial", parent_);
                parent_.Destroy();
            }
        }
    }
}