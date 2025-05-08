/*!*****************************************************************************
\par filename: VisionCone.cpp
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the Vision Cone behavior, which the attendant has. 
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "QuickMaths.h"
#include "PlayerController.h"
#include "AttendantA.h"
#include "Scene.h"
#include "VisionCone.h"
#include "CCTV.h"

namespace FFunctional
{
    void VisionConeBehavior::Read(const rapidjson::Value&)
    {
    }

    bool VisionConeBehavior::Lose(float dt)
    {
        alertCounter_ += dt;

        if (alertCounter_ > 1.0f)
        {
            PlayerController& player = GetParent().GetParent()->ComponentGetFirst<PlayerController>(); 
            player.SetStatus(false);

            std::string fileName = "./Data/LoseScreenMain.json";
            Manager<Scene>* sceneManager = engine_->GetSystem<Manager<Scene>>();
            std::shared_ptr<Scene> newScene = LoadScene(*sceneManager, fileName.c_str());
            
            GetParent().GetParent()->Clear();
            GetParent().GetParent()->SetActive(false);

            return true; 
        }
        return false; 
    }

    void VisionConeBehavior::EnterCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        if (other.GetParent().Has<PlayerController>())
        {
            colliding_ = true; 
        }
    }

    void VisionConeBehavior::ExitCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        if (other.GetParent().Has<PlayerController>())
        {
            colliding_ = false;
            alertCounter_ = 0.0f; 
        }
    }

    void VisionConeBehavior::Update(float dt)
    {
        ECS* ecs = GetParent().GetParent();

        if (ecs->GetActive())
        {
            AttendantBehavior* attendant = &ecs->ComponentGetFirst<AttendantBehavior>();
            Entity attendantEntity = attendant->GetParent();
            Transform* attendantTransform = &attendantEntity.ComponentGet<Transform>();

            glm::vec3 offset = *attendantTransform->GetTranslation();
            glm::vec3 scale = *GetParent().ComponentGet<Transform>().GetScale(); 

            int route = attendant->GetRouteFromBehavior();

            if (colliding_ == true && attendant->GetDisabled() == false)
            {
                if (Lose(dt))
                {
                    return; 
                }
            }
            else
            {
                if (alertCounter_ > 0)
                {
                    alertCounter_ -= dt;
                }
                else
                {
                    alertCounter_ = 0;
                }
            }

            if (route == 0)
            {
                offset.y += 0.25f;
                GetParent().ComponentGet<Transform>().SetTranslation(&offset);
                GetParent().ComponentGet<Transform>().SetRotation(270);
            }
            else if (route == 1)
            {
                offset.x -= 0.25f;
                GetParent().ComponentGet<Transform>().SetTranslation(&offset);
                GetParent().ComponentGet<Transform>().SetRotation(180);

            }
            else if (route == 2)
            {
                offset.y -= 0.35f; 
                GetParent().ComponentGet<Transform>().SetTranslation(&offset);
                GetParent().ComponentGet<Transform>().SetRotation(90);

            }
            else if (route == 3)
            {
                offset.x += 0.25f; 
                GetParent().ComponentGet<Transform>().SetTranslation(&offset);
                GetParent().ComponentGet<Transform>().SetRotation(0);

            }

            if (attendant->GetDisabled())
            {
                GetParent().ComponentGet<Sprite>().SetOpacity(0.0f);
            }
        }


    }

    bool VisionConeBehavior::GetColliding()
    {
        return colliding_;
    }

}