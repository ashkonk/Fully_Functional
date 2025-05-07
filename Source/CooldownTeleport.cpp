/*!*****************************************************************************
\par filename: TeleportBehavior.cpp
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
#include "Teleport.h"
#include "AttendantA.h"
#include "PlayerController.h"

namespace FFunctional
{
    void TeleportBehavior::Read(const rapidjson::Value&)
    {
    }

    void TeleportBehavior::Update(float dt)
    {
        dt;
        PlayerController player = parent_.GetParent()->ComponentGetFirst<PlayerController>();
        if (player.GetMoveState() == 3)
        {
            parent_.ComponentGet<Sprite>().SetOpacity(0.5f);
        }
        else
        {
            parent_.ComponentGet<Transform>().SetTranslation(player.GetParent().ComponentGet<Transform>().GetTranslation());
            parent_.ComponentGet<Sprite>().SetOpacity(0.0f);
        }
    }
}