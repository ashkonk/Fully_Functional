/*!*****************************************************************************
\par filename: Antag.h
\par  authors: Jonathan Meyer
\par     date: 03/31/2023

\brief
    This file contains the behavior component and the state machine belonging to
    our Antagonist.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Component.h"
#include "Behavior.h"
#include "Sound.h"
#include <memory>
/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      Antagonist Behavior class.
  ***************************************************************************/
  class AntagBehavior : public Behavior<AntagBehavior>
  {
  private:
    Engine* engine_;
    bool ending_ = false; 
    bool eventBool = false;
    float distThresh = 1.0f;
    glm::vec3 runDir = glm::vec3(1, 0, 0);
    float cutsceneDur_ = 0.0f;
    float durationCap_ = 1.0f;
    std::shared_ptr<Sound> eventSound_{ nullptr };
    std::shared_ptr<Sound> walkSound_{ nullptr };

  public:
    AntagBehavior(Engine* engine, Entity parent) : Behavior(parent),
    engine_(engine) {}

    void Read(const rapidjson::Value&);
    void Update(float dt);

    void Init();

    void Exit();

    static std::string_view Name() { return "AntagBehavior"; }
  };
};