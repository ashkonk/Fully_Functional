/*!*****************************************************************************
\par filename: stub.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "StateMachine.h"
#include "Component.h"
#include "Scene.h"
#include "Engine.h"
#include "Entity.h"
#include "SceneLoader.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"
#include <string>

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

  static void DoorBehaviorCollisionHandler(Entity door, Entity player);

  class DoorBehavior : public Component
  {
  private:
    Engine* engine_{ nullptr };
    std::string sceneDir_;
    bool open_{ false };

  public:
    DoorBehavior(Engine* engine, Entity parent) : Component(parent), engine_(engine), sceneDir_ { } {}
     
    void Init();           //!< Our ExampleComponent Init Function. 

    void Read(const rapidjson::Value& value);

    void Update(float dt); //!< Our ExampleComponent Update Function.

    static std::string_view Name() { return "DoorBehavior"; }

    void Open(Entity doorTrigger);

  }; /* ExampleComponent */

}; /* Normalloc */