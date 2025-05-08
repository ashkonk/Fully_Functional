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
#include "Shadow.h"
#include "AttendantA.h"
#include "PlayerController.h"
#include "AntagBehavior.h"

namespace FFunctional
{
    void Shadow::Read(const rapidjson::Value& value)
    {
        if (value.HasMember("ParentNumber"))
        {
            parentNumber_ = value["ParentNumber"].GetInt();
        }

        if (parentNumber_ == 0)
        {
            parent_ = GetParent().GetParent()->ComponentGetFirst<PlayerController>().GetParent();
        }
        else if (parentNumber_ == 1)
        {
            parent_ = GetParent().GetParent()->ComponentGetFirst<AntagBehavior>().GetParent();
        }
    }

    void Shadow::Update(float dt)
    {
        dt;
        glm::vec3 parentTranslate = *parent_.ComponentGet<Transform>().GetTranslation();
        glm::vec3 offset = { 0.0, -0.1775, 0.0 };
        offset += parentTranslate;
        GetParent().ComponentGet<Transform>().SetTranslation(&offset);
    }
}