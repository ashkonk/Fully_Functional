/*!*****************************************************************************
\par filename: CCTV.cpp
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the implementation for both the Fusebox behavior, and 
    the CCTV behavior.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Physics.h"
#include "Transform.h"
#include "ParticleEmitter.h"
#include "CCTV.h"
#include "Scene.h"
//#include <glm/gtx/transform.hpp>
#include "CCTV.h"
#include "Entity.h"
#include "Manager.h"
#include "StealthBar.h"
#include "SceneLoader.h"
#include <memory>
#include <string>

namespace FFunctional
{
    /*void FuseBoxBehavior::Read(const rapidjson::Value& value)
    {
        active_ = value["active"].GetBool();
    }


    void FuseBoxBehavior::Update(float)
    {
        if (colliding_)
        {
            TurnOff();
        }
    }

    bool FuseBoxBehavior::GetActive() const
    {
        return active_;
    }

    void FuseBoxBehavior::TurnOff()
    {

        Input* input = engine_->GetSystem<Input>();
        if (active_ == true)
        {
            if (input->GetKeyState(GLFW_KEY_G) == Keystate::Down || input->GetKeyState(GLFW_KEY_G) == Keystate::Released)
            {
                ECS* ecs = parent_.GetParent();
                PlayerController playerBehavior = ecs->ComponentGetFirst<PlayerController>();
                Entity player = playerBehavior.GetParent();
                Physics* playerPhys = &player.ComponentGet<Physics>();
                glm::vec3 vec = { 0, 0, 0 };
                playerPhys->SetAcceleration(&vec);
                std::unordered_map<std::string, Entity&> inventory = *playerBehavior.GetInventory();
                if (inventory.contains("FuseboxKey"))
                {
                    active_ = false;
                    CCTVBehavior& cctv = ecs->ComponentGetFirst<CCTVBehavior>();
                    cctv.SetActive(false);
                    GetParent().ComponentGet<ParticleEmitter>().SetEmit(1);
                }
            }
        }
    }

    void FuseBoxBehavior::ExitCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        ECS* ecs = parent_.GetParent();
        if (ecs->Has<PlayerController>(other.GetParent().GetEntity()))
        {
            colliding_ = false;
        }
    }

    void FuseBoxBehavior::EnterCollision2D(const Collider& self,
    const Collider& other)
    {
      self;
        ECS* ecs = parent_.GetParent();
        if (ecs->Has<PlayerController>(other.GetParent().GetEntity()))
        {
            colliding_ = true;
        }
    }*/


    void CCTVBehavior::Read(const rapidjson::Value& value)
    {
        active_ = value["Active"].GetBool();
        rotateSpeed_ = value["RotateSpeed"].GetFloat();
        maxRotation_ = value["MaxRotation"].GetFloat();
        rotateDirection_ = value["RotateDirection"].GetFloat();
        detectTime_ = value["DetectTime"].GetFloat();
    }

    // either have vision cone as a 2nd sprite to CCTV or
    // add the vfx to the sprite itself?

    void CCTVBehavior::Init()
    {
        Behavior<CCTVBehavior>::Init();
        rapidjson::Document docu = OpenJson("./Data/VisionConeVFX.json");
        originalRotation_ = GetParent().ComponentGet<Transform>().GetRotation();

        //Transform& coneTrans = visionCone_.ComponentGet<Transform>();
        //coneTrans.SetRotation(originalRotation_);
        //coneTrans.SetTranslation(GetParent().ComponentGet<Transform>().GetTranslation());
        //coneTrans.SetScale(GetParent().ComponentGet<Transform>().GetScale());

        timer_ = 0.0f;
    }

    bool CCTVBehavior::GetActive() const
    {
        return active_;
    }

    void CCTVBehavior::EntityFound(const Vision& self, const Entity& other)
    {
        self;
        if (other.Has<PlayerController>())
        {
            colliding_ = true;
        }
    }

    void CCTVBehavior::EntityLost(const Vision& self, const Entity& other)
    {
        self;
        if (other.Has<PlayerController>())
        {
            colliding_ = false;
        }
    }

    void CCTVBehavior::Update(float dt)
    {

        if (colliding_)
        {
            Transform& cctvTransform = parent_.ComponentGet<Transform>();
            glm::vec3 cctvTranslation = *cctvTransform.GetTranslation();
            glm::vec3 pos = *parent_.GetParent()->ComponentGetFirst<PlayerController>().GetParent().ComponentGet<Transform>().GetTranslation();

           // timer_ += dt * (glm::distance(pos, cctvTranslation) < detectTime_);
            timer_ += dt/detectTime_;
            //std::cout << timer_ << std::endl;
            StealthBar::SetFill(timer_ );
        }
        else
        {
            timer_ = 0.0f; 
        }

        Transform& cctvTransform = parent_.ComponentGet<Transform>();
        rotationSum_ += rotateSpeed_ * dt * rotateDirection_;


        if (rotationSum_ >= maxRotation_)
        {
            rotationSum_ = maxRotation_;
            rotateDirection_ = -1;
        }

        if (rotationSum_ <= 0.0f)
        {
            rotationSum_ = 0;
            rotateDirection_ = 1;
        }
        cctvTransform.SetRotation(((rotationSum_ + originalRotation_)));
    }

    void CCTVBehavior::SetActive(bool active)
    {
        active_ = active;
    }

    bool CCTVBehavior::GetColliding()
    {
        return colliding_;
    }

}