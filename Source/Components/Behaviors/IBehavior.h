/*!*****************************************************************************
\par filename: IBehavior.h
\par  authors: Jonathan Meyer
\par     date: 11/15/2022

\brief
  This file contains the public declarations for the behavior component.

  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include "Component.h"
#include <string_view>
#include <string>
#include "Stream.h"
#include "Collider.h"
#include "Vision.h"
#include "iostream"


/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{

  /*!***************************************************************************
    \brief
      This is the behavior component class
  *****************************************************************************/
  class IBehavior : public Component
  {

  public:

    IBehavior(Entity parent) : Component(parent) {} ;

    virtual void EnterCollision2D(const Collider& self, const Collider& other)
      = 0;

    virtual void ExitCollision2D(const Collider& self, const Collider& other)
      = 0;

    virtual void EntityFound(const Vision& self, const Entity& other)
      = 0;

    virtual void EntityLost(const Vision& self, const Entity& other)
      = 0;

    virtual std::string_view TypeName() const = 0;

    virtual ~IBehavior() { };

  };
}; /* FFunctional */