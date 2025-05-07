/*!*****************************************************************************
\par filename: GuardBehavior.cpp
\par  authors: Alijah Rosner
\par     date: 11/16/2022

\brief
    This file contains the behavior component and the state machine belonging to
    the Guard Drone.
      Copyright © 2022 DigiPen (USA) Corporation.

*******************************************************************************/
#include "Sprite.h"
#include "GuardBehavior.h"
#include "Physics.h"
#include "Transform.h"
#include "QuickMaths.h"
#include <glm/vec3.hpp>
#include "PlayerController.h"
#include "TrashFire.h"
#include <iostream>
#include "StealthBar.h"

namespace FFunctional
{
  // have a colision shape for vision
  // on player entering vision
  // increase alertness 
  // if play is in visionc code > 3 seconds 
  // investigate players old translation
  // if player is STILL within
  // chase 


  bool GuardBehavior::GetColliding() const
  {
    return colliding_;
  }

  bool GuardBehavior::GetDistracted() const
  {
    return distracted_;
  }

  void GuardBehavior::Read(const rapidjson::Value& value)
  {
    movement_->Read(value["GuardMovement"]);
//    rapidjson::Document docu = OpenJson("./Data/DroneVision.json");
    walk_ = engine_->GetSystem<Audio>()->GetSound("Chef");
    walk_->SetInRange(true);
    walk_->Play();
  }



  void GuardBehavior::Update(float dt)
  {
    movement_->Update(dt);
    if (GetParent().Has<Animation>())
    {
        Animation& animation = GetParent().ComponentGet<Animation>();
        animation.SetIsLooping(true);
        Physics& physics = GetParent().ComponentGet<Physics>();
        Transform& transform = GetParent().ComponentGet<Transform>();

        glm::vec3 velocity = *physics.GetVelocity();

        if (velocity.x >= 0)
        {
            glm::vec3 scale = { 0.5, 0.5, 0 };
            transform.SetScale(&scale);

            animation.TryPlay("Walk");

        }
        else
        {
            glm::vec3 scale = { -0.5, 0.5, 0 };
            transform.SetScale(&scale);
            animation.TryPlay("Walk");
        }
    }
    if (colliding_)
    {
        Transform& cctvTransform = parent_.ComponentGet<Transform>();
        glm::vec3 cctvTranslation = *cctvTransform.GetTranslation();
        glm::vec3 pos = *parent_.GetParent()->ComponentGetFirst<PlayerController>().GetParent().ComponentGet<Transform>().GetTranslation();
        // timer_ += dt * (glm::distance(pos, cctvTranslation) < detectTime_);
        timer_ += dt / movement_->GetDetectTime();
        //std::cout << timer_ << std::endl;
        StealthBar::SetFill(timer_);
    }
    else
    {
        timer_ = 0.0f;
    }
  }


  void GuardMovement::UpdatePassivePosition(float dt)
  {
    dt;
    alertCounter_ = 0.0f;

    Physics& phys = parent_.ComponentGet<Physics>();
    // Occluder& occluder = parent_.ComponentGet<Occluder>();
     //Attendant physics and transform
    Transform& trans = parent_.ComponentGet<Transform>();
    Transform& playerTrans = parent_.GetParent()->ComponentGetFirst<PlayerController>().GetParent().ComponentGet<Transform>();

    //Velocity, position scale
    glm::vec3 currVelocity = *phys.GetVelocity();
    glm::vec3 currPos = *trans.GetTranslation();

    //destination stuff
    glm::vec3 destination;
    glm::vec3 dirVec;
    //float destRot;

    destination = positions_.at(route_);
    dirVec = destination - currPos;
    dirVec = glm::normalize(dirVec);
    dirVec *= speed_ + dt;
    //destRot = (Vector2DToAngleRad(&dirVec));
    //trans.SetRotation(destRot);
    phys.SetVelocity(&dirVec);

    if (glm::distance(currPos, destination) <= 0.05)
    {
        ++route_;
        if (route_ == positions_.size())
        {
            route_ = 0;
        }
    }

    if (playerTrans.GetTranslation()->x > 1.0)
    {
        speed_ *= 3; 
        SetNextState(GuardState::Puzzle);
    }
  }

