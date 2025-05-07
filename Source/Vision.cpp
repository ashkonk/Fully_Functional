/*!*****************************************************************************
\par filename: Vision.cpp
\par  authors: Alijah Rosner
\par     date: 09/11/2022

\brief
  This is the implementation of occluder
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Graphics.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp> 
#include "Stream.h"
#include "Vision.h"

#include "PlayerController.h"
#include "IBehavior.h"
#include "Occluder.h"
#include <iostream>

namespace FFunctional
{
    void Vision::Read(const rapidjson::Value& value)
    {
      lastVisible_ = false;
      angle_ = 0.0f, fov_ = 90.0f;
      distance_ = 1.0f;

      if (value.HasMember("Angle"))
        SetAngle(value["Angle"].GetFloat());
      if (value.HasMember("FOV"))
        SetFOV(value["FOV"].GetFloat());
      if (value.HasMember("Distance"))
        SetViewDist(value["Distance"].GetFloat());
      if (value.HasMember("Offset"))
      {
        JsonConvert(value["Offset"], offset_);
      }
    }

    std::map<Component*, Vision&> visions;

    void Vision::Init()
    {

    }

    float Vision::minDistance_ = 1e10;

    float Vision::GetMinDistance()
    {
      return minDistance_;
    }

    void Vision::SetMinDistance(float newDist)
    {
      minDistance_ = std::min(GetMinDistance(), newDist);
    }

    void Vision::WipeMinDistance()
    {
      minDistance_ = 1e10;
    }

    void Vision::SetAngle(float theta)
    {
      angle_ = theta;
    }

    float Vision::GetAngle() const
    {
      if (GetParent().Has<Transform>())
      {
        Transform& visionPos = GetParent().ComponentGet<Transform>();
        return angle_ + visionPos.GetRotation();
      }
      return angle_;
    }

    void Vision::SetFOV(float fov)
    {
      fov_ = fov;
    }

    float Vision::GetFOV() const
    {
      return fov_;
    }

    void Vision::SetViewDist(float dist)
    {
      distance_ = dist;
    }

    float Vision::GetViewDist() const
    {
      return distance_;
    }

    Vision::~Vision()
    {
      Graphics::UnsetVision(int(distance(visions.begin(), visions.find(this))));

      if (visions.find(this) != visions.end())
        visions.erase(visions.find(this));
    }

    glm::vec2 Vision::GetPos() const
    {
      if (GetParent().IsDestroyed())
      {
        return glm::vec2(0, 0);
      }
      if (GetParent().GetParent()->Has<Transform>(GetParent().GetEntity()))
      {
        Transform& visionPos = GetParent().ComponentGet<Transform>();

        glm::vec3 lhsPos = *visionPos.GetTranslation();

        return glm::vec2(lhsPos.x, lhsPos.y);
      }
      return glm::vec2(0, 0);
    }

    glm::vec3 Vision::GetFrustrum() const
    {
      //radius, angle, FOV
      glm::vec3 vec;
      
      vec.x = GetViewDist();
      vec.y = GetAngle();
      vec.z = GetFOV();
      return vec;
    }

    //SetVisions(const std::map<Component*, Vision&>& nodes)

    void Vision::Update(float dt)
    {
      //Graphics::SetVision(0, GetPos(), GetFrustrum());
      visions.emplace(this, *this);
      Graphics::SetVisions(visions);

      dt;
      if (!GetParent().GetParent()->HasManager<PlayerController>())
        return;

      PlayerController& controller = GetParent().GetParent()->ComponentGetFirst<PlayerController>();
      const Entity &player = controller.GetParent();
      Transform& playerPos = player.ComponentGet<Transform>();
      Transform& visionPos = GetParent().ComponentGet<Transform>();

      glm::vec3 lhsPos = *visionPos.GetTranslation();
      glm::vec3 rhsPos = *playerPos.GetTranslation();

      lhsPos.z = 0, rhsPos.z = 0;
      
      float d = glm::distance(lhsPos, rhsPos);


      float dir = glm::radians(GetAngle());
      glm::vec3 viewDir = glm::vec3(cosf(dir), sinf(dir), 0);

      // make sure Z doesnt affect everything else
      
      float p = glm::dot(glm::normalize(rhsPos - lhsPos), viewDir);
      bool isVisible = p >= cosf(glm::radians(GetFOV()/2)) && d<GetViewDist();
      //bool isVisible = true;

      glm::vec3 photon = lhsPos;
      
      
      if (isVisible)
      {
        // more checks, occlusion stuff :)))
        ComponentManagerPtr<Occluder> occluders;
        occluders = GetParent().GetParent()->GetManager<Occluder>();

        if (occluders)
        {
          for (unsigned k = 0; k < 64; ++k)
          {
            float sdf = 1e10;

            for (unsigned j = 0; j < occluders->GetSize(); ++j)
              if ((*occluders)[j].GetDepth() >= controller.GetHeight())
                sdf = std::min(sdf, ((*occluders)[j]).SignedDistance(photon));
          
            photon += glm::normalize(rhsPos - lhsPos) * sdf;
          }
        }
      }

      isVisible &= glm::length(lhsPos - photon) >= d;
      SetMinDistance(d - glm::length(lhsPos - photon));

      if (isVisible && !lastVisible_)
      {
        /* if an entity is spotted. */
        std::vector<Signal> signals = GetParent().GetSignals("EntityFound");
        for (int i = 0; i < signals.size(); ++i)
        {
          Entity testEnt = Entity(signals.at(i).parentEcs, signals.at(i).parent);
          void* comp = testEnt.ComponentGetByName(signals.at(i).componentName);
          if (comp)
            ((IBehavior*)comp)->EntityFound(*this, player);
        }
      }
      else if (!isVisible && lastVisible_)
      {
        /* if an entity is spotted. */
        std::vector<Signal> signals = GetParent().GetSignals("EntityLost");
        for (int i = 0; i < signals.size(); ++i)
        {
          Entity testEnt = Entity(signals.at(i).parentEcs, signals.at(i).parent);
          void* comp = testEnt.ComponentGetByName(signals.at(i).componentName);
          if (comp)
            ((IBehavior*)comp)->EntityLost(*this, player);
        }
      }

      lastVisible_ = isVisible;
    }

}
