/*!*****************************************************************************
\par filename: stub.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "CarToCarCollider.h"
#include "PlayerController.h"

namespace FFunctional
{

  void DoorBehavior::Init()
  {
    GetParent().ComponentGet<ColliderCircle>().SetCollisionHandler(DoorBehaviorCollisionHandler);
  }

  void DoorBehavior::Read(const rapidjson::Value& value)
  {
    if (value.HasMember("Filepath"))
    {
      sceneDir_ = value["Filepath"].GetString();
    }
  }

  void DoorBehavior::Update(float dt)
  {
    dt;
  }

  void DoorBehavior::Open(Entity doorTrigger)
  {
    if (doorTrigger.Has<PlayerController>())
    {
      std::shared_ptr<Scene> newScene = LoadScene(*(engine_->GetSystem<Manager<Scene>>()),
        sceneDir_.c_str());
      ECS& newEcs = newScene->getECS();
      ECS* currEcs = doorTrigger.GetParent();
      newEcs.EntityCopy(currEcs, doorTrigger.GetEntity());
      currEcs->AddToKillList(doorTrigger.GetEntity());
    }
    else
    {
      ECS& newEcs = newScene->getECS();
      ECS* currEcs = doorTrigger.GetParent();
    }
  }

  void DoorBehaviorCollisionHandler(Entity door, Entity doorTrigger)
  {
    door.ComponentGet<DoorBehavior>().Open(doorTrigger);
  }
}