  /*void GuardMovement::UpdateAlertedPosition(float dt)
  {
    Transform& guardTransform = parent_.ComponentGet<Transform>();
    glm::vec3 guardTranslation = *guardTransform.GetTranslation();
    glm::vec3 pos = *investigateEntity_.ComponentGet<Transform>().GetTranslation();

    alertCounter_ += dt / detectTime_;
  }


  static float sig(float f)
  {
    float e_x = std::pow(2.71828f, f);
    return e_x / (e_x + 1);
  }

  void GuardMovement::UpdateInvestigating(float dt)
  {
    dt;
    Transform& guardTransform = parent_.ComponentGet<Transform>();
    glm::vec3 guardTranslation = *guardTransform.GetTranslation();

    glm::vec3 pos = *investigateEntity_.ComponentGet<Transform>().GetTranslation();

    float distance = glm::distance(pos, guardTranslation);

    StealthBar::SetFill(alertCounter_);


    if (alertCounter_ < 2.0f)
    {
      if (speed_ * dt > distance)
        return;

      
      glm::vec3 direction = glm::normalize(pos - guardTranslation) * 2.0f*(sig(distance) - 0.5f);
      direction *= speed_;
      Physics& guardPhysics = parent_.ComponentGet<Physics>();

      float dot = glm::dot(glm::normalize(direction), glm::normalize(*guardPhysics.GetVelocity()));
      if (dot < 0.0f)
      {
        glm::vec3 vel = (*guardPhysics.GetVelocity())*(1 + 0.25f*dot);
        guardPhysics.SetVelocity(&vel);
      }


      direction = glm::normalize(direction) * speed_;
      guardPhysics.SetAcceleration(&direction);
    }
    else
    {
      caught_ = true;
    }


  }*/

  bool GuardMovement::GetCaught() const
  {
    return caught_;
  }

  void GuardMovement::UpdateChase(float dt)
  {
    dt;
  }

  void GuardMovement::Read(const rapidjson::Value& value)
  {
    speed_ = value["MoveSpeed"].GetFloat();
    detectTime_ = value["DetectTime"].GetFloat();

    for (const auto& arrayValue : value["WalkingPoints"].GetArray())
    {
      glm::vec3 position;
      JsonConvert(arrayValue, position);
      positions_.push_back(position);
    }

    for (const auto& arrayValue : value["WalkingPointsTwo"].GetArray())
    {
        glm::vec3 position;
        JsonConvert(arrayValue, position);
        puzzlePositions_.push_back(position);
    }

    SetNextState((GuardState)value["BehaviorState"].GetInt());

  }

  void GuardMovement::OnInit()
  {
    switch (GetCurrState())
    {
    case GuardState::Passive:
    {
      break;
    }
    case GuardState::Investigating:
    {
      break;
    }
    case GuardState::Chase:
    {
      break;
    }
    }
  }

  void GuardMovement::OnUpdate(float dt)
  {
    switch (GetCurrState())
    {
    case GuardState::Passive:
    {
      UpdatePassivePosition(dt);
      break;
    }
    case GuardState::Investigating:
    {
      //UpdateInvestigating(dt);
      //UpdateAlertedPosition(dt);
      break;
    }
    case GuardState::Chase:
    {
      //UpdateChase(dt);
      break;
    }
    case GuardState::Puzzle:
        UpdatePuzzlePosition(dt);
    }
  }

  void GuardMovement::UpdatePuzzlePosition(float dt)
  {
      alertCounter_ += dt; 

      if (alertCounter_ < 30.0f)
      {
          Physics& phys = parent_.ComponentGet<Physics>();
          // Occluder& occluder = parent_.ComponentGet<Occluder>();
           //Attendant physics and transform
          Transform& trans = parent_.ComponentGet<Transform>();

          //Velocity, position scale
          glm::vec3 currVelocity = *phys.GetVelocity();
          glm::vec3 currPos = *trans.GetTranslation();

          //destination stuff
          glm::vec3 destination;
          glm::vec3 dirVec;
          //float destRot;

          destination = puzzlePositions_.at(puzzleRoute_);
          dirVec = destination - currPos;
          dirVec = glm::normalize(dirVec);
          dirVec *= speed_ + dt;
          //destRot = (Vector2DToAngleRad(&dirVec));
          //trans.SetRotation(destRot);
          phys.SetVelocity(&dirVec);

          if (glm::distance(currPos, destination) <= 0.05)
          {
              ++puzzleRoute_;
              if (puzzleRoute_ == puzzlePositions_.size())
              {
                  puzzleRoute_ = 0;
              }
          }

      }
      else
      {
          parent_.Destroy();
      }

  }

  void GuardMovement::OnExit()
  {

  }

  float GuardMovement::GetAlertCounter() const
  {
    return alertCounter_;
  }

  void GuardMovement::SetInvestigatePosition(const glm::vec3 vector)
  {
    investigatePosition_ = vector;
  }

  float GuardMovement::GetDetectTime() const
  {
      return detectTime_;
  }

  void GuardMovement::SetInvestigateEntity(const Entity& entity)
  {
    investigateEntity_ = entity;
  }


  void GuardBehavior::EntityFound(const Vision& self, const Entity& other)
  {
    self;
    self;
    if (other.Has<PlayerController>())
    {
        colliding_ = true;
    }
  }

  void GuardBehavior::EntityLost(const Vision& self, const Entity& other)
  {
    self;
    other;
    self;
    if (other.Has<PlayerController>())
    {
        colliding_ = false;
    }
  }

}