/*!*****************************************************************************
\par filename: Behavior.h
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
#include "IBehavior.h"
#include "iostream"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  typedef class Collider Collider;

  template<typename T>
  inline std::string_view Name()
  {
    return T::Name();
  }
  template<>
  inline std::string_view Name<int>()
  {
    return "int";
  }

  /*!***************************************************************************
    \brief
      This is the behavior component class
  *****************************************************************************/
  template <class DerivedBehavior>
  class Behavior : public IBehavior
  {
  private:
    bool enabledEnterCollision2D = false;
    bool enabledExitCollision2D = false;

    bool enabledEntityFound = false;
    bool enabledEntityLost = false;

  public:
    virtual void EnterCollision2D(const Collider& self, const Collider& other)
    {
      self; other;
    };

    virtual void ExitCollision2D(const Collider& self, const Collider& other)
    {
      self; other;
    };

    virtual void EntityFound(const Vision& self, const Entity& other)
    {
      self; other;
    };

    virtual void EntityLost(const Vision& self, const Entity& other)
    {
      self; other;
    };


    Behavior(Entity parent) : IBehavior(parent)
    {
      enabledEnterCollision2D =
        (&DerivedBehavior::EnterCollision2D != &IBehavior::EnterCollision2D);
      
      enabledExitCollision2D =
        (&DerivedBehavior::ExitCollision2D != &IBehavior::ExitCollision2D);

      enabledEntityFound =
        (&DerivedBehavior::EntityFound != &IBehavior::EntityFound);

      enabledEntityLost =
        (&DerivedBehavior::EntityLost != &IBehavior::EntityLost);
    };

    void Init()
    {
      //if (true || enabledExitCollision2D)
      //{
      //  GetParent().AddSignal("ExitCollider", Signal(*this));
      //}
      //if (true || enabledEnterCollision2D)
      //{
      //  GetParent().AddSignal("EnterCollider", Signal(*this));
      //}
      GetParent().AddSignal("ExitCollider", Signal(*this));
      GetParent().AddSignal("EnterCollider", Signal(*this));
      GetParent().AddSignal("EntityFound", Signal(*this));
      GetParent().AddSignal("EntityLost", Signal(*this));
    }

    void Test(const Collider& collider)
    {

      if (!GetParent().IsDestroyed())
        EnterCollision2D(collider);

    }

    static std::string_view Name()
    {
      return FFunctional::Name<DerivedBehavior>();
    }

 
    std::string_view TypeName() const override
    {
      return FFunctional::Name<DerivedBehavior>();
    }

  };
}; /* FFunctional */