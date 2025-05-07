/*!*****************************************************************************
\par filename: Armament.cpp
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Interactable file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Armament.h"
#include "Scene.h"
#include "Camera.h"
#include "Graphics.h"
#include "ComponentManager.h"
#include "PlayerController.h"


namespace FFunctional
{
  //static float Lerp(float lerpee);

  std::pair<bool, glm::vec3> Armament::CursorClick()
  {
    std::pair<bool, glm::vec3> cursorPos;

    // save the cursor position

    cursorPos.second = glm::vec3(input_->GetCursorPos(), 1);
    // only calculate the position if you have to (when placing)
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

    lastKeyState_ = currentKeyState_;
    currentKeyState_ = input_->GetButtonState(GLFW_MOUSE_BUTTON_LEFT);

    lastRightState_ = currRightState_;
    currRightState_ = input_->GetButtonState(GLFW_MOUSE_BUTTON_RIGHT);

    //lastCycleState_ = currCycleState_;
    //currCycleState_ = input_->GetKeyState(GLFW_KEY_T);


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
  bool Armament::GetState()
  {
    return ready_;
  }

  bool Armament::GetActive()
  {
    return active_;
  }

  void Armament::SetDown()
  {

  }

  void Armament::Update(float dt)
  {
    //still working on him
    //itemState_->Update(dt);
    //ecs to access player
    //Sprite& sprite = GetParent().ComponentGet<Sprite>();
    ECS* ecs = GetParent().GetParent();

    //item transform
    Transform& itemTransform = GetParent().ComponentGet<Transform>();

    //cursor click and position
    std::pair cursorData_ = CursorClick();
    bool triggered = cursorData_.first;
    const glm::vec3 cursorPos = cursorData_.second;

    //disgusting fix for placing on level start
    if (triggered && !ready_)
    {
      if (!active_)
      {
        ready_ = true;
        return;
      }
    }

    //update item in inventory
    if (!active_ && !triggered)
    {
      Transform& cTransform = GetParent().GetParent()->ComponentGetFirst<Camera>().GetParent().ComponentGet<Transform>();
      sending_ = false;
      const glm::vec3 cTranslation = *cTransform.GetTranslation();
      UpdateIdle(cTranslation);
    }

    //throw the item
    if (triggered && ready_ && armamentCount_ == armamentId)
    {
      //start at player position
      if (!active_ && !sending_)
      {
        playerPos_ = *ecs->ComponentGetFirst<PlayerController>().GetParent().ComponentGet<Transform>().GetTranslation();
        SetPlayerPos(playerPos_);
        itemTransform.SetTranslation(&playerPos_);
        SetDestination(cursorPos);
        sending_ = true;
      }
    }

    if (sending_)
    {

      if (currentKeyState_ == Keystate::Up && lastKeyState_ == Keystate::Down)
      {
        armamentCount_ = armamentId + 1;
      }
      active_ = true;
      ready_ = false;
      Fire(GetDestination(), GetPlayerPos());
    }
    //update item when active
    if (active_ && !retrieving_)
    {
      if (lastRightState_ == Keystate::Down && currRightState_ == Keystate::Up)
      {
        retrieving_ = true;
      }
    }
    if (retrieving_)
    {
      Recall(*ecs->ComponentGetFirst<PlayerController>().GetParent().ComponentGet<Transform>().GetTranslation());
    }

    dt;
  }

  void Armament::UpdateIdle(const glm::vec3 cameraPos)
  {
    Transform& itemTransform = GetParent().ComponentGet<Transform>();
    glm::vec3 itemTranslation = *itemTransform.GetTranslation();

    itemTranslation = cameraPos + offset_;
    itemTransform.SetTranslation(&itemTranslation);
  }

  void Armament::Fire(const glm::vec3 cursorPos, const glm::vec3 playerPos)
  {

    Transform& itemTransform = GetParent().ComponentGet<Transform>();

    glm::vec3 itemTranslation = *itemTransform.GetTranslation();
    glm::vec2 item2d = itemTranslation;
    const glm::vec2 player2d = playerPos;
    const glm::vec2 cursor2d = cursorPos;
    const glm::vec3 dirVec = (GetDestination() - itemTranslation);

    const float numerator = glm::distance(player2d, item2d);
      
    const float denomenator = glm::distance(player2d, cursor2d);

    const float ratio = glm::clamp((numerator / denomenator), 0.0f, 1.f);

    itemTranslation += glm::normalize(dirVec) * 0.03f;
    itemTransform.SetTranslation(&itemTranslation);

    Component::SetTimeNode(this, *itemTransform.GetTranslation(), radius_wheel[radiusIndex] * (ratio), time_wheel[timeIndex], curve_wheel[curveIndex]);
    engine_->GetTrace().Message("Radius: ", radius_wheel[radiusIndex], "\n", "Destination: ", cursor2d.x, ", ", cursor2d.y, "\n ",
      "Position: ", item2d.x, " , ", item2d.y, "\n", "Ratio: ", ratio);
    //weve reached the destination boys
    if (ratio >= 0.99f)
    {
      //if (glm::distance(itemTranslation, GetDestination()) < 1.0f)
      engine_->GetTrace().Message("Radius: ", radius_wheel[radiusIndex], "\n", "Destination: ", cursorPos.x, ", ", cursorPos.y, "\n ",
        "Position: ", itemTranslation.x, " , ", itemTranslation.y, "\n", "Ratio: ", ratio);
      Component::UnsetTimeNode(this);
      Component::SetTimeNode(this, *itemTransform.GetTranslation(), radius_wheel[radiusIndex], time_wheel[timeIndex], curve_wheel[curveIndex]);
      sending_ = false;
    }
  }

  void Armament::Recall(const glm::vec3 playerPos)
  {
    Transform& itemTransform = GetParent().ComponentGet<Transform>();

    glm::vec3 itemTranslation = *itemTransform.GetTranslation();
    glm::vec3 dirVec = playerPos - itemTranslation;

    itemTranslation += glm::normalize(dirVec) * 0.05f;
    itemTransform.SetTranslation(&itemTranslation);

    float dist = glm::distance(playerPos, *itemTransform.GetTranslation()) / glm::distance(playerPos, GetDestination());
    dist = (1.f < dist) ? 1.0f : dist;

    Component::SetTimeNode(this, *itemTransform.GetTranslation(), radius_wheel[radiusIndex] * (dist), time_wheel[timeIndex], curve_wheel[curveIndex]);
    //if (glm::distance(itemTranslation, GetDestination()) < 1.0f)

    if (dist < 0.1f)
    {
      Component::UnsetTimeNode(this);
      retrieving_ = false;
      active_ = false;
      ready_ = true;
      armamentCount_ = (armamentCount_ > armamentId) ? armamentId : armamentCount_;
    }
  }

  void Armament::SetDestination(glm::vec3 destPos)
  {
    destination_ = destPos;
  }

  const glm::vec3 Armament::GetDestination()
  {
    return destination_;
  }

  void Armament::Read(const rapidjson::Value& value)
  {
    //radius_ = 0.5f;
    //timeRate_ = 0.5f;
    //curve_ = 3.0f;

    time_wheel.clear();
    time_wheel.push_back(0.5f);
    radius_wheel.clear();
    radius_wheel.push_back(0.5f);
    curve_wheel.clear();
    curve_wheel.push_back(3.0f);


    armamentId = -1;
    //if (value.HasMember("Type"))
    //{
    //  type_ = value["Type"].GetInt();
    //  switch (type_)
    //  {
    //  case 1:
    //    state_.SetType(ItemType::Click);
    //    break;

    //  case 2:
    //    state_.SetType(ItemType::Drag);
    //    break;

    //  default:
    //    state_.SetType(ItemType::Invalid);
    //    break;
    //  }
    //}
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

  void Armament::SetPlayerPos(glm::vec3 playerPos)
  {
    playerPos_ = playerPos;
  }

  const glm::vec3 Armament::GetPlayerPos()
  {
    return playerPos_;
  }

  std::string_view Armament::Name()
  {
    return "Armament";
  }

  void Armament::ResetArmament()
  {
    armamentCount_ = 0;
  }

  int Armament::armamentCount_ = 0;

  //void InteractableState::OnInit()
  //{
  //  switch (GetCurrState())
  //  {
  //  case ItemState::Ready:
  //  case ItemState::Active:
  //  case ItemState::Sending:
  //  case ItemState::Retrieving:
  //  }
  //}

  //void InteractableState::OnUpdate(float dt)
  //{
  //  switch (GetCurrState())
  //  {
  //  case ItemState::Ready:

  //  case ItemState::Active:

  //  case ItemState::Sending:

  //  case ItemState::Retrieving:
  //  }

  //}

  //void InteractableState::OnExit()
  //{

  //}

  //void InteractableState::Read(const rapidjson::Value& value)
  //{

  //}

  /*static float Lerp(float lerpee)
  {
    if (lerpee < 0.5f)
    {
      return 2.f * lerpee * lerpee;
    }
    else
    {
      return (2.f * (lerpee - 0.5f) * (1.f - (lerpee - 0.5f)) + 0.5f);
    }
  }*/
}