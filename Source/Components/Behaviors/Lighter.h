/*!*****************************************************************************
\par filename: Lighter.h
\par  authors: Ashkon Khalkhali
\par     date: 11/23/2022

\brief
    This file contains the lighter behavior.
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
        Lighter behavior class. 
    ***************************************************************************/
    class LighterBehavior : public Behavior<LighterBehavior>
    {
    private:
        Engine* engine_{ nullptr };
        bool pickup_; 

    public:
        LighterBehavior(Engine* engine, Entity parent) : engine_(engine), pickup_(false), Behavior(parent) {}
        void Read(const rapidjson::Value&);
        void Update(float dt);
        bool GetLighterBehaviorPickup() const;
        void SetLighterBehaviorPickup(bool flag);
        void EnterCollision2D(const Collider& self, const Collider& other);

        static std::string_view Name() { return "LighterBehavior"; }


    };

};