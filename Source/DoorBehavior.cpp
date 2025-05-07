/*!*****************************************************************************
\par filename: DoorBehavior.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/09/2022

\brief
  This is the implementation for the Door Behavior class. 
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "DoorBehavior.h"
#include "PlayerController.h"
#include "CameraBehavior.h"
#include "Camera.h"
#include "RoomCover.h"

#include "Disruptions.h"
#include "FadeController.h"

namespace FFunctional
{
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

  static bool wasDown = false;
  void DoorBehavior::Update(float dt)
  {
      Input* input = engine_->GetSystem<Input>();

      if (input->GetKeyState(GLFW_KEY_EQUAL) == Keystate::Down && !wasDown)
          debugTasks++;

      wasDown = (input->GetKeyState(GLFW_KEY_EQUAL) == Keystate::Down);

      //wasNotDown = input->GetKeyState(GLFW_KEY_K) == Keystate::Up;

    dt;
  }

  //Moves Entity "doorTrigger" from current scene 
  //to scene specified in JSON.
  void DoorBehavior::Open(Entity doorTrigger)
  {
    Disruptions& tasks = GetParent().GetParent()->ComponentGetFirst<Disruptions>();
      
    /* completed the three required tasks for the level. */
    bool winCondition = (tasks.GetTasksDone() >= 3);

    if (!winCondition)
        return;
   
    ECS* currEcs = doorTrigger.GetParent();
    Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());

    //load new scene
    std::shared_ptr<Scene> newScene = LoadScene(*sceneManager,
      sceneDir_.c_str());
    ECS& newEcs = newScene->getECS();

    //if it's the player
    if (doorTrigger.Has<PlayerController>())
    {
      //move door Trigger to new door
      Transform& playerTrans = doorTrigger.ComponentGet<Transform>();
      glm::vec3 playerPos = *playerTrans.GetTranslation();
      playerPos = entryPos_;
      playerTrans.SetTranslation(&playerPos);


      //copy player to new scene, and camera
      CameraBehavior& sceneCam = currEcs->ComponentGetFirst<CameraBehavior>();
      newEcs.EntityCopy(currEcs, sceneCam.GetParent().GetEntity());
      newEcs.EntityCopy(currEcs, doorTrigger.GetEntity());

      //remove them from this scene
      currEcs->AddToKillList(sceneCam.GetParent().GetEntity());
      currEcs->AddToKillList(doorTrigger.GetEntity());

      SetECSState(*currEcs, false);
    }
    else
    {
      //Attendant's move and vision object don't get added to new ECS
      //the set parent isn't called on children the behavior holds.
      newEcs.EntityCopy(currEcs, doorTrigger.GetEntity());
      currEcs->AddToKillList(doorTrigger.GetEntity());
      SetECSState(newEcs, false);
    }
  }


  int DoorBehavior::GetDebugTasks() const
  {
      return debugTasks;
  }

  void DoorBehavior::EnterCollision2D(const Collider& self,
  const Collider& other)
  {

    self;
    Entity triggerEntity = other.GetParent();
    if (triggerEntity.Has<PlayerController>())
    {
      //Disruptions& tasks = GetParent().GetParent()->ComponentGetFirst<Disruptions>();

      /* completed the three required tasks for the level. */
      //bool winCondition = (tasks.GetTasksDone() + debugTasks >= 3);

      /*if (!winCondition)
        return;*/

      

      if (GetParent().GetParent()->HasManager<FadeControllerBehavior>())
      {
        GetParent().GetParent()->ComponentGetFirst<FadeControllerBehavior>().FadeOut();
      }
      else
      {
        Manager<Scene>* sceneManager = engine_->GetSystem<Manager<Scene>>();
        std::shared_ptr<Scene> newScene = LoadScene(*sceneManager, sceneDir_.c_str());

        GetParent().GetParent()->Clear();
        GetParent().GetParent()->SetActive(false);

        engine_->SetLastLevel(engine_->GetLastLevel() + 1);
      }


      //Open(triggerEntity);
    }
  }
}