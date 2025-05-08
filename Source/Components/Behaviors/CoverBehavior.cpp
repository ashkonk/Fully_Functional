/*!*****************************************************************************
\par filename: stub.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "CoverBehavior.h"
#include "PlayerController.h"


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
    //if (GetParent().Has<Sprite>())
    //{
      Sprite* coverSprite = &GetParent().ComponentGet<Sprite>();
      if (hidden_ == true)
      {
        coverSprite->SetOpacity(1.0f);
      }
      else
      {
        coverSprite->SetOpacity(0.0f);
      }
    //}
  }

  void RoomCover::EnterCollision2D(const Collider& collider)
  {
    if (collider.GetParent().Has<PlayerController>())
    {
      hidden_ = false;
    }
  }

  void RoomCover::ExitCollision2D(const Collider& collider)
  {
    if (collider.GetParent().Has<PlayerController>())
    {
      hidden_ = true;
    }
  }

  //void DoorBehaviorCollisionHandler(Entity door, Entity doorTrigger)
  //{
    //door.ComponentGet<DoorBehavior>().Open(doorTrigger);
  //}
}