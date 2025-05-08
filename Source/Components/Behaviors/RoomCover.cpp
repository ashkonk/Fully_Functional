/*!*****************************************************************************
\par filename: RoomCover
\par  authors: Ashkon Khalkhali, Alijah Rosner
\par     date: 09/09/2022

\brief
  This is the implimentation for the room cover behavior.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "RoomCover.h"
#include "PlayerController.h"
#include "VisionCone.h"
#include "GuardBehavior.h"
#include "CCTV.h"

namespace FFunctional
{
    void RoomCover::Read(const rapidjson::Value& value)
    {
        value;
        if (value.HasMember("RoomPosition"))
        {
            JsonConvert(value["RoomPosition"], roomPos_);
        }
    }

    void RoomCover::Update(float dt)
    {
        dt;
        ECS* ecs = GetParent().GetParent();
        if (!ecs->HasManager<VisionConeBehavior>() || !ecs->HasManager<CCTVBehavior>() || !ecs->HasManager<GuardBehavior>()
          || !ecs->HasManager<PlayerController>())
        {
          return;
        }
        Sprite* coverSprite = &GetParent().ComponentGet<Sprite>();
        PlayerController* player = &GetParent().GetParent()->ComponentGetFirst<PlayerController>();
        VisionConeBehavior* visionCone = &GetParent().GetParent()->ComponentGetFirst<VisionConeBehavior>();
        
        if (hidden_ == true)
        {
            
            if (coverSprite->GetOpacity() < 1.0f)
            {
                coverSprite->SetOpacity(coverSprite->GetOpacity() + (dt * 1.5f));
            }

            if (player->GetVisionState() == 1)
            {
                visionCone->GetParent().ComponentGet<Sprite>().SetOpacity(0.5f);
                
            }
            else
            {
                visionCone->GetParent().ComponentGet<Sprite>().SetOpacity(0.0f);

            }
        }
        else
        {

            if (coverSprite->GetOpacity() > 0.0f)
            {
                coverSprite->SetOpacity(coverSprite->GetOpacity() - (dt * 1.5f));
            }
        }
    }

    void RoomCover::EnterCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        if (other.GetParent().Has<PlayerController>())
        {
            hidden_ = false;
        }
    }

    void RoomCover::ExitCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        if (other.GetParent().Has<PlayerController>())
        {
            hidden_ = true;
        }
    }
}