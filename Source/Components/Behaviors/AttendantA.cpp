/*!*****************************************************************************
\par filename: AttendantA.cpp
\par  authors: Ashkon Khalkhali
\par     date: 10/16/2022

\brief
    This file contains the behavior component and the state machine belonging to
    Attendant A.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "AttendantA.h"
#include "PlayerController.h"
#include "Physics.h"
#include "Transform.h"
#include "QuickMaths.h"
#include "Animation.h"
#include <glm/vec3.hpp>
#include <cmath>

namespace FFunctional
{

    void AttendantMovement::Read(const rapidjson::Value& value)
    {
        Speed_ = value["MoveSpeed"].GetFloat();

        for (const auto& arrayValue : value["WalkingPoints"].GetArray())
        {
            glm::vec3 position;
            JsonConvert(arrayValue, position);
            Positions_.push_back(position);
        }
        SetNextState((BehaviorState)value["BehaviorState"].GetInt());

    }

    void AttendantMovement::OnInit()
    {
        switch (GetCurrState())
        {
        case BehaviorState::Passive:
            break;
        case BehaviorState::Alerted:
            break;
        }
    }


    void AttendantBehavior::EnterCollision2D(const Collider& self,
      const Collider& other)
    {
      self;
        if (other.GetParent().Has<PlayerController>())
        {
            PlayerController* controller = &other.GetParent().ComponentGet<PlayerController>();
            std::unordered_map<std::string, Entity&> inventory = *controller->GetInventory();
        }
    }



    void AttendantMovement::OnUpdate(float dt)
    {
        switch (GetCurrState())
        {
        case BehaviorState::Passive:
            UpdatePassivePosition(dt);

        case BehaviorState::Alerted:
            break;
        }

    }

    void AttendantMovement::OnExit()
    {
    }

    void AttendantMovement::UpdatePassivePosition(float dt)
    {
        if (!Parent_.IsDestroyed())
        {
            Animation& animation = Parent_.ComponentGet<Animation>();
            Physics& phys = Parent_.ComponentGet<Physics>();

            if (Parent_.ComponentGet<AttendantBehavior>().GetDisabled())
            {
                animation.TryPlay("Dizzy");
                glm::vec3 dizzyVec = { 0, 0, 0 };
                phys.SetVelocity(&dizzyVec);
            }
            else
            {
                //Attendant physics and transform
                Transform& trans = Parent_.ComponentGet<Transform>();
                //Velocity, position scale
                glm::vec3 currVelocity = *phys.GetVelocity();
                glm::vec3 currPos = *trans.GetTranslation();
                glm::vec3 scale = *trans.GetScale();


                //destination stuff
                glm::vec3 destination;
                glm::vec3 dirVec;
                //float destRot;

                destination = Positions_.at(Route_);
                dirVec = destination - currPos;
                dirVec = glm::normalize(dirVec);
                dirVec *= Speed_ + dt;
                //destRot = (Vector2DToAngleRad(&dirVec));
                //trans.SetRotation(destRot);
                phys.SetVelocity(&dirVec);

                float direction = glm::degrees(atan2(dirVec.y, dirVec.x));

                if (direction < 0)
                {
                    direction += 360; 
                }
                
                if (direction > 0 && direction < 90)
                {
                    animation.TryPlay("BackWalk");
                    scale = glm::vec3(0.2, 0.4, 1);
                }
                else if (direction >= 180 && direction < 270)
                {
                    animation.TryPlay("FrontWalk");
                    scale = glm::vec3(0.2, 0.4, 1);
                }
                else
                {
                    animation.TryPlay("SideWalk");
                    if (destination.x > currPos.x)
                    {
                        scale = glm::vec3(0.2, 0.4, 1);
                    }
                    else
                    {
                        scale = glm::vec3(-0.2, 0.4, 1);
                    }
                }
                
                if (glm::distance(currPos, destination) <= 0.05)
                {
                    ++Route_;
                    if (Route_ == Positions_.size())
                    {
                        Route_ = 0;
                    }
                }

                trans.SetScale(&scale);
            }
        }
    }

    void AttendantMovement::UpdateAlertedPosition(float)
    {

    }

    int AttendantMovement::GetRoute()
    {
        return Route_;
    }

    int AttendantBehavior::GetRouteFromBehavior()
    {
        return Movement_->GetRoute();
    }

    void AttendantBehavior::Read(const rapidjson::Value& value)
    {
        Movement_->Read(value["AttendantMovement"]);
    }

    bool AttendantBehavior::GetDisabled() const
    {
        return isDisabled_;
    }

    void AttendantBehavior::SetDisabled(bool truth)
    {
        isDisabled_ = truth;
    }

    void AttendantBehavior::Update(float dt)
    {
        if (parent_.GetParent()->GetActive())
        {

            if (!isDisabled_)
            {
                Movement_->Update(dt);
            }
            else
            {
                glm::vec3 velocity = { 0, 0, 0 };
                GetParent().ComponentGet<Physics>().SetVelocity(&velocity);
                if (!particlesDestroyed_)
                {

                    if (!particlesCreated_)
                    {
                        rapidjson::Document docu = OpenJson("./Data/PoisonThrow.json");
                        particles_.Read(docu, engine_);
                        particlesCreated_ = true;
                        particles_.ComponentGet<Transform>().SetTranslation(parent_.ComponentGet<Transform>().GetTranslation());
                        Animation* animation = &GetParent().ComponentGet<Animation>();
                        animation->SetIsLooping(false);
                        animation->TryPlay("Dizzy");
                    }
                    else
                    {
                        Animation* vfxAnim = &particles_.ComponentGet<Animation>();
                        Animation* animation = &GetParent().ComponentGet<Animation>();
                        Sprite* vfxSprite = &particles_.ComponentGet<Sprite>();
                        vfxAnim->SetIsLooping(false);
                        vfxSprite->SetPriority(5.0f);
                        vfxAnim->TryPlay("Active");
                        if (vfxAnim->GetIsDone() && animation->GetIsDone())
                        {
                            particles_.Destroy();
                            particlesDestroyed_ = true; 
                        }
                    }
                }
            }
        }
    }
}