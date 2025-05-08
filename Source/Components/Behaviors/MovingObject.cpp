/*!*****************************************************************************
\par filename: MovingObject.cpp
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the behavior component and the state machine belonging to
    Attendant A.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "MovingObject.h"
#include "Physics.h"
#include "Transform.h"
#include "QuickMaths.h"
#include "Occluder.h"
#include "Sprite.h"
#include "Animation.h"
#include <glm/vec3.hpp>
#include <cmath>

namespace FFunctional
{

    void MovingObject::UpdatePosition(float dt)
    {

        if (!parent_.IsDestroyed())
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
                    trans.SetTranslation(&positions_.at(route_));
                }
            }
            if (parent_.Has<Animation>())
                parent_.ComponentGet<Animation>().Play("2");
            //occluder.SetOffset(*trans.GetTranslation());
        }
    }

    void MovingObject::UpdateDissolve()
    {
        if (dissolve_)
        {
            Sprite& spriteRef = parent_.ComponentGet<Sprite>();
            if (route_ == positions_.size() - 1 && glm::distance(*parent_.ComponentGet<Transform>().GetTranslation(),
                positions_.at(route_)) <= 0.1f)
            {
                if (glm::distance(*parent_.ComponentGet<Transform>().GetTranslation(),
                    positions_.at(route_)) >= 0.001f)
                {
                    //spriteRef.Dissolve();
                    float currOpacity = spriteRef.GetOpacity();
                    currOpacity -= 0.05f;
                    spriteRef.SetOpacity(currOpacity);
                }
            }
            else
            {
                if (spriteRef.GetOpacity() < 1.0f)
                {
                    float currOpacity = spriteRef.GetOpacity();
                    currOpacity += 0.025f;
                    spriteRef.SetOpacity(currOpacity);
                }
            }
        }
    }

    void MovingObject::Read(const rapidjson::Value& value)
    {
        if (value.HasMember("Dissolve"))
        {
            dissolve_ = value["Dissolve"].GetBool();
        }
        speed_ = value["MoveSpeed"].GetFloat();

        for (const auto& arrayValue : value["WalkingPoints"].GetArray())
        {
            glm::vec3 position;
            JsonConvert(arrayValue, position);
            positions_.push_back(position);
        }
    }

    void MovingObject::Update(float dt)
    {
        if (parent_.GetParent()->GetActive())
        {
            UpdatePosition(dt);
            UpdateDissolve();
        }
    }

    void MovingObject::EnterCollision2D(const Collider& self, const Collider& other)
    {
        self;
        other;
        parent_.ComponentGet<Sprite>().SetOpacity(0.0f);
    }

    void MovingObject::ExitCollision2D(const Collider& self, const Collider& other)
    {
        self;
        other;
        parent_.ComponentGet<Sprite>().SetOpacity(1.0f);
    }
}