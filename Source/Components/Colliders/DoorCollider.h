/*!*****************************************************************************
\par filename: DoorCollider.h
\par  authors: Ashkon Khalkhali
\par     date: 09/09/2022

\brief
  This is the Door Collider file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <string>
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "StateMachine.h"
#include "Component.h"
#include "Scene.h"
#include "Engine.h"
#include "Entity.h"
#include "SceneLoader.h"
#include "Behavior.h"

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

  class DoorBehavior : public Behavior<DoorBehavior>
  {
  private:
    Engine* engine_{ nullptr };
    std::string sceneDir_;
    bool open_{ false };

  public:
    DoorBehavior(Engine* engine, Entity parent) : Behavior(parent), engine_(engine), sceneDir_ { } {}
     
    /* im hiding Init since its literally unused everywhere else and I need to */
    /* reserve the function. SORRY NOT SORRY! :))) */
    //void Init();           //!< Our ExampleComponent Init Function. 

    void Read(const rapidjson::Value& value);

    void Update(float dt); //!< Our ExampleComponent Update Function.

    static std::string_view Name() { return "DoorBehavior"; }

    void Open(Entity doorTrigger);

  }; /* ExampleComponent */

}; /* Normalloc */