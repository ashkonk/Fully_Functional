/*!*****************************************************************************
\par filename: stub.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "DoorBehavior.h"
#include "PlayerController.h"


namespace FFunctional
{

  //void DoorBehavior::Init()
  //{
    //GetParent().ComponentGet<ColliderCircle>().SetCollisionHandler(DoorBehaviorCollisionHandler);
  //}

  void DoorBehavior::Read(const rapidjson::Value& value)
  {
    if (value.HasMember("Filepath"))
    {
      sceneDir_ = value["Filepath"].GetString();
    }
    if (value.HasMember("EntryPosition"))
    {
      JsonConvert(value["EntryPosition"], entryPos_);
    }
  }

  void DoorBehavior::Update(float dt)
  {
    dt;
  }

  void DoorBehavior::Open(Entity doorTrigger)
  {
    //Check if the scene already exists (if sceneDir_ == Scene.GetName()
    //Then just move entity to that scene's ecs
    //if it's the player
    ECS* currEcs = doorTrigger.GetParent();
    Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());

    if (doorTrigger.Has<PlayerController>())
    {
      //move door Trigger to new door
      Transform& triggerTrans = doorTrigger.ComponentGet<Transform>();
      glm::vec3 triggerPos = *triggerTrans.GetTranslation();
      triggerPos = entryPos_;
      triggerTrans.SetTranslation(&triggerPos);

      //load new scene
      std::shared_ptr<Scene> newScene = LoadScene(*sceneManager,
        sceneDir_.c_str());
      ECS& newEcs = newScene->getECS();
      newEcs.EntityCopy(currEcs, doorTrigger.GetEntity());
      currEcs->AddToKillList(doorTrigger.GetEntity());
      SetECSState(*currEcs, true);
    }
  }

  //void DoorBehaviorCollisionHandler(Entity door, Entity doorTrigger)
  //{
    //door.ComponentGet<DoorBehavior>().Open(doorTrigger);
  //}
}