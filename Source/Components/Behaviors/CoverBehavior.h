/*!*****************************************************************************
\par filename: stub.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
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
      This is an example of a component class.
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


    void EnterCollision2D(const Collider& collider);

    void ExitCollision2D(const Collider& collider);

  }; /* ExampleComponent */

}; /* Normalloc */