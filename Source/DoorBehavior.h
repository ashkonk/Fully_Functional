/*!*****************************************************************************
\par filename: DoorBehavior.h
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  This is the header file for the door behavior. 
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <string>
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "StateMachine.h"
#include "Behavior.h"
#include "Scene.h"
#include "Engine.h"
#include "Entity.h"
#include "SceneLoader.h"
#include "Transform.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      This is the door behavior class.
  ***************************************************************************/
  class DoorBehavior : public Behavior<DoorBehavior>
  {
  private:
    Engine* engine_{ nullptr };
    bool open_{ false };
    //Data of scene to load
    std::string sceneDir_;
    //Location (in world space) of next scene entrance
    glm::vec3 entryPos_;

    int debugTasks = 0;

  public:
    DoorBehavior(Engine* engine, Entity parent) : Behavior(parent), engine_(engine), sceneDir_{ }, entryPos_{ } {}

    void Read(const rapidjson::Value& value);

    void Update(float dt); //!< Our ExampleComponent Update Function.

    static std::string_view Name() { return "DoorBehavior"; }

    void Open(Entity doorTrigger);

    void EnterCollision2D(const Collider& self, const Collider& other);

    int GetDebugTasks() const;

  }; /* ExampleComponent */

}; /* Normalloc */