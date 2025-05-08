/*!*****************************************************************************
\par filename: FuseboxKey.h
\par  authors: Ashkon Khalkhali
\par     date: 11/23/2022

\brief
    This file contains the fuse box key behavior. 
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
        FuseboxKey Behavior class
    ***************************************************************************/
    class FuseboxKeyBehavior : public Behavior<FuseboxKeyBehavior>
    {
    private:
        Engine* engine_{ nullptr };
        bool pickup_; 

    public:
        FuseboxKeyBehavior(Engine* engine, Entity parent) : engine_(engine), pickup_(false), Behavior(parent) {}
        void Read(const rapidjson::Value&);
        void Update(float dt);
        bool GetFuseboxKeyBehaviorPickup() const;
        void SetFuseboxKeyBehaviorPickup(bool flag);
        void EnterCollision2D(const Collider& self, const Collider& other);

        static std::string_view Name() { return "FuseboxKeyBehavior"; }


    };

};