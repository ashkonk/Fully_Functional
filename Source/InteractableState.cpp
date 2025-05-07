/*!*****************************************************************************
\par filename: stub.cpp
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "stub.h"
#include "InteractableState.h"
#include "Transform.h"
#include "ECS.h"
#include "Camera.h"
#include "PlayerController.h"

namespace FFunctional
{
  void InteractableState::OnInit()
  {
    switch (GetCurrState())
    {
    case ClickState::Ready:

      break;

    case ClickState::Sending:
    {
      parent_.ComponentGet<Transform>().SetTranslation(parent_.GetParent()
        ->ComponentGetFirst<PlayerController>().GetParent().ComponentGet<Transform>()
        .GetTranslation());
      SetDestination(ClickData().second);

      break;
    }

    case ClickState::Active:
    {
      Transform& itemTransform = parent_.GetParent()->ComponentGetFirst<PlayerController>().
        GetParent().ComponentGet<Transform>();
      itemTransform.SetTimeNode(&itemTransform, *itemTransform.GetTranslation(),
        radius_wheel[radiusIndex], time_wheel[timeIndex], curve_wheel[curveIndex]);
    }
      break;

    case ClickState::Retrieving:
      
      break;

    default:

      break;
      
    }
  }
  void InteractableState::OnUpdate(float dt)
  {
    switch (GetCurrState())
    {
    case ClickState::Ready:

      ClickUpdateReady();
      
      break;

    case ClickState::Sending:
      
      ClickUpdateSending();

      break;

    case ClickState::Active:

      break;

    case ClickState::Retrieving:

      SetDestination(*parent_.GetParent()->ComponentGetFirst<PlayerController>().
        GetParent().ComponentGet<Transform>().GetTranslation());
      ClickUpdateRetrieving();

      break;

    default:

      break;
    }
    dt;
  }

  void InteractableState::OnExit()
  {
    switch (GetCurrState())
    {

    case ClickState::Ready:
      break;

    case ClickState::Sending:
      break;

    case ClickState::Active:
      break;

    case ClickState::Retrieving:
      break;

    default:
      break;
    }
  }

  void InteractableState::Read(const rapidjson::Value& value)
  {
    if (value.HasMember("Offset"))
    {
      JsonConvert(value["Offset"], offset_);
    }

    if (value.HasMember("TimeRate"))
    {
      auto timeRate = value["TimeRate"].GetArray();

      time_wheel.clear();
      for (unsigned i = 0; i < timeRate.Size(); ++i)
        time_wheel.push_back(timeRate[i].GetFloat());
    }

    if (value.HasMember("Curve"))
    {
      auto curve = value["Curve"].GetArray();

      curve_wheel.clear();
      for (unsigned i = 0; i < curve.Size(); ++i)
        curve_wheel.push_back(curve[i].GetFloat());
    }

    if (value.HasMember("Radius"))
    {
      auto radius = value["Radius"].GetArray();

      radius_wheel.clear();
      for (unsigned i = 0; i < radius.Size(); ++i)
        radius_wheel.push_back(radius[i].GetFloat());
    }



    if (value.HasMember("ItemIndex"))
      armamentId = value["ItemIndex"].GetInt();
  }

  std::pair<bool, glm::vec3> InteractableState::ClickData()
  {

    std::pair<bool, glm::vec3> cursorPos;

    // save the cursor position

    cursorPos.second = glm::vec3(input_->GetCursorPos(), 1);
    // only calculate the position if you have to (when placing)
    glm::vec3 Scale = *parent_.ComponentGet<Transform>().GetScale();
    glm::vec3 Translation = *parent_.ComponentGet<Transform>().GetTranslation();

    // fetch the window width and height then scale
    int WindowWidth;
    int WindowHeight;
    glfwGetWindowSize(window_, &WindowWidth, &WindowHeight);


    //// get transform of camera
    Transform cTransform = parent_.GetParent()->ComponentGetFirst<Camera>().GetParent().ComponentGet<Transform>();


    // putting into camera space
    cursorPos.second.x = (cursorPos.second.x / WindowWidth) - 0.5f;
    cursorPos.second.y = -(cursorPos.second.y / WindowHeight) + 0.5f;

    //scaling by camera perspective and position
    const glm::vec3 cScale = *cTransform.GetScale();
    cursorPos.second *= cScale;
    cursorPos.second += *cTransform.GetTranslation();

    lastKeyState_ = currentKeyState_;
    currentKeyState_ = input_->GetButtonState(GLFW_MOUSE_BUTTON_LEFT);

    lastRightState_ = currRightState_;
    currRightState_ = input_->GetButtonState(GLFW_MOUSE_BUTTON_RIGHT);

    lastCycleState_ = currCycleState_;
    currCycleState_ = input_->GetKeyState(GLFW_KEY_T);


    if (lastCycleState_ == Keystate::Down && currCycleState_ == Keystate::Up)
    {
      ++radiusIndex;
      ++curveIndex;
      ++timeIndex;

      radiusIndex %= radius_wheel.size();
      curveIndex %= curve_wheel.size();
      timeIndex %= time_wheel.size();
    }

    cursorPos.first = (currentKeyState_ == Keystate::Down);

    return cursorPos;
  }

  void InteractableState::ClickUpdateReady()
  {
    Transform& camTransform = parent_.GetParent()->ComponentGetFirst<Camera>().GetParent().ComponentGet<Transform>();
    const glm::vec3 cameraPos = *camTransform.GetTranslation();

    Transform& itemTransform = parent_.ComponentGet<Transform>();
    //glm::vec3 itemTranslation = *itemTransform.GetTranslation();
    const glm::vec3 dest = (cameraPos + offset_);
    //itemTranslation = cameraPos + offset_;
    itemTransform.SetTranslation(&dest);
    if (ClickData().first)
    {
      const glm::vec3 playerPos = *parent_.GetParent()->ComponentGetFirst<PlayerController>().GetParent().ComponentGet<Transform>().GetTranslation();
      itemTransform.SetTranslation(&playerPos);
      SetDestination(ClickData().second);
      SetNextState(ClickState::Sending);
    }
  }

  void InteractableState::ClickUpdateSending()
  {
    Transform& itemTransform = parent_.ComponentGet<Transform>();
    Transform& playerTrans = parent_.GetParent()->ComponentGetFirst<PlayerController>().GetParent().ComponentGet<Transform>();
    const glm::vec2 playerPos = *playerTrans.GetTranslation();

    glm::vec3 itemTranslation = *itemTransform.GetTranslation();
    glm::vec2 item2d = itemTranslation;

    const glm::vec2 cursor2d = GetDestination();


    const float numerator = glm::distance(playerPos, item2d);
    const float denomenator = glm::distance(playerPos, cursor2d);

    const float ratio = glm::clamp((numerator / denomenator), 0.0f, 1.f);

    const glm::vec3 dirVec = (GetDestination() - itemTranslation);
    itemTranslation += glm::normalize(dirVec) * 0.03f;
    itemTransform.SetTranslation(&itemTranslation);

    //commenting out time stuff for now

    //itemTransform.SetTimeNode(&itemTransform, *itemTransform.GetTranslation(), radius_wheel[radiusIndex] * (ratio), time_wheel[timeIndex], curve_wheel[curveIndex]);
    //input_->GetParent()->GetTrace().Message("Radius: ", radius_wheel[radiusIndex], "\n", "Destination: ", cursorPos.x, ", ", cursorPos.y, "\n ",
      //"Position: ", itemTranslation.x, " , ", itemTranslation.y, "\n", "Ratio: ", ratio);

    //weve reached the destination boys
    if (ratio >= 0.98f)
    {
    //input_->GetParent()->GetTrace().Message("Radius: ", radius_wheel[radiusIndex], "\n", "Destination: ", cursorPos.x, ", ", cursorPos.y, "\n ",
    //"Position: ", itemTranslation.x, " , ", itemTranslation.y, "\n", "Ratio: ", ratio);

     /* itemTransform.UnsetTimeNode(&itemTransform);
      itemTransform.SetTimeNode(&itemTransform, *itemTransform.GetTranslation(), radius_wheel[radiusIndex], time_wheel[timeIndex], curve_wheel[curveIndex]);*/
      SetNextState(ClickState::Active);
    }
    //if click again
    if (ClickData().first)
    {
      SetNextState(ClickState::Retrieving);
    }
  }
  
  void InteractableState::ClickUpdateRetrieving()
  {
    //Transform& playerTrans = parent_.GetParent()->ComponentGetFirst<PlayerController>().
    //  GetParent().ComponentGet<Transform>();
    //const glm::vec3 playerPos = *playerTrans.GetTranslation();

    Transform& itemTransform = parent_.ComponentGet<Transform>();
    const glm::vec3 playerPos = GetDestination();

    glm::vec3 itemTranslation = *itemTransform.GetTranslation();
    glm::vec3 dirVec = playerPos - itemTranslation;

    itemTranslation += glm::normalize(dirVec) * 0.05f;
    itemTransform.SetTranslation(&itemTranslation);

    float dist = glm::distance(playerPos, *itemTransform.GetTranslation()) / glm::distance(playerPos, GetDestination());
    dist = (1.f < dist) ? 1.0f : dist;
    if (dist < 0.1f)
    {
      SetNextState(ClickState::Ready);
    }
  }

  void InteractableState::SetDestination(const glm::vec3 pos)
  {
    destination_ = pos;
  }

  void InteractableState::ClickUpdateActive()
  {
    if (ClickData().first == true)
    {
      SetNextState(ClickState::Retrieving);
    }
  }

  const glm::vec3 InteractableState::GetDestination()
  {
    return destination_;
  }

}