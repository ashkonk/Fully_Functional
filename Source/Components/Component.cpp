/*!*****************************************************************************
\par filename: Component.cpp
\par  authors: Alijah Rosner
\par     date: 09/11/2022

\brief
  This is the Component implementation
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Component.h"
#include "Entity.h"
#include "Transform.h"
#include "Scene.h"
#include "Camera.h"
#include <map>


namespace FFunctional
{
  void Component::Init()
  {

  }


  void Component::Update(float)
  {

  }


  void Component::Draw() const
  {

  }


  Entity Component::GetParent() const
  {
    return parent_;
  }


  Component::Component(Entity parent) : parent_(parent)
  {

  }


  void Component::SetParent(Entity parent)
  {
    parent_ = parent;
  }


  void Component::SetParent(ECS* ecs, EntityID id)
  {
    parent_ = Entity(ecs, id);
  }


  std::map<Component*, TimeNode> nodes;


  static float evaluate(const glm::vec3& translation)
  {
    float time = 1.0f;
    for (auto const& [c, t] : nodes)
    {
      float x = glm::distance(translation, t.pos);

      float y = (1 - std::pow(x / t.radius, t.curve));

      y = (y >= 0)? y : 0; // clamp y to positive values only.

      time = time*(1 - y) + t.rate*(y);
    }

    if (fabsf(time) < 0.1f)
      time = (time >= 0)? 0.1f : -0.1f;

    return time;
  }

  std::map<Component*, TimeNode>& Component::GetNodes()
  {
    return nodes;
  }


  void Component::SetTimeNode(Component* cid, glm::vec3 pos, float rate,
  float radius, float curve)
  {
    nodes[cid] = TimeNode(pos, rate, radius, curve);
  }

  void Component::UnsetTimeNode(Component* cid)
  {
    nodes.erase(cid);
  }

  Component::~Component()
  {
    Component::UnsetTimeNode(this);
  }


  void Component::SetGlobalTimeDialation(float f)
  {
    globalTime_ = f;
  }
  
  float Component::GetGlobalTimeDialation()
  {
    return globalTime_;
  }

  float Component::GetDilation(bool relativity) const
  {
    if (GetParent().Has<Transform>())
    {
      Transform& transform = GetParent().ComponentGet<Transform>();


      /* get a reference to the ECS container for our current scene.  * * * * */
      
      ECS* ecs = GetParent().GetParent();

      if (ecs && relativity)
      {
        ComponentManagerPtr<Camera> cameraMgr = ecs->GetManager<Camera>();
      
        if (cameraMgr->GetSize())
        {
          Camera targetCam = (*cameraMgr)[0];

          Transform& cam_transform = targetCam.GetParent().ComponentGet<Transform>();


          return evaluate(*transform.GetTranslation()) / evaluate(*cam_transform.GetTranslation());
        }
      }
      return globalTime_*(0.01f + evaluate(*transform.GetTranslation()));
    }
    return globalTime_;
  }

  float Component::globalTime_ = 1.0f;

  //if (ecs->HasManager<Interactable>())
  //{
  //  //ComponentManagerPtr<Camera> cameraMgr = ecs->GetManager<Camera>();
  //  ComponentManagerPtr<Interactable> interactMgr = ecs->GetManager<Interactable>();


  //  /* if (cameraMgr->GetSize())
  //   {
  //     Camera targetCam = (*cameraMgr)[0];

  //     Transform& cam_transform = targetCam.GetParent().ComponentGet<Transform>();*/
  //  if (interactMgr->GetSize())
  //  {
  //    Interactable interact = (*interactMgr)[0];
  //    Transform& interactTrans = interact.GetParent().ComponentGet<Transform>();
  //    // const glm::vec3 checkVec = glm::vec3(0.5f, 0.5f, 0.f);
  //    if (interact.GetActive() && glm::distance(*transform.GetTranslation(), *interactTrans.GetTranslation()) < 1.0f)
  //    {
  //      return evaluate(*transform.GetTranslation()) / evaluate(*interactTrans.GetTranslation());
  //    }


}
