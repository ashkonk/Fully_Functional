/*!*****************************************************************************
\par filename: PoisonVial.h
\par  authors: Ashkon Khalkhali
\par     date: 10/16/2022

\brief
    This file contains the poison vial behavior.
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
        Poison vial behavior class.
    ***************************************************************************/
    class PoisonVialBehavior : public Behavior<PoisonVialBehavior>
    {
    private:
        Engine* engine_{ nullptr };
        bool used_;
        bool pickup_; 

    public:
        PoisonVialBehavior(Engine* engine, Entity parent) : engine_(engine),  used_(false), pickup_(false), Behavior(parent) {}
        void Read(const rapidjson::Value&);
        void Update(float dt);
        bool GetPoisonVialPickup() const;
        bool GetPoisonVialUsed() const;
        void SetPoisonVialPickup(bool flag);
        void SetPoisonVialUsed(bool flag);
        void EnterCollision2D(const Collider& self, const Collider& other);

        static std::string_view Name() { return "PoisonVialBehavior"; }


    };

};