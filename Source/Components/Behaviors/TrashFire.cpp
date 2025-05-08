/*!*****************************************************************************
\par filename: TrashFire.cpp
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the behavior component for the trash fire, which
    disables the guard.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "QuickMaths.h"
#include "TrashFire.h"
#include "PlayerController.h"
#include "Animation.h"
#include <unordered_map>

namespace FFunctional
{
    void TrashFireBehavior::Read(const rapidjson::Value& value)
    {
        used_ = value["used"].GetBool();
    }

    void TrashFireBehavior::Update(float dt)
    {
        dt;
        if (colliding_)
        {
            TurnOff();
        }
    }

    void TrashFireBehavior::TurnOff()
    {

        Input* input = engine_->GetSystem<Input>();
        if (used_ == false)
        {
            if (input->GetKeyState(GLFW_KEY_G) == Keystate::Down || input->GetKeyState(GLFW_KEY_G) == Keystate::Released)
            {
                ECS* ecs = GetParent().GetParent();
                PlayerController* playerController = &ecs->ComponentGetFirst<PlayerController>();
                std::unordered_map<std::string, Entity&> inventory = *playerController->GetInventory();

                if (inventory.contains("Lighter"))
                {
                    Entity vfx(ecs, ecs->EntityCreate());
                    rapidjson::Document docu = OpenJson("./Data/SmokeVFX.json");
                    vfx.Read(docu, engine_);

                    Transform* vfxTrans = &vfx.ComponentGet<Transform>();
                    //glm::vec3 vfxTranslate = *vfxTrans->GetTranslation();

                    Transform* trashTransform = &GetParent().ComponentGet<Transform>();
                    glm::vec3 trashTranslate = *trashTransform->GetTranslation();

                    trashTranslate.y += 0.2f;
                    Animation vfxAnim = vfx.ComponentGet<Animation>();
                    //vfxAnim.SetIsLooping(true);
                    vfxAnim.TryPlay("Active");
                    //vfxTrans->SetTranslation(GetParent().ComponentGet<Transform>().GetTranslation());
                    vfxTrans->SetTranslation(&trashTranslate);
                    used_ = true;
                }
            }
        }
    }

    bool TrashFireBehavior::GetTrashFireUsed() const
    {
        return used_;
    }


    void TrashFireBehavior::SetTrashFireUsed(bool flag)
    {
        used_ = flag;

    }

    void TrashFireBehavior::ExitCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        ECS* ecs = parent_.GetParent();
        if (ecs->GetActive() && other.GetParent().Has<PlayerController>())
        {
            colliding_ = false;
            //PlayerController* player = &collider.GetParent().ComponentGet<PlayerController>();
        }
    }

    void TrashFireBehavior::EnterCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        ECS* ecs = parent_.GetParent();

        if (ecs->GetActive() && other.GetParent().Has<PlayerController>())
        {
            colliding_ = true;
            //PlayerController* player = &collider.GetParent().ComponentGet<PlayerController>();
        }
    }
}
