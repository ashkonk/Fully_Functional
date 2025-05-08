/*!*****************************************************************************
\par filename: BrokenDoorBehavior.c
\par  authors: Jonathan Meyer
\par     date: 03/31/2023

\brief
  This is the header file for the jammed door behavior.
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "BrokenDoorBehavior.h"
#include "PlayerController.h"
#include "glm/glm.hpp"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    void BrokenDoorBehavior::Read(const rapidjson::Value& value)
    {
        if (value.HasMember("DtThresh"))
            dt_thresh = value["DtThresh"].GetFloat();
    }

    void BrokenDoorBehavior::Update(float dt)
    {
        dt;
        //gGetParent().ComponentGet<Collider>().SetSolid(!colliding_);

        /*if (GetParent().GetParent()->ComponentGetFirst<PlayerController>().GetMoveState() != 3 && colliding_ == true)
        {
            colliding_ = false; 
        }*/

        timer_ += dt; 

        if (colliding_)
        {
            timer_ += dt; 
            PlayerController& controller = GetParent().GetParent()->ComponentGetFirst<PlayerController>();

            if (controller.GetMoveState() != 3)
            {
                Transform& controllerTransform = controller.GetParent().ComponentGet<Transform>();

                Transform& transform = GetParent().ComponentGet<Transform>();

                glm::vec3 controlTrans = *controllerTransform.GetTranslation();
                glm::vec3 trans = *transform.GetTranslation();


                if ((glm::distance(trans, controlTrans)) < 0.25)
                {
                    colliding_ = true;
                }
                else
                {
                    colliding_ = false; 
                }
                timer_ = 0.0f;
            }
        }


        Collider& coll = GetParent().ComponentGet<Collider>();

        isLocked = true;

        if (colliding_)
        {
            isLocked = false; 
        }
        coll.SetEnabled(isLocked);


    }

    void BrokenDoorBehavior::ExitCollision2D(const Collider& self, const Collider& other)
    {
        self;
        other;

           // found_ = false;
          // colliding_ = false;
           // GetParent().ComponentGet<Collider>().SetSolid(true);
        
    }

    void BrokenDoorBehavior::EnterCollision2D(const Collider& self, const Collider& other)
    {
        self;
        if (other.GetParent().ComponentGet<Collider>().GetTag() == "Teleporter")
        {
            teleportID_ = other.GetParent().GetEntity();
            colliding_ = true;
            // GetParent().ComponentGet<Collider>().SetSolid(false);
             //*teleporter_ = other.GetParent();
             //found_ = true; 
        }

    }

}; /* Normalloc */