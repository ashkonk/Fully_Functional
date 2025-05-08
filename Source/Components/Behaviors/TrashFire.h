/*!*****************************************************************************
\par filename: TrashFire.h
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file holds the Trash fire behavior class. 
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "Stream.h"
#include <glm/glm.hpp>
#include <vector>
#include "Engine.h"
#include "StateMachine.h"
#include "Collider.h"
#include "Behavior.h"


/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        Trash fire behavior class
    ***************************************************************************/
    class TrashFireBehavior : public Behavior<TrashFireBehavior>
    {
    private:
        Engine* engine_{ nullptr };
        bool used_;
        bool colliding_;

    public:
        TrashFireBehavior(Engine* engine, Entity parent) : engine_(engine), colliding_(false), used_(false), Behavior(parent) {}
        void Read(const rapidjson::Value&);
        void Update(float dt);
        void TurnOff();
        bool GetTrashFireUsed() const;
        void SetTrashFireUsed(bool flag);
        void ExitCollision2D(const Collider& self, const Collider& other);
        void EnterCollision2D(const Collider& self, const Collider& other);

        static std::string_view Name() { return "TrashFireBehavior"; }


    };

};