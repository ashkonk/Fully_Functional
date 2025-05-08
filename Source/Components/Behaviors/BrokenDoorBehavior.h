/*!*****************************************************************************
\par filename: BrokenDoorBehavior.h
\par  authors: Jonathan Meyer
\par     date: 03/31/2023

\brief
  This is the header file for the jammed door behavior.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "StateMachine.h"
#include "Behavior.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      This is the jamemd door behavior class.
  ***************************************************************************/
  class BrokenDoorBehavior : public Behavior<BrokenDoorBehavior>
  {
  private:
    Engine* engine_;
    float dt_thresh = 0;
    bool isLocked = false;
    bool colliding_ = false;
    EntityID teleportID_ = 100000; 
    Entity* teleporter_;
    bool found_ = false; 
    float timer_ = 0.0f;

  public:
    BrokenDoorBehavior(Engine* engine, Entity parent) : Behavior(parent), engine_(engine), isLocked(false), colliding_(false), found_(false) {}

    void Read(const rapidjson::Value& value);

    void Update(float dt); //!< Our ExampleComponent Update Function.

    void ExitCollision2D(const Collider& self, const Collider& other);

    void EnterCollision2D(const Collider& self, const Collider& other);

    static std::string_view Name() { return "BrokenDoorBehavior"; }

  }; /* ExampleComponent */

}; /* Normalloc */