/*!*****************************************************************************
\par filename: Antag.cpp
\par  authors: Jonathan Meyer
\par     date: 03/31/2023

\brief
    This file contains the behavior component and the state machine belonging to
    our Antagonist.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "AntagBehavior.h"
#include "PlayerController.h"
#include "Camera.h"
#include "CameraBehavior.h"
#include "Physics.h"
#include "Animation.h"
#include "Collider.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    void AntagBehavior::Read(const rapidjson::Value& val)
    {
        eventBool = false;
        if (val.HasMember("DistThresh"))
            distThresh = val["DistThresh"].GetFloat();

        if (val.HasMember("RunDir"))
            JsonConvert(val["RunDir"], runDir);

        if (val.HasMember("CameraTime"))
            durationCap_ = val["CameraTime"].GetFloat();

        if (val.HasMember("Ending"))
            ending_ = val["Ending"].GetBool();

        eventSound_ = engine_->GetSystem<Audio>()->GetSound("Creep");
        //rapidjson::Document docu = OpenJson("./Data/Audio/SFX/Creep.json");
        /*eventSound_.Load(docu, engine_->GetSystem<Audio>()); */
        eventSound_->SetInRange(true);
        eventSound_->SetIsLooping(false);
    }

    void AntagBehavior::Init()
    {
        //run in direction.
        Animation& anim = GetParent().ComponentGet<Animation>();
        anim.TryPlay("FrontIdle");
    }

    void AntagBehavior::Exit()
    {
        eventBool = false;
        cutsceneDur_ = 0.0f;
    }

    void AntagBehavior::Update(float dt)
    {
        Transform& transform = GetParent().ComponentGet<Transform>();
        glm::vec3 pos = *transform.GetTranslation();


        if (eventBool)
        {
            //get camera to follow antagonist for a view seconds
            cutsceneDur_ += dt / GetDilation();
            if (!eventSound_->GetIsPlaying())
                eventSound_->Play();

            // turn off collider

            Collider& collider = GetParent().ComponentGet<Collider>();
            collider.SetEnabled(false);

            if (cutsceneDur_ > 0.25f)
            {
                //run in direction.
                Physics& physics = GetParent().ComponentGet<Physics>();
                physics.SetVelocity(&runDir);

                //run in direction.
                Animation& anim = GetParent().ComponentGet<Animation>();
                anim.TryPlay("WalkRight");
            }


            //get camera position and size
            if (GetParent().GetParent()->HasManager<Camera>())
            {
                if (cutsceneDur_ <= durationCap_)
                {
                    CameraBehavior& controller = GetParent().GetParent()->ComponentGetFirst<CameraBehavior>();
                    controller.Target(pos);
                }
                else
                {
                    CameraBehavior& controller = GetParent().GetParent()->ComponentGetFirst<CameraBehavior>();
                    controller.Untarget();
                }
            }
            return;
        }

        float playerDist = 1e6, cameraDist = 1e6;

        //get player position
        if (GetParent().GetParent()->HasManager<PlayerController>())
        {
            PlayerController& controller = GetParent().GetParent()->ComponentGetFirst<PlayerController>();
            const Entity& player = controller.GetParent();

            Transform& playerTransform = player.ComponentGet<Transform>();
            glm::vec3 playerPos = *playerTransform.GetTranslation();
            playerDist = glm::distance(pos, playerPos);
        }

        //get camera position and size
        if (GetParent().GetParent()->HasManager<Camera>())
        {
            Camera& controller = GetParent().GetParent()->ComponentGetFirst<Camera>();
            const Entity& cam = controller.GetParent();

            Transform& camTransform = cam.ComponentGet<Transform>();
            glm::vec3 camPos = *camTransform.GetTranslation();
            cameraDist = glm::distance(pos, camPos);
        }

        //calculate distance from antagonist to camera and player
        float dist = std::min(playerDist, cameraDist); //posibly to dist check

        /* possible NEW IMPLEMENTATION: check camera distance by using camera space */
        /* to calculate it seperate with a different threshold from player distance */

        if (dist < distThresh)
        {
            eventBool = true;
        }
    }
};