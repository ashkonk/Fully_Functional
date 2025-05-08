/*!*****************************************************************************
\par filename: RoomCover.h
\par  authors: Ashkon Khalkhali, Alijah Rosner
\par     date: 09/09/2022

\brief
  The header file for our room cover behavior.
  Copyright © 2022 DigiPen (USA) Corporation.
*****************************************************************************!*/
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
      This is the room cover behavior class
  ***************************************************************************/
  class RoomCover : public Behavior<RoomCover>
  {
  private:
    Engine* engine_{ nullptr };
    bool hidden_{ true };
    glm::vec3 roomPos_{ };

  public:
    RoomCover(Engine* engine, Entity parent) 
      : Behavior(parent), engine_(engine), roomPos_{} {}

    void Read(const rapidjson::Value& value);

    void Update(float dt); //!< Our ExampleComponent Update Function.

    static std::string_view Name() { return "CoverBehavior"; }


    void EnterCollision2D(const Collider& self, const Collider& other);

    void ExitCollision2D(const Collider& self, const Collider& other);

  };

}; /* Normalloc */