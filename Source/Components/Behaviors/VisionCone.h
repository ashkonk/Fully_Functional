/*!*****************************************************************************
\par filename: VisionCone.h
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the Vision Cone Behavior.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "Engine.h"
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
        Vision Cone Behavior class.
    ***************************************************************************/
    class VisionConeBehavior : public Behavior<VisionConeBehavior>
    {

    private:
        Engine* engine_;
        float alertCounter_; 
        bool colliding_; 


    public:
        VisionConeBehavior(Engine* engine, Entity parent) : engine_(engine), colliding_(false), Behavior(parent) {}
        void Read(const rapidjson::Value&);
        bool Lose(float dt);
        void EnterCollision2D(const Collider& self, const Collider& other);
        void ExitCollision2D(const Collider& self, const Collider& other);
        void Update(float dt);
        bool GetColliding();


        static std::string_view Name() { return "VisionConeBehavior"; }

    };
};