/*!*****************************************************************************
\par filename: Interactable.cpp
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Interactable file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Interactable.h"
#include "Scene.h"
#include "Camera.h"
#include "Graphics.h"
#include "ComponentManager.h"


namespace FFunctional
{
  std::pair<bool, glm::vec3> Interactable::CursorInside()
  {
    std::pair<bool, glm::vec3> cursorPos;
    // save the cursor position along with scale and translation
    cursorPos.second = glm::vec3(input_->GetCursorPos(), 1);

    glm::vec3 Scale = *GetParent().ComponentGet<Transform>().GetScale();
    glm::vec3 Translation = *GetParent().ComponentGet<Transform>().GetTranslation();

    // fetch the window width and height then scale
    int WindowWidth;
    int WindowHeight;
    glfwGetWindowSize(window_, &WindowWidth, &WindowHeight);


    //// get transform of camera
    Transform cTransform = GetParent().GetParent()->ComponentGetFirst<Camera>().GetParent().ComponentGet<Transform>();


    // putting into camera space
    cursorPos.second.x = (cursorPos.second.x / WindowWidth) - 0.5f;
    cursorPos.second.y = -(cursorPos.second.y / WindowHeight) + 0.5f;

    //scaling by camera perspective and position
    const glm::vec3 cScale = *cTransform.GetScale();
    cursorPos.second *= cScale;
    cursorPos.second += *cTransform.GetTranslation();

    //obtaining edges of collision
    float leftSide = Translation.x - Scale.x / 2.0f;
    float rightSide = Translation.x + Scale.x / 2.0f;

    float botSide = Translation.y - Scale.x / 2.0f;
    float topSide = Translation.y + Scale.y / 2.0f;

    // if the cursor is within the interactable return true
    if ((cursorPos.second.x > leftSide && cursorPos.second.x < rightSide)
      && (cursorPos.second.y > botSide && cursorPos.second.y < topSide))
    {
      cursorPos.first = true;
      return cursorPos;
    }
    cursorPos.first = false;
    return cursorPos;
  }
  bool Interactable::GetPickup()
  {
    return pickup_;
  }

  bool Interactable::GetActive()
  {
    return active_;
  }

  void Interactable::SetDown()
  {
    Transform& itemTransform = GetParent().ComponentGet<Transform>();

    ECS* ecs = GetParent().GetParent();
    glm::vec3 itemTranslation = *itemTransform.GetTranslation();
    if (ecs->HasManager<Transform>())
    {
      ComponentManagerPtr<Transform> transformManager = ecs->GetManager<Transform>();
      nearest = nullptr;
      for (const Transform& transform : transformManager->GetComponentList())
      {
        if (transform.GetParent().GetEntity() == itemTransform.GetParent().GetEntity())
        {
          continue;
        }
        if (!nearest)
        {
          nearest = &transform;
        }
        else
        {
          if (glm::distance(itemTranslation, *transform.GetTranslation())
            < glm::distance(itemTranslation, *nearest->GetTranslation()))
          {
            nearest = &transform;
          }
        }
      }
      if (nearest)
      {
        itemTranslation = *nearest->GetTranslation();
        itemTransform.SetTranslation(&itemTranslation);
      }
    }
    active_ = true;
    pickup_ = false;
  }

  void Interactable::Update(float dt)
  {
    std::pair cursorData_ = CursorInside();
    Transform& itemTransform = GetParent().ComponentGet<Transform>();

    //store transform or transform reference somewhere that the evaluate() function can access.
    
    if (active_)
    {
      Component::SetTimeNode(this, *itemTransform.GetTranslation(), 0.5f, 0.2f, 3.0f);
      //      component source-^  position-^                  radius-^ rate-^     ^-curve
    }
    else
    {
      Component::UnsetTimeNode(this);
      //      component source-^
    }
    

    if (!active_ && !pickup_)
    {
      glm::vec3 itemTranslation = *itemTransform.GetTranslation();
      Transform& cTransform = GetParent().GetParent()->ComponentGetFirst<Camera>().GetParent().ComponentGet<Transform>();
      const glm::vec3 cTranslation = *cTransform.GetTranslation();
      itemTranslation = cTranslation + offset_;
      itemTransform.SetTranslation(&itemTranslation);
    }
    if (active_ == false)
    {
      if (cursorData_.first && input_->GetButtonState(GLFW_MOUSE_BUTTON_LEFT) == Keystate::Down)
      {
        pickup_ = true;
      }
      else if (pickup_ && (input_->GetButtonState(GLFW_MOUSE_BUTTON_LEFT) == Keystate::Up))
      {
        SetDown();
      }
      if (pickup_)
      {
        //Camera& camera = GetParent().GetParent()->ComponentGetFirst<Camera>();
        //Transform& cTransform = camera.GetParent().ComponentGet<Transform>();
        glm::vec3 itemTranslation = *itemTransform.GetTranslation();
        itemTransform.SetTranslation(&cursorData_.second);
      }
    }
    dt;
  }

  void Interactable::Read(const rapidjson::Value& value)
  {
    if (value.HasMember("Static"))
    {
      static_ = value["Static"].GetBool();
    }
    if (value.HasMember("Offset"))
    {
      JsonConvert(value["Offset"], offset_);
    }
  }

  std::string_view Interactable::Name()
  {
    return "Interactable";
  }
}