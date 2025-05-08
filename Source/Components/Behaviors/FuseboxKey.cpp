/*!*****************************************************************************
\par filename: FuseboxKey.cpp
\par  authors: Alijah Rosner
\par     date: 11/23/2022

\brief
    This file contains the behavior component for the fusebox key, which 
    disables the fusebox.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "FuseboxKey.h"
#include "AttendantA.h"
#include "PlayerController.h"

namespace FFunctional
{
    /* Reads in private variables */
    void FuseboxKeyBehavior::Read(const rapidjson::Value& value)
    {
        pickup_ = value["pickup"].GetBool();

    }

    void FuseboxKeyBehavior::Update(float dt)
    {
        dt;
    }

    bool FuseboxKeyBehavior::GetFuseboxKeyBehaviorPickup() const
    {
        return pickup_;
    }

    void FuseboxKeyBehavior::SetFuseboxKeyBehaviorPickup(bool flag)
    {
        pickup_ = flag;
    }


    void FuseboxKeyBehavior::EnterCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
      other;
        //ECS* ecs = parent_.GetParent();

        //if (ecs->GetActive() && collider.GetParent().Has<PlayerController>())
        //{
        //    PlayerController* player = &collider.GetParent().ComponentGet<PlayerController>();
        //    if (pickup_ == false)
        //    {
        //        pickup_ = true;
        //        player->AddToInventory("FuseboxKey", parent_);
        //        parent_.Destroy();
        //    }
        //}
    }
